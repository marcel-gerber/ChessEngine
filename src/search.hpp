#ifndef CHESSENGINE_SEARCH_HPP
#define CHESSENGINE_SEARCH_HPP

#include "board.hpp"
#include "constants.hpp"

#include <array>
#include <atomic>
#include <chrono>

// Class for calculating time per move
struct TimeManager {
    int time = 0;
    int increment = 0;
    int move_time = 0;
    uint8_t moves_to_go = 0;

    /// Returns a time in milliseconds that we have for finding a best move
    /// Returns -1 if there is no time control
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
    std::array<std::array<Move, Constants::MAX_PLY>, Constants::MAX_PLY> pv = {};
    std::array<int, Constants::MAX_PLY> pv_length = {};

    // Stop flag for search thread
    std::atomic<bool> stop_flag = {false};

    /// Main search function. Returns score of best move in centi pawns
    int negamax(int depth, int alpha, int beta, int ply);

    /// Quiescence search function. Should only be called by negamax()
    int quiescence(int alpha, int beta);

    /// Iterative Deepening. Calls negamax() iteratively with increasing depth and prints info
    void iterativeDeepening(int max_depth);

    /// Returns 'true' when the time is up
    [[nodiscard]] bool isTimeUp() const;

    /// Resets all the data used for the search. Should always be called before starting a new search
    void resetData();

    /// Prints search information in UCI format
    void printInfo(int depth, int score, int nodes, uint64_t time);

public:
    explicit Search(Board &board);

    /// Starts a search until the given depth
    void start(const int &depth);

    /// Stops a search
    void stop();

    TimeManager* getTimeManager() {
        return &time_manager;
    }
};

#endif
