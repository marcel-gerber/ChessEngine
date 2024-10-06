//
// Created by Marcel on 05.09.2024.
//

#ifndef CHESSENGINE_UCI_HPP
#define CHESSENGINE_UCI_HPP

#include "board.hpp"
#include "grid.hpp"
#include "thread.hpp"
#include "tt.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

class UCICommand {

public:
    virtual void execute(const std::vector<std::string> &args) = 0;
    virtual ~UCICommand() = default;
};

class UCICommandUCI : public UCICommand {

public:
    void execute(const std::vector<std::string> &args) override {
        std::cout << "id name ChessEngine v1.0" << std::endl;
        std::cout << "id author Marcel Gerber" << std::endl;
        std::cout << "uciok" << std::endl;
    }
};

class UCICommandIsReady : public UCICommand {

public:
    void execute(const std::vector<std::string> &args) override {
        std::cout << "readyok" << std::endl;
    }
};

class UCICommandUCINewGame : public UCICommand {

private:
    Board &board;

public:
    explicit UCICommandUCINewGame(Board &b) : board(b) { }

    void execute(const std::vector<std::string> &args) override {
        board.reset();
        TT::clear();
    }
};

class UCICommandPosition : public UCICommand {

private:
    Board &board;

    Move uciToMove(const std::string &input) {
        const std::string s_from = input.substr(0, 2);
        const std::string s_to = input.substr(2, 2);

        const Square from = Square(s_from);
        const Square to = Square(s_to);
        const auto piece = board.getPiece(from.index());

        // En Passant Move
        if(piece.type().value() == PieceType::PAWN && to.index() == board.getEnPassantSquare()->index()) {
            return Move::create<MoveType::EN_PASSANT>(from.index(), to.index());
        }

        // Castling Move
        if(piece.type().value() == PieceType::KING && std::abs(from.index() - to.index()) == 2) {
            return Move::create<MoveType::CASTLING>(from.index(), to.index());
        }

        switch(input.size()) {
            case 4:
                return Move::create<MoveType::NORMAL>(from.index(), to.index());
            case 5:
                const auto promotion_piece = Piece(input[4]);
                return Move::create<MoveType::PROMOTION>(from.index(), to.index(), promotion_piece.type());
        }
        return { };
    }

    static std::pair<int, std::string> getIndexAndFen(const std::vector<std::string> &args) {
        std::string fen = args[1];
        int index = 2;

        for(; index < args.size(); index++) {
            if(args[index] == "moves") break;
            fen.append(" ");
            fen.append(args[index]);
        }
        return {index, fen};
    }

public:
    explicit UCICommandPosition(Board &b) : board(b) { }

    void execute(const std::vector<std::string> &args) override {
        if(args.empty()) return;
        int index = 0;

        if(args[0] == "startpos") {
            index = 1;
            board.reset();
            board.setFen(Constants::START_POS);
        } else if(args[0] == "fen") {
            const auto pair_index_fen = getIndexAndFen(args);

            index = pair_index_fen.first;
            board.reset();
            board.setFen(pair_index_fen.second);
        }

        if(args[index] == "moves") {
            index++;

            while(index < args.size()) {
                const std::string &uci_move = args[index];
                const auto move = uciToMove(uci_move);

                board.makeMove(move);
                index++;
            }
        }
    }
};

class UCICommandGo : public UCICommand {

private:
    Board &board;
    SearchThread &search_thread;

public:
    explicit UCICommandGo(Board &b, SearchThread &st) : board(b), search_thread(st) { }

    void execute(const std::vector<std::string> &args) override {
        if(args.empty()) {
            search_thread.start(Constants::MAX_PLY);
            return;
        }

        const std::string time_str = board.getSideToMove() == Color::WHITE ? "wtime" : "btime";
        const std::string inc_str = board.getSideToMove() == Color::WHITE ? "winc" : "binc";

        for(int index = 0; index < args.size(); index++) {
            if(args[index] == "infinite") {
                search_thread.start(Constants::MAX_PLY);
                return;
            }

            if(args[index] == "depth") {
                int depth = std::stoi(args[index + 1]);
                search_thread.start(depth);
                return;
            }

            if(args[index] == time_str) {
                int time = std::stoi(args[index + 1]);
                search_thread.getSearch()->getTimeManager()->time = time;
                continue;
            }

            if(args[index] == inc_str) {
                int inc = std::stoi(args[index + 1]);
                search_thread.getSearch()->getTimeManager()->increment = inc;
                continue;
            }

            if(args[index] == "movestogo") {
                int moves_to_go = std::stoi(args[index + 1]);
                search_thread.getSearch()->getTimeManager()->moves_to_go = moves_to_go;
                continue;
            }
        }
        search_thread.start(Constants::MAX_PLY);
    }
};

class UCICommandStop : public UCICommand {

private:
    SearchThread &search_thread;

public:
    explicit UCICommandStop(SearchThread &st) : search_thread(st) { }

    void execute(const std::vector<std::string> &args) override {
        search_thread.stop();
    }
};

class UCIHandler {

private:
    std::unordered_map<std::string, std::unique_ptr<UCICommand>> commands;
    SearchThread searchThread;
    Board board;

    static std::pair<std::string, std::vector<std::string>> split(const std::string &input) {
        std::vector<std::string> arguments;
        std::stringstream stringstream(input);
        std::string command, argument;

        // get command
        std::getline(stringstream, command, ' ');

        // get all arguments of command
        while(std::getline(stringstream, argument, ' ')) {
            arguments.push_back(argument);
        }
        return {command, arguments};
    }

    void handleCommand(const std::string &input) {
        const auto split_pair = split(input);
        const auto &command = split_pair.first;
        const auto &args = split_pair.second;

        if(commands.find(command) != commands.end()) {
            commands[command]->execute(args);
            return;
        }
    }

public:
    UCIHandler() : board(Board()), searchThread(SearchThread(board)) {
        commands["uci"] = std::make_unique<UCICommandUCI>();
        commands["isready"] = std::make_unique<UCICommandIsReady>();
        commands["ucinewgame"] = std::make_unique<UCICommandUCINewGame>(board);
        commands["position"] = std::make_unique<UCICommandPosition>(board);
        commands["go"] = std::make_unique<UCICommandGo>(board, searchThread);
        commands["stop"] = std::make_unique<UCICommandStop>(searchThread);
    }

    void inputLoop() {
        std::string input;

        while(std::getline(std::cin, input)) {
            if(input == "quit") {
                searchThread.stop();
                return;
            }

            handleCommand(input);
        }
    }
};

#endif