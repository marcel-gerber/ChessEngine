#include "attacks.hpp"
#include "utils/bits.hpp"
#include "board.hpp"

#include <cassert>
#include <sstream>

Board::Board() {
    for(auto & piece : pieces) {
        piece = Piece::NONE;
    }

    castling_rights = Castling();
}

[[nodiscard]] Castling* Board::getCastlingRights() {
    return &castling_rights;
}

[[nodiscard]] const Square* Board::getEnPassantSquare() const {
    return &en_passant_square;
}

void Board::setEnPassantSquare(Square square) {
    this->en_passant_square = square;
}

[[nodiscard]] Color Board::getSideToMove() const {
    return this->side_to_move;
}

void Board::setSideToMove(Color color) {
    this->side_to_move = color;
}

[[nodiscard]] uint8_t Board::getHalfMoveClock() const {
    return this->half_move_clock;
}

void Board::setHalfMoveClock(const uint8_t &half_move) {
    this->half_move_clock = half_move;
}

void Board::placePiece(const Piece &piece, const uint8_t &index) {
    Bits::set(bb_pieces[piece.getType().getIndex()], index);
    Bits::set(bb_sides[piece.getColor().getValue()], index);
    pieces[index] = piece;
}

void Board::removePiece(const Piece &piece, const uint8_t &index) {
    Bits::unset(bb_pieces[piece.getType().getIndex()], index);
    Bits::unset(bb_sides[piece.getColor().getValue()], index);
    pieces[index] = Piece::NONE;
}

[[nodiscard]] uint64_t Board::getPieces(const Color &color, const PieceType &pieceType) const {
    return bb_sides[color.getValue()] & bb_pieces[pieceType.getIndex()];
}

[[nodiscard]] Piece Board::getPiece(const uint8_t &index) const {
    return pieces[index];
}

[[nodiscard]] uint64_t Board::getSide(Color color) const {
    return bb_sides[color.getValue()];
}

[[nodiscard]] uint64_t Board::getOccupancy() const {
    return bb_sides[0] | bb_sides[1];
}

[[nodiscard]] uint8_t Board::getKingIndex(Color color) const {
    uint64_t bb_king = getPieces(color, PieceType::KING);
    return Bits::lsb(bb_king);
}

void Board::makeMove(const Move &move) {
    const uint8_t from = move.from_index();
    const uint8_t to = move.to_index();
    const uint16_t type = move.type();

    const Piece moved = getPiece(from);
    const Piece captured = getPiece(to);

    StateInfo stateInfo = StateInfo(castling_rights, en_passant_square, half_move_clock, captured);
    prev_state_infos.push(stateInfo);

    half_move_clock++;

    if(en_passant_square.getValue() != Square::NONE) {
        en_passant_square = Square::NONE;
    }

    if(captured != Piece::NONE) {
        removePiece(captured, to);

        // Remove castling right if rook has been captured
        if(captured.getType().getValue() == PieceType::ROOK) {
            const Castling::Value castling = Castling::getFromRookIndex(to);
            this->castling_rights.unset(castling);
        }
    }

    if(castling_rights.has(side_to_move)) {
        if(moved.getType().getValue() == PieceType::KING) {
            // Remove castling rights if king moves
            castling_rights.unset(side_to_move);
        } else if(moved.getType().getValue() == PieceType::ROOK) {
            // Remove castling rights if rook moves
            const Castling::Value castling = Castling::getFromRookIndex(from);
            this->castling_rights.unset(castling);
        }
    }

    if(moved.getType().getValue() == PieceType::PAWN) {
        half_move_clock = 0;

        // Double push
        if(std::abs(from - to) == 16) {
            const uint8_t ep_square_index = Square::getEnPassantSquare(to);
            const uint64_t ep_mask = Attacks::getPawnAttacks(side_to_move, ep_square_index);

            // if enemy pawns are attacking the en passant square -> set board's en passant square
            if(ep_mask & getPieces(side_to_move.getOppositeColor(), PieceType::PAWN)) {
                en_passant_square = Square(ep_square_index);
            }
        }
    }

    if(type == MoveType::CASTLING) {
        const Castling::Value castling = Castling::getFromKingIndex(to);
        const uint8_t starting_rook_index = Castling::getStartingRookIndex(castling);
        const uint8_t ending_rook_index = Castling::getEndingRookIndex(castling);

        const Piece rook = getPiece(starting_rook_index);

        // Remove king and rook
        removePiece(moved, from);
        removePiece(rook, starting_rook_index);

        // Place king and rook at new positions
        placePiece(moved, to);
        placePiece(rook, ending_rook_index);
    } else if(type == MoveType::PROMOTION) {
        const PieceType promotion_type = PieceType(move.promotion_type());
        const Piece promotion_piece = Piece(promotion_type, side_to_move);

        removePiece(moved, from);
        placePiece(promotion_piece, to);
    } else {
        // Place the moved piece at the new position
        removePiece(moved, from);
        placePiece(moved, to);
    }

    if(type == MoveType::EN_PASSANT) {
        const uint8_t ep_square_index = Square::getEnPassantSquare(to);
        const Piece pawn = Piece(PieceType::PAWN, side_to_move.getOppositeColor());

        // TODO: understanding this ??
        removePiece(pawn, ep_square_index);
    }

    side_to_move = side_to_move.getOppositeColor();
}

