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
# include <queue>
# include <mutex>
# include <condition_variable>
# include <fcntl.h>
# include <utility>
# include <random>
# include <algorithm>

enum square {
    empty,
    old,
    current
};

enum menuMode {
    home,
    settings,
    paused,
    gameover
};

class Tetris;

// menu.cpp
class Menu {
public:
    bool isOpen() { return !closed; }
    void toggle() { closed = !closed; }
    int onLine() { return currentLine; }
    void open(menuMode md);
    Menu(Tetris *tetris) {
        app = tetris;
    };

    void press(const std::string key);
private:
    menuMode mode = home;
    bool closed = true;
    int currentLine = 1;
    void printMenu();
    void clearMenu();
    void setMode(menuMode md) { mode = md; }
    Tetris *app = nullptr;
};

// game.cpp
class Game {
public:
    bool isPaused() {return paused;}
    bool isFinished() {return finished;}

    void start() { printDisplay(true); }
    void pause();
    void resume();
    void moveRight();
    void moveLeft();
    void moveDown();
    void update();

    Game(): tetroQueue(newTetrominos()) {
        gd.display = std::vector<std::vector<square>>(22, std::vector<square>(10, empty));
        gd.curTet = {};
        addTetromino(nextTetromino());
    };
private:
    struct GD {
        std::vector<std::vector<square>> display;
        std::vector<std::pair<int, int>> curTet;
    } gd;
    bool paused = false;
    bool finished = false;
    std::vector<char> tetroQueue;

    bool checkEnd();
    void addTetromino(char type);
    void dropTetromino();
    void printDisplay(bool dots);
    void printTetromino();
    void placeTetromino();
    void checkTetris();
    char nextTetromino();
    std::vector<char> newTetrominos();
};

// tetris.cpp
class Tetris {
public:
    void start();
    void exit() { quit = true; }
    void newGame();
    void quitGame();
    bool onPause() { return game && game->isPaused(); }
    void pauseGame() { game->pause(); }
    void resumeGame() { game->resume(); }
    void startGame() { game->start(); }
private:
    std::queue<char> inputQueue;
    std::mutex mtx;
    std::condition_variable cv;
    bool quit = false;
    Menu menu{this};
    Game *game = nullptr;

    void inputLoop();
    void outputLoop();
    void gameLoop();
};


// utils.cpp
void rawMode(bool start);
std::string readFileToString(const std::string& filePath);
void printAtPosition(int x, int y, const std::string& text);
void printCorners();
void printDisplayCorners();
void printMessage(int code);

#endif
