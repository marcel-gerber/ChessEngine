//
// Created by Marcel on 16.09.2024.
//

#ifndef CHESSENGINE_THREAD_HPP
#define CHESSENGINE_THREAD_HPP

#include "board.hpp"
#include "search.hpp"

#include <memory>
#include <thread>

class SearchThread {

private:
    Search search;
    std::thread thread;

public:
    SearchThread(Board &b) : search(Search(b)) { }

    void start(const int depth) {
        if(thread.joinable()) {
            thread.join();
        }
        thread = std::thread(&Search::start, &search, depth);
    }

    void stop() {
        if(thread.joinable()) {
            search.stop();
            thread.join();
        }
    }

    Search* getSearch() {
        return &search;
    }

};

#endif