
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

bool CPlayerAI::inBombRange(vector<CBomb> &bombsArr) {
    for (auto a: bombsArr){
        pair<int, int> coords =  a.getCoords();
        if (hitPlayer(coords.first, coords.second, flame)) return true;
    }
    return false;
}

void CPlayerAI::control(CMap * Map, vector <CBomb> & bombsArr) {
//   if (inBombRange(bombsArr)){
//       if(Map->isEmpty(x, y - 1)){
//           step('U', Map);
//       }
//       else if (Map->isEmpty(x - 2, y)){
//           step('L', Map);
//       }
//       else if (Map->isEmpty(x + 2, y)){
//           step('R', Map);
//       }
//       else if (Map->isEmpty(x, y + 1)){
//           step('D', Map);
//       }
//   }
   if ((rand() % 4 == 0)){
       srand(time(nullptr));
           bool moved = false;
           for (int i = 0; i < 4; i++) {
               int dir = rand() % 4;
               switch (dir) {
                   case 0:
                       if (Map->isEmpty(x, y - 1)) {
                           step('U', Map);
                           moved = true;
                       }
                       break;
                   case 1:
                       if (Map->isEmpty(x - 2, y)) {
                           step('L', Map);
                           moved = true;
                       }
                       break;
                   case 2:
                       if (Map->isEmpty(x + 2, y)) {
                           step('R', Map);
                           moved = true;
                       }
                       break;
                   case 3:
                       if (Map->isEmpty(x, y + 1)) {
                           step('D', Map);
                           moved = true;
                       }
                       break;
               }
               if (moved) break;
               if (random() % 25 == 0){
                   if(placeBomb()) bombsArr.push_back(CBomb(x, y, num));
               }
           }
   }
}
