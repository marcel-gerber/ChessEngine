# ChessEngine

This a basic bitboard UCI Chess Engine for learning purposes. So this is for 
beginners who might want to start with chess programming like me. I don't
plan on integrating new big features in the future (like several pruning 
techniques), because I want this project to stay basic. I may fork this project
in the future and start building a stronger engine.

## Acknowledgements

During development, I got heavily inspired by Disservins' [chess-library](https://github.com/Disservin/chess-library),
particularly in the design of the bitboard move generation. So I definitely want to give
credit to him.

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

## Usage

You will need a UCI-compatible Chess-GUI to play against this engine. My personal
preference is [en-croissant](https://github.com/franciscoBSalgueiro/en-croissant).

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
