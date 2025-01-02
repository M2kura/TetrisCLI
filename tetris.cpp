#include "tetris.hpp"

void Tetris::start() {
    rawMode(true);
    menu.open(home);
    std::thread inputThread(&Tetris::inputLoop, this);
    std::thread outputThread(&Tetris::outputLoop, this);
    std::thread gameThread(&Tetris::gameLoop, this);
    inputThread.join();
    outputThread.join();
    gameThread.join();
    rawMode(false);
}

void Tetris::quitGame() {
    if (game) {
        delete game;
        game = nullptr;
    }
    printMessage(0);
}

void Tetris::newGame() {
    if (game) delete game;
    game = new Game();
    game->countDown();
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
                if (read(STDIN_FILENO, &seq[0], 1) <= 0) {
                    inputQueue.push('q');
                    cv.notify_one();
                } else if (seq[0] != '[') {
                    int ok = 1;
                    while (ok == 1) ok = read(STDIN_FILENO, &seq[0], 1);
                    continue;
                } else {
                    read(STDIN_FILENO, &seq[1], 1);
                    if (seq[1] == 'A' || seq[1] == 'B' || seq[1] == 'C' || seq[1] == 'D') {
                        inputQueue.push(seq[1]);
                        cv.notify_one();
                    } else continue;
                }
            } else {
                inputQueue.push(input);
                cv.notify_one();
            }
        } else std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Tetris::outputLoop() {
    while (!quit) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !inputQueue.empty() || quit; });

        while(!inputQueue.empty()) {
            char input = inputQueue.front();
            inputQueue.pop();
            lock.unlock();
            if (menu.isOpen()) {
                if (input == '\n') menu.press("enter");
                else if (input == 'k' || input == 'A') menu.press("up");
                else if (input == 'j' || input == 'B') menu.press("down");
            } else if (!game->isFinished()) {
                if (input == 'q') {
                    game->pause();
                    menu.open(paused);
                } else if (input == 'C') game->moveRight();
                else if (input == 'D') game->moveLeft();
                else if (input == 'B') game->softDrop();
                else if (input == ' ') game->hardDrop();
                else if (input == 'A' || input == 'x') game->rotate();
            }
            lock.lock();
        }
    }
}

void Tetris::gameLoop() {
    while(!quit) {
        if (game) {
            if (!game->isPaused() && !game->isFinished()) game->update();
            else if(game->isFinished() && !menu.isOpen()) menu.open(gameover);
            else std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    Tetris tetris;
    tetris.start();
    return 0;
}
