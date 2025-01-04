#include "tetris.hpp"

void Game::printDisplay(bool dots) {
    const char *background = dots ? " ." : "  ";
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
}

void Game::printNext() {
    for (int i = 1; i < 18; i++) if (i % 3 != 0) printAtPosition(6, 9+i, WHITE, " . . . .");
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

void Game::countDown() {
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
    for (auto& cords : gd.curTet) {
        if (cords.first < gd.highest) gd.highest = cords.first;
        gd.display[cords.first][cords.second].old = true;
        gd.display[cords.first][cords.second].color = gd.color;
    }
    checkClear();
    addTetromino(nextTetromino());
    dropTetromino();
    printNext();
    printDisplay(true);
}

void Game::dropTetromino() {
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 || gd.display[cords.first+1][cords.second].old) {
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
    if (finished || paused) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    for (auto& cords : gd.curTet) {
        if (cords.second == 9 ||
        gd.display[cords.first][cords.second+1].old) return;
    }
    for (auto& cords : gd.curTet) cords.second++;
    printDisplay(true);
    printTetromino();
}

void Game::moveLeft() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    for (auto& cords : gd.curTet) {
        if (cords.second == 0 ||
        gd.display[cords.first][cords.second-1].old) return;
    }
    for (auto& cords : gd.curTet) cords.second--;
    printDisplay(true);
    printTetromino();
}

bool Game::touchGroud() {
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 ||
        gd.display[cords.first+1][cords.second].old) return true;
    }
    return false;
}

void Game::softDrop() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    if (!touchGroud()) {
        for (auto& cords : gd.curTet) cords.first++;
        score++;
        printDisplay(true);
        printTetromino();
        printScore();
    }
}

void Game::hardDrop() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    if (!touchGroud()) {
        while(!touchGroud()) {
            for (auto& cords : gd.curTet) cords.first++;
            score += 2;
        }
        placeTetromino();
        printTetromino();
        printScore();
    }
}


void Game::rotate(bool right) {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    auto row = gd.curTet[2].first;
    auto col = gd.curTet[2].second;
    if (gd.type == 'I') rotateI(row, col, right);
    else if (gd.type == 'J') rotateJ(row, col, right);
    else if (gd.type == 'L') rotateL(row, col, right);
    else if (gd.type == 'S') rotateS(row, col, right);
    else if (gd.type == 'T') rotateT(row, col, right);
    else if (gd.type == 'Z') rotateZ(row, col, right);
    printDisplay(true);
    printTetromino();
}

