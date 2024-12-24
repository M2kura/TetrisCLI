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

struct gameDisplay {
    std::vector<std::vector<int>> display;
    gameDisplay(): display(20, std::vector<int>(10, 0)) {}
};

class Tetris;

class Menu {
public:
    bool isOpen() { return !closed; }
    void toggle() { closed = !closed; }
    int onLine() { return currentLine; }
    void open() {
        toggle();
        printMenu();
    }
    Menu(Tetris *tetris) {
        app = tetris;
    };

    void press(const std::string key);
private:
    bool closed = true;
    int currentLine = 1;
    void printMenu();
    Tetris *app;
};

class Tetris {
public:
    void start();
    void exit() { quit = true; }
private:
    std::queue<char> inputQueue;
    std::mutex mtx;
    std::condition_variable cv;
    bool quit = false;
    Menu menu{this};

    void inputLoop();
    void outputLoop();
};

// game.cpp
class Game {
public:
    bool isPaused() {return paused;}
    bool isFinished() {return finished;}

    void startGame();

    Game(): gd() {};
private:
    std::queue<std::string> inputQueue;
    bool paused = false;
    bool finished = false;
    gameDisplay gd;
};
void printDisplay(gameDisplay gd, bool dots);

// utils.cpp
void rawMode(bool start);
std::string readFileToString(const std::string& filePath);
void printAtPosition(int x, int y, const std::string& text);
void setArrowKey(char* key);
void printCorners();
void printDisplayCorners();

#endif
