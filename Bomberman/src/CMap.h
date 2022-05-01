
#ifndef BOMBERMAN_CMAP_H
#define BOMBERMAN_CMAP_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include "Constants.h"
#include "CBomb.h"
using namespace std;

class CMap {
private:
    string map_arr[WIDTH][HEIGHT];
    vector <CBomb> bombs;
public:
    void loadMap(int gamemode,int mapNum, WINDOW * game_win);
    void redraw(WINDOW * game_win);
    bool isEmpty(int x, int y);
    void changeMap(int x, int y, string a);
    void placeBomb(int x, int y);
    void explode();
    bool explodeEmpty (int x, int y);
    string getMap(int x, int y);
};


#endif //BOMBERMAN_CMAP_H


