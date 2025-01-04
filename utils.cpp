#include "tetris.hpp"

void rawMode(bool start) {
    system("clear");
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (start) {
	system("printf '\\e[8;35;54t'"); // set terminal size
	std::cout << "\e[?25l"; // hide cursor
	tty.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
	tty.c_iflag &= ~(IXON);
	tty.c_oflag &= ~(OPOST);
    } else {
	tcflush(STDIN_FILENO, TCIFLUSH); // clear input buffer
	std::cout << "\e[?25h"; // show cursor
	tty.c_lflag |= (ICANON | ECHO | ISIG | IEXTEN);
	tty.c_iflag |= (IXON);
	tty.c_oflag |= (OPOST);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

std::string readFileToString(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printAtPosition(int x, int y, const std::string& color, const std::string& text) {
    std::cout << "\033[" << y << ";" << x << "H" << color << text << RESET << std::flush;
}

void emptyScreen() {
    for (int i = 1; i < 35; i++) {
	if (i == 1 || i == 34) {
	    for (int j = 1; j < 55; j++) {
		if (j == 1 || j == 54) {
		    printAtPosition(j, i, WHITE, ".");
		} else {
		    printAtPosition(j, i, WHITE, "_");
		}
	    }
	} else {
	    for (int j = 1; j < 55; j++) {
		if (j == 1 || j == 54) {
		    printAtPosition(j, i, WHITE, "|");
		} else {
		    printAtPosition(j, i, WHITE, " ");
		}
	    }
	}
    }
}

void printGameInterface() {
    for (int i = 0; i < 22; i++) {
	if (i == 0 || i == 21) {
	    printAtPosition(17, 7+i, WHITE, "+                    +");
	    if (i == 0) printAtPosition(18, 7, WHITE, "--------------------");
	    else printAtPosition(18, 28, WHITE, "====================");
	} else {
	    printAtPosition(17, 7+i, WHITE, "|                    |");
	}
    }
    printAtPosition(8, 8, WHITE, "NEXT");
    for (int i = 0; i <= 18; i++) {
	if (i == 0 || i == 18) printAtPosition(5, 9+i, WHITE, "+--------+");
	else if (i % 3 == 0) printAtPosition(5, 9+i, WHITE, "|--------|");
	else printAtPosition(5, 9+i, WHITE, "|        |");
    }
    printAtPosition(41, 14, WHITE, "SCORE:");
    printAtPosition(41, 15, WHITE, "0");
    printAtPosition(41, 17, WHITE, "LEVEL:");
    printAtPosition(41, 18, WHITE, "1");
    printAtPosition(41, 20, WHITE, "LINES:");
    printAtPosition(41, 21, WHITE, "0");
    printAtPosition(44, 8, WHITE, "HOLD");
    for (int i = 1; i <= 4; i++) {
	if (i == 1 || i == 4) printAtPosition(41, 8+i, WHITE, "+--------+");
	else printAtPosition(41, 8+i, WHITE, "| . . . .|");
    }
}

void printMessage(int code) {
    if (code == 0) {
	printAtPosition(20, 31, WHITE, "                ");
    } else if (code == 1) {
	printAtPosition(23, 31, WHITE, "Game Paused");
    } else if (code == 2) {
	printAtPosition(23, 31, WHITE, "Game Over");
    }
}
