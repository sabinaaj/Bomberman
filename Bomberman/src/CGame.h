//
// Created by sabina on 20.4.22.
//

#ifndef BOMBERMAN_CGAME_H
#define BOMBERMAN_CGAME_H
#include <thread>
#include "CMap.h"
#include "CPlayer.h"
#include "CBomb.h"


class CGame {
private:
    int gamemode;
    int mapNum;
    vector <CPlayer> players;
    WINDOW * game_win;
    CMap Map;

public:
    CGame(int gm,int map);
    void gameloop();
    void input();
};


#endif //BOMBERMAN_CGAME_H
