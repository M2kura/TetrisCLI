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
# include <cmath>

#define RESET   "\033[0m"
#define RED     "\033[38;5;196m"
#define GREEN   "\033[38;5;46m"
#define YELLOW  "\033[38;5;226m"
#define BLUE    "\033[38;5;21m"
#define PURPLE  "\033[38;5;201m"
#define CYAN    "\033[38;5;51m"
#define ORANGE  "\033[38;5;208m"
#define WHITE   "\033[38;5;15m"
#define GRAY    "\033[38;5;250m"

enum menuMode {
    home,
    settings,
    paused,
    gameover
};

struct square {
    bool old;
    std::string color;
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

    void pause();
    void resume();
    void countDown();
    void moveRight();
    void moveLeft();
    void softDrop();
    void hardDrop();
    void rotate(bool right);
    void hold();
    void update();

    Game(): tetroQueue(newTetrominos()) {
        gd.display = std::vector<std::vector<square>>(22, std::vector<square>(10, {false, ""}));
        gd.curTet = {};
        addTetromino(nextTetromino());
        printDisplay(true);
        printNext();
    };
private:
    struct GD {
        std::vector<std::vector<square>> display;
        std::vector<std::pair<int, int>> curTet;
        std::string color;
        char type;
        int stage;
        char hold = ' ';
        bool canHold = true;
        int highest = 22;
    } gd;
    bool paused = true;
    bool finished = false;
    std::vector<char> tetroQueue;
    int score = 0;
    int level = 1;
    int combo = -1;
    int lines = 0;
    std::vector<std::string> moves = {};
    std::mutex displayMutex;

    bool checkEnd();
    void addTetromino(char type);
    bool dropTetromino();
    void printDisplay(bool dots);
    void printNext();
    void printHold();
    void printTetromino();
    void placeTetromino();
    bool touchGround();
    void checkClear();
    void checkPerfectClear(int cleared);
    void printScore();
    void printLines();
    void printLevel();
    int countTime();
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
void printAtPosition(int x, int y, const std::string& color, const std::string& text);
void emptyScreen();
void printGameInterface();
void printMessage(int code);

#endif
