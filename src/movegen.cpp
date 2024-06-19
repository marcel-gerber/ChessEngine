#include "attacks.hpp"
#include "grid.hpp"
#include "movegen.hpp"

void MoveGen::initSquaresBetween() {
    uint64_t squares_bb = 0ULL;
    uint64_t attacks = 0ULL;

    for(Square square1 = 0; square1 < 64; square1++) {
        for(Square square2 = 0; square2 < 64; square2++) {
            if(square1.getIndex() == square2.getIndex()) continue;

            squares_bb = (1ULL << square1.getIndex()) | (1ULL << square2.getIndex());

            if(square1.getFileIndex() == square2.getFileIndex() || square1.getRankIndex() == square2.getRankIndex()) {

                attacks = Attacks::getRookAttacks(square1.getIndex(), squares_bb)
                          & Attacks::getRookAttacks(square2.getIndex(), squares_bb);
                SQUARES_BETWEEN[square1.getIndex()][square2.getIndex()] = attacks;

                continue;
            }

            if(square1.getDiagonalIndex() == square2.getDiagonalIndex()
               || square1.getAntiDiagonalIndex() == square2.getAntiDiagonalIndex()) {

                attacks = Attacks::getBishopAttacks(square1.getIndex(), squares_bb)
                          & Attacks::getBishopAttacks(square2.getIndex(), squares_bb);
                SQUARES_BETWEEN[square1.getIndex()][square2.getIndex()] = attacks;

                continue;
            }
        }
    }
}

template<Color::Value color>
uint64_t MoveGen::pinMaskHV(const Board &board) {
    const uint8_t king_index = board.getKingIndex(color);
    const uint64_t bb_us = board.getSide(color);
    const uint64_t bb_opp = board.getSide(Color::opposite<color>());

    const uint64_t opp_rook = board.getPieces(Color::opposite<color>(), PieceType::ROOK);
    const uint64_t opp_queen = board.getPieces(Color::opposite<color>(), PieceType::QUEEN);

    uint64_t rook_attacks = Attacks::getRookAttacks(king_index, bb_opp) & (opp_rook | opp_queen);
    uint64_t pin_hv = 0ULL;

    while(rook_attacks) {
        uint8_t index = Bits::pop(rook_attacks);

        const uint64_t possible_pin = SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
        if(Bits::popcount(possible_pin & bb_us) == 1) {
            pin_hv |= possible_pin;
        }
    }
    return pin_hv;
}

template<Color::Value color>
uint64_t MoveGen::pinMaskDiagonal(const Board &board) {
    const uint8_t king_index = board.getKingIndex(color);
    const uint64_t bb_us = board.getSide(color);
    const uint64_t bb_opp = board.getSide(Color::opposite<color>());

    const uint64_t opp_bishop = board.getPieces(Color::opposite<color>(), PieceType::BISHOP);
    const uint64_t opp_queen = board.getPieces(Color::opposite<color>(), PieceType::QUEEN);

    uint64_t bishop_attacks = Attacks::getBishopAttacks(king_index, bb_opp) & (opp_bishop | opp_queen);
    uint64_t pin_d = 0ULL;

    while(bishop_attacks) {
        uint8_t index = Bits::pop(bishop_attacks);

        const uint64_t possible_pin = SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
        if(Bits::popcount(possible_pin & bb_us) == 1) {
            pin_d |= possible_pin;
        }
    }
    return pin_d;
}

template<Color::Value color>
uint64_t MoveGen::attackedSquares(const Board &board) {
    const uint8_t opp_king = board.getKingIndex(Color::opposite<color>());

    const uint64_t occupied = board.getOccupancy() & ~Square::toBitboard(opp_king);
    const uint64_t queens = board.getPieces(color, PieceType::QUEEN);

    uint64_t pawns = board.getPieces(color, PieceType::PAWN);
    uint64_t knights = board.getPieces(color, PieceType::KNIGHT);
    uint64_t bishops = board.getPieces(color, PieceType::BISHOP) | queens;
    uint64_t rooks = board.getPieces(color, PieceType::ROOK) | queens;

    uint64_t attacked = Attacks::getPawnLeftAttacks(pawns, color) | Attacks::getPawnRightAttacks(pawns, color);

    while(knights) {
        const uint8_t index = Bits::pop(knights);
        attacked |= Attacks::getKnightAttacks(index);
    }

    while(bishops) {
        const uint8_t index = Bits::pop(bishops);
        attacked |= Attacks::getBishopAttacks(index, occupied);
    }

    while(rooks) {
        const uint8_t index = Bits::pop(rooks);
        attacked |= Attacks::getRookAttacks(index, occupied);
    }

    const uint8_t king_index = board.getKingIndex(color);
    attacked |= Attacks::getKingAttacks(king_index);

    return attacked;
}

