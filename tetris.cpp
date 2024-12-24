#include "tetris.hpp"

void Menu::press(const std::string key) {
    if (key == "enter") {
        if (onLine() == 1) {
            toggle();
            printCorners();
            Game game;
            game.startGame();
        } else if (onLine() == 3) app->exit();
    } else if (key == "down" && onLine() < 3) {
        printAtPosition(3, onLine()+2, " ");
        currentLine++;
        printAtPosition(3, onLine()+2, "*");
    } else if (key == "up" && onLine() > 1) {
        printAtPosition(3, onLine()+2, " ");
        currentLine--;
        printAtPosition(3, onLine()+2, "*");
    }
}

void Menu::printMenu() {
    printCorners();
    printAtPosition(4, 3, "a) Start Game");
    printAtPosition(4, 4, "b) Options");
    printAtPosition(4, 5, "c) Quit");
    printAtPosition(3, 3, "*");
}

void Tetris::start() {
    rawMode(true);
    menu.open();
    std::thread inputThread(&Tetris::inputLoop, this);
    std::thread outputThread(&Tetris::outputLoop, this);
    inputThread.join();
    outputThread.join();
    rawMode(false);
}

void Tetris::inputLoop() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    char input;
    while (!quit) {
        if (read(STDIN_FILENO, &input, 1) == 1) {
            std::lock_guard<std::mutex> lock(mtx);
            if (input == '\e') {
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) == 0 || seq[0] != '[') continue;
                else {
                    read(STDIN_FILENO, &seq[1], 1);
                    if (seq[1] == 'A' || seq[1] == 'B') {
                        inputQueue.push(input);
                        inputQueue.push(seq[1]);
                        cv.notify_one();
                    } else continue;
                }
            } else {
                inputQueue.push(input);
                cv.notify_one();
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void Tetris::outputLoop() {
    while (!quit) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !inputQueue.empty() || quit; });

        while(!inputQueue.empty()) {
            char input = inputQueue.front();
            inputQueue.pop();
            char key = 0;
            if (input == '\e') {
                key = inputQueue.front();
                inputQueue.pop();
            }
            lock.unlock();
            if (menu.isOpen()) {
                if (input == '\n') menu.press("enter");
                else if (input == 'k' || key == 'A') menu.press("up");
                else if (input == 'j' || key == 'B') menu.press("down");
            } else if (input == 'q') menu.open();
            lock.lock();
        }
    }
}

int main() {
    Tetris tetris;
    tetris.start();
    return 0;
}
