
#include "CPlayer.h"

CPlayer::CPlayer(int number) {
    if (number == 0) {
        x = A_X;
        y = A_Y;
        num = number;
        image = "A";

    } else {
        x = B_X;
        y = B_Y;
        num = number;
        image = "B";
    }
    background = " ";
    lives = LIVES;
    bombs = BOMBS;
    flame = FLAME;
    score = 0;
}


void CPlayer::step(char dir, CMap *Map) {
    int empty = 0;
    switch (dir) {
        case 'U':
            empty = Map->isEmpty(x, y - 1);
            if (empty) {
                Map->changeMap(x, y, background);
                y--;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
                if (empty == 2) pickBonus(Map);
            }
            break;
        case 'L':
            empty = Map->isEmpty(x - 2, y);
            if (empty) {
                Map->changeMap(x, y, background);
                x -= 2;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
                if (empty == 2) pickBonus(Map);
            }
            break;
        case 'R':
            empty = Map->isEmpty(x + 2, y);
            if (empty) {
                Map->changeMap(x, y, background);
                x += 2;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
                if (empty == 2) pickBonus(Map);
            }
            break;
        case 'D':
            empty = Map->isEmpty(x, y + 1);
            if (empty) {
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
    if (bombs > 0) {
        background = "0";
        bombs--;
        return true;
    }
    return false;
}

void CPlayer::explode(int bombX, int bombY, CMap *Map) {
    bombs++;
    Map->explode(bombX, bombY, flame, "X");
    sleep(EXPLODE_SLEEP);
    Map->explode(bombX, bombY, flame, " ");
}

bool CPlayer::hitPlayer(int bombX, int bombY, int flameRange) {
    if (x == bombX && y == bombY) {
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

void CPlayer::gotHit2(CMap *Map) {
    Map->changeMap(x, y, image);
    background = " ";
}

void CPlayer::pickBonus(CMap *Map) {
    background = " ";
    int randBonus = rand() % 2;
    switch (randBonus) {
        case 0:
            if (flame < MAX_FLAME) {
                flame += 1;
                score += BONUS_SCORE;
                thread t(&CMap::bonusText, Map, "Player " + image + " got bigger flame");
                t.detach();
            } else {
                thread t(&CMap::bonusText, Map, "Player " + image + " have max flame");
                t.detach();
            }
            break;
        case 1:
            if (bombs < MAX_BOMBS) {
                bombs += 1;
                score += BONUS_SCORE;
                thread t(&CMap::bonusText, Map, "Player " + image + " got +1 bomb");
                t.detach();
            } else {
                thread t(&CMap::bonusText, Map, "Player " + image + " can't got more bombs");
                t.detach();
            }
            break;
    }
}

void CPlayerNotAI::control(CMap *Map, vector<CBomb> &bombsArr) {
    Map->drawStats(lives, bombs, score, num);
}



