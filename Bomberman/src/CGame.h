
#ifndef BOMBERMAN_CGAME_H
#define BOMBERMAN_CGAME_H

#include "CPlayerAI.h"

/**
 * Class takes care of bombs, players and map
 */
class CGame {
private:
    /** 1 = one player, 2 = two players */
    int gamemode;
    /** number of chosen map */
    int mapNum;
    /** if game is over or not */
    int end;
    /** vector stores bombs in map */
    vector<CBomb> bombs;
    /** vector stores 2 players */
    vector<CPlayer *> players;
    /** map object */
    CMap Map;

    /**
     * Takes input from user
     */
    void input();

    /**
     *  Makes bomb explodes and draw flames
     */
    void explode(vector<CBomb>::iterator bomb);

public:
    /**
     * Constructor
     * @param gm gamemode
     * @param map number of chosen map
     */
    CGame(int gm, int map);

    /**
     * Destructor destroys players
     */
    ~CGame();

    /**
     *  Main games loop. Takes input and change the game
     */
    void gameloop();

    void endGame();

};


#endif //BOMBERMAN_CGAME_H
