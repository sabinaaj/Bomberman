//
// Created by sabina on 19.4.22.
//

#ifndef BOMBERMAN_CMENU_H
#define BOMBERMAN_CMENU_H

#include <vector>
#include <string>
#include "Constants.h"
using namespace std;

class CMenu {
private:
    WINDOW * menu_win;
    int gamemode;
    int choice;
    size_t highlight;
    vector<std::string> choices;
public:
    void init();
    void chooseGamemode();
    void chooseMap();
    void print();
    void arrows();

};


#endif //BOMBERMAN_CMENU_H
