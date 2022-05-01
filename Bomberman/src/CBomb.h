#ifndef BOMBERMAN_CBOMB_H
#define BOMBERMAN_CBOMB_H
#include <utility>
#include "Constants.h"
using namespace std;


class CBomb{
private:
    int time;
    int x, y;
public:
    CBomb(int pos_x, int pos_y);
    bool timer();
    inline pair<int, int> getCoords() const {return make_pair(x, y);};
};


#endif //BOMBERMAN_CBOMB_H
