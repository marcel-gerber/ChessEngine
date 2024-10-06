#include "attacks.hpp"
#include "utils/bits.hpp"
#include "constants.hpp"

#include <functional>

void Attacks::addRay(const Square &square, const Direction::Value &direction,
                     uint64_t &attacks_bb, const uint64_t &occupied) {
    const int8_t direction_value = Direction::toValue(direction);
    uint8_t prev_file_index = square.fileIndex();
    Square next = Square(square.index() + direction_value);

    while(next.isValid()) {
        if(std::abs(prev_file_index - next.fileIndex()) > 1) return;

        attacks_bb |= 1ULL << next.index();
        if(Bits::isSet(occupied, next.index())) return;

        prev_file_index = next.fileIndex();
        next = Square(next.index() + direction_value);
    }
}

uint64_t Attacks::calculateRook(Square &square, uint64_t occupied) {
    uint64_t attacks = 0ULL;

    addRay(square, Direction::NORTH, attacks, occupied);
    addRay(square, Direction::EAST, attacks, occupied);
    addRay(square, Direction::SOUTH, attacks, occupied);
    addRay(square, Direction::WEST, attacks, occupied);

    return attacks;
}

/// Function used for initializing Magic Table
uint64_t Attacks::calculateBishop(Square &square, uint64_t occupied) {
    uint64_t attacks = 0ULL;

    addRay(square, Direction::NORTH_EAST, attacks, occupied);
    addRay(square, Direction::SOUTH_EAST, attacks, occupied);
    addRay(square, Direction::SOUTH_WEST, attacks, occupied);
    addRay(square, Direction::NORTH_WEST, attacks, occupied);

    return attacks;
}

void Attacks::initMagic(PieceType &&pieceType, Square &&square, Magic table[], uint64_t magic) {
    // Determine PieceType (Rook or Bishop) and store corresponding function to calculate its attacks
    std::function<uint64_t(Square &, uint64_t)> calculateAttacks = (pieceType.value() == PieceType::ROOK)
            ? calculateRook : calculateBishop;

    const uint64_t edges = ((Rank::RANK_1BB | Rank::RANK_8BB) & ~Rank::bitboard(square.rankIndex()) |
                            ((File::FILE_ABB | File::FILE_HBB) & ~File::bitboard(square.fileIndex())));

    Magic* entry = &table[square.index()];
    entry->magic = magic;

    uint64_t mask = calculateAttacks(square, 0ULL);
    mask &= ~edges;

    entry->mask = mask;
    entry->shift = 64 - Bits::popcount(mask);

    auto indices = Bits::getIndices(mask);
    int numBlockers = 1 << indices.size();

    // point pointer of the next square to current pointer + numBlockers
    if(square.index() + 1 < 64) {
        table[square.index() + 1].attacks = entry->attacks + numBlockers;
    }

    uint64_t blocker = 0ULL;

    // calculate all blocker bitboards and add them to 'attacks'
    for(int i = 0; i < numBlockers; i++) {
        for(int j = 0; j < indices.size(); j++) {
            int bit = (i >> j) & 1;
            blocker |= (uint64_t) bit << indices[j];
        }
        // calculate attacks with 'blocker' mask and add it to the 'attacks' magic table at the 'magic index'
        entry->attacks[table[square.index()].getIndex(blocker)] = calculateAttacks(square, blocker);
        blocker = 0ULL;
    }
}

uint64_t Attacks::pawnRight(const uint64_t &bb_pawns, const Color &color) {
    return color == Color::WHITE ? (bb_pawns << 9 & ~File::FILE_ABB) : (bb_pawns >> 9 & ~File::FILE_HBB);
}

uint64_t Attacks::pawnLeft(const uint64_t &bb_pawns, const Color &color) {
    return color == Color::WHITE ? (bb_pawns << 7 & ~File::FILE_HBB) : (bb_pawns >> 7 & ~File::FILE_ABB);
}

uint64_t Attacks::pawn(const Color &color, const uint8_t &index) {
    return Constants::PawnAttacks[color.value()][index];
}

uint64_t Attacks::knight(const uint8_t &index) {
    return Constants::KnightAttacks[index];
}

uint64_t Attacks::king(const uint8_t &index) {
    return Constants::KingAttacks[index];
}

uint64_t Attacks::rook(const uint8_t &index, const uint64_t &occupied) {
    return RookTable[index].attacks[RookTable[index].getIndex(occupied)];
}

uint64_t Attacks::bishop(const uint8_t &index, const uint64_t &occupied) {
    return BishopTable[index].attacks[BishopTable[index].getIndex(occupied)];
}

uint64_t Attacks::queen(const uint8_t &index, const uint64_t &occupied) {
    return rook(index, occupied) | bishop(index, occupied);
}

void Attacks::initMagics() {
    // Point the pointer 'attacks' to an array where the attacks are being stored
    RookTable[0].attacks = RookAttacks;
    BishopTable[0].attacks = BishopAttacks;

    for(int i = 0; i < 64; i++) {
        initMagic(PieceType::ROOK, Square(i), RookTable, Constants::RookMagics[i]);
        initMagic(PieceType::BISHOP, Square(i), BishopTable, Constants::BishopMagics[i]);
    }
}