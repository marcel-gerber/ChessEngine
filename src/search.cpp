//
// Created by Marcel on 26.08.2024.
//

#include "eval.hpp"
#include "movegen.hpp"
#include "movepick.hpp"
#include "search.hpp"
#include "tt.hpp"

#include <cstdint>
#include <iomanip>
#include <vector>

const int INFINITY = 0x7FFFFFFF;
const int VALUE_MATE = 32000;

int mated(const int &ply) {
    return ply - VALUE_MATE;
}

int TimeManager::getTimeForMove() const {
    if(move_time > 0) return move_time;

    if(time == 0) return -1;

    int time_for_move;
    if(moves_to_go == 0) {
        // If moves_to_go is not given, we assume that there are 30 moves to play
        time_for_move = time / 30 + increment;
    } else
        time_for_move = time / moves_to_go + increment;

    // No more than half of the remaining time and at least 100 ms
    time_for_move = std::min(time_for_move, time / 2);
    time_for_move = std::max(time_for_move, 100);

    return time_for_move;
}

Search::Search(Board &board) : board(board), stop_flag(false) { }

void Search::start(const int &depth) {
    stop_flag = false;
    iterativeDeepening(depth);
}

void Search::stop() {
    stop_flag = true;
}

int Search::negamax(int depth, int alpha, int beta, int ply) {
    if(isTimeUp()) return 0;

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
    if(ply != 0 && entry->zobrist_key == zobrist_hash) {
        if(entry->depth >= depth) {
            if(entry->flag == Flag::EXACT) {
                return entry->evaluation;
            } else if(entry->flag == Flag::LOWER_BOUND) {
                alpha = std::max(alpha, entry->evaluation);
            } else if(entry->flag == Flag::UPPER_BOUND) {
                beta = std::min(beta, entry->evaluation);
            }

            if(alpha >= beta) return entry->evaluation;
        }
    }

    if(depth == 0) return quiescence(alpha, beta);

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

    thinking_time = time_manager.getTimeForMove();
    start_time = std::chrono::high_resolution_clock::now();
    Move best_move = {};

    for(int depth = 1; depth <= max_depth && !stop_flag; depth++) {
        int temp_score = negamax(depth, -INFINITY, INFINITY, 0);

        // If the time is up we break. We only want to update the best move after a complete search.
        // If a search was incomplete, we also don't want to print the info since it could contain illegal pv moves.
        if(isTimeUp()) break;

        best_move = pv[0][0];

        auto current_time = std::chrono::high_resolution_clock::now();
        uint64_t time_spent = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

        printInfo(depth, temp_score, nodes_searched, time_spent);
    }

    if(best_move.raw() == 0) best_move = pv[0][0];
    std::cout << "bestmove " << best_move.toUCI() << std::endl;
}

bool Search::isTimeUp() const {
    if(thinking_time == -1) return false;
    auto now = std::chrono::high_resolution_clock::now();
    uint64_t time_passed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    return time_passed >= thinking_time;
}

void Search::resetData() {
    nodes_searched = 0;
    thinking_time = 0;
    pv.fill({});
    pv_length.fill({});
}

void Search::printInfo(int depth, int score, int nodes, uint64_t time) {
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