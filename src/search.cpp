//
// Created by Marcel on 26.08.2024.
//

#include "eval.hpp"
#include "movegen.hpp"
#include "movepick.hpp"
#include "search.hpp"
#include "tt.hpp"

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <vector>

const int INFINITY = 0x7FFFFFFF;
const int VALUE_MATE = 32000;

int mated(const int &ply) {
    return ply - VALUE_MATE;
}

Search::Search(Board &board) : board(board) {

}

void Search::start(const int &depth) {
    iterativeDeepening(depth);
}

int Search::negamax(int depth, int alpha, int beta, int ply) {
    nodes_searched++;
    pv_length[ply] = ply;

    if(board.isRepetition()) return -1;

    const bool is_check = board.isCheck();
    const GameResult gameResult = board.checkForDraw(is_check);

    if(gameResult != GameResult::NONE) return gameResult == GameResult::LOSS ? mated(ply) : 0;

    const uint64_t zobrist_hash = board.getZobrist();
    const int orig_alpha = alpha;

    // Transposition Table look up
    const TT::Entry* entry = TT::getEntry(zobrist_hash);
    if(ply != 0 && zobrist_hash == entry->zobrist_key) {
        if(entry->depth >= depth) {
            if(entry->nodeType == NodeType::PV_NODE) {
                return entry->evaluation;
            } else if(entry->nodeType == NodeType::CUT_NODE) {
                alpha = std::max(alpha, entry->evaluation);
            } else if(entry->nodeType == NodeType::ALL_NODE) {
                beta = std::min(beta, entry->evaluation);
            }

            if(alpha >= beta) {
                return entry->evaluation;
            }
        }
    }

    if(depth == 0) {
        return quiescence(alpha, beta);
    }

    int max_score = -INFINITY;
    Move local_best_move = {};
    std::vector<Move> moves = {};
    MoveGen::legalMoves<MoveGenType::ALL>(board, moves);

    // If there are no legal moves we are either in checkmate or stalemate
    if(moves.empty()) return is_check ? mated(ply) : 0;

    MovePicker::scoreMoves(board, entry, moves);
    MovePicker::sortMoves(moves);

    for(const Move &move : moves) {
        board.makeMove(move);
        int score = -negamax(depth - 1, -beta, -alpha, ply + 1);
        board.unmakeMove(move);

        if(score > max_score) {
            max_score = score;
            local_best_move = move;

            // Update PV
            pv[ply][ply] = move;
            for(int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
                pv[ply][next_ply] = pv[ply + 1][next_ply];
            }
            pv_length[ply] = pv_length[ply + 1];
        }

        alpha = std::max(alpha, max_score);

        // Beta-Cutoff
        if(alpha >= beta) {
            break;
        }
    }

    TT::addEntry(zobrist_hash, local_best_move, depth, max_score, orig_alpha, beta);
    return max_score;
}

int Search::quiescence(int alpha, int beta) {
    nodes_searched++;
    int stand_pat = Eval::evaluate(board);
    if(stand_pat >= beta) {
        return beta;
    }

    alpha = std::max(alpha, stand_pat);

    std::vector<Move> captures = {};
    MoveGen::legalMoves<MoveGenType::CAPTURE>(board, captures);

    for(const Move &capture : captures) {
        board.makeMove(capture);
        int score = -quiescence(-beta, -alpha);
        board.unmakeMove(capture);

        if(score >= beta) {
            return beta;
        }

        alpha = std::max(alpha, score);
    }

    return alpha;
}

void Search::iterativeDeepening(int max_depth) {
    resetData();

    auto start_time = std::chrono::high_resolution_clock::now();

    for(int depth = 1; depth <= max_depth; depth++) {
        int temp_score = negamax(depth, -INFINITY, INFINITY, 0);

        auto current_time = std::chrono::high_resolution_clock::now();
        int time_spent = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

        printInfo(depth, temp_score, nodes_searched, time_spent);
    }
    std::cout << "bestmove " << getBestMove().toUCI() << std::endl;
}

void Search::resetData() {
    nodes_searched = 0;
    pv.fill({});
    pv_length.fill({});
}

void Search::printInfo(int depth, int score, int nodes, int time) {
    time = (time == 0) ? 1 : time;

    std::cout << std::fixed << std::setprecision(0)
              << "info depth "<< depth
              << " score cp " << score
              << " nodes " << nodes
              << " nps " << (nodes / ((double) time / 1000))
              << " time " << time
              << " pv ";

    for(int i = 0; i < pv_length[0]; i++) {
        std::cout << pv[0][i].toUCI() << " ";
    }
    std::cout << std::endl;
}