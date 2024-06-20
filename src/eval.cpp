#include "utils/bits.hpp"
#include "eval.hpp"

int Eval::evaluate(Board &board) {
    int eval = 0;

    // white pieces
    uint64_t white_pawns = board.getPieces(Color::WHITE, PieceType::PAWN);
    while(white_pawns) {
        uint8_t index = Bits::pop(white_pawns);
        eval += Eval::PAWN_VALUE;
        eval += Eval::PAWN_POSITIONS[0][index];
    }

    uint64_t white_knights = board.getPieces(Color::WHITE, PieceType::KNIGHT);
    while(white_knights) {
        uint8_t index = Bits::pop(white_knights);
        eval += Eval::KNIGHT_VALUE;
        eval += Eval::KNIGHT_POSITIONS[0][index];
    }

    uint64_t white_bishops = board.getPieces(Color::WHITE, PieceType::BISHOP);
    while(white_bishops) {
        uint8_t index = Bits::pop(white_bishops);
        eval += Eval::BISHOP_VALUE;
        eval += Eval::BISHOP_POSITIONS[0][index];
    }

    uint64_t white_rooks = board.getPieces(Color::WHITE, PieceType::ROOK);
    while(white_rooks) {
        uint8_t index = Bits::pop(white_rooks);
        eval += Eval::ROOK_VALUE;
        eval += Eval::ROOK_POSITIONS[0][index];
    }

    uint64_t white_queens = board.getPieces(Color::WHITE, PieceType::QUEEN);
    while(white_queens) {
        uint8_t index = Bits::pop(white_queens);
        eval += Eval::QUEEN_VALUE;
        eval += Eval::QUEEN_POSITIONS[0][index];
    }

    // black pieces
    uint64_t black_pawns = board.getPieces(Color::BLACK, PieceType::PAWN);
    while(black_pawns) {
        uint8_t index = Bits::pop(black_pawns);
        eval -= Eval::PAWN_VALUE;
        eval -= Eval::PAWN_POSITIONS[1][index];
    }

    uint64_t black_knights = board.getPieces(Color::BLACK, PieceType::KNIGHT);
    while(black_knights) {
        uint8_t index = Bits::pop(black_knights);
        eval -= Eval::KNIGHT_VALUE;
        eval -= Eval::KNIGHT_POSITIONS[1][index];
    }

    uint64_t black_bishops = board.getPieces(Color::BLACK, PieceType::BISHOP);
    while(black_bishops) {
        uint8_t index = Bits::pop(black_bishops);
        eval -= Eval::BISHOP_VALUE;
        eval -= Eval::BISHOP_POSITIONS[1][index];
    }

    uint64_t black_rooks = board.getPieces(Color::BLACK, PieceType::ROOK);
    while(black_rooks) {
        uint8_t index = Bits::pop(black_rooks);
        eval -= Eval::ROOK_VALUE;
        eval -= Eval::ROOK_POSITIONS[1][index];
    }

    uint64_t black_queens = board.getPieces(Color::BLACK, PieceType::QUEEN);
    while(black_queens) {
        uint8_t index = Bits::pop(black_queens);
        eval -= Eval::QUEEN_VALUE;
        eval -= Eval::QUEEN_POSITIONS[1][index];
    }

    // Kings
    eval += KING_MIDDLEGAME_POSITIONS[0][board.getKingIndex(Color::WHITE)];
    eval -= KING_MIDDLEGAME_POSITIONS[1][board.getKingIndex(Color::BLACK)];

    return board.getSideToMove() == Color::WHITE ? eval : (eval * -1);
}