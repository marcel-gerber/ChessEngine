//
// Created by Marcel on 06.04.2024.
//

#ifndef CHESSENGINE_DEBUG_HPP
#define CHESSENGINE_DEBUG_HPP

#include <random>
#include <string>

#include "chess.hpp"
#include "board.hpp"
#include "fen_parser.hpp"

class Debug {
private:
    static uint64_t perft_excpected(chess::Board &board, int depth) {
        chess::Movelist moves;
        chess::movegen::legalmoves(moves, board);

        if(depth == 1) {
            return moves.size();
        }

        uint64_t nodes = 0ULL;

        for(const auto &move : moves) {
            board.makeMove(move);
            nodes += perft_excpected(board, depth - 1);
            board.unmakeMove(move);
        }

        return nodes;
    }

    static bool play_random_moves(chess::Board &board, const int &amount) {
        chess::Movelist moves;

        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());

        for(int i = 0; i < amount; i++) {
            moves.clear();
            chess::movegen::legalmoves(moves, board);

            if(moves.empty()) return false;

            std::uniform_int_distribution<> distribution(0, moves.size() - 1);
            int random_number = distribution(gen);

            chess::Move random_move = moves[random_number];
            board.makeMove(random_move);
        }
        return true;
    }

public:
    static void run(const std::string &fen) {
        chess::Board board_expected(fen);

        while(play_random_moves(board_expected, 4)) {
            const std::string current_fen = board_expected.getFen();

            Board board;
            Fen::setFen(board, current_fen);

            Perft perft(board);

            const uint64_t nodes_board = perft.nodes(3);
            const uint64_t nodes_expected = perft_excpected(board_expected, 3);

            if(nodes_board != nodes_expected) {
                std::cout << current_fen << std::endl;
                board.print();
                break;
            }
        }
    }

};

#endif
