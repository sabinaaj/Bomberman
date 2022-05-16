
#ifndef BOMBERMAN_CGAME_H
#define BOMBERMAN_CGAME_H

#include "CPlayerAI.h"

/**
 * Class takes care of bombs, players and map
 */
class CGame {
private:
    /** 0 = one player, 1 = two players */
    int gamemode;
    /** number of chosen map */
    int mapNum;
    /** if game is over or not */
    int endGame;
    /** vector stores bombs in map */
    vector<CBomb> bombs;
    /** vector stores 2 players */
    vector<CPlayer *> players;
    /** map object */
    CMap Map;


public:
    /**
     * Constructor
     * @param gm gamemode
     * @param map number of chosen map
     */
    CGame(int gm, int map);

    /**
     * destructor destroys players
     */
    ~CGame();

    /**
     *  Main games loop. Takes input and change the game
     */
    void gameloop();

    /**
     * Takes input from user
     */
    void input();

    /**
     *  Makes bomb explodes and draw flames
     */
    void explode(vector<CBomb>::iterator bomb);

};


#endif //BOMBERMAN_CGAME_H
