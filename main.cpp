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

std::string readFileToString(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void clearInputBuffer() {
    tcflush(STDIN_FILENO, TCIFLUSH);
}

void printAtPosition(int x, int y, const std::string& text) {
    // 1-based indexing
    std::cout << "\033[" << y << ";" << x << "H" << text << std::flush;
    std::cout << "\033[1;1H" << std::flush;
}

int main() {
    system("clear");
    std::cout << "\033[2J\033[H";
    disableInput();
    try {
        auto text = readFileToString("../test");
        printAtPosition(1, 2, text);
        std::cout << "\033[1;1H" << std::flush;
        char input;
        int curY = 4;
        while (true) {
            if (read(STDIN_FILENO, &input, 1) == 1) {
                if (input == 'q') break;
                if (input == 'j' && curY < 6) {
                    printAtPosition(3, curY, " ");
                    curY++;
                    printAtPosition(3, curY, "*");
                } else if (input == 'k' && curY > 4) {
                    printAtPosition(3, curY, " ");
                    curY--;
                    printAtPosition(3, curY, "*");
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        system("clear");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    clearInputBuffer();
    enableInput();
    return 0;
}