template<Color::Value color>
std::tuple<uint64_t, uint8_t> MoveGen::checkMask(const Board &board) {
    uint8_t double_check = 0;
    Color opp_color = Color::opposite<color>();
    const uint8_t king_index = board.getKingIndex(color);

    const uint64_t opp_queen = board.getPieces(opp_color, PieceType::QUEEN);

    const uint64_t opp_pawns = board.getPieces(opp_color, PieceType::PAWN);
    const uint64_t opp_knights = board.getPieces(opp_color, PieceType::KNIGHT);
    const uint64_t opp_bishops = board.getPieces(opp_color, PieceType::BISHOP) | opp_queen;
    const uint64_t opp_rooks = board.getPieces(opp_color, PieceType::ROOK) | opp_queen;

    // knight checks
    uint64_t knight_attacks = Attacks::getKnightAttacks(king_index) & opp_knights;
    double_check += bool(knight_attacks);

    // initializing check_mask
    uint64_t check_mask = knight_attacks;

    // pawn checks
    // Important note: using color and not opponent color here!
    uint64_t pawn_attacks = Attacks::getPawnAttacks(color, king_index) & opp_pawns;
    double_check += bool(pawn_attacks);
    check_mask |= pawn_attacks;

    // bishop checks
    uint64_t bishop_attacks = Attacks::getBishopAttacks(king_index, board.getOccupancy()) & opp_bishops;

    if(bishop_attacks) {
        const uint8_t index = Bits::lsb(bishop_attacks);
        check_mask |= SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
        double_check++;
    }

    // rook checks
    uint64_t rook_attacks = Attacks::getRookAttacks(king_index, board.getOccupancy()) & opp_rooks;

    if(rook_attacks) {
        if(Bits::popcount(rook_attacks) > 1) {
            return {check_mask, 2};
        }

        const uint8_t index = Bits::lsb(rook_attacks);
        check_mask |= SQUARES_BETWEEN[king_index][index] | Square::toBitboard(index);
        double_check++;
    }

    if(!check_mask) {
        return {0xFFFFFFFFFFFFFFFFULL, double_check};
    }

    return {check_mask, double_check};
}

void MoveGen::getPromotionMoves(std::vector<Move> &moves, const uint8_t &target_index, const int8_t &direction) {
    const uint8_t from_index = target_index + direction;

    moves.push_back(Move::create<MoveType::PROMOTION>(from_index, target_index, PieceType::KNIGHT));
    moves.push_back(Move::create<MoveType::PROMOTION>(from_index, target_index, PieceType::BISHOP));
    moves.push_back(Move::create<MoveType::PROMOTION>(from_index, target_index, PieceType::ROOK));
    moves.push_back(Move::create<MoveType::PROMOTION>(from_index, target_index, PieceType::QUEEN));
}

