
#ifndef BOMBERMAN_PLAYERAI_H
#define BOMBERMAN_PLAYERAI_H
#include "CPlayer.h"

/**
 * Child class takes care of computer player
 */
class CPlayerAI : public CPlayer {
private:
    /** previous direction where player went  */
    char prev;
    /** vector of directions where player can go*/
    vector<char> isEmpty;
public:
    /**
    * Constructor
    * @param num player number
    */
    CPlayerAI(int num) : CPlayer(num) {
        prev = ' ';
    }

    /**
     * Checks if player was hit
     * @param bombX x of bomb
     * @param bombY y of bomb
     * @param flameRange size of flame
     * @param playerX x of player
     * @param playerY y of player
     * @return true if flame hits player and false if doesn't
     */
    bool hitPlayer(int bombX, int bombY, int flameRange, int playerX, int playerY);

    /**
     * Checks if player would be hit by a bomb if doesn't move
     * @param bombsArr vector of placed bombs
     * @param playerX x of player
     * @param playerY y of player
     * @return true if player is in bomb range and false if it isn't
     */
    bool inBombRange(const vector<CBomb> &bombsArr, int playerX, int playerY);

    /**
     * Makes vector of directions where player can go
     * @param Map pointer to map
     * @param bombsArr vector of placed bombs
     */
    void canGo(CMap *Map, const vector<CBomb> &bombsArr);

    /**
     * Makes AI player move
     * @param Map pointer to map
     * @param bombsArr vector of placed bombs
     */
    void control(CMap *Map, vector<CBomb> &bombsArr) override;
};


#endif //BOMBERMAN_PLAYERAI_H
