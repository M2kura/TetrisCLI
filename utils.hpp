#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>

void disableInput() {
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void enableInput() {
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void clearInputBuffer() {
    tcflush(STDIN_FILENO, TCIFLUSH);
}

// Function to hide the cursor
void hideCursor() {
    std::cout << "\e[?25l";
}

// Function to show the cursor
void showCursor() {
    std::cout << "\e[?25h";
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
    // 1-based indexing
    std::cout << "\033[" << y << ";" << x << "H" << text << std::flush;
    std::cout << "\033[1;1H" << std::flush;
}

void setArrowKey(char* key) {
	char seq[2];
	if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1 && seq[0] == '[') {
		*key = seq[1]; 
	}
}