template<Color::Value color>
void MoveGen::generatePawnMoves(const Board &board, std::vector<Move> &moves, const uint64_t &pin_hv,
                                const uint64_t &pin_d, const uint64_t &checkmask) {
    const int8_t UP = color == Color::WHITE ? 8 : -8;
    const int8_t DOWN = color == Color::WHITE ? -8 : 8;
    const int8_t DOWN_LEFT = color == Color::WHITE ? -9 : 9;
    const int8_t DOWN_RIGHT = color == Color::WHITE ? -7 : 7;

    const uint64_t RANK_PROMO = Rank::getPromoRank<color>();
    const uint64_t DOUBLE_PUSH_RANK = Rank::getDoublePushRank<color>();

    const uint64_t bb_empty = ~board.getOccupancy();
    const uint64_t bb_opp = board.getSide(Color::opposite<color>());
    const uint64_t pawns = board.getPieces(color, PieceType::PAWN);

    const uint64_t pawns_pinned_hv = pawns & pin_hv;
    const uint64_t pawns_pinned_d = pawns & pin_d;
    const uint64_t pawns_not_pinned = pawns & ~pin_hv & ~pin_d;
    const uint64_t pawns_not_pinned_hv = pawns & ~pin_hv;

    // these pawns can walk forward
    uint64_t single_push_unpinned = shift(pawns_not_pinned, UP) & bb_empty;

    // these need special check as horizontally pinned pawns cannot move at all
    // but vertically pinned pawns can walk forward
    uint64_t single_push_pinned = shift(pawns_pinned_hv, UP) & bb_empty;

    // these pawns can (maybe) do a double push
    uint64_t double_push_unpinned = single_push_unpinned & DOUBLE_PUSH_RANK;
    uint64_t double_push_pinned = single_push_pinned & DOUBLE_PUSH_RANK;

    single_push_unpinned &= checkmask;
    single_push_pinned &= checkmask;

    while(single_push_unpinned) {
        const uint8_t target_index = Bits::pop(single_push_unpinned);

        if(Square::toBitboard(target_index) & RANK_PROMO) {
            getPromotionMoves(moves, target_index, DOWN);
            continue;
        }

        moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN, target_index));
    }

    while(single_push_pinned) {
        const uint8_t target_index = Bits::pop(single_push_pinned);
        if(!(Square::toBitboard(target_index) & pin_hv)) continue;

        moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN, target_index));
    }

    // if there are no pawns that can double push -> skip this part
    if((double_push_unpinned | double_push_pinned)) {
        double_push_unpinned = shift(double_push_unpinned, UP) & bb_empty & checkmask;
        double_push_pinned = shift(double_push_pinned, UP) & bb_empty & checkmask;

        while(double_push_unpinned) {
            const uint8_t target_index = Bits::pop(double_push_unpinned);
            moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN + DOWN, target_index));
        }

        while(double_push_pinned) {
            const uint8_t target_index = Bits::pop(double_push_pinned);
            if(!(Square::toBitboard(target_index) & pin_hv)) continue;

            moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN + DOWN, target_index));
        }
    }

    // these pawns can take left and right as they are not pinned
    uint64_t pawns_left_unpinned = Attacks::getPawnLeftAttacks(pawns_not_pinned, color) & bb_opp & checkmask;
    uint64_t pawns_right_unpinned = Attacks::getPawnRightAttacks(pawns_not_pinned, color) & bb_opp & checkmask;

    while(pawns_left_unpinned) {
        const uint8_t target_index = Bits::pop(pawns_left_unpinned);

        if(Square::toBitboard(target_index) & RANK_PROMO) {
            getPromotionMoves(moves, target_index, DOWN_RIGHT);
            continue;
        }

        moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN_RIGHT, target_index));
    }

    while(pawns_right_unpinned) {
        const uint8_t target_index = Bits::pop(pawns_right_unpinned);

        if(Square::toBitboard(target_index) & RANK_PROMO) {
            getPromotionMoves(moves, target_index, DOWN_LEFT);
            continue;
        }

        moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN_LEFT, target_index));
    }

    // these pawns need special check as they are diagonally pinned
    uint64_t pawns_left_pinned = Attacks::getPawnLeftAttacks(pawns_pinned_d, color) & bb_opp & checkmask;
    uint64_t pawns_right_pinned = Attacks::getPawnRightAttacks(pawns_pinned_d, color) & bb_opp & checkmask;

    while(pawns_left_pinned) {
        const uint8_t target_index = Bits::pop(pawns_left_pinned);
        const uint64_t target_sq_bb = Square::toBitboard(target_index);

        if(!(target_sq_bb & pin_d)) continue;

        if(target_sq_bb & RANK_PROMO) {
            getPromotionMoves(moves, target_index, DOWN_RIGHT);
            continue;
        }

        moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN_RIGHT, target_index));
    }

    while(pawns_right_pinned) {
        const uint8_t target_index = Bits::pop(pawns_right_pinned);
        const uint64_t target_sq_bb = Square::toBitboard(target_index);

        if(!(target_sq_bb & pin_d)) continue;

        if(target_sq_bb & RANK_PROMO) {
            getPromotionMoves(moves, target_index, DOWN_LEFT);
            continue;
        }

        moves.push_back(Move::create<MoveType::NORMAL>(target_index + DOWN_LEFT, target_index));
    }

    // En Passant
    const Square* en_passant = board.getEnPassantSquare();

    if(en_passant->getValue() != Square::NONE) {
        const uint8_t en_passant_index = en_passant->getIndex();

        const uint8_t ep_pawn_capture = en_passant_index + DOWN;

        // If the en passant square and the enemy pawn that double pushed
        // is not on the checkmask, en passant is not available
        if((checkmask & (Square::toBitboard(en_passant_index) | Square::toBitboard(ep_pawn_capture))) == 0ULL) return;

        // possible en passant pawns
        uint64_t ep_pawns_bb = Attacks::getPawnAttacks(Color::opposite<color>(), en_passant_index) & pawns_not_pinned_hv;

        // we need to know where our king and opponent sliders are to check
        // whether our king is in check after en passant
        const uint8_t king_index = board.getKingIndex(color);
        const uint64_t opp_queens = board.getPieces(Color::opposite<color>(), PieceType::QUEEN);
        const uint64_t opp_rooks = board.getPieces(Color::opposite<color>(), PieceType::ROOK) | opp_queens;
        const uint64_t opp_bishops = board.getPieces(Color::opposite<color>(), PieceType::BISHOP) | opp_queens;

        // occupancy mask without the possible en passant capture pawn
        uint64_t ep_mask = board.getOccupancy() & ~Square::toBitboard(ep_pawn_capture);

        // Up to 2 pawns can take en passant
        while(ep_pawns_bb) {
            const uint8_t from = Bits::pop(ep_pawns_bb);

            // 'removing' the possible en passant pawn from 'ep_mask'
            uint64_t mask = ep_mask & ~Square::toBitboard(from);
            mask |= Square::toBitboard(en_passant_index);

            const uint64_t rook_attacks = Attacks::getRookAttacks(king_index, mask);
            const uint64_t bishop_attacks = Attacks::getBishopAttacks(king_index, mask);

            if(((rook_attacks & opp_rooks) | (bishop_attacks & opp_bishops)) != 0ULL) continue;

            moves.push_back(Move::create<MoveType::EN_PASSANT>(from, en_passant_index));
        }
    }
}

