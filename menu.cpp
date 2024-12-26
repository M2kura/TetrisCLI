#include "tetris.hpp"

void Menu::open(bool re) {
    toggle();
    printMenu(re);
}

void Menu::press(const std::string key) {
    if (key == "enter") {
        if (onLine() == 1) {
            toggle();
            printCorners();
            printDisplayCorners();
            if (app->onPause()) app->resumeGame();
            else {
                inGame = true;
                app->newGame();
                app->startGame();
            }
        } else if (onLine() == 3) {
            if (!inGame) app->exit();
            else {
                inGame = false;
                app->quitGame();
                printMenu(false);
            }
        }
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

void Menu::printMenu(bool re) {
    if (!re) {
        printCorners();
        printAtPosition(4, 3, "a) Start Game");
        printAtPosition(4, 5, "c) Quit Tetris");
    } else {
        printAtPosition(4, 3, "a) Resume Game");
        printAtPosition(4, 5, "c) Quit Game");
    }
    printAtPosition(4, 4, "b) Options");
    printAtPosition(3, currentLine+2, "*");
}
