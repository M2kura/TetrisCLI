#include "tetris.hpp"

void printMenu() {
    printCorners();
    printAtPosition(4, 3, "a) Start Game");
    printAtPosition(4, 4, "b) Options");
    printAtPosition(4, 5, "c) Quit");
}

void printDisplay(gameDisplay gd, bool dots) {
    const char *background = dots ? " ." : "  ";
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (gd.display[i][j] == 1) {
                printAtPosition(18+(j*2), 8+i, "[]");
            } else {
                printAtPosition(18+(j*2), 8+i, background);
            }
        }
    }
}

void Game::startGame() {
    gd.display[0][0] = 1;
    gd.display[0][1] = 1;
    gd.display[0][2] = 1;
    gd.display[1][1] = 1;
    printCorners();
    printDisplayCorners();
    printDisplay(gd, true);
    char input;
    while (true) {
        if (read(STDIN_FILENO, &input, 1) == 1) {
            if (input == 'q') break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    printMenu();
    printAtPosition(3, 3, "*");
}
