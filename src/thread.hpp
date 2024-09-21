//
// Created by Marcel on 16.09.2024.
//

#ifndef CHESSENGINE_THREAD_HPP
#define CHESSENGINE_THREAD_HPP

#include "board.hpp"
#include "search.hpp"

#include <memory>
#include <thread>

class SearchWrapper {

private:
    std::unique_ptr<Search> search;

public:
    SearchWrapper(Board &board) {
        search = std::make_unique<Search>(board);
    }

    void start(const int depth) const {
        search->start(depth);
    }

};

class SearchThread {

private:
    SearchWrapper searchWrapper;
    std::thread thread;

public:
    SearchThread(Board &b) : searchWrapper(SearchWrapper(b)) { }

    void start(const int depth) {
        if(thread.joinable()) {
            thread.join();
        }
        thread = std::thread(&SearchWrapper::start, &searchWrapper, depth);
    }

};

#endif