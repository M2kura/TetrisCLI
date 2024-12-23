#include "tetris.hpp"

void Tetris::start() {
    rawMode(true);
    printMenu();
    char input;
    char key = 0;
    int curY = 3;
    printAtPosition(3, 3, "*");
    while (true) {
        if (read(STDIN_FILENO, &input, 1) == 1) {
            if (input == '\n' && curY == 3) {
                Game game;
                game.startGame();
            }
            if (input == '\n' && curY == 5) break;
            if (input == '\e') setArrowKey(&key);
            if ((input == 'j' || key == 'B') && curY < 5) {
                printAtPosition(3, curY, " ");
                curY++;
                printAtPosition(3, curY, "*");
            } else if ((input == 'k' || key == 'A') && curY > 3) {
                printAtPosition(3, curY, " ");
                curY--;
                printAtPosition(3, curY, "*");
            }
            key = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    rawMode(false);
}

int main() {
    Tetris tetris;
    tetris.start();
    return 0;
}
