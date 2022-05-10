#ifndef BOMBERMAN_CBOMB_H
#define BOMBERMAN_CBOMB_H
#include <utility>
#include "Constants.h"
using namespace std;

/**
 * Class that takes care of bombs
 */
class CBomb{
private:
    /** number od loops before bomb explode */
    int time;
    /** coords of bomb */
    int x, y;
    /** player who drop the bomb */
    int player;
public:
    /**
     * Constructor
     * @param x coords
     * @param y coords
     * @param player player number
     */
    CBomb(int x, int y, int player);

    /**
     * Decreases time
     * @return true if bomb is ready to explode, false if it is not
     */
    bool timer();

    /**
     * Gets coords of the bomb
     * @return coords
     */
    inline pair<int, int> getCoords() const {return make_pair(x, y);}

    /**
     * get number of player who drop the bomb
     * @return player
     */
    inline int getPlayer() {return player;}
};


#endif //BOMBERMAN_CBOMB_H
