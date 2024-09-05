//
// Created by Marcel on 05.09.2024.
//

#ifndef CHESSENGINE_UCI_HPP
#define CHESSENGINE_UCI_HPP

#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

class UCICommand {

public:
    virtual void execute() = 0;
    virtual ~UCICommand() = default;
};

class UCICommandUCI : public UCICommand {

public:
    void execute() override {
        std::cout << "id name ChessEngine v1.0" << std::endl;
        std::cout << "id author Marcel Gerber" << std::endl;
        std::cout << "uciok" << std::endl;
    }
};

class UCICommandIsReady : public UCICommand {

public:
    void execute() override {
        std::cout << "readyok" << std::endl;
    }
};

class UCIHandler {

private:
    std::unordered_map<std::string, std::unique_ptr<UCICommand>> commands;

public:
    UCIHandler() {
        commands["uci"] = std::make_unique<UCICommandUCI>();
        commands["isready"] = std::make_unique<UCICommandIsReady>();
    }

    void handleCommand(const std::string &input) {
        std::istringstream istringstream(input);
        std::string command;
        istringstream >> command;

        if(commands.find(command) != commands.end()) {
            commands[command]->execute();
            return;
        }
        std::cout << "Unknown command: " << command << std::endl;
    }
};

#endif