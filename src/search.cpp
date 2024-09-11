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
#include <vector>

const int INFINITY = 0x7FFFFFFF;

Search::Search(Board &board) : board(board) {

}

int Search::start(const int &depth) {
    orig_depth = depth;
    return iterativeDeepening(depth);
}

int Search::negamax(int depth, int alpha, int beta, int ply) {
    nodes_searched++;
    pv_length[ply] = ply;

    const uint64_t zobrist_hash = board.getZobrist();
    const int orig_alpha = alpha;

    // Transposition Table look up
    auto entry = TT::getEntry(zobrist_hash);
    if(zobrist_hash == entry.zobrist_key) {
        if(entry.depth >= depth) {
            if(entry.nodeType == NodeType::PV_NODE) {
                return entry.evaluation;
            } else if(entry.nodeType == NodeType::CUT_NODE) {
                alpha = std::max(alpha, entry.evaluation);
            } else if(entry.nodeType == NodeType::ALL_NODE) {
                beta = std::min(beta, entry.evaluation);
            }

            if(alpha >= beta) {
                return entry.evaluation;
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

    if(depth == orig_depth) {
        best_move = local_best_move;
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

int Search::iterativeDeepening(int max_depth) {
    int current_score = 0;

    resetData();

    auto start_time = std::chrono::high_resolution_clock::now();

    for(int depth = 1; depth <= max_depth; depth++) {
        int temp_score = negamax(depth, -INFINITY, INFINITY, 0);

        auto current_time = std::chrono::high_resolution_clock::now();
        int time_spent = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

        current_score = temp_score;

        printInfo(depth, max_depth, temp_score, getSearchedNodes(), time_spent);
    }

    return current_score;
}

void Search::resetData() {
    nodes_searched = 0;
    pv.fill({});
    pv_length.fill({});
}

void Search::printInfo(int depth, int seldepth, int score, int nodes, int time) {
    std::cout << "info depth " << depth
              << " seldepth " << seldepth
              << " score cp " << score
              << " nodes " << nodes
              << " time " << time
              << " pv ";

    for(int i = 0; i < pv_length[0]; i++) {
        std::cout << pv[0][i].toUCI() << " ";
    }
    std::cout << std::endl;
}