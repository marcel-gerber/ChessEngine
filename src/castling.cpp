#include "castling.hpp"

Castling::Castling() {
    castling_rights =  static_cast<uint8_t>(NO_CASTLING);
}

[[nodiscard]] uint8_t Castling::raw() const {
    return castling_rights;
}

void Castling::set(const Value &castling) {
    castling_rights |= static_cast<uint8_t>(castling);
}

void Castling::unset(const Value &castling) {
    castling_rights &= ~static_cast<uint8_t>(castling);
}

void Castling::unset(const Color color) {
    switch(color.value()) {
        case static_cast<uint8_t>(Color::WHITE):
            unset(WHITE_00);
            unset(WHITE_000);
            return;
        case static_cast<uint8_t>(Color::BLACK):
            unset(BLACK_00);
            unset(BLACK_000);
            return;
        default:
            return;
    }
}

void Castling::reset() {
    castling_rights =  static_cast<uint8_t>(NO_CASTLING);
}

[[nodiscard]] bool Castling::has(const Value &castling) const {
    const auto value = static_cast<uint8_t>(castling);
    return (castling_rights & value) == value;
}

[[nodiscard]] bool Castling::has(const Color color) const {
    switch(color.value()) {
        case static_cast<uint8_t>(Color::WHITE):
            return has(WHITE_00) || has(WHITE_000);
        case static_cast<uint8_t>(Color::BLACK):
            return has(BLACK_00) || has(BLACK_000);
        default:
            return false;
    }
}

[[nodiscard]] bool Castling::hasNoCastling() const {
    return castling_rights == static_cast<uint8_t>(NO_CASTLING);
}