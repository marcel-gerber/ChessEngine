//
// Created by Marcel on 05.09.2024.
//

#ifndef CHESSENGINE_UCI_HPP
#define CHESSENGINE_UCI_HPP

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

class UCIHandler {

private:
    std::unordered_map<std::string, std::unique_ptr<UCICommand>> commands;

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
        commands["uci"] = std::make_unique<UCICommandUCI>();
        commands["isready"] = std::make_unique<UCICommandIsReady>();
    }

    void handleCommand(const std::string &input) {
        const auto split_pair = split(input);
        const auto &command = split_pair.first;
        const auto &args = split_pair.second;

        if(commands.find(command) != commands.end()) {
            commands[command]->execute(args);
            return;
        }
        std::cout << "Unknown command: " << command << std::endl;
    }
};

#endif