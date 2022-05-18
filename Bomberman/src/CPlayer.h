
#ifndef BOMBERMAN_CPLAYER_H
#define BOMBERMAN_CPLAYER_H

#include "CMap.h"
#include "CBomb.h"

using namespace std;

/**
 * Abstract class makes and take care of player
 */
class CPlayer {
protected:
    /** coords where player is */
    int x, y;
    /** player number */
    int num;
    /** number of lives */
    int lives;
    /** number of bombs */
    int bombs;
    /** score counter */
    int score;
    /** size of flame */
    int flame;
    /** string which represents player */
    string image;
    /** string which remembers what is behind player */
    string background;

    /**
     * Picks and applies bonus effect
     * @param Map pointer to map
     */
    void pickBonus(CMap *Map);

public:
    /**
     * Constructor
     * @param num of player
     */
    CPlayer(int num);

    /**
     * Default destructor
     */
    virtual ~CPlayer() {}

    /**
     * Change background of player to bomb image and decreases bombs number
     * @return true if bomb was successfully placed
     */
    bool placeBomb();

    /**
     * Moves player in given direction
     * @param dir direction where player go
     * @param Map pointer to map
     */
    void step(char dir, CMap *Map);

    /**
     * Makes bomb explodes and draw flames
     * @param bombX x of bomb
     * @param bombY y of bomb
     * @param Map pointer to map
     */
    void explode(int bombX, int bombY, CMap *Map);

    /**
     * Checks if player was hit
     * @param bombX x of bomb
     * @param bombY y of bomb
     * @param flameRange size of flame
     * @return true if flame hits player and false if doesn't
     */
    bool hitPlayer(int bombX, int bombY, int flameRange);

    /**
     * Decreases lives and changes background
     */
    void gotHit();

    /**
     * Changes background and draws player after explosion
     * @param Map pointer to map
     */
    void afterHit(CMap *Map);

    /**
     * Call all functions to update the player status
     * @param Map pointer to map
     * @param bombsArr vector of placed bombs
     */
    virtual void control(CMap *Map, vector<CBomb> &bombsArr) = 0;

    inline pair<int, int> getCoords() const { return make_pair(x, y); }

    inline int getLives() const { return lives; }

    inline int getFlame() { return flame; }

    inline string getImage() const { return image; }

    inline int getScore() const { return score; }

    inline void addScore(int num) { score += num; }
};

/**
 * Child class takes care of user player
 */
class CPlayerNotAI : public CPlayer {
public:
    /**
     * Constructor
     * @param num player number
     */
    CPlayerNotAI(int num) : CPlayer(num) {}

    /**
     * Call all functions to update the player status
     * @param Map pointer to map
     * @param bombsArr vector of bombs
     */
    void control(CMap *Map, vector<CBomb> &bombsArr) override;
};

#endif //BOMBERMAN_CPLAYER_H
