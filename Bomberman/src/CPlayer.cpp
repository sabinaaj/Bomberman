
#include "CPlayer.h"

CPlayer::CPlayer(int num){
    if (num == 0){
        x = 2;
        y = 1;
        image = "A";
    }
    else{
        x = 46;
        y = 21;
        image = "B";
    }
    background = " ";
}


void CPlayer::step(char dir, CMap * Map){
    switch (dir) {
        case 'U':
            if (Map->isEmpty(x, y - 1)){
                Map->changeMap(x, y, background);
                y--;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
            }
            break;
        case 'L':
            if (Map->isEmpty(x - 2, y)){
                Map->changeMap(x, y, background);
                x -= 2;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
            }
            break;
        case 'R':
            if (Map->isEmpty(x + 2, y)){
                Map->changeMap(x, y, background);
                x += 2;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
            }
            break;
        case 'D':
            if (Map->isEmpty(x, y + 1)){
                Map->changeMap(x, y, background);
                y++;
                background = Map->getMap(x, y);
                Map->changeMap(x, y, image);
            }
    }
}

void CPlayer::placeBomb(CMap *Map) {
    Map->placeBomb(x, y);
    background = "0";
}

