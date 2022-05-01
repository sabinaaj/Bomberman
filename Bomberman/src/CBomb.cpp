
#include "CBomb.h"

CBomb::CBomb(int pos_x, int pos_y) {
    time = BOMB_TIME;
    x = pos_x;
    y = pos_y;
}

bool CBomb::timer() {
    time--;
    if (time) return false;
    return true;
}
