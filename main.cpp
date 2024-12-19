#include "utils.hpp"

int main() {
    system("clear");
    system("printf '\\e[8;35;54t'");
    hideCursor();
    disableInput();
    try {
        auto text = readFileToString("../test");
        printAtPosition(1, 2, text);
        std::cout << "\033[1;1H" << std::flush;
        char input;
        char key = 0;
        int curY = 3;
        while (true) {
            if (read(STDIN_FILENO, &input, 1) == 1) {
                if (input == 'q') break;
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
        system("clear");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    clearInputBuffer();
    enableInput();
    showCursor();
    return 0;
}
