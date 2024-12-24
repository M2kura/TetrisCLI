#include "tetris.hpp"

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
    printDisplayCorners();
    printDisplay(gd, true);
}
