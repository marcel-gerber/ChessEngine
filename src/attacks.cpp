#include "attacks.hpp"
#include "utils/bits.hpp"
#include "constants.hpp"

uint64_t Attacks::calculateRookAttacks(Square &square, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    Square current{};

    // North-Direction
    current = Square(square.getIndex() + 8);
    while(current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() + 8);
    }

    // East-Direction
    current = Square(square.getIndex() + 1);
    while(current.getFileIndex() != 0 && current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() + 1);
    }

    // South-Direction
    current = Square(square.getIndex() - 8);
    while(current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() - 8);
    }

    // West-Direction
    current = Square(square.getIndex() - 1);
    while(current.getFileIndex() != 7 && current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() - 1);
    }

    return attacks;
}

/// Function used for initializing Magic Table
uint64_t Attacks::calculateBishopAttacks(Square &square, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    Square current{};

    // North-East-Direction
    current = Square(square.getIndex() + 9);
    while(current.getFileIndex() != 0 && current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() + 9);
    }

    // South-East-Direction
    current = Square(square.getIndex() - 7);
    while(current.getFileIndex() != 0 && current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() - 7);
    }

    // South-West-Direction
    current = Square(square.getIndex() - 9);
    while(current.getFileIndex() != 7 && current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() - 9);
    }

    // North-West-Direction
    current = Square(square.getIndex() + 7);
    while(current.getFileIndex() != 7 && current.isValid()) {
        attacks |= 1ULL << current.getIndex();
        if(Bits::isSet(occupied, current.getIndex())) break;
        current = Square(current.getIndex() + 7);
    }

    return attacks;
}

void Attacks::initMagic(PieceType &&pieceType, Square &&square, Magic table[], uint64_t magic) {
    const uint64_t edges = ((Rank::RANK_1BB | Rank::RANK_8BB) & ~Rank::getBitboard(square.getRankIndex()) |
                            ((File::FILE_ABB | File::FILE_HBB) & ~File::getBitboard(square.getFileIndex())));

    Magic* entry = &table[square.getIndex()];
    entry->magic = magic;

    uint64_t mask = (pieceType.getValue() == PieceType::ROOK
                     ? calculateRookAttacks(square, 0ULL) : calculateBishopAttacks(square, 0ULL));
    mask &= ~edges;

    entry->mask = mask;
    entry->shift = 64 - Bits::popcount(mask);

    auto indices = Bits::getIndices(mask);
    int numBlockers = 1 << indices.size();

    // point pointer of the next square to current pointer + numBlockers
    if(square.getIndex() + 1 < 64) {
        table[square.getIndex() + 1].attacks = entry->attacks + numBlockers;
    }

    uint64_t blocker = 0ULL;

    // calculate all blocker bitboards and add them to 'attacks'
    for(int i = 0; i < numBlockers; i++) {
        for(int j = 0; j < indices.size(); j++) {
            int bit = (i >> j) & 1;
            blocker |= (uint64_t) bit << indices[j];
        }
        // calculate attack with 'blocker' mask and add it to the 'attacks' magic table at the 'magic index'
        entry->attacks[table[square.getIndex()].getIndex(blocker)] = (pieceType.getValue() == PieceType::ROOK
                                                                      ? calculateRookAttacks(square, blocker) : calculateBishopAttacks(square, blocker));
        blocker = 0ULL;
    }
}

uint64_t Attacks::getPawnRightAttacks(const uint64_t &bb_pawns, const Color &color) {
    return color == Color::WHITE ? (bb_pawns << 9 & ~File::FILE_ABB) : (bb_pawns >> 9 & ~File::FILE_HBB);
}

uint64_t Attacks::getPawnLeftAttacks(const uint64_t &bb_pawns, const Color &color) {
    return color == Color::WHITE ? (bb_pawns << 7 & ~File::FILE_HBB) : (bb_pawns >> 7 & ~File::FILE_ABB);
}

uint64_t Attacks::getPawnAttacks(const Color &color, const uint8_t &index) {
    return Constants::PawnAttacks[color.getValue()][index];
}

uint64_t Attacks::getKnightAttacks(const uint8_t &index) {
    return Constants::KnightAttacks[index];
}

uint64_t Attacks::getKingAttacks(const uint8_t &index) {
    return Constants::KingAttacks[index];
}

uint64_t Attacks::getRookAttacks(const uint8_t &index, const uint64_t &occupied) {
    return RookTable[index].attacks[RookTable[index].getIndex(occupied)];
}

uint64_t Attacks::getBishopAttacks(const uint8_t &index, const uint64_t &occupied) {
    return BishopTable[index].attacks[BishopTable[index].getIndex(occupied)];
}

uint64_t Attacks::getQueenAttacks(const uint8_t &index, const uint64_t &occupied) {
    return getRookAttacks(index, occupied) | getBishopAttacks(index, occupied);
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