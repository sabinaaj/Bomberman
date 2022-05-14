
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
    /** how much is player slower */
    int speedDelay;
    /** size of flame */
    int flame;
    /** string which represents player */
    string image;
    /** string which remembers what is behind player */
    string background;

public:
    /**
     * Constructor
     * @param num of player
     */
    CPlayer(int num);

    /**
     * Change background of player to bomb image and decreases bombs number
     * @return true if bomb was successfully placed
     */
    bool placeBomb();

    /**
     * Calls CMap::drawStats
     * @param Map pointer to map
     */
    void drawStats(CMap * Map);

    /**
     * Moves player in given direction
     * @param dir direction where player go
     * @param Map pointer to map
     */
    void step(char dir, CMap * Map);

    /**
     * Makes bomb explodes and draw flames
     * @param bombX x of bomb
     * @param bombY y of bomb
     * @param Map pointer to map
     */
    void explode(int bombX, int bombY, CMap * Map);

    /**
     * @param bombX
     * @param bombY
     * @param flameRange size of flame
     * @return true if flame hits player and false if doesn't
     */
    bool hitPlayer(int bombX, int bombY, int flameRange);

    /**
     * Decreases lives and changes background
     */
    void gotHit1();

    /**
     * Changes background and draws player after explosion
     * @param Map pointer to map
     */
    void gotHit2(CMap * Map);

    /**
     * Picks and applies bonus effect
     * @param Map pointer to map
     */
    void pickBonus(CMap * Map);

    /**
     * Gets coords
     * @return Coords of player
     */
    inline pair<int, int> getCoords() const {return make_pair(x, y);}

    /**
     * Gets lives
     * @return number of lives
     */
    inline int getLives() const {return lives;}

    /**
     * Gets size of flame
     * @return size of flame
     */
    inline int getFlame() {return flame;}

    virtual void control(CMap * Map, vector <CBomb> & bombsArr);

    /**
     * Gets image
     * @return string which represents player
     */
    inline string getImage() const {return image;}
};

/**
 * Child class takes care of user player
 */
class CPlayerNotAI: public CPlayer{
public:
    /**
     * Constructor
     * @param num player number
     */
    CPlayerNotAI(int num): CPlayer(num){}
};

/**
 * Child class takes care of computer player
 */
class CPlayerAI: public CPlayer{
private:
    char prev;
    vector<char> isEmpty;
public:
    /**
    * Constructor
    * @param num player number
    */
    CPlayerAI(int num): CPlayer(num){
        prev = ' ';
    }
    bool hitPlayer(int bombX, int bombY, int flameRange, int playerX, int playerY);

    bool inBombRange(vector <CBomb> & bombsArr, int playerX, int playerY);

    void canGo(CMap * Map, vector <CBomb> & bombsArr);

    void control(CMap * Map, vector <CBomb> & bombsArr) override;
};
#endif //BOMBERMAN_CPLAYER_H
