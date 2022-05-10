
#include "CBomb.h"

CBomb::CBomb(int x, int y, int player) {
    time = BOMB_TIME;
    this->x = x;
    this->y = y;
    this->player = player;
}

bool CBomb::timer() {
    time--;
    if (time) return false;
    return true;
}
