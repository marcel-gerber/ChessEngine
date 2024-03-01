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

namespace fen {

std::vector<std::string> splitString(const std::string &fen) {
    std::vector<std::string> result;
    std::stringstream stringstream(fen);
    std::string item;

    while(std::getline(stringstream, item, ' ')) {
        result.push_back(item);
    }
    return result;
}

void setFen(const Board &board, const std::string &&fen) {
    const auto split = splitString(fen);
    assert(!split.empty());

    const auto pieces = split[0];
    const auto side_to_move = split.size() > 1 ? split[1] : "w";
    const auto castling = split.size() > 2 ? split[2] : "-";
    const auto en_passant = split.size() > 3 ? split[3] : "-";
    const auto half_move_clock_str = split.size() > 4 ? split[4] : "0";
    const auto full_move_counter_str = split.size() > 5 ? split[5] : "1";

    const uint8_t half_move_clock = std::stoi(half_move_clock_str);
    const uint16_t full_move_counter = std::stoi(full_move_counter_str);

    std::cout << +half_move_clock << std::endl;
    std::cout << +full_move_counter << std::endl;
}



}

#endif
