
#include "CPlayer.h"

CPlayer::CPlayer(int number){
    if (number == 0){
        x = 2;
        y = 1;
        num = number;
        image = "A";

    }
    else{
        x = 46;
        y = 21;
        num = number;
        image = "B";
    }
    background = " ";
    lives = 3;
    bombs = 1;
    flame = 2;
    speedDelay = 50000;
}


void CPlayer::step(char dir, CMap * Map){
    int empty = 0;
    switch (dir) {
        case 'U':
            empty = Map->isEmpty(x, y - 1);
            if (empty){
                Map->changeMap(x, y, background);
                y--;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
                if (empty == 2) pickBonus(Map);
            }
            break;
        case 'L':
            empty = Map->isEmpty(x - 2, y);
            if (empty){
                Map->changeMap(x, y, background);
                x -= 2;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
                if (empty == 2) pickBonus(Map);
            }
            break;
        case 'R':
            empty = Map->isEmpty(x + 2, y);
            if (empty){
                Map->changeMap(x, y, background);
                x += 2;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
                if (empty == 2) pickBonus(Map);
            }
            break;
        case 'D':
            empty = Map->isEmpty(x, y + 1);
            if (empty){
                Map->changeMap(x, y, background);
                y++;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
                if (empty == 2) pickBonus(Map);
            }
            break;
    }
}

bool CPlayer::placeBomb() {
    if(bombs > 0){
        background = "0";
        bombs--;
        return true;
    }
    return false;
}

void CPlayer::drawStats(CMap * Map) {
    Map->drawStats(lives, bombs, num);
}

void CPlayer::explode(int bombX, int bombY, CMap *Map) {
    bombs++;
    Map->explode(bombX, bombY, flame, "X");
    sleep(1);
    Map->explode(bombX, bombY, flame, " ");
}

bool CPlayer::hitPlayer(int bombX, int bombY, int flameRange) {
    if (x == bombX && y == bombY ) {
        return true;
    }
    for (int i = 1; i < flameRange; i++) {
        if (x == bombX + 2 * i && y == bombY) return true;
        if (x == bombX - 2 * i && y == bombY) return true;
        if (x == bombX && y == bombY + i) return true;
        if (x == bombX && y == bombY - i) return true;
    }
    return false;
}


void CPlayer::gotHit1() {
    lives--;
    background = "X";
}

void CPlayer::gotHit2(CMap * Map) {
    Map->changeMap(x, y, image);
    background = " ";
}

void CPlayer::pickBonus(CMap * Map) {
    background = " ";
    int randBonus = rand() % 3;
    switch (randBonus) {
        case 0:
            if (flame < MAX_FLAME) {
                flame += 1;
                thread t (&CMap::bonusText, Map, "Player " + image + " got bigger flame" );
                t.detach();
            }
            else{
                thread t (&CMap::bonusText, Map, "Player " + image + " have max flame" );
                t.detach();
            }
            break;
        case 1:
            if (bombs < MAX_BOMBS){
                bombs += 1;
                thread t (&CMap::bonusText, Map, "Player " + image + " got +1 bomb" );
                t.detach();
            }
            else{
                thread t (&CMap::bonusText, Map, "Player " + image + " can't got more bombs" );
                t.detach();
            }
            break;
        case 2:
            speedDelay /= 10;
            thread t (&CMap::bonusText, Map, "Player " + image + " got speed bonus");
            t.detach();
            break;
    }
}

void CPlayer::control(CMap * Map, vector <CBomb> & bombsArr) {}

bool CPlayerAI::inBombRange(vector<CBomb> &bombsArr, int playerX, int playerY) {
    for (auto a: bombsArr){
        pair<int, int> coords =  a.getCoords();
        if (hitPlayer(coords.first, coords.second, flame, playerX, playerY)) return true;
    }
    return false;
}

bool CPlayerAI::hitPlayer(int bombX, int bombY, int flameRange, int playerX, int playerY) {
    if (playerX == bombX && playerY == bombY ) {
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

void CPlayerAI::canGo(CMap *Map, vector <CBomb> & bombsArr) {
    if (Map->isEmpty(x, y - 1) && !inBombRange(bombsArr, x, y - 1)) isEmpty.push_back('U');
    if (Map->isEmpty(x - 2, y) && !inBombRange(bombsArr, x - 2, y)) isEmpty.push_back('L');
    if (Map->isEmpty(x + 2, y) && !inBombRange(bombsArr, x + 2, y)) isEmpty.push_back('R');
    if (Map->isEmpty(x, y + 1) && !inBombRange(bombsArr, x, y + 1)) isEmpty.push_back('D');
}

void CPlayerAI::control(CMap * Map, vector <CBomb> & bombsArr) {
    if (inBombRange(bombsArr, x, y)) {
        if (Map->isEmpty(x, y - 1)) {
            step('U', Map);
            prev = 'U';
        } else if (Map->isEmpty(x - 2, y)) {
            step('L', Map);
            prev = 'L';
        } else if (Map->isEmpty(x + 2, y)) {
            step('R', Map);
            prev = 'R';
        } else if (Map->isEmpty(x, y + 1)) {
            step('D', Map);
            prev = 'D';
        }
    } else if (rand() % 4 == 0) {
        canGo(Map, bombsArr);
        if (isEmpty.size() == 1) {
            step(isEmpty[0], Map);
            prev = isEmpty[0];
        } else if (!isEmpty.empty()){
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