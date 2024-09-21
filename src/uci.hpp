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
    UCICommandUCINewGame(Board &b) : board(b) { }

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
        const auto piece = board.getPiece(from.getIndex());

        // En Passant Move
        if(piece.getType().getValue() == PieceType::PAWN && to.getIndex() == board.getEnPassantSquare()->getIndex()) {
            return Move::create<MoveType::EN_PASSANT>(from.getIndex(), to.getIndex());
        }

        // Castling Move
        if(piece.getType().getValue() == PieceType::KING && std::abs(from.getIndex() - to.getIndex()) == 2) {
            return Move::create<MoveType::CASTLING>(from.getIndex(), to.getIndex());
        }

        switch(input.size()) {
            case 4:
                return Move::create<MoveType::NORMAL>(from.getIndex(), to.getIndex());
            case 5:
                const auto promotion_piece = Piece(input[4]);
                return Move::create<MoveType::PROMOTION>(from.getIndex(), to.getIndex(), promotion_piece.getType());
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
    UCICommandPosition(Board &b) : board(b) { }

    void execute(const std::vector<std::string> &args) override {
        if(args.empty()) return;
        int index = 0;

        if(args[0] == "startpos") {
            index = 1;
            board.setFen(Constants::START_POS);
        } else if(args[0] == "fen") {
            const auto pair_index_fen = getIndexAndFen(args);

            index = pair_index_fen.first;
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
    SearchThread searchThread;

public:
    UCICommandGo(Board &b) : board(b), searchThread(SearchThread(b)) { }

    void execute(const std::vector<std::string> &args) override {
        if(args.empty()) {
            searchThread.start(Constants::STANDARD_DEPTH);
            return;
        }

        if(args.size() == 2) {
            if(args[0] == "depth") {
                int depth = std::stoi(args[1]);
                searchThread.start(depth);
                return;
            }

            if(args[0] == "infinite") {
                // TODO: Implement this with Iterative Deepening
            }
        }
    }
};

class UCIHandler {

private:
    std::unordered_map<std::string, std::unique_ptr<UCICommand>> commands;
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

public:
    UCIHandler() {
        board = Board();

        commands["uci"] = std::make_unique<UCICommandUCI>();
        commands["isready"] = std::make_unique<UCICommandIsReady>();
        commands["ucinewgame"] = std::make_unique<UCICommandUCINewGame>(board);
        commands["position"] = std::make_unique<UCICommandPosition>(board);
        commands["go"] = std::make_unique<UCICommandGo>(board);
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
};

#endif