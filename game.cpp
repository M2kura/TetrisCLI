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

void Game::checkTetris() {
    std::vector<int> rows = {};
    for (int i = 21; i >= gd.highest; i--) {
        for (int j = 0; j < 10; j++) {
            if (gd.display[i][j].type == "empty") break;
            if (j == 9) {
                rows.push_back(i);
                for (int k = i; k < 9; k++) {
                    gd.display[i][k].type = "empty";
                    gd.display[i][k].color = "";
                }
            }
        }
    }
    while (rows.size() != 0) {
        int row = rows.back();
        rows.pop_back();
        for (int i = row; i >= gd.highest; i--) {
            gd.display[i] = gd.display[i-1];
        }
        gd.highest++;
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
        if (cords.first < gd.highest) gd.highest = cords.first;
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
    std::lock_guard<std::mutex> lock(displayMutex);
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
    std::lock_guard<std::mutex> lock(displayMutex);
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
    std::lock_guard<std::mutex> lock(displayMutex);
    for (auto& cords : gd.curTet) {
        if (cords.first == 21 ||
        gd.display[cords.first+1][cords.second].type == "old") return;
    }
    for (auto& cords : gd.curTet) cords.first++;
    printDisplay(true);
    printTetromino();
}

void Game::rotate() {
    if (finished) return;
    std::lock_guard<std::mutex> lock(displayMutex);
    auto row = gd.curTet[2].first;
    auto col = gd.curTet[2].second;
    if (gd.type == 'I') rotateI(row, col);
    else if (gd.type == 'J') rotateJ(row, col);
    else if (gd.type == 'L') rotateL(row, col);
    else if (gd.type == 'S') rotateS(row, col);
    else if (gd.type == 'T') rotateT(row, col);
    else if (gd.type == 'Z') rotateZ(row, col);
    printDisplay(true);
    printTetromino();
}

void Game::rotateI(int row, int col) {
    if (gd.stage == 1) {
        if (row < 20 && row > 0 && gd.display[row+1][col].type != "old" &&
            gd.display[row+2][col].type != "old" && gd.display[row-1][col].type != "old") {
            gd.curTet = {{row-1,col},{row,col},{row+1,col},{row+2,col}};
            gd.stage++;
        }
    } else if (gd.stage == 2) {
        if (col < 9 && col > 1 && gd.display[row][col+1].type != "old" &&
            gd.display[row][col-1].type != "old" && gd.display[row][col-2].type != "old") {
            gd.curTet = {{row,col+1},{row,col},{row,col-1},{row,col-2}};
            gd.stage++;
        }
    } else if (gd.stage == 3) {
        if (row < 21 && row > 1 && gd.display[row+1][col].type != "old" &&
            gd.display[row-2][col].type != "old" && gd.display[row-1][col].type != "old") {
            gd.curTet = {{row+1,col},{row,col},{row-1,col},{row-2,col}};
            gd.stage++;
        }
    } else if (gd.stage == 4) {
        if (col < 8 && col > 0 && gd.display[row][col+1].type != "old" &&
            gd.display[row][col-1].type != "old" && gd.display[row][col+2].type != "old") {
            gd.curTet = {{row,col-1},{row,col},{row,col+1},{row,col+2}};
            gd.stage = 1;
        }
    }
}
void Game::rotateT(int row, int col) {
    if (gd.stage == 1) {
        if (row < 21 && gd.display[row+1][col].type != "old") {
            gd.curTet = {{row,col+1},{row-1,col},{row,col},{row+1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 2) {
        if (col > 0 && gd.display[row][col-1].type != "old") {
            gd.curTet = {{row+1,col},{row,col+1},{row,col},{row,col-1}};
            gd.stage++;
        }
    } else if (gd.stage == 3) {
        if (row > 0 && gd.display[row-1][col].type != "old") {
            gd.curTet = {{row,col-1},{row+1,col},{row,col},{row-1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 4) {
        if (col < 9 && gd.display[row][col+1].type != "old") {
            gd.curTet = {{row-1,col},{row,col-1},{row,col},{row,col+1}};
            gd.stage++;
        }
    }
}
void Game::rotateS(int row, int col) {
    if (gd.stage == 1) {
        if (row < 21 && gd.display[row][col+1].type != "old" && gd.display[row+1][col+1].type != "old") {
            gd.curTet = {{row+1,col+1},{row,col+1},{row,col},{row-1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 2) {
        if (col > 0 && gd.display[row+1][col].type != "old" && gd.display[row+1][col-1].type != "old") {
            gd.curTet = {{row+1,col-1},{row+1,col},{row,col},{row,col+1}};
            gd.stage++;
        }
    } else if (gd.stage == 3) {
        if (row > 0 && gd.display[row][col-1].type != "old" && gd.display[row-1][col-1].type != "old") {
            gd.curTet = {{row-1,col-1},{row,col-1},{row,col},{row+1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 4) {
        if (col < 9 && gd.display[row-1][col].type != "old" && gd.display[row-1][col+1].type != "old") {
            gd.curTet = {{row-1,col+1},{row-1,col},{row,col},{row,col-1}};
            gd.stage = 1;
        }
    }
}
void Game::rotateZ(int row, int col) {
    if (gd.stage == 1) {
        if (row < 21 && gd.display[row-1][col+1].type != "old" && gd.display[row+1][col].type != "old") {
            gd.curTet = {{row-1,col+1},{row,col+1},{row,col},{row+1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 2) {
        if (col > 0 && gd.display[row][col-1].type != "old" && gd.display[row+1][col+1].type != "old") {
            gd.curTet = {{row+1,col+1},{row+1,col},{row,col},{row,col-1}};
            gd.stage++;
        }
    } else if (gd.stage == 3) {
        if (row > 0 && gd.display[row-1][col].type != "old" && gd.display[row+1][col-1].type != "old") {
            gd.curTet = {{row+1,col-1},{row,col-1},{row,col},{row-1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 4) {
        if (col < 9 && gd.display[row-1][col-1].type != "old" && gd.display[row][col+1].type != "old") {
            gd.curTet = {{row-1,col-1},{row-1,col},{row,col},{row,col+1}};
            gd.stage = 1;
        }
    }
}
void Game::rotateJ(int row, int col) {
    if (gd.stage == 1) {
        if (row < 21 && gd.display[row-1][col+1].type != "old" && gd.display[row-1][col].type != "old" && 
            gd.display[row+1][col].type != "old") {
            gd.curTet = {{row-1,col+1},{row-1,col},{row,col},{row+1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 2) {
        if (col > 0 && gd.display[row+1][col+1].type != "old" && gd.display[row][col+1].type != "old" && 
            gd.display[row][col-1].type != "old") {
            gd.curTet = {{row+1,col+1},{row,col+1},{row,col},{row,col-1}};
            gd.stage++;
        }
    } else if (gd.stage == 3) {
        if (row > 0 && gd.display[row+1][col-1].type != "old" && gd.display[row+1][col].type != "old" && 
            gd.display[row-1][col].type != "old") {
            gd.curTet = {{row+1,col-1},{row+1,col},{row,col},{row-1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 4) {
        if (col < 9 && gd.display[row-1][col-1].type != "old" && gd.display[row][col-1].type != "old" && 
            gd.display[row][col+1].type != "old") {
            gd.curTet = {{row-1,col-1},{row,col-1},{row,col},{row,col+1}};
            gd.stage = 1;
        }
    }
}
void Game::rotateL(int row, int col) {
    if (gd.stage == 1) {
        if (row < 21 && gd.display[row+1][col+1].type != "old" && gd.display[row+1][col].type != "old" && 
            gd.display[row-1][col].type != "old") {
            gd.curTet = {{row+1,col+1},{row+1,col},{row,col},{row-1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 2) {
        if (col > 0 && gd.display[row+1][col-1].type != "old" && gd.display[row][col-1].type != "old" && 
            gd.display[row][col+1].type != "old") {
            gd.curTet = {{row+1,col-1},{row,col-1},{row,col},{row,col+1}};
            gd.stage++;
        }
    } else if (gd.stage == 3) {
        if (row > 0 && gd.display[row-1][col-1].type != "old" && gd.display[row-1][col].type != "old" && 
            gd.display[row+1][col].type != "old") {
            gd.curTet = {{row-1,col-1},{row-1,col},{row,col},{row+1,col}};
            gd.stage++;
        }
    } else if (gd.stage == 4) {
        if (col < 9 && gd.display[row-1][col+1].type != "old" && gd.display[row][col+1].type != "old" && 
            gd.display[row][col-1].type != "old") {
            gd.curTet = {{row-1,col+1},{row,col+1},{row,col},{row,col-1}};
            gd.stage = 1;
        }
    }
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
    {
    std::lock_guard<std::mutex> lock(displayMutex);
    dropTetromino();
    if (finished) return;
    printDisplay(true);
    printTetromino();
    }
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
