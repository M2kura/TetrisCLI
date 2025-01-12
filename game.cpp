#include "tetris.hpp"

void Game::setOptions(options set) {
    settings.dots = set.dots;
    settings.ghost = set.ghost;
}

void Game::printDisplay() {
    const char *background = settings.dots ? " ." : "  ";
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            auto cord = gd.display[i+2][j];
            if (cord.old) printAtPosition(18+(j*2), 8+i, cord.color, "[]");
            else printAtPosition(18+(j*2), 8+i, WHITE, background);
        }
    }
}

void Game::printScore() {
    printAtPosition(41, 15, WHITE, std::to_string(score));
}

void Game::printLines() {
    printAtPosition(41, 21, WHITE, std::to_string(lines));
}

void Game::printLevel() {
    printAtPosition(41, 18, WHITE, std::to_string(level));
}

void Game::printTetromino() {
    for (auto& cords : gd.curTet) {
        if (cords.first > 1) printAtPosition(18+(cords.second*2), 6+cords.first, gd.color, "[]");
    }
    if (!touchGround() && settings.ghost) {
        auto ghost = gd.curTet;
        bool touch = false;
        while (!touch) {
            for (auto& cords : ghost) cords.first++;
            for (auto& cords : ghost) {
                if (cords.first == 21 ||
                gd.display[cords.first+1][cords.second].old) touch = true;
            }
        }
        for (auto& cords : ghost) {
            bool print = true;
            for (auto& cordsCur : gd.curTet) {
                if (cords.first == cordsCur.first && cords.second == cordsCur.second) {
                    print = false;
                    break;
                }
            }
            if (print) printAtPosition(18+(cords.second*2), 6+cords.first, GRAY, "88");
        }
    }
}

void Game::printNext() {
    const char *background = settings.dots ? " . . . ." : "        ";
    for (int i = 1; i < 18; i++) if (i % 3 != 0) printAtPosition(6, 9+i, WHITE, background);
    for (int i = 0; i < 6; i++) {
        char piece = tetroQueue[i];
        if (piece == 'I') printAtPosition(6, 11+3*i, CYAN, "[][][][]");
        else if (piece == 'J') {
            printAtPosition(6, 10+3*i, BLUE, "[]");
            printAtPosition(6, 11+3*i, BLUE, "[][][]");
        } else if (piece == 'L') {
            printAtPosition(10, 10+3*i, ORANGE, "[]");
            printAtPosition(6, 11+3*i, ORANGE, "[][][]");
        } else if (piece == 'O') {
            printAtPosition(8, 10+3*i, YELLOW, "[][]");
            printAtPosition(8, 11+3*i, YELLOW, "[][]");
        } else if (piece == 'S') {
            printAtPosition(8, 10+3*i, GREEN, "[][]");
            printAtPosition(6, 11+3*i, GREEN, "[][]");
        } else if (piece == 'Z') {
            printAtPosition(6, 10+3*i, RED, "[][]");
            printAtPosition(8, 11+3*i, RED, "[][]");
        } else if (piece == 'T') {
            printAtPosition(8, 10+3*i, PURPLE, "[]");
            printAtPosition(6, 11+3*i, PURPLE, "[][][]");
        }
    }
}

void Game::printHold() {
    const char *background = settings.dots ? " . . . ." : "        ";
    for (int i = 0; i < 2; i++) printAtPosition(42, 10+i, WHITE, background);
    if (gd.hold == 'I') printAtPosition(42, 11, CYAN, "[][][][]");
    else if (gd.hold == 'J') {
        printAtPosition(42, 10, BLUE, "[]");
        printAtPosition(42, 11, BLUE, "[][][]");
    } else if (gd.hold == 'L') {
        printAtPosition(46, 10, ORANGE, "[]");
        printAtPosition(42, 11, ORANGE, "[][][]");
    } else if (gd.hold == 'O') {
        printAtPosition(44, 10, YELLOW, "[][]");
        printAtPosition(44, 11, YELLOW, "[][]");
    } else if (gd.hold == 'S') {
        printAtPosition(44, 10, GREEN, "[][]");
        printAtPosition(42, 11, GREEN, "[][]");
    } else if (gd.hold == 'Z') {
        printAtPosition(42, 10, RED, "[][]");
        printAtPosition(44, 11, RED, "[][]");
    } else if (gd.hold == 'T') {
        printAtPosition(44, 10, PURPLE, "[]");
        printAtPosition(42, 11, PURPLE, "[][][]");
    }
}

void Game::countDown() {
    printDisplay();
    printTetromino();
    printNext();
    printHold();
    printAtPosition(27, 6, WHITE, "03");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printAtPosition(27, 6, WHITE, "02");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printAtPosition(27, 6, WHITE, "01");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printAtPosition(27, 6, WHITE, "  ");
    paused = false;
}

void Game::addTetromino(char type) {
    gd.type = type;
    gd.stage = 1;
    switch (type) {
        case 'I':
            gd.curTet = {{1,3},{1,4},{1,5},{1,6}};
            gd.color = CYAN;
            break;
        case 'J':
            gd.curTet = {{0,3},{1,3},{1,4},{1,5}};
            gd.color = BLUE;
            break;
        case 'L':
            gd.curTet = {{0,5},{1,5},{1,4},{1,3}};
            gd.color = ORANGE;
            break;
        case 'O':
            gd.curTet = {{1,4},{1,5},{0,4},{0,5}};
            gd.color = YELLOW;
            break;
        case 'S':
            gd.curTet = {{0,5},{0,4},{1,4},{1,3}};
            gd.color = GREEN;
            break;
        case 'T':
            gd.curTet = {{0,4},{1,3},{1,4},{1,5}};
            gd.color = PURPLE;
            break;
        case 'Z':
            gd.curTet = {{0,3},{0,4},{1,4},{1,5}};
            gd.color = RED;
            break;
    }
}

