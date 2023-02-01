# Xtetris

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


## Getting Started

To play Xtetris, follow these steps:

1. Clone or download this repository
2. Compile the source code with a C compiler
3. Run the program and choose the game mode from the initial menu



<div align="center">
 <table>
   <tr>
<td><img src="http://vdapoi.altervista.org/ezgif-3-4b13a6c0a4.gif" width="250" height="200" /><br>
  <em>singleplayer</em></td> 
    <td><img src="http://vdapoi.altervista.org/ezgif.com-gif-maker-2.gif" width="250" height="200" /><br>
  <em>multiplayer(player1 vs player2)</em></td> 
      <td><img src="http://vdapoi.altervista.org/ezgif.com-gif-maker-5.gif" width="250" height="200" /><br>
  <em>multiplayer(player1 vs CPU)</em></td> 
   </tr>
  </table>
</div>


## Requirements

- `C compiler`
- `#include <stdio.h>` for input/output operations
- `#include <stdlib.h>` for standard library functions
- `#include <termios.h>` useful to change settings for terminal env
- `#include <time.h>` for time-related functions
- `#include <fcntl.h>` for file control options
- `#include <AudioToolbox/AudioServices.h>` (only available on macOS) for audio services sound for the blocks gravity ([look at the code](https://github.com/andreramolivaz/Xtetris/blob/149726a35198c259936bb1c76ccc528d61d4548a/tetris.c#L693))

## Project Structure

    Xtetris               
    ├── main.c                   
    ├── tetris.c                  
    └── tetris.h

- `main.c` contains the main function and initial menu
- `Tetris.c` implements the game logic
- `Tetris.h` contains the declarations of functions used in Tetris.c

## Documentation

You can find the detailed documentation of the project [here]( https://andreramolivaz.github.io/XTetris-documentation/).


