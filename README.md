# ChessEngine

This a basic bitboard UCI chess engine for learning purposes. I started this 
project because I got interested in chess programming. It is intended for 
beginners who, like me, are looking to get started with chess programming. 
I don't plan on integrating any major new features (like several pruning
techniques), because I want to keep this project simple. In the future, 
I may fork this project and work on developing a stronger engine.

## Acknowledgements

During development, I got heavily inspired by Disservins' [chess-library](https://github.com/Disservin/chess-library),
particularly in the design of the bitboard move generation. So I definitely want to give
credit to him.

## Features

- Bitboard move generation
- Magic bitboards (fancy approach)
- Transposition Tables
- Piece-Square Tables
- Zobrist hashing
- Alpha-Beta search
- Quiescence search
- Iterative Deepening
- UCI support
- Time control

## Usage

You will need a UCI-compatible Chess-GUI to play against this engine. My personal
preference is [en-croissant](https://github.com/franciscoBSalgueiro/en-croissant).

## Build from source

Clone project.

````bash
git clone --recurse-submodules https://github.com/marcel-gerber/ChessEngine.git
cd ChessEngine/
````

Configure CMake.

```bash
cmake -S . -B build
```

Build executable.

```bash
cmake --build build --target ChessEngine
```

Executable will be in the ``build/src`` directory.

## Running Tests

After cloning the project and configuring CMake, build the test-executable.

```bash
cmake --build build --target ChessEngineTests
```

Run tests.

```bash
cd build/test
./ChessEngineTests
```
