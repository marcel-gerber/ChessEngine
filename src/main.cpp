#include <iostream>
#include "board.hpp"
#include "fen_parser.hpp"
#include "attacks.hpp"
#include "movegen.hpp"
#include "perft.hpp"

int main() {
    Attacks::initMagics();
    MoveGen::initSquaresBetween();

//    Debug::run("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

    Board board = Board();

    Fen::setFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
//    std::cout << +board.getHalfMoveClock() << std::endl;
//    std::cout << +board.getSideToMove() << std::endl;
//    std::cout << +board.getCastlingRights()->getCastlingRights() << std::endl;
//    std::cout << +board.getEnPassantSquare()->getIndex() << std::endl;

//    Fen::setFen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 10");
//    board.print();
//
//    uint64_t attacked = MoveGen::attackedSquares(board, Color::WHITE);
//    Bits::print(attacked);

//    Fen::setFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

    Perft perft(board);
    perft.run(6);

//    Move move = moves[0];
//    board.makeMove(move);
//    board.print();
//    std::cout << std::endl;
//    board.unmakeMove(move);
//    board.print();

//    std::cout << "Unmake move: " << std::endl;
//    board.unmakeMove(move);
//    board.print();

//    uint8_t from = 48;
//    uint8_t to = 56;
//    Move move = Move::create<MoveType::PROMOTION>(from, to, PieceType::QUEEN);
//
//    std::cout << +move.from_index() << std::endl;
//    std::cout << +move.to_index() << std::endl;
//    std::cout << +move.type() << std::endl;
//    std::cout << +move.promotion_type() << std::endl;

//    auto square = Square(Square::Value::E7);
//    uint64_t attacks = Attacks::getQueenAttacks(square, board.getOccupancy());
//    Bits::print(attacks);

    return 0;
}