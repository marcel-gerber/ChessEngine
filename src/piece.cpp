#include "piece.hpp"

Piece::Piece(Piece::Value piece) : piece(piece) {

}

Piece::Piece(const char &c) : piece(Value::NONE) {
    switch(c) {
        case 'P':
            piece = WHITE_PAWN;
            break;
        case 'N':
            piece = WHITE_KNIGHT;
            break;
        case 'B':
            piece = WHITE_BISHOP;
            break;
        case 'R':
            piece = WHITE_ROOK;
            break;
        case 'Q':
            piece = WHITE_QUEEN;
            break;
        case 'K':
            piece = WHITE_KING;
            break;
        case 'p':
            piece = BLACK_PAWN;
            break;
        case 'n':
            piece = BLACK_KNIGHT;
            break;
        case 'b':
            piece = BLACK_BISHOP;
            break;
        case 'r':
            piece = BLACK_ROOK;
            break;
        case 'q':
            piece = BLACK_QUEEN;
            break;
        case 'k':
            piece = BLACK_KING;
            break;
        default:
            piece = NONE;
            break;
    }
}

Piece::Piece(PieceType type, Color color) {
    if(color == Color::NONE) {
        piece = Piece::NONE;
        return;
    }

    if(type.getValue() == PieceType::NONE) {
        piece = Piece::NONE;
        return;
    }

    piece = static_cast<Value>(color.getValue() * 6 + type.getIndex());
}

bool Piece::operator!=(const Piece &p) const {
    return piece != p.piece;
}

[[nodiscard]] Color Piece::getColor() const {
    if(piece >= WHITE_PAWN && piece <= WHITE_KING) {
        return Color::WHITE;
    }

    if(piece >= BLACK_PAWN && piece <= BLACK_KING) {
        return Color::BLACK;
    }
    return Color::NONE;
}

[[nodiscard]] char Piece::getCharacter() const {
    switch(piece) {
        case Value::WHITE_PAWN:
            return 'P';
        case Value::WHITE_KNIGHT:
            return 'N';
        case Value::WHITE_BISHOP:
            return 'B';
        case Value::WHITE_ROOK:
            return 'R';
        case Value::WHITE_QUEEN:
            return 'Q';
        case Value::WHITE_KING:
            return 'K';
        case Value::BLACK_PAWN:
            return 'p';
        case Value::BLACK_KNIGHT:
            return 'n';
        case Value::BLACK_BISHOP:
            return 'b';
        case Value::BLACK_ROOK:
            return 'r';
        case Value::BLACK_QUEEN:
            return 'q';
        case Value::BLACK_KING:
            return 'k';
        case Value::NONE:
            return ' ';
        default:
            return 0;
    }
}

[[nodiscard]] PieceType Piece::getType() const {
    if(piece == NONE) return PieceType::NONE;
    return static_cast<PieceType::Value>((uint8_t) piece % 6);
}

[[nodiscard]] uint8_t Piece::getValue() const {
    return static_cast<uint8_t>(piece);
}