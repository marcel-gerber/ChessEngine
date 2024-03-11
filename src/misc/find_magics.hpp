//
// Created by Marcel on 08.03.2024.
//

#ifndef CHESSENGINE_FIND_MAGICS_HPP
#define CHESSENGINE_FIND_MAGICS_HPP

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iomanip>

// Class for finding Magic Numbers for Rook and Bishop Magic Table
// Source Code by Tord Romstad
// https://www.chessprogramming.org/Looking_for_Magics#Feeding_in_Randoms
class FindMagics {

private:
    static uint64_t random_uint64_t() {
        uint64_t u1, u2, u3, u4;
        u1 = (uint64_t)(rand()) & 0xFFFF; u2 = (uint64_t)(rand()) & 0xFFFF;
        u3 = (uint64_t)(rand()) & 0xFFFF; u4 = (uint64_t)(rand()) & 0xFFFF;
        return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
    }

    static uint64_t random_uint64_t_fewbits() {
        return random_uint64_t() & random_uint64_t() & random_uint64_t();
    }

    static int count_1s(uint64_t b) {
        int r;
        for(r = 0; b; r++, b &= b - 1);
        return r;
    }

    constexpr static const int BitTable[64] = {
            63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
            51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
            26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
            58, 20, 37, 17, 36, 8
    };

    static int pop_1st_bit(uint64_t *bb) {
        uint64_t b = *bb ^ (*bb - 1);
        unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
        *bb &= (*bb - 1);
        return BitTable[(fold * 0x783a9b23) >> 26];
    }

    static uint64_t index_to_uint64_t(int index, int bits, uint64_t m) {
        int i, j;
        uint64_t result = 0ULL;
        for(i = 0; i < bits; i++) {
            j = pop_1st_bit(&m);
            if(index & (1 << i)) result |= (1ULL << j);
        }
        return result;
    }

    static uint64_t rmask(int sq) {
        uint64_t result = 0ULL;
        int rk = sq/8, fl = sq%8, r, f;
        for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
        for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
        for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
        for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
        return result;
    }

    static uint64_t bmask(int sq) {
        uint64_t result = 0ULL;
        int rk = sq/8, fl = sq%8, r, f;
        for(r=rk+1, f=fl+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
        for(r=rk+1, f=fl-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
        for(r=rk-1, f=fl+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
        for(r=rk-1, f=fl-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
        return result;
    }

    static uint64_t ratt(int sq, uint64_t block) {
        uint64_t result = 0ULL;
        int rk = sq/8, fl = sq%8, r, f;
        for(r = rk+1; r <= 7; r++) {
            result |= (1ULL << (fl + r*8));
            if(block & (1ULL << (fl + r*8))) break;
        }
        for(r = rk-1; r >= 0; r--) {
            result |= (1ULL << (fl + r*8));
            if(block & (1ULL << (fl + r*8))) break;
        }
        for(f = fl+1; f <= 7; f++) {
            result |= (1ULL << (f + rk*8));
            if(block & (1ULL << (f + rk*8))) break;
        }
        for(f = fl-1; f >= 0; f--) {
            result |= (1ULL << (f + rk*8));
            if(block & (1ULL << (f + rk*8))) break;
        }
        return result;
    }

    static uint64_t batt(int sq, uint64_t block) {
        uint64_t result = 0ULL;
        int rk = sq/8, fl = sq%8, r, f;
        for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
            result |= (1ULL << (f + r*8));
            if(block & (1ULL << (f + r * 8))) break;
        }
        return result;
    }


    static int transform(uint64_t b, uint64_t magic, int bits) {
        return (int)((b * magic) >> (64 - bits));
    }

    static uint64_t find_magic(int sq, int m, int bishop) {
        uint64_t mask, b[4096], a[4096], used[4096], magic;
        int i, j, k, n, fail;

        mask = bishop? bmask(sq) : rmask(sq);
        n = count_1s(mask);

        for(i = 0; i < (1 << n); i++) {
            b[i] = index_to_uint64_t(i, n, mask);
            a[i] = bishop? batt(sq, b[i]) : ratt(sq, b[i]);
        }
        for(k = 0; k < 100000000; k++) {
            magic = random_uint64_t_fewbits();
            if(count_1s((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
            for(i = 0; i < 4096; i++) used[i] = 0ULL;
            for(i = 0, fail = 0; !fail && i < (1 << n); i++) {
                j = transform(b[i], magic, m);
                if(used[j] == 0ULL) used[j] = a[i];
                else if(used[j] != a[i]) fail = 1;
            }
            if(!fail) return magic;
        }
        std::cout << "***Failed***" << std::endl;
        return 0ULL;
    }

    static constexpr int RBits[64] = {
            12, 11, 11, 11, 11, 11, 11, 12,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            12, 11, 11, 11, 11, 11, 11, 12
    };

    static constexpr int BBits[64] = {
            6, 5, 5, 5, 5, 5, 5, 6,
            5, 5, 5, 5, 5, 5, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5,
            6, 5, 5, 5, 5, 5, 5, 6
    };

public:
    static void run() {
        int square;

        std::cout << "static constexpr uint64_t RookMagics[64] = {" << std::endl;
        for(square = 0; square < 64; square++) {
            uint64_t magic = find_magic(square, RBits[square], 0);

            if(square % 4 == 0) {
                std::cout << "        ";
            }

            std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << magic << "ULL, ";

            if(square % 4 == 3) {
                std::cout << std::endl;
            }
        }
        std::cout << "};" << std::endl << std::endl;

        std::cout << "static constexpr uint64_t BishopMagics[64] = {" << std::endl;
        for(square = 0; square < 64; square++) {
            uint64_t magic = find_magic(square, BBits[square], 1);

            if(square % 4 == 0) {
                std::cout << "        ";
            }

            std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0') << magic << "ULL, ";

            if(square % 4 == 3) {
                std::cout << std::endl;
            }
        }
        std::cout << "};" << std::endl << std::endl;
    }

};

#endif
