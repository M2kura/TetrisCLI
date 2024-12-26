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

void printAtPosition(int x, int y, const std::string& text) {
    std::cout << "\033[" << y << ";" << x << "H" << text << std::flush;
}

void printCorners() {
    for (int i = 1; i < 35; i++) {
	if (i == 1 || i == 34) {
	    for (int j = 1; j < 55; j++) {
		if (j == 1 || j == 54) {
		    printAtPosition(j, i, ".");
		} else {
		    printAtPosition(j, i, "_");
		}
	    }
	} else {
	    for (int j = 1; j < 55; j++) {
		if (j == 1 || j == 54) {
		    printAtPosition(j, i, "|");
		} else {
		    printAtPosition(j, i, " ");
		}
	    }
	}
    }
}

void printDisplayCorners() {
    for (int i = 0; i < 22; i++) {
	if (i == 0 || i == 21) {
	    for (int j = 0; j < 22; j++) {
		if (j == 0 || j ==21) printAtPosition(17+j, 7+i, "+");
		else {
		    if (i == 0) printAtPosition(17+j, 7+i, "-");
		    else printAtPosition(17+j, 7+i, "=");
		}
	    }
	} else {
	    printAtPosition(17, 7+i, "|");
	    printAtPosition(38, 7+i, "|");
	}
    }
}

void printMessage(int code) {
    if (code == 0) {
	printAtPosition(20, 31, "                ");
    } else if (code == 1) {
	printAtPosition(23, 31, "Game Paused");
    } else if (code == 2) {
	printAtPosition(23, 31, "Game Over");
    }
}