uint64_t MoveGen::generateKnightMoves(const uint8_t &index) {
    return Attacks::getKnightAttacks(index);
}

uint64_t MoveGen::generateBishopMoves(const uint8_t &index, const uint64_t &pin_d, const uint64_t &bb_occupied) {
    // If the bishop is pinned diagonally, it can only move on this diagonal
    if(pin_d & Square::toBitboard(index)) {
        return Attacks::getBishopAttacks(index, bb_occupied) & pin_d;
    }
    return Attacks::getBishopAttacks(index, bb_occupied);
}

uint64_t MoveGen::generateRookMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &bb_occupied) {
    // If the rook is pinned horizontally or vertically, it can only move horizontal/vertical
    if(pin_hv & Square::toBitboard(index)) {
        return Attacks::getRookAttacks(index, bb_occupied) & pin_hv;
    }
    return Attacks::getRookAttacks(index, bb_occupied);
}

uint64_t MoveGen::generateQueenMoves(const uint8_t &index, const uint64_t &pin_hv, const uint64_t &pin_d,
                                     const uint64_t &bb_occupied) {
    uint64_t bb_square = Square::toBitboard(index);

    if(pin_hv & bb_square) {
        return Attacks::getRookAttacks(index, bb_occupied) & pin_hv;
    }

    if(pin_d & bb_square) {
        return Attacks::getBishopAttacks(index, bb_occupied) & pin_d;
    }

    return Attacks::getQueenAttacks(index, bb_occupied);
}

uint64_t MoveGen::generateKingMoves(const uint8_t &index, const uint64_t &bb_attacked, const uint64_t &bb_movable_squares) {
    return Attacks::getKingAttacks(index) & bb_movable_squares & ~bb_attacked;
}

