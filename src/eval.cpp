#include "utils/bits.hpp"
#include "eval.hpp"

template<GameState gameState>
int Eval::evaluate(const Board &board) {
    int eval = 0;

    // white pieces
    // We do addition here
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

    // ---------------------------------------------------------------------- \\

    // black pieces
    // We do subtraction here
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

    // ---------------------------------------------------------------------- \\

    // kings
    if constexpr(gameState == GameState::MIDDLEGAME) {
        eval += KING_MIDDLEGAME_POSITIONS[0][board.getKingIndex(Color::WHITE)];
        eval -= KING_MIDDLEGAME_POSITIONS[1][board.getKingIndex(Color::BLACK)];
    } else {
        eval += KING_ENDGAME_POSITIONS[0][board.getKingIndex(Color::WHITE)];
        eval -= KING_ENDGAME_POSITIONS[1][board.getKingIndex(Color::BLACK)];
    }

    // Since we do addition on white pieces and subtraction on black pieces,
    // we have to negate the eval when black is to move
    return board.getSideToMove() == Color::WHITE ? eval : (eval * -1);
}

int Eval::evaluate(const Board &board) {
    // Get bitboard of all major (queens and rooks) and minor (bishops and knights) pieces
    uint64_t major_and_minor = board.getOccupancy() & ~board.getPieces(PieceType::PAWN);

    // Count pieces and subtract both kings'
    const uint8_t piece_count = Bits::popcount(major_and_minor) - 2;

    if(piece_count < 7) {
        return evaluate<GameState::ENDGAME>(board);
    }
    return evaluate<GameState::MIDDLEGAME>(board);
}