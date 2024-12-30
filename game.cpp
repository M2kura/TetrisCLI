#include "tetris.hpp"

void Game::printDisplay(bool dots) {
    const char *background = dots ? " ." : "  ";
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            auto cord = gd.display[i+2][j];
            if (cord.type == "old") printAtPosition(18+(j*2), 8+i, cord.color, "[]");
            else printAtPosition(18+(j*2), 8+i, WHITE, background);
        }
    }
}

void Game::printTetromino() {
    for (auto& cords : gd.curTet) {
        if (cords.first > 1) printAtPosition(18+(cords.second*2), 6+cords.first, gd.color, "[]");
    }
}

void Game::addTetromino(char type) {
    gd.curTet.clear();
    switch (type) {
        case 'I':
            gd.curTet = {{1,3},{1,4},{1,5},{1,6}};
            gd.color = CYAN;
            break;
        case 'J':
            gd.curTet = {{1,3},{1,4},{1,5},{0,3}};
            gd.color = BLUE;
            break;
        case 'L':
            gd.curTet = {{1,3},{1,4},{1,5},{0,5}};
            gd.color = ORANGE;
            break;
        case 'O':
            gd.curTet = {{1,4},{1,5},{0,4},{0,5}};
            gd.color = YELLOW;
            break;
        case 'S':
            gd.curTet = {{1,3},{1,4},{0,4},{0,5}};
            gd.color = GREEN;
            break;
        case 'T':
            gd.curTet = {{1,3},{1,4},{1,5},{0,4}};
            gd.color = PURPLE;
            break;
        case 'Z':
            gd.curTet = {{1,4},{1,5},{0,4},{0,3}};
            gd.color = RED;
            break;
    }
}

void Game::checkTetris() {
    for (int i = 21; i > 1; i--) {
        for (int j = 0; j < 10; j++) {
            if (gd.display[i][j].type == "empty") break;
            if (j == 9) {
                for (int k = i; k > 1; k--) {
                    gd.display[k] = gd.display[k-1];
                }
            }
        }
    }
}

char Game::nextTetromino() {
    char piece = tetroQueue.front();
    tetroQueue.erase(tetroQueue.begin());
    if (tetroQueue.size() == 1) {
        std::vector<char> vec = newTetrominos();
        tetroQueue.insert(tetroQueue.end(), vec.begin(), vec.end());
    }
    return piece;
}

std::vector<char> Game::newTetrominos() {
    std::vector<char> vec = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);
    return vec;
}

void Game::placeTetromino() {
    for (auto& cords : gd.curTet) {
        gd.display[cords.first][cords.second].type = "old";
        gd.display[cords.first][cords.second].color = gd.color;
    }
    checkTetris();
    addTetromino(nextTetromino());
    printDisplay(true);
}

void Game::dropTetromino() {
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 || gd.display[cords.first+1][cords.second].type == "old") {
            if (checkEnd()) {
                printMessage(2);
                finished = true;
            } else placeTetromino();
            return;
        }
    }
    for (auto& cords : gd.curTet) {
        cords.first++;
    }
}

void Game::moveRight() {
    if (finished) return;
    for (auto& cords : gd.curTet) {
        if (cords.second == 9 ||
        gd.display[cords.first][cords.second+1].type == "old") return;
    }
    for (auto& cords : gd.curTet) cords.second++;
    printDisplay(true);
    printTetromino();
}

void Game::moveLeft() {
    if (finished) return;
    for (auto& cords : gd.curTet) {
        if (cords.second == 0 ||
        gd.display[cords.first][cords.second-1].type == "old") return;
    }
    for (auto& cords : gd.curTet) cords.second--;
    printDisplay(true);
    printTetromino();
}

void Game::moveDown() {
    if (finished) return;
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 ||
        gd.display[cords.first+1][cords.second].type == "old") return;
    }
    for (auto& cords : gd.curTet) cords.first++;
    printDisplay(true);
    printTetromino();
}

bool Game::checkEnd() {
    for (const auto& cords : gd.curTet) {
        if (cords.first == 1 && gd.display[2][cords.second].type == "old") {
            return true;
        }
    }
    return false;
}

void Game::update() {
    dropTetromino();
    if (finished) return;
    printDisplay(true);
    printTetromino();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Game::pause() { 
    printMessage(1);
    paused = true;
}

void Game::resume() { 
    printMessage(0);
    paused = false;
}
