#ifndef CHESSENGINE_THREAD_HPP
#define CHESSENGINE_THREAD_HPP

#include "board.hpp"
#include "search.hpp"

#include <memory>
#include <thread>

// Class for managing a thread for the search
class SearchThread {

private:
    Search search;
    std::thread thread;

public:
    explicit SearchThread(Board &b) : search(Search(b)) { }

    /// Starts a new search with the given depth in a separate thread
    void start(const int depth) {
        if(thread.joinable()) {
            thread.join();
        }
        thread = std::thread(&Search::start, &search, depth);
    }

    /// Stops the search and the thread
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