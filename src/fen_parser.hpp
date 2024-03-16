//
// Created by Marcel on 01.03.2024.
//

#ifndef CHESSENGINE_FEN_PARSER_HPP
#define CHESSENGINE_FEN_PARSER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include "board.hpp"

class Fen {

private:
    static std::vector<std::string> splitString(const std::string &fen) {
        std::vector<std::string> result;
        std::stringstream stringstream(fen);
        std::string item;

        while(std::getline(stringstream, item, ' ')) {
            result.push_back(item);
        }
        return result;
    }

public:
    static void setFen(Board &board, const std::string &&fen) {
        const auto split = splitString(fen);
        assert(!split.empty());

        const auto& pieces = split[0];
        const auto side_to_move = split.size() > 1 ? split[1] : "w";
        const auto castling = split.size() > 2 ? split[2] : "-";
        const auto en_passant = split.size() > 3 ? split[3] : "-";
        const auto half_move_clock_str = split.size() > 4 ? split[4] : "0";
        const auto full_move_counter_str = split.size() > 5 ? split[5] : "1";

        const uint8_t half_move_clock = std::stoi(half_move_clock_str);
        const uint16_t full_move_counter = std::stoi(full_move_counter_str);

        board.setHalfMoveClock(half_move_clock);
        board.setSideToMove(side_to_move == "w" ? Color::WHITE : Color::BLACK);
        board.setEnPassantSquare(Square(en_passant));

        // start at upper left corner of board
        uint8_t index = 56;
        for(const char &c : pieces) {
            auto piece = Piece(c);

            if(piece != Piece::NONE) {
                board.placePiece(piece, index);
                index++;
                continue;
            }

            if(c == '/') {
                index -= 16;
                continue;
            }

            if(isdigit(c)) {
                index += (c - '0'); // get the numeric value of the character c
            }
        }

        const auto castling_rights = board.getCastlingRights();

        for(const char &c : castling) {
            if(c == '-') break;

            if(c == 'K') {
                castling_rights->set(Castling::WHITE_00);
                continue;
            }

            if(c == 'Q') {
                castling_rights->set(Castling::WHITE_000);
                continue;
            }

            if(c == 'k') {
                castling_rights->set(Castling::BLACK_00);
                continue;
            }

            if(c == 'q') {
                castling_rights->set(Castling::BLACK_000);
                continue;
            }
        }
    }

};

#endif
