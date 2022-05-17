
#include "CPlayerAI.h"

bool CPlayerAI::inBombRange(const vector<CBomb> &bombsArr, int playerX, int playerY) {
    for (auto a: bombsArr) {
        pair<int, int> coords = a.getCoords();
        if (hitPlayer(coords.first, coords.second, flame, playerX, playerY)) return true;
    }
    return false;
}

bool CPlayerAI::hitPlayer(int bombX, int bombY, int flameRange, int playerX, int playerY) {
    if (playerX == bombX && playerY == bombY) {
        return true;
    }
    for (int i = 1; i < flameRange; i++) {
        if (playerX == bombX + 2 * i && playerY == bombY) return true;
        if (playerX == bombX - 2 * i && playerY == bombY) return true;
        if (playerX == bombX && playerY == bombY + i) return true;
        if (playerX == bombX && playerY == bombY - i) return true;
    }
    return false;
}

void CPlayerAI::canGo(CMap *Map, const vector<CBomb> &bombsArr) {
    if (Map->isEmpty(x, y - 1) && !inBombRange(bombsArr, x, y - 1)) isEmpty.push_back('U');
    if (Map->isEmpty(x - 2, y) && !inBombRange(bombsArr, x - 2, y)) isEmpty.push_back('L');
    if (Map->isEmpty(x + 2, y) && !inBombRange(bombsArr, x + 2, y)) isEmpty.push_back('R');
    if (Map->isEmpty(x, y + 1) && !inBombRange(bombsArr, x, y + 1)) isEmpty.push_back('D');
}

void CPlayerAI::control(CMap *Map, vector<CBomb> &bombsArr) {
    Map->drawStats(lives, bombs, score, num);

    if (inBombRange(bombsArr, x, y)) {
        if (Map->isEmpty(x, y - 1) &&
            (Map->isEmpty(x - 2, y - 1) || Map->isEmpty(x + 2, y - 1) || Map->isEmpty(x, y - 2))) {
            step('U', Map);
            prev = 'U';
        } else if (Map->isEmpty(x - 2, y) &&
                   (Map->isEmpty(x - 2, y + 1) || Map->isEmpty(x - 2, y - 1) || Map->isEmpty(x - 4, y))) {
            step('L', Map);
            prev = 'L';
        } else if (Map->isEmpty(x + 2, y) &&
                   (Map->isEmpty(x + 2, y + 1) || Map->isEmpty(x + 2, y - 1) || Map->isEmpty(x + 4, y))) {
            step('R', Map);
            prev = 'R';
        } else if (Map->isEmpty(x, y + 1) &&
                   (Map->isEmpty(x - 2, y + 1) || Map->isEmpty(x + 2, y + 1) || Map->isEmpty(x, y + 2))) {
            step('D', Map);
            prev = 'D';
        }
    } else if (rand() % 4 == 0) {
        canGo(Map, bombsArr);
        if (isEmpty.size() == 1) {
            step(isEmpty[0], Map);
            prev = isEmpty[0];
        } else if (!isEmpty.empty()) {
            srand(time(nullptr));

            char dontGo = ' ';
            if (prev == 'U') dontGo = 'D';
            else if (prev == 'D') dontGo = 'U';
            else if (prev == 'R') dontGo = 'L';
            else if (prev == 'L') dontGo = 'R';

            auto pos = lower_bound(isEmpty.begin(), isEmpty.end(), dontGo);
            if (!(pos == isEmpty.end() || *pos != dontGo)) { //is found
                isEmpty.erase(pos);
            }
            int dir = rand() % isEmpty.size();
            step(isEmpty[dir], Map);
            prev = isEmpty[dir];
        }
        isEmpty.clear();

        if ((Map->isInRange(x, y, ".") && random() % 5 == 0) || Map->isInRange(x, y, "A") ||
            Map->isInRange(x, y, "B")) {
            if (placeBomb()) bombsArr.push_back(CBomb(x, y, num));
        }
    }
}