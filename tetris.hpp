#ifndef TETRIS_HPP
# define TETRIS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <format>
# include <string>
# include <thread>
# include <chrono>
# include <termios.h>
# include <unistd.h>
# include <vector>

struct gameDisplay {
    std::vector<std::vector<int>> display;
    gameDisplay(): display(20, std::vector<int>(10, 0)) {}
};

class Tetris {
public:
    void start();
private:
};

class Game {
public:
    bool isPaused() {
        return paused;
    }

    bool isFinished() {
        return finished;
    }

    void startGame();

    Game(): gd() {};
private:
    bool paused = false;
    bool finished = false;
    gameDisplay gd;
};

// game.cpp
void printMenu();
void printDisplay(gameDisplay gd, bool dots);

// utils.cpp
void rawMode(bool start);
std::string readFileToString(const std::string& filePath);
void printAtPosition(int x, int y, const std::string& text);
void setArrowKey(char* key);
void printCorners();
void printDisplayCorners();

#endif