template<Color::Value color>
void MoveGen::generateCastleMoves(Board &board, std::vector<Move> &moves, const uint64_t &bb_attacked) {
    const auto castling_rights = board.getCastlingRights();
    if(castling_rights->hasNoCastling()) return;

    const uint8_t king_index = board.getKingIndex(color);

    for(const auto castle : Castling::getCastlings<color>()) {
        if(!castling_rights->has(castle)) continue;

        const uint8_t end_king_index = Castling::getEndingKingIndex(castle);
        const uint8_t start_rook_index = Castling::getStartingRookIndex(castle);

        // Squares that have to be empty
        const uint64_t not_occ_path = SQUARES_BETWEEN[king_index][start_rook_index];

        // Squares that are not allowed to be attacked by the enemy
        const uint64_t not_attacked_path = SQUARES_BETWEEN[king_index][end_king_index] | Square::toBitboard(end_king_index);

        // Bitboard with all empty squares and squares that are not attacked by the enemy
        const uint64_t empty_not_attacked = ~board.getOccupancy() & ~bb_attacked;

        // If the squares that have to be empty are not empty -> continue
        if((not_occ_path & ~board.getOccupancy()) != not_occ_path) {
            continue;
        }

        // If the squares that the king moves through are attacked -> continue
        if((not_attacked_path & empty_not_attacked) != not_attacked_path) {
            continue;
        }

        // If the king is in check -> continue
        if((bb_attacked & Square::toBitboard(king_index)) != 0) {
            continue;
        }

        moves.push_back(Move::create<MoveType::CASTLING>(king_index, end_king_index));
    }
}

template<Color::Value color>
void MoveGen::legalMoves(Board &board, std::vector<Move> &moves) {
    uint64_t bb_king = board.getPieces(color, PieceType::KING);

    uint64_t bb_occupied = board.getOccupancy();
    uint64_t bb_occ_us = board.getSide(color);

    // All empty squares and opponent pieces
    uint64_t movable_squares = ~bb_occ_us;

    auto [checkmask, double_check] = MoveGen::checkMask<color>(board);
    uint64_t pin_hv = pinMaskHV<color>(board);
    uint64_t pin_d = pinMaskDiagonal<color>(board);

    // Squares that are attacked by the enemy
    uint64_t attacked = attackedSquares<Color::opposite<color>()>(board);

    // King moves
    addMoveToMoveList(moves, bb_king, [&](uint8_t index) {
        return generateKingMoves(index, attacked, movable_squares);
    });

    // Castling moves
    generateCastleMoves<color>(board, moves, attacked);

    movable_squares &= checkmask;

    if(double_check == 2) return;

    // Pawn moves
    generatePawnMoves<color>(board, moves, pin_hv, pin_d, checkmask);

    // Knight moves (those that are not pinned)
    uint64_t bb_knights = board.getPieces(color, PieceType::KNIGHT) & ~(pin_hv | pin_d);
    addMoveToMoveList(moves, bb_knights, [&](uint8_t index) {
        return generateKnightMoves(index) & movable_squares;
    });

    // Bishop moves (those that are not horizontally/vertically pinned)
    uint64_t bb_bishops = board.getPieces(color, PieceType::BISHOP) & ~pin_hv;
    addMoveToMoveList(moves, bb_bishops, [&](uint8_t index) {
        return generateBishopMoves(index, pin_d, bb_occupied) & movable_squares;
    });

    // Rook moves (those that are not diagonally pinned)
    uint64_t bb_rooks = board.getPieces(color, PieceType::ROOK) & ~pin_d;
    addMoveToMoveList(moves, bb_rooks, [&](uint8_t index) {
        return generateRookMoves(index, pin_hv, bb_occupied) & movable_squares;
    });

    // Queen moves (those that are not both horizontally/vertically and diagonally pinned)
    uint64_t bb_queens = board.getPieces(color, PieceType::QUEEN) & ~(pin_hv & pin_d);
    addMoveToMoveList(moves, bb_queens, [&](uint8_t index) {
        return generateQueenMoves(index, pin_hv, pin_d, bb_occupied) & movable_squares;
    });
}

void MoveGen::legalMoves(Board &board, std::vector<Move> &moves) {
    if(board.getSideToMove() == Color::WHITE) {
        legalMoves<Color::WHITE>(board, moves);
        return;
    }
    legalMoves<Color::BLACK>(board, moves);
}