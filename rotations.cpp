#include "tetris.hpp"

bool Game::canRotate(std::vector<std::pair<int, int>> cords) {
    for (auto cord : cords) {
        if (cord.first < 0 || cord.first > 21 || cord.second < 0 || cord.second > 9) return false;
        if (gd.display[cord.first][cord.second].old) return false;
    }
    return true;
}

void shiftCords(std::vector<std::pair<int, int>> *cords, int x, int y) {
    for (auto &cord : *cords) {
        cord.first += x;
        cord.second += y;
    }
}

void Game::rotate(bool right) {
    if (finished || gd.type == 'O') return;
    std::lock_guard<std::mutex> lock(displayMutex);
    auto row = gd.curTet[2].first;
    auto col = gd.curTet[2].second;
    std::vector<std::pair<int, int>> rotateTo;
    std::vector<std::pair<int, int>> xy;
    if (gd.stage == 1 && right) {
        xy = {{0,-1},{-1,-1},{2,0},{2,-1}};
        if (gd.type == 'I') {
            xy = {{0,-2},{0,1},{1,-2},{-2,1}};
            rotateTo = {{row-1,col},{row,col},{row+1,col},{row+2,col}};
        } else if (gd.type == 'T')
            rotateTo = {{row,col+1},{row-1,col},{row,col},{row+1,col}};
        else if (gd.type == 'S')
            rotateTo = {{row+1,col+1},{row,col+1},{row,col},{row-1,col}};
        else if (gd.type == 'Z')
            rotateTo = {{row-1,col+1},{row,col+1},{row,col},{row+1,col}};
        else if (gd.type == 'J')
            rotateTo = {{row-1,col+1},{row-1,col},{row,col},{row+1,col}};
        else if (gd.type == 'L')
            rotateTo = {{row+1,col+1},{row+1,col},{row,col},{row-1,col}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage++;
        moves.push_back("rr");
    } else if (gd.stage == 1) {
        xy = {{0,1},{-1,1},{2,0},{2,1}};
        if (gd.type == 'I') {
            xy = {{0,-1},{0,2},{-2,-1},{1,2}};
            rotateTo = {{row+2,col-1},{row+1,col-1},{row,col-1},{row-1,col-1}};
        } else if (gd.type == 'T')
            rotateTo = {{row,col-1},{row-1,col},{row,col},{row+1,col}};
        else if (gd.type == 'S')
            rotateTo = {{row-1,col-1},{row,col-1},{row,col},{row+1,col}};
        else if (gd.type == 'Z')
            rotateTo = {{row+1,col-1},{row,col-1},{row,col},{row-1,col}};
        else if (gd.type == 'J')
            rotateTo = {{row+1,col-1},{row+1,col},{row,col},{row-1,col}};
        else if (gd.type == 'L')
            rotateTo = {{row-1,col-1},{row-1,col},{row,col},{row+1,col}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage = 4;
        moves.push_back("rl");
    } else if (gd.stage == 2 && right) {
        xy = {{0,1},{1,1},{-2,0},{-2,1}};
        if (gd.type == 'I') {
            xy = {{0,-1},{0,2},{-2,-1},{1,2}};
            rotateTo = {{row,col+1},{row,col},{row,col-1},{row,col-2}};
        } else if (gd.type == 'T')
            rotateTo = {{row+1,col},{row,col+1},{row,col},{row,col-1}};
        else if (gd.type == 'S')
            rotateTo = {{row+1,col-1},{row+1,col},{row,col},{row,col+1}};
        else if (gd.type == 'Z')
            rotateTo = {{row+1,col+1},{row+1,col},{row,col},{row,col-1}};
        else if (gd.type == 'J')
            rotateTo = {{row+1,col+1},{row,col+1},{row,col},{row,col-1}};
        else if (gd.type == 'L')
            rotateTo = {{row+1,col-1},{row,col-1},{row,col},{row,col+1}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage++;
        moves.push_back("rr");
    } else if (gd.stage == 2) {
        xy = {{0,1},{1,1},{-2,0},{-2,1}};
        if (gd.type == 'I') {
            xy = {{0,2},{0,-1},{-1,2},{2,-1}};
            rotateTo = {{row-1,col-2},{row-1,col-1},{row-1,col},{row-1,col+1}};
        } else if (gd.type == 'T')
            rotateTo = {{row-1,col},{row,col+1},{row,col},{row,col-1}};
        else if (gd.type == 'S')
            rotateTo = {{row-1,col+1},{row-1,col},{row,col},{row,col-1}};
        else if (gd.type == 'Z')
            rotateTo = {{row-1,col-1},{row-1,col},{row,col},{row,col+1}};
        else if (gd.type == 'J')
            rotateTo = {{row-1,col-1},{row,col-1},{row,col},{row,col+1}};
        else if (gd.type == 'L')
            rotateTo = {{row-1,col+1},{row,col+1},{row,col},{row,col-1}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage--;
        moves.push_back("rl");
    } else if (gd.stage == 3 && right) {
        xy = {{0,1},{-1,1},{2,0},{2,1}};
        if (gd.type == 'I') {
            xy = {{0,2},{0,-1},{-1,2},{2,-1}};
            rotateTo = {{row+1,col},{row,col},{row-1,col},{row-2,col}};
        } else if (gd.type == 'T')
            rotateTo = {{row,col-1},{row+1,col},{row,col},{row-1,col}};
        else if (gd.type == 'S')
            rotateTo = {{row-1,col-1},{row,col-1},{row,col},{row+1,col}};
        else if (gd.type == 'Z')
            rotateTo = {{row+1,col-1},{row,col-1},{row,col},{row-1,col}};
        else if (gd.type == 'J')
            rotateTo = {{row+1,col-1},{row+1,col},{row,col},{row-1,col}};
        else if (gd.type == 'L')
            rotateTo = {{row-1,col-1},{row-1,col},{row,col},{row+1,col}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage++;
        moves.push_back("rr");
    } else if (gd.stage == 3) {
        xy = {{0,-1},{-1,-1},{2,0},{2,-1}};
        if (gd.type == 'I') {
            xy = {{0,1},{0,-2},{2,1},{-1,-2}};
            rotateTo = {{row-2,col+1},{row-1,col+1},{row,col+1},{row+1,col+1}};
        } else if (gd.type == 'T')
            rotateTo = {{row,col+1},{row+1,col},{row,col},{row-1,col}};
        else if (gd.type == 'S')
            rotateTo = {{row+1,col+1},{row,col+1},{row,col},{row-1,col}};
        else if (gd.type == 'Z')
            rotateTo = {{row-1,col+1},{row,col+1},{row,col},{row+1,col}};
        else if (gd.type == 'J')
            rotateTo = {{row-1,col+1},{row-1,col},{row,col},{row+1,col}};
        else if (gd.type == 'L')
            rotateTo = {{row+1,col+1},{row+1,col},{row,col},{row-1,col}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage--;
        moves.push_back("rl");
    } else if (gd.stage == 4 && right) {
        xy = {{0,-1},{1,-1},{-2,0},{-2,-1}};
        if (gd.type == 'I') {
            xy = {{0,1},{0,-2},{2,1},{-1,-2}};
            rotateTo = {{row,col-1},{row,col},{row,col+1},{row,col+2}};
        } else if (gd.type == 'T')
            rotateTo = {{row-1,col},{row,col+1},{row,col},{row,col-1}};
        else if (gd.type == 'S')
            rotateTo = {{row-1,col+1},{row-1,col},{row,col},{row,col-1}};
        else if (gd.type == 'Z')
            rotateTo = {{row-1,col-1},{row-1,col},{row,col},{row,col+1}};
        else if (gd.type == 'J')
            rotateTo = {{row-1,col-1},{row,col-1},{row,col},{row,col+1}};
        else if (gd.type == 'L')
            rotateTo = {{row-1,col+1},{row,col+1},{row,col},{row,col-1}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage = 1;
        moves.push_back("rr");
    } else {
        xy = {{0,-1},{1,-1},{-2,0},{-2,-1}};
        if (gd.type == 'I') {
            xy = {{0,-2},{0,1},{1,-2},{-2,1}};
            rotateTo = {{row+1,col+2},{row+1,col+1},{row+1,col},{row+1,col-1}};
        } else if (gd.type == 'T')
            rotateTo = {{row+1,col},{row,col+1},{row,col},{row,col-1}};
        else if (gd.type == 'S')
            rotateTo = {{row+1,col-1},{row+1,col},{row,col},{row,col+1}};
        else if (gd.type == 'Z')
            rotateTo = {{row+1,col+1},{row+1,col},{row,col},{row,col-1}};
        else if (gd.type == 'J')
            rotateTo = {{row+1,col+1},{row,col+1},{row,col},{row,col-1}};
        else if (gd.type == 'L')
            rotateTo = {{row+1,col-1},{row,col-1},{row,col},{row,col+1}};
        if (canRotate(rotateTo))
            gd.curTet = rotateTo;
        else {
            for (int i = 0; i < 4; i++) {
                shiftCords(&rotateTo, xy[i].first, xy[i].second);
                if (canRotate(rotateTo)) {
                    gd.curTet = rotateTo;
                    break;
                }
                if (i == 3) return;
            }
        }
        gd.stage--;
        moves.push_back("rl");
    }
    printDisplay(true);
    printTetromino();
}
