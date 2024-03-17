#include <iostream>
#include "board.hpp"
#include "fen_parser.hpp"
#include "attacks.hpp"
#include "movegen.hpp"

int main() {
    Attacks::initMagics();
    MoveGen::initSquaresBetween();

    Board board = Board();

//    Fen::setFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
//    std::cout << +board.getHalfMoveClock() << std::endl;
//    std::cout << +board.getSideToMove() << std::endl;
//    std::cout << +board.getCastlingRights()->getCastlingRights() << std::endl;
//    std::cout << +board.getEnPassantSquare()->getIndex() << std::endl;

//    Fen::setFen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 10");
//    board.print();
//
//    uint64_t attacked = MoveGen::attackedSquares(board, Color::WHITE);
//    Bits::print(attacked);

    /*Fen::setFen(board, "rnbk1b1r/pp3ppp/2p5/4q1B1/4n3/8/PPP2PPP/2KR1BNR b - - 1 10");
    board.print();
    auto [mask, double_check] = MoveGen::checkMask(board, Color::BLACK);
    Bits::print(mask);
    std::cout << +double_check << std::endl;*/

    Fen::setFen(board, "r2qkbnr/p1pp1ppp/bp1np3/8/3PPB2/5N2/PPPQ1PPP/R3K2R w KQkq - 0 1");
    board.print();

    uint64_t attacked = MoveGen::attackedSquares(board, Color::BLACK);
    uint64_t castling = MoveGen::generateCastleMoves(board, Color::WHITE, attacked);
    Bits::print(castling);

//    auto square = Square(Square::Value::E7);
//    uint64_t attacks = Attacks::getQueenAttacks(square, board.getOccupancy());
//    Bits::print(attacks);

    return 0;
}
