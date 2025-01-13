#include "tetris.hpp"

void Menu::open(menuMode md) {
    toggle();
    setMode(md);
    printMenu();
}

void Menu::clearMenu() {
    printAtPosition(3, 3, WHITE, "                            ");
    printAtPosition(3, 4, WHITE, "                            ");
    printAtPosition(3, 5, WHITE, "                            ");
}

void Menu::press(const std::string key) {
    if (key == "enter") {
        if (onLine() == 1) {
            clearMenu();
            if (mode != gameover && mode != settings) toggle();
            if (mode == paused) app->resumeGame();
            if (mode == gameover) emptyScreen();
            if (mode == settings && app->onPause()) {
                mode = paused;
                printMenu();
            } else if (mode == settings || mode == gameover) {
                mode = home;
                printMenu();
            } else if (mode == home) {
                mode = paused;
                printGameInterface();
                app->newGame();
            }
        } else if (onLine() == 2) {
            if (mode == home || mode == paused) {
                clearMenu();
                mode = settings;
                printMenu();
            } else app->toggleDots();
        } else {
            if (mode == home) app->exit();
            else if (mode == paused) {
                mode = home;
                app->quitGame();
                printMenu();
            } else app->toggleGhost();
        }
    } else if (mode != gameover) {
        if (key == "down" && onLine() < 3) {
            printAtPosition(3, onLine()+2, WHITE, " ");
            currentLine++;
            printAtPosition(3, onLine()+2, WHITE, "*");
        } else if (key == "up" && onLine() > 1) {
            printAtPosition(3, onLine()+2, WHITE, " ");
            currentLine--;
            printAtPosition(3, onLine()+2, WHITE, "*");
        }
    }
}

void Menu::printMenu() {
    currentLine = 1;
    if (mode == home) {
        emptyScreen();
        printAtPosition(4, 3, WHITE, "a) Start Game");
        printAtPosition(4, 5, WHITE, "c) Quit Tetris");
    } else if (mode == paused) {
        printAtPosition(4, 3, WHITE, "a) Resume Game");
        printAtPosition(4, 5, WHITE, "c) Quit Game");
    } else if (mode == settings) {
        printAtPosition(4, 3, WHITE, "a) Back");
        printAtPosition(4, 4, WHITE, ":> Dotted background");
        printAtPosition(4, 5, WHITE, ":> Ghost piece");
        if (app->dotsOn()) printAtPosition(27, 4, WHITE, "on ");
        else printAtPosition(27, 4, WHITE, "off");
        if (app->ghostOn()) printAtPosition(27, 5, WHITE, "on ");
        else printAtPosition(27, 5, WHITE, "off");
    } else if (mode == gameover) printAtPosition(4, 3, WHITE, "a) Back to Menu");
    if (mode == home || mode == paused) printAtPosition(4, 4, WHITE, "b) Options");
    printAtPosition(3, 3, WHITE, "*");
}