void Game::rotateI(int row, int col, bool right) {
    int i = 0;
    if (!right) i = 1;
    if (gd.stage == 1) {
        if (row < 20 && row > 0 && !gd.display[row+1][col-i].old &&
            !gd.display[row+2][col-i].old && !gd.display[row-1][col-i].old) {
            gd.curTet = {{row-1,col-i},{row,col-i},{row+1,col-i},{row+2,col-i}};
            if (right) gd.stage++;
            else gd.stage = 4;
        }
    } else if (gd.stage == 2) {
        if (col < 9 && col > 1 && !gd.display[row-i][col+1].old &&
            !gd.display[row-i][col-1].old && !gd.display[row-i][col-2].old) {
            gd.curTet = {{row-i,col+1},{row-i,col},{row-i,col-1},{row-i,col-2}};
            if (right) gd.stage++;
            else gd.stage--;
        }
    } else if (gd.stage == 3) {
        if (row < 21 && row > 1 && !gd.display[row+1][col+i].old &&
            !gd.display[row-2][col+i].old && !gd.display[row-1][col+i].old) {
            gd.curTet = {{row+1,col+i},{row,col+i},{row-1,col+i},{row-2,col+i}};
            if (right) gd.stage++;
            else gd.stage--;
        }
    } else if (gd.stage == 4) {
        if (col < 8 && col > 0 && !gd.display[row+i][col+1].old &&
            !gd.display[row+i][col-1].old && !gd.display[row+i][col+2].old) {
            gd.curTet = {{row+i,col-1},{row+i,col},{row+i,col+1},{row+i,col+2}};
            if (right) gd.stage = 1;
            else gd.stage--;
        }
    }
}
void Game::rotateT(int row, int col, bool right) {
    int i = 0;
    if (!right) i = 2;
    if (gd.stage == 1) {
        if (row < 21 && !gd.display[row+1][col].old) {
            if (right) gd.stage++;
            else gd.stage = 4;
            gd.curTet = {{row,col+1-i},{row-1,col},{row,col},{row+1,col}};
        }
    } else if (gd.stage == 2) {
        if (col > 0 && !gd.display[row][col-1].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row+1-i,col},{row,col+1},{row,col},{row,col-1}};
        }
    } else if (gd.stage == 3) {
        if (row > 0 && !gd.display[row-1][col].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row,col-1+i},{row+1,col},{row,col},{row-1,col}};
        }
    } else if (gd.stage == 4) {
        if (col < 9 && !gd.display[row][col+1].old) {
            if (right) gd.stage = 1;
            else gd.stage--;
            gd.curTet = {{row-1+i,col},{row,col-1},{row,col},{row,col+1}};
        }
    }
}
void Game::rotateS(int row, int col, bool right) {
    int i = 0;
    if (!right) i = 1;
    if (gd.stage == 1) {
        if (row < 21 && !gd.display[row+1-i-i][col+1-i-i].old && !gd.display[row+i][col+1-i-i].old) {
            if (right) gd.stage++;
            else gd.stage = 4;
            gd.curTet = {{row+1-i-i,col+1-i-i},{row,col+1-i-i},{row,col},{row-1+i+i,col}};
        }
    } else if (gd.stage == 2) {
        if (col > 0 && !gd.display[row+1-i-i][col-1+i+i].old && !gd.display[row+1-i-i][col-i].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row+1-i-i,col-1+i+i},{row+1-i-i,col},{row,col},{row,col+1-i-i}};
            gd.stage++;
        }
    } else if (gd.stage == 3) {
        if (row > 0  && !gd.display[row-1+i+i][col-1+i+i].old && !gd.display[row-i][col-1+i+i].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row-1+i+i,col-1+i+i},{row,col-1+i+i},{row,col},{row+1-i-i,col}};
            gd.stage++;
        }
    } else if (gd.stage == 4) {
        if (col < 9  && !gd.display[row-1+i+i][col+1-i-i].old && !gd.display[row-1+i+i][col+i].old) {
            if (right) gd.stage = 1;
            else gd.stage--;
            gd.curTet = {{row-1+i+i,col+1-i-i},{row-1+i+i,col},{row,col},{row,col-1+i+i}};
            gd.stage = 1;
        }
    }
}
void Game::rotateZ(int row, int col, bool right) {
    int i = 0;
    if (!right) i = 1;
    if (gd.stage == 1) {
        if (row < 21 && !gd.display[row-1+i+i][col+1-i-i].old && !gd.display[row+1-i-i][col-i].old) {
            if (right) gd.stage++;
            else gd.stage = 4;
            gd.curTet = {{row-1+i+i,col+1-i-i},{row,col+1-i-i},{row,col},{row+1-i-i,col}};
        }
    } else if (gd.stage == 2) {
        if (col > 0 && !gd.display[row+1-i-i][col+1-i-i].old && !gd.display[row-i][col-1+i+i].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row+1-i-i,col+1-i-i},{row+1-i-i,col},{row,col},{row,col-1+i+i}};
        }
    } else if (gd.stage == 3) {
        if (row > 0 && !gd.display[row+1-i-i][col-1+i+i].old && !gd.display[row-1+i+i][col+i].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row+1-i-i,col-1+i+i},{row,col-1+i+i},{row,col},{row-1+i+i,col}};
        }
    } else if (gd.stage == 4) {
        if (col < 9 && !gd.display[row-1+i+i][col-1+i+i].old && !gd.display[row+i][col+1-i-i].old) {
            if (right) gd.stage = 1;
            else gd.stage--;
            gd.curTet = {{row-1+i+i,col-1+i+i},{row-1+i+i,col},{row,col},{row,col+1-i-i}};
        }
    }
}
void Game::rotateJ(int row, int col, bool right) {
    int i = 0;
    if (!right) i = 2;
    if (gd.stage == 1) {
        if (row < 21 && !gd.display[row-1+i][col+1-i].old && !gd.display[row-1][col].old && 
            !gd.display[row+1][col].old) {
            if (right) gd.stage++;
            else gd.stage = 4;
            gd.curTet = {{row-1+i,col+1-i},{row-1+i,col},{row,col},{row+1-i,col}};
        }
    } else if (gd.stage == 2) {
        if (col > 0 && !gd.display[row+1-i][col+1-i].old && !gd.display[row][col+1].old && 
            !gd.display[row][col-1].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row+1-i,col+1-i},{row,col+1-i},{row,col},{row,col-1+i}};
        }
    } else if (gd.stage == 3) {
        if (row > 0 && !gd.display[row+1-i][col-1+i].old && !gd.display[row+1][col].old && 
            !gd.display[row-1][col].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row+1-i,col-1+i},{row+1-i,col},{row,col},{row-1+i,col}};
        }
    } else if (gd.stage == 4) {
        if (col < 9 && !gd.display[row-1+i][col-1+i].old && !gd.display[row][col-1].old && 
            !gd.display[row][col+1].old) {
            if (right) gd.stage = 1;
            else gd.stage--;
            gd.curTet = {{row-1+i,col-1+i},{row,col-1+i},{row,col},{row,col+1-i}};
        }
    }
}
void Game::rotateL(int row, int col, bool right) {
    int i = 0;
    if (!right) i = 2;
    if (gd.stage == 1) {
        if (row < 21 && !gd.display[row+1-i][col+1-i].old && !gd.display[row+1][col].old && 
            !gd.display[row-1][col].old) {
            if (right) gd.stage++;
            else gd.stage = 4;
            gd.curTet = {{row+1-i,col+1-i},{row+1-i,col},{row,col},{row-1+i,col}};
        }
    } else if (gd.stage == 2) {
        if (col > 0 && !gd.display[row+1-i][col-1+i].old && !gd.display[row][col-1].old && 
            !gd.display[row][col+1].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row+1-i,col-1+i},{row,col-1+i},{row,col},{row,col+1-i}};
        }
    } else if (gd.stage == 3) {
        if (row > 0 && !gd.display[row-1+i][col-1+i].old && !gd.display[row-1][col].old && 
            !gd.display[row+1][col].old) {
            if (right) gd.stage++;
            else gd.stage--;
            gd.curTet = {{row-1+i,col-1+i},{row-1+i,col},{row,col},{row+1-i,col}};
        }
    } else if (gd.stage == 4) {
        if (col < 9 && !gd.display[row-1+i][col+1-i].old && !gd.display[row][col+1].old && 
            !gd.display[row][col-1].old) {
            if (right) gd.stage = 1;
            else gd.stage--;
            gd.curTet = {{row-1+i,col+1-i},{row,col+1-i},{row,col},{row,col-1+i}};
        }
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
    {
    std::lock_guard<std::mutex> lock(displayMutex);
    dropTetromino();
    if (finished) return;
    printDisplay(true);
    printTetromino();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(countTime()));
}

void Game::pause() { 
    printMessage(1);
    paused = true;
}

void Game::resume() { 
    printMessage(0);
    countDown();
}
