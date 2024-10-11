# ChessEngine

A basic bitboard UCI Chess Engine for learning purposes.

## Acknowledgements

Lorem ipsum oder so

## Features

- Bitboard move generation
- Magic bitboards (fancy approach)
- Transposition Tables
- Alpha-Beta search
- Quiescence search
- Iterative Deepening
- Piece-Square Tables
- UCI support
- Time control
- Zobrist hashing

## Build from source

Configure CMake.

```bash
cmake -S . -B build
```

Build executable.

```bash
cmake --build build --target ChessEngine
```

## Running Tests

Configure CMake.

```bash
cmake -S . -B build
```

Build test-executable.

```bash
cmake --build build --target ChessEngineTests
```

Run tests.

```bash
cd build/test
./ChessEngineTests
```
