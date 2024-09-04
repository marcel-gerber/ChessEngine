//
// Created by Marcel on 26.08.2024.
//

#include "eval.hpp"
#include "movegen.hpp"
#include "movepick.hpp"
#include "search.hpp"
#include "tt.hpp"

#include <cstdint>
#include <vector>

const int INFINITY = 0x7FFFFFFF;

Search::Search(Board &board) : board(board) {

}

int Search::search(const int &depth) {
    orig_depth = depth;
    return negamax(depth, -INFINITY, INFINITY);
}

int Search::negamax(int depth, int alpha, int beta) {
    nodes_searched++;
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

    MovePicker::scoreMoves(board, moves);
    MovePicker::sortMoves(moves);

    for(const Move &move : moves) {
        board.makeMove(move);
        int score = -negamax(depth - 1, -beta, -alpha);
        board.unmakeMove(move);

        if(score > max_score) {
            max_score = score;
            local_best_move = move;
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