# TetrisCLI

## How to start
1. Clone the repo locally
2. Create a build folder inside the repo
3. Inside build folder run `cmake ..`, then `make`
4. Run the executable `./tetris` or `./tetris --help` to check the commad manual listed below

## Programm controls
### Menu controls:
**j, arrow down** - move cursor to the line below  
**k, arrow up**   - move cursor to the line above   
**enter**         - choose current line option

### Game controls:
**arrow down**  - soft drop the piece   
**space**       - hard drop the piece   
**arrow right** - move the piece to the right   
**arrow left**  - move the piece to the left    
**arrow up, x** - rotate the piece clockwise    
**enter, z**    - rotate the piece counter clockwise    
**c**           - hold piece    
**q**           - pause the game    

### How to play
In the main menu, to start a game press Start game. To open settings press Options. To quit tetris press Quit Tetris.  
During the game you can press q to pause the current game, the menu will open.  
To unpause a game press Resume game. To open settings press Options.To finish game and get back to the main menu press Quit Game.  
If the game is over, press Back to menu to get back to the main menu.  

## Project implementation
### Files and their meaning:
* CMakeList - used for building the app
* tetris.hpp - main header file that includes all the libraries, definition of global variable, structs, funtions and classes
* tetris.cpp - implements the Tetris class and have the main class that start the programm
* game.cpp - implements the Game class
* rotations.cpp - implements rotation functions from Game class
* menu.cpp - implements the Menu class
* utils.cpp - implements helper functions
* data - stores the programm settings

### Clasess
---
**Tetris** - main class of the programm. Have 3 running threads - input thread, output thread and game thread.  
Input thread collect the input from the the user and pass it to output thread.  
Output thread evaluates the input and performes an action, depending on the programm state.   
Game thread is responsible for constantly updating the game instance.   

**Game** - this class represents an instanse of unique game session.  
It has the printing functions for updating the game display after change (printDisplay, printTetromino, printScore, printHold...)   
The computing fuction (canRotate, checkClear, touchGroud...) for checking if some conditions are  
Function for inplementing game loop (countTime, dropTetromino, placeTetromino, nextTetromino...)  
Piece controlling functions (rotate*, tryKick, hold...)  
And some other helper functions  
Game class has a struct GD that represent the display matrix and a current piece

**Menu** - this class represents the menu. It has 4 states - home, settings, paused, gameover.  
Home is for main menu screen  
Settings if for changing the game options  
Paused is for options available when the game is paused
Gameover gives a way to come back to the home page after game over

### Fitures of the game
* Colored output
* Ghost piece
* [SRS](https://harddrop.com/wiki/SRS)
* Next pieces preview
* Piece hold
* Score system
* Settings