void Board::unmakeMove(const Move &move) {
    const StateInfo prev = prev_state_infos.top();
    prev_state_infos.pop();

    castling_rights = prev.castling_rights;
    en_passant_square = prev.en_passant;
    half_move_clock = prev.half_move_clock;
    side_to_move = side_to_move.getOppositeColor();
    Piece captured = prev.captured;

    const uint8_t from = move.from_index();
    const uint8_t to = move.to_index();
    const uint16_t type = move.type();

    if(type == MoveType::CASTLING) {
        const Castling::Value castling = Castling::getFromKingIndex(to);

        // Positions for the rook
        const uint8_t rook_from_index = Castling::getEndingRookIndex(castling);
        const uint8_t rook_to_index = Castling::getStartingRookIndex(castling);

        const Piece rook = getPiece(rook_from_index);
        const Piece king = getPiece(to);

        // Remove king and rook
        removePiece(rook, rook_from_index);
        removePiece(king, to);

        // Place king and rook at previous positions
        placePiece(rook, rook_to_index);
        placePiece(king, from);

        return;
    }

    if(type == MoveType::PROMOTION) {
        const Piece pawn = Piece(PieceType::PAWN, side_to_move);
        const Piece promoted = getPiece(to);

        // Remove promoted piece
        removePiece(promoted, to);

        // Place pawn at previous position
        placePiece(pawn, from);

        if(captured != Piece::NONE) {
            placePiece(captured, to);
        }

        return;
    }

    const Piece moved = getPiece(to);

    // Place moved piece at previous position
    removePiece(moved, to);
    placePiece(moved, from);

    if(type == MoveType::EN_PASSANT) {
        const Piece pawn = Piece(PieceType::PAWN, side_to_move.getOppositeColor());
        const uint8_t pawn_to_index = en_passant_square.getIndex() ^ 8;

        placePiece(pawn, pawn_to_index);

        return;
    }

    if(captured != Piece::NONE) {
        placePiece(captured, to);
    }
}

void Board::setFen(const std::string &fen) {
    auto splitString = [](const std::string &string) -> std::vector<std::string> {
        std::vector<std::string> result;
        std::stringstream stringstream(string);
        std::string item;

        while(std::getline(stringstream, item, ' ')) {
            result.push_back(item);
        }
        return result;
    };

    const auto split = splitString(fen);

    const auto& fen_pieces = split[0];
    const auto fen_side_to_move = split.size() > 1 ? split[1] : "w";
    const auto fen_castling = split.size() > 2 ? split[2] : "-";
    const auto fen_en_passant = split.size() > 3 ? split[3] : "-";
    const auto half_move_clock_str = split.size() > 4 ? split[4] : "0";
    const auto full_move_counter_str = split.size() > 5 ? split[5] : "1";

    const uint8_t fen_half_move_clock = std::stoi(half_move_clock_str);
    const uint16_t fen_full_move_counter = std::stoi(full_move_counter_str);

    this->setHalfMoveClock(fen_half_move_clock);
    this->setSideToMove(fen_side_to_move == "w" ? Color::WHITE : Color::BLACK);
    this->setEnPassantSquare(Square(fen_en_passant));

    // start at upper left corner of board
    uint8_t index = 56;
    for(const char &c : fen_pieces) {
        auto piece = Piece(c);

        if(piece != Piece::NONE) {
            this->placePiece(piece, index);
            index++;
            continue;
        }

        if(c == '/') {
            index -= 16;
            continue;
        }

        if(isdigit(c)) {
            index += (c - '0'); // get the numeric value of the character c
        }
    }

    for(const char &c : fen_castling) {
        if(c == '-') break;

        if(c == 'K') {
            this->castling_rights.set(Castling::WHITE_00);
            continue;
        }

        if(c == 'Q') {
            this->castling_rights.set(Castling::WHITE_000);
            continue;
        }

        if(c == 'k') {
            this->castling_rights.set(Castling::BLACK_00);
            continue;
        }

        if(c == 'q') {
            this->castling_rights.set(Castling::BLACK_000);
            continue;
        }
    }
}

void Board::print() const {
    std::stringstream ss;
    uint8_t index = 56;
    Piece piece{};

    ss << "---------------------------------\n";

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            piece = getPiece(index);
            ss << "| " << piece.getCharacter() << " ";

            index++;
        }
        ss << "|\n";
        ss << "---------------------------------\n";
        index -= 16;
    }

    std::cout << ss.str();
}