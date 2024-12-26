#include "tetris.hpp"

void Menu::open(menuMode md) {
    toggle();
    setMode(md);
    printMenu();
}

void Menu::clearMenu() {
    printAtPosition(3, 3, "                            ");
    printAtPosition(3, 4, "                            ");
    printAtPosition(3, 5, "                            ");
}

void Menu::press(const std::string key) {
    if (key == "enter") {
        if (onLine() == 1) {
            if (mode != gameover) toggle();
            clearMenu();
            if (mode == paused) app->resumeGame();
            else if (mode == gameover) {
                printCorners();
                mode = home;
                printMenu();
            } else if (mode == home) {
                mode = paused;
                printDisplayCorners();
                app->newGame();
                app->startGame();
            }
        } else if (onLine() == 3) {
            if (mode == home) app->exit();
            else if (mode == paused) {
                mode = home;
                app->quitGame();
                printMenu();
            }
        }
    } else if (mode != gameover) {
        if (key == "down" && onLine() < 3) {
            printAtPosition(3, onLine()+2, " ");
            currentLine++;
            printAtPosition(3, onLine()+2, "*");
        } else if (key == "up" && onLine() > 1) {
            printAtPosition(3, onLine()+2, " ");
            currentLine--;
            printAtPosition(3, onLine()+2, "*");
        }
    }
}

void Menu::printMenu() {
    currentLine = 1;
    if (mode == home) {
        printCorners();
        printAtPosition(4, 3, "a) Start Game");
        printAtPosition(4, 5, "c) Quit Tetris");
    } else if (mode == paused) {
        printAtPosition(4, 3, "a) Resume Game");
        printAtPosition(4, 5, "c) Quit Game");
    } else if (mode == gameover) printAtPosition(4, 3, "() Back to Menu");
    if (mode == home || mode == paused) printAtPosition(4, 4, "b) Options");
    printAtPosition(3, currentLine+2, "*");
}
