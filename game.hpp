struct gameDisplay {
    std::vector<std::vector<int>> display;
    gameDisplay(): display(20, std::vector<int>(10, 0)) {}
};

void printMenu() {
    printCorners();
    printAtPosition(3, 3, "*a) Start Game");
    printAtPosition(3, 4, " b) Options");
    printAtPosition(3, 5, " c) Quit");
}


void startGame() {
    gameDisplay display;
    printCorners();
    printDisplayCorners();
    // printDisplay(display);
    char input;
    while (true) {
        if (read(STDIN_FILENO, &input, 1) == 1) {
	    if (input == 'q') break;
	}
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    printMenu();
}

void tetris() {
    printMenu();
    char input;
    char key = 0;
    int curY = 3;
    while (true) {
        if (read(STDIN_FILENO, &input, 1) == 1) {
	    if (input == '\n' && curY == 3) startGame();
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
}
