#include "gtest/gtest.h"
#include "../src/perft.hpp"
#include "../src/fen_parser.hpp"

class PerftTest : public testing::Test {
protected:
    PerftTest() {
        Attacks::initMagics();
        MoveGen::initSquaresBetween();
    }
};

struct PerftResult {
    std::string fen;
    uint64_t expected_nodes;
    uint8_t depth;
};

// https://www.chessprogramming.org/Perft_Results
TEST_F(PerftTest, StandardPositions) {
    const PerftResult perft_results[] = {
            {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 119060324, 6},
            {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 193690690, 5},
            {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 178633661, 7},
            {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 15833292, 5},
            {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 89941194, 5},
            {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 1", 164075551, 5}
    };

    for(const auto &perft_result : perft_results) {
        Board board = Board();
        Fen::setFen(board, perft_result.fen);

        Perft perft(board);
        const uint64_t nodes = perft.nodes(perft_result.depth);

        ASSERT_EQ(nodes, perft_result.expected_nodes) << "Perft test failed. " << std::endl
                                                      << "FEN: " << perft_result.fen << std::endl
                                                      << "Depth: " << perft_result.depth << std::endl
                                                      << "Expected Nodes: " << perft_result.expected_nodes << std::endl
                                                      << "Got: " << nodes << std::endl;
    }
}

// https://www.chessprogramming.net/perfect-perft/
TEST_F(PerftTest, AdvancedPositions) {
    const PerftResult perft_results[] = {
            {"3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 1134888, 6},
            {"8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1", 1015133, 6},
            {"8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 1440467, 6},
            {"5k2/8/8/8/8/8/8/4K2R w K - 0 1", 661072, 6},
            {"3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", 803711, 6},
            {"r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 1274206, 4},
            {"r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 1720476, 4},
            {"2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", 3821001, 6},
            {"8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", 1004658, 5},
            {"4k3/1P6/8/8/8/8/K7/8 w - - 0 1", 217342, 6},
            {"8/P1k5/K7/8/8/8/8/8 w - - 0 1", 92683, 6},
            {"K1k5/8/P7/8/8/8/8/8 w - - 0 1", 2217, 6},
            {"8/k1P5/8/1K6/8/8/8/8 w - - 0 1", 567584, 7},
            {"8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 23527, 4}
    };

    for(const auto &perft_result : perft_results) {
        Board board = Board();
        Fen::setFen(board, perft_result.fen);

        Perft perft(board);
        const uint64_t nodes = perft.nodes(perft_result.depth);

        ASSERT_EQ(nodes, perft_result.expected_nodes) << "Perft test failed. " << std::endl
                                                      << "FEN: " << perft_result.fen << std::endl
                                                      << "Depth: " << perft_result.depth << std::endl
                                                      << "Expected Nodes: " << perft_result.expected_nodes << std::endl
                                                      << "Got: " << nodes << std::endl;
    }

}