#include "attacks.hpp"
#include "utils/bits.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "zobrist.hpp"

#include <cassert>
#include <sstream>

Board::Board() {
    std::fill(std::begin(pieces), std::end(pieces), Piece::NONE);
    castling_rights = Castling();
}

void Board::placePiece(const Piece &piece, const uint8_t &index) {
    Bits::set(bb_pieces[piece.type().index()], index);
    Bits::set(bb_sides[piece.color().value()], index);
    pieces[index] = piece;
}

void Board::removePiece(const Piece &piece, const uint8_t &index) {
    Bits::unset(bb_pieces[piece.type().index()], index);
    Bits::unset(bb_sides[piece.color().value()], index);
    pieces[index] = Piece::NONE;
}

[[nodiscard]] uint64_t Board::getPieces(const Color &color, const PieceType &pieceType) const {
    return bb_sides[color.value()] & bb_pieces[pieceType.index()];
}

[[nodiscard]] uint64_t Board::getPieces(const PieceType &pieceType) const {
    return bb_pieces[pieceType.index()];
}

[[nodiscard]] Piece Board::getPiece(const uint8_t &index) const {
    return pieces[index];
}

[[nodiscard]] uint64_t Board::getSide(Color color) const {
    return bb_sides[color.value()];
}

[[nodiscard]] uint64_t Board::getOccupancy() const {
    return bb_sides[0] | bb_sides[1];
}

[[nodiscard]] uint8_t Board::getKingIndex(Color color) const {
    uint64_t bb_king = getPieces(color, PieceType::KING);
    return Bits::lsb(bb_king);
}

void Board::makeMove(const Move &move) {
    const uint8_t from = move.fromIndex();
    const uint8_t to = move.toIndex();
    const uint16_t type = move.type();

    const Piece moved = getPiece(from);
    const Piece captured = getPiece(to);

    StateInfo stateInfo = StateInfo(zobrist_hash, castling_rights, en_passant_square, half_move_clock, captured);
    prev_state_infos.push(stateInfo);

    half_move_clock++;

    if(en_passant_square.value() != Square::NONE) {
        zobrist_hash ^= Zobrist::enPassant(en_passant_square.index());
        en_passant_square = Square::NONE;
    }

    if(captured != Piece::NONE) {
        removePiece(captured, to);

        half_move_clock = 0;
        zobrist_hash ^= Zobrist::piece(captured, to);

        // Remove castling right if rook has been captured
        if(captured.type().value() == PieceType::ROOK) {
            const Castling::Value castling = Castling::fromRookSourceIndex(to);

            // Update zobrist hashes and unset castling rights
            zobrist_hash ^= Zobrist::castling(castling_rights.raw());
            castling_rights.unset(castling);
            zobrist_hash ^= Zobrist::castling(castling_rights.raw());
        }
    }

    if(castling_rights.has(side_to_move)) {
        if(moved.type().value() == PieceType::KING) {
            // Remove castling rights if king moves
            zobrist_hash ^= Zobrist::castling(castling_rights.raw());
            castling_rights.unset(side_to_move);
            zobrist_hash ^= Zobrist::castling(castling_rights.raw());
        } else if(moved.type().value() == PieceType::ROOK) {
            // Remove castling rights if rook moves
            const Castling::Value castling = Castling::fromRookSourceIndex(from);

            zobrist_hash ^= Zobrist::castling(castling_rights.raw());
            castling_rights.unset(castling);
            zobrist_hash ^= Zobrist::castling(castling_rights.raw());
        }
    }

    if(moved.type().value() == PieceType::PAWN) {
        half_move_clock = 0;

        // Double push
        if(std::abs(from - to) == 16) {
            const uint8_t ep_square_index = Square::enPassantIndex(to);
            const uint64_t ep_mask = Attacks::pawn(side_to_move, ep_square_index);

            // if enemy pawns are attacking the en passant square -> set board's en passant square
            if(ep_mask & getPieces(side_to_move.opposite(), PieceType::PAWN)) {
                en_passant_square = Square(ep_square_index);
                zobrist_hash ^= Zobrist::enPassant(ep_square_index);
            }
        }
    }

    if(type == MoveType::CASTLING) {
        const Castling::Value castling = Castling::fromKingTargetIndex(to);
        const uint8_t starting_rook_index = Castling::rookSourceIndex(castling);
        const uint8_t ending_rook_index = Castling::rookTargetIndex(castling);

        const Piece rook = getPiece(starting_rook_index);

        // Remove king and rook
        removePiece(moved, from);
        removePiece(rook, starting_rook_index);

        // Place king and rook at new positions
        placePiece(moved, to);
        placePiece(rook, ending_rook_index);

        zobrist_hash ^= Zobrist::piece(moved, from) ^ Zobrist::piece(moved, to);
        zobrist_hash ^= Zobrist::piece(rook, starting_rook_index) ^ Zobrist::piece(rook, ending_rook_index);
    } else if(type == MoveType::PROMOTION) {
        const PieceType promotion_type = PieceType(move.promotionType());
        const Piece promotion_piece = Piece(promotion_type, side_to_move);

        removePiece(moved, from);
        placePiece(promotion_piece, to);

        zobrist_hash ^= Zobrist::piece(moved, from) ^ Zobrist::piece(promotion_piece, to);
    } else {
        // Place the moved piece at the new position
        removePiece(moved, from);
        placePiece(moved, to);

        zobrist_hash ^= Zobrist::piece(moved, from) ^ Zobrist::piece(moved, to);
    }

    if(type == MoveType::EN_PASSANT) {
        const uint8_t ep_square_index = Square::enPassantIndex(to);
        const Piece pawn = Piece(PieceType::PAWN, side_to_move.opposite());

        removePiece(pawn, ep_square_index);

        zobrist_hash ^= Zobrist::piece(pawn, ep_square_index);
    }

    zobrist_hash ^= Zobrist::sideToMove();
    side_to_move = side_to_move.opposite();

    incrementRepetition(zobrist_hash);
}

