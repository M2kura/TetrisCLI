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

void Tetris::printHelp() {
    std::cout << "Menu controls:" << std::endl;
    std::cout << "j, arrow down - move cursor to the line below" << std::endl;
    std::cout << "k, arrow up   - move cursor to the line above" << std::endl;
    std::cout << "enter         - choose current line option\n" << std::endl;
    std::cout << "Game controls:" << std::endl;
    std::cout << "arrow down    - soft drop the piece" << std::endl;
    std::cout << "space         - hard drop the piece" << std::endl;
    std::cout << "arrow right   - move the piece to the right" << std::endl;
    std::cout << "arrow left    - move the piece to the left" << std::endl;
    std::cout << "arrow up, x   - rotate the piece clockwise" << std::endl;
    std::cout << "enter, z      - rotate the piece counter clockwise" << std::endl;
    std::cout << "c             - hold piece" << std::endl;
    std::cout << "q             - pause the game\n" << std::endl;
    std::cout << "In the main menu, to start a game press Start game." << std::endl;
    std::cout << "To open settings press Options." << std::endl;
    std::cout << "To quit tetris press Quit Tetris.\n" << std::endl;
    std::cout << "During the game you can press q to pause the current\ngame, the menu will open." << std::endl;
    std::cout << "To unpause a game press Resume game." << std::endl;
    std::cout << "To open settings press Options." << std::endl;
    std::cout << "To finish game and get back to the main menu press\nQuit Game.\n" << std::endl;
    std::cout << "If the game is over, press Back to menu to get back\nto the main menu." << std::endl;
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
                else if (input == 'c') game->hold();
                else if (input == 'A' || input == 'x') game->rotate(true);
                else if (input == '\n' || input == 'z') game->rotate(false);
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


int main(int argc, char* argv[]) {
    Tetris tetris;
    if (argc == 1) tetris.start();
    else if (argc == 2 && std::string(argv[1]) == "--help") tetris.printHelp();
    else {
        std::cout << "Invalid arguments" << std::endl;
        std::cout << "Run a programm with no arguments to start Tetris" << std::endl;
        std::cout << "Run a programm with --help argument to read the help manual" << std::endl;
    }
    return 0;
}
