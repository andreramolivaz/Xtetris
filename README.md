# X-Tetris

A Tetris game in C language with rules from [Wikipedia](https://it.wikipedia.org/wiki/Tetris) and additional variations.

## Features

- Singleplayer mode
  - Initial 20 pieces of each type, called tetrominoes
  - One move consists of choosing the piece, where to drop it and its rotation
  - 10x15 playing field
  - Points based on number of rows cleared at once
  - Game ends when pieces run out or player fails to place a tetromino within the height limit
- Multiplayer mode
  - Two players take alternate turns each in their own playing field but drawing from the same set of tetrominoes
  - Twice as many pieces available 
  - Player vs. Player or Player vs. CPU
  - Player loses if they fail to place a piece correctly
  - In case of clearing 3 or more lines with a single move, opponent's field is modified by inverting the corresponding number of lines in the lower part     of the playing field
  - Player with highest score wins when pieces run out
<div align="center">
<img src="http://vdapoi.altervista.org/ezgif-3-4b13a6c0a4.gif" width="300" height="250" />
  <br>
  <em>image_caption</em>
</div>

## Getting Started

To play Xtetris, follow these steps:

1. Clone or download this repository
2. Compile the source code with a C compiler (i.e. XCode)
3. Run the program and choose the game mode from the initial menu

## Requirements

C compiler
termios library (for simulating gravity of the game, only available on Linux and macOS)

## Project Structure

    Xtetris               
    ├── main.c                   
    ├── tetris.c                  
    └── tetris.h

- `main.c` contains the main function and initial menu
- `Tetris.c` implements the game logic
- `Tetris.h` contains the declarations of functions used in Tetris.c

## Documentation

You can find the detailed documentation of the project at https://andreramolivaz.github.io/XTetris-documentation/.


