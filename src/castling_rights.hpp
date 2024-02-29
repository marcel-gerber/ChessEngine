//
// Created by Marcel on 29.02.2024.
//

#ifndef CHESSENGINE_CASTLING_RIGHTS_HPP
#define CHESSENGINE_CASTLING_RIGHTS_HPP

#include <cstdint>

enum Castling : uint8_t {
    NO_CASTLING = 0,
    WHITE_00 = 0b00000001,
    WHITE_000 = 0b00000010,
    BLACK_00 = 0b00000100,
    BLACK_000 = 0b00001000
};

class CastlingRights {
private:
    uint8_t castling_rights;

public:
    CastlingRights() {
        castling_rights = Castling::NO_CASTLING;
    }

    [[nodiscard]] uint8_t getCastlingRights() const {
        return castling_rights;
    }

    void set(Castling &&castling) {
        castling_rights |= castling;
    }

    void unset(Castling &&castling) {
        castling_rights &= ~castling;
    }

    [[nodiscard]] bool has(Castling &&castling) const {
        return (castling_rights & castling) == castling;
    }
};

#endif //CHESSENGINE_CASTLING_RIGHTS_HPP
