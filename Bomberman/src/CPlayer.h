
#ifndef BOMBERMAN_CPLAYER_H
#define BOMBERMAN_CPLAYER_H
#include <string>
#include <vector>
#include "Constants.h"
#include "CMap.h"
#include "ncurses.h"
using namespace std;

class CPlayer {
public:
    int x;
    int y;
    string image;
    string background;

    CPlayer(int num);
    void placeBomb(CMap * Map);
    void step(char dir, CMap * Map);
};

#endif //BOMBERMAN_CPLAYER_H