void Board::unmakeMove(const Move &move) {
    const StateInfo prev = prev_state_infos.top();
    prev_state_infos.pop();

    // We need to decrement the repetition value before we get the previous hash
    decrementRepetition(zobrist_hash);

    zobrist_hash = prev.hash;
    castling_rights = prev.castling_rights;
    en_passant_square = prev.en_passant;
    half_move_clock = prev.half_move_clock;
    side_to_move = side_to_move.opposite();
    Piece captured = prev.captured;

    const uint8_t from = move.fromIndex();
    const uint8_t to = move.toIndex();
    const uint16_t type = move.type();

    if(type == MoveType::CASTLING) {
        const Castling::Value castling = Castling::fromKingTargetIndex(to);

        // Positions for the rook
        const uint8_t rook_from_index = Castling::rookTargetIndex(castling);
        const uint8_t rook_to_index = Castling::rookSourceIndex(castling);

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
        const Piece pawn = Piece(PieceType::PAWN, side_to_move.opposite());
        const uint8_t pawn_to_index = en_passant_square.index() ^ 8;

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

    const uint8_t fen_half_move_clock = std::stoi(half_move_clock_str);

    half_move_clock = fen_half_move_clock;
    side_to_move = fen_side_to_move == "w" ? Color::WHITE : Color::BLACK;
    en_passant_square = Square(fen_en_passant);

    // start at upper left corner of board
    uint8_t index = 56;
    for(const char &c : fen_pieces) {
        auto piece = Piece(c);

        if(piece != Piece::NONE) {
            placePiece(piece, index);
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
            castling_rights.set(Castling::WHITE_00);
            continue;
        }

        if(c == 'Q') {
            castling_rights.set(Castling::WHITE_000);
            continue;
        }

        if(c == 'k') {
            castling_rights.set(Castling::BLACK_00);
            continue;
        }

        if(c == 'q') {
            castling_rights.set(Castling::BLACK_000);
            continue;
        }
    }
    zobrist_hash = calculateZobrist();
}

void Board::incrementRepetition(const uint64_t &zobrist_key) {
    repetition_table[zobrist_key]++;
}

void Board::decrementRepetition(const uint64_t &zobrist_key) {
    auto it = repetition_table.find(zobrist_key);

    if(it != repetition_table.end()) {
        if(it->second > 1) {
            it->second--;
            return;
        }
        repetition_table.erase(it);
    }
}

bool Board::isRepetition() const {
    auto it = repetition_table.find(zobrist_hash);
    return it != repetition_table.end() && it->second == 3;
}

bool Board::isCheck() const {
    const uint8_t king_index = getKingIndex(side_to_move);
    const Color opponent = side_to_move.opposite();
    const uint64_t occ_bb = getOccupancy();
    const uint64_t opp_queen = getPieces(opponent, PieceType::QUEEN);

    if(getPieces(opponent, PieceType::PAWN) & Attacks::pawn(side_to_move, king_index)) return true;
    if(getPieces(opponent, PieceType::KNIGHT) & Attacks::knight(king_index)) return true;
    if((getPieces(opponent, PieceType::BISHOP) | opp_queen) & Attacks::bishop(king_index, occ_bb)) return true;
    if((getPieces(opponent, PieceType::ROOK) | opp_queen) & Attacks::rook(king_index, occ_bb)) return true;

    return false;
}

uint64_t Board::calculateZobrist() const {
    uint64_t hash = 0ULL;

    uint64_t w_pieces = getSide(Color::WHITE);
    while(w_pieces) {
        uint8_t index = Bits::pop(w_pieces);
        hash ^= Zobrist::piece(getPiece(index), index);
    }

    uint64_t b_pieces = getSide(Color::BLACK);
    while(b_pieces) {
        uint8_t index = Bits::pop(b_pieces);
        hash ^= Zobrist::piece(getPiece(index), index);
    }

    uint64_t ep_hash = 0ULL;

    if(en_passant_square.value() != Square::NONE) {
        ep_hash ^= Zobrist::enPassant(en_passant_square.index());
    }

    uint64_t castling_hash = Zobrist::castling(castling_rights.raw());
    uint64_t side_hash = side_to_move == Color::WHITE ? Zobrist::sideToMove() : 0;

    return hash ^ ep_hash ^ castling_hash ^ side_hash;
}

GameResult Board::checkForDraw(const bool &is_check) const {
    if(half_move_clock >= 100) {
        std::vector<Move> moves;
        MoveGen::legalMoves<MoveGenType::ALL>(*this, moves);

        if(is_check && moves.empty()) return GameResult::LOSS;
        return GameResult::DRAW;
    }

    const uint8_t amount_pieces = Bits::popcount(getOccupancy());

    if(amount_pieces == 2) return GameResult::DRAW;

    // Check for draw by insufficient material
    if(amount_pieces == 3) {
        if(getPieces(PieceType::KNIGHT)) return GameResult::DRAW;
        if(getPieces(PieceType::BISHOP)) return GameResult::DRAW;
    }

    if(amount_pieces == 4) {
        if(Bits::popcount(getPieces(Color::WHITE, PieceType::KNIGHT)) == 1 &&
           Bits::popcount(getPieces(Color::BLACK, PieceType::KNIGHT)) == 1) return GameResult::DRAW;

        if(Bits::popcount(getPieces(Color::WHITE, PieceType::BISHOP)) == 1 &&
           Bits::popcount(getPieces(Color::BLACK, PieceType::BISHOP)) == 1) return GameResult::DRAW;
    }

    return GameResult::NONE;
}

void Board::reset() {
    std::fill(std::begin(bb_pieces), std::end(bb_pieces), 0);
    std::fill(std::begin(bb_sides), std::end(bb_sides), 0);
    std::fill(std::begin(pieces), std::end(pieces), Piece::NONE);

    zobrist_hash = 0ULL;
    castling_rights.reset();
    en_passant_square = Square::NONE;
    half_move_clock = 0;
    side_to_move = Color::WHITE;

    repetition_table.clear();
}

[[nodiscard]] uint64_t Board::getZobrist() const {
    return zobrist_hash;
}

[[nodiscard]] Castling Board::getCastlingRights() const {
    return castling_rights;
}

[[nodiscard]] const Square* Board::getEnPassantSquare() const {
    return &en_passant_square;
}

[[nodiscard]] Color Board::getSideToMove() const {
    return side_to_move;
}

[[nodiscard]] uint8_t Board::getHalfMoveClock() const {
    return half_move_clock;
}

void Board::print() const {
    std::stringstream ss;
    uint8_t index = 56;
    Piece piece{};

    ss << "---------------------------------\n";

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            piece = getPiece(index);
            ss << "| " << piece.character() << " ";

            index++;
        }
        ss << "|\n";
        ss << "---------------------------------\n";
        index -= 16;
    }

    std::cout << ss.str();
}