void Game::checkClear() {
    std::vector<int> rows = {};
    for (int i = 21; i >= gd.highest; i--) {
        for (int j = 0; j < 10; j++) {
            if (!gd.display[i][j].old) break;
            if (j == 9) {
                rows.push_back(i);
                for (int k = i; k < 9; k++) {
                    gd.display[i][k].old = false;
                    gd.display[i][k].color = "";
                }
            }
        }
    }
    if (rows.size() != 0) {
        int cleared = rows.size();
        lines += cleared;
        if (cleared == 1) score += 100 * level;
        else if (cleared == 2) score += 300 * level;
        else if (cleared == 3) score += 500 * level;
        else score += 800 * level;
        if ((lines/10) == level) {
            level++;
            printLevel();
        }
        while (rows.size() != 0) {
            int row = rows.back();
            rows.pop_back();
            for (int i = row; i >= gd.highest; i--) {
                gd.display[i] = gd.display[i-1];
            }
            gd.highest++;
        }
        checkPerfectClear(cleared);
        printScore();
        printLines();
    }
}

void Game::checkPerfectClear(int cleared) {
    for (int i = 18; i <= 21; i++) {
        for (int j = 0; j <= 9; j++) {
            if (gd.display[i][j].old) return;
            if (i == 21 && j == 9) {
                if (cleared == 1) score += 800 * level;
                else if (cleared == 2) score += 1200 * level;
                else if (cleared == 3) score += 1800 * level;
                else score += 2000 * level;
            }
        }
    }
}

char Game::nextTetromino() {
    char piece = tetroQueue.front();
    tetroQueue.erase(tetroQueue.begin());
    if (tetroQueue.size() == 6) {
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
    bool place = false;
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 || (cords.first != 21 && gd.display[cords.first+1][cords.second].old)) {
            place = true;
            break;
        }
    }
    if (place) {
        for (auto& cords : gd.curTet) {
            if (cords.first < gd.highest) gd.highest = cords.first;
            gd.display[cords.first][cords.second].old = true;
            gd.display[cords.first][cords.second].color = gd.color;
        }
        checkClear();
        addTetromino(nextTetromino());
        dropTetromino();
        printNext();
        printDisplay();
        if (!gd.canHold) gd.canHold = true;
    }
}

bool Game::dropTetromino() {
    if (checkEnd()) {
        printMessage(2);
        finished = true;
        return false;
    }
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 || gd.display[cords.first+1][cords.second].old) {
            return true;
        }
    }
    for (auto& cords : gd.curTet) {
        cords.first++;
    }
    printDisplay();
    printTetromino();
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 || gd.display[cords.first+1][cords.second].old) {
            return true;
        }
    }
    return false;
}

void Game::moveRight() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    for (auto& cords : gd.curTet) {
        if (cords.second == 9 ||
        gd.display[cords.first][cords.second+1].old) return;
    }
    for (auto& cords : gd.curTet) cords.second++;
    printDisplay();
    printTetromino();
    moves.push_back("mr");
}

void Game::moveLeft() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    for (auto& cords : gd.curTet) {
        if (cords.second == 0 ||
        gd.display[cords.first][cords.second-1].old) return;
    }
    for (auto& cords : gd.curTet) cords.second--;
    printDisplay();
    printTetromino();
    moves.push_back("ml");
}

bool Game::touchGround() {
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 ||
        gd.display[cords.first+1][cords.second].old) return true;
    }
    return false;
}

void Game::softDrop() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    if (!touchGround()) {
        for (auto& cords : gd.curTet) cords.first++;
        score++;
        printDisplay();
        printTetromino();
        printScore();
        moves.push_back("sd");
    }
}

void Game::hardDrop() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    if (!touchGround()) {
        while(!touchGround()) {
            for (auto& cords : gd.curTet) cords.first++;
            score += 2;
        }
        placeTetromino();
        printTetromino();
        printScore();
        moves.push_back("hd");
    }
}

void Game::hold() {
    if (gd.canHold && !finished) {
        std::lock_guard<std::mutex> lock(displayMutex);
        if (gd.hold == ' ') {
            gd.hold = gd.type;
            addTetromino(nextTetromino());
            dropTetromino();
            printNext();
            printHold();
        } else {
            char cur = gd.type;
            addTetromino(gd.hold);
            dropTetromino();
            gd.hold = cur;
            printHold();
        }
        gd.canHold = false;
    }
}


bool Game::checkEnd() {
    for (const auto& cords : gd.curTet) {
        if (cords.first == 1 && gd.display[2][cords.second].old) {
            return true;
        }
    }
    return false;
}

int Game::countTime() {
    return static_cast<int>(std::pow(0.8-((level-1)*0.007), level-1)*1000);
}

void Game::update() {
    bool place;
    {
    std::lock_guard<std::mutex> lock(displayMutex);
    place = dropTetromino();
    if (finished) return;
    }
    if (place) {
        for (int i = 0; i < 3; i++) {
            int movesCount = moves.size();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if (moves.size() == movesCount && touchGround()) {
                std::lock_guard<std::mutex> lock(displayMutex);
                placeTetromino();
                break;
            } else if (!touchGround()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(countTime()));
                break;
            }
        }
        std::lock_guard<std::mutex> lock(displayMutex);
        placeTetromino();
    } else std::this_thread::sleep_for(std::chrono::milliseconds(countTime()));
}

void Game::pause() { 
    printMessage(1);
    paused = true;
}

void Game::resume() { 
    printMessage(0);
    countDown();
}
