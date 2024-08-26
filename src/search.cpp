//
// Created by Marcel on 26.08.2024.
//

#include "eval.hpp"
#include "movegen.hpp"
#include "search.hpp"
#include "tt.hpp"

#include <cstdint>
#include <vector>

const int INFINITY = 0x7FFFFFFF;

int Search::negamax(Board &board, int depth, int alpha, int beta) {
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
        return Eval::evaluate(board);
    }

    int max_score = -INFINITY;
    Move best_move = {};
    std::vector<Move> moves = {};
    MoveGen::legalMoves<MoveGenType::ALL>(board, moves);

    for(const Move &move : moves) {
        board.makeMove(move);
        int score = -negamax(board, depth - 1, -beta, -alpha);
        board.unmakeMove(move);

        if(score > max_score) {
            max_score = score;
            best_move = move;
        }

        if(max_score > alpha) {
            alpha = max_score;
        }

        // Beta-Cutoff
        if(alpha >= beta) {
            break;
        }
    }

    TT::addEntry(zobrist_hash, best_move, depth, max_score, orig_alpha, beta);
    return max_score;
}