//
// Created by Marcel on 26.08.2024.
//

#ifndef CHESSENGINE_SEARCH_HPP
#define CHESSENGINE_SEARCH_HPP

#include "board.hpp"
#include "constants.hpp"

#include <array>
#include <atomic>
#include <chrono>

struct TimeManager {
    int time = 0;
    int increment = 0;
    int move_time = 0;
    uint8_t moves_to_go = 0;

    [[nodiscard]] int getTimeForMove() const;
};

class Search {

private:
    Board &board;
    int nodes_searched = 0;

    // Time management
    TimeManager time_manager = {};
    std::chrono::high_resolution_clock::time_point start_time = {};
    int thinking_time = 0;

    // PV Nodes
    std::array<std::array<Move, Constants::MAX_PLY>, Constants::MAX_PLY> pv;
    std::array<int, Constants::MAX_PLY> pv_length;

    // Stop flag for search thread
    std::atomic<bool> stop_flag;

    int negamax(int depth, int alpha, int beta, int ply);
    int quiescence(int alpha, int beta);
    void iterativeDeepening(int max_depth);

    [[nodiscard]] bool isTimeUp() const;

    void resetData();
    void printInfo(int depth, int score, int nodes, uint64_t time);

public:
    explicit Search(Board &board);

    void start(const int &depth);

    void stop();

    [[nodiscard]] Move getBestMove() const {
        return pv[0][0];
    }

    TimeManager* getTimeManager() {
        return &time_manager;
    }
};

#endif
