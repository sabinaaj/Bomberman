
#ifndef BOMBERMAN_CMENU_H
#define BOMBERMAN_CMENU_H
#include "CGame.h"
using namespace std;

/**
 * Makes and handle game menu
 */
class CMenu {
private:
    /** pointer to menu window */
    WINDOW * menu_win;
    /** 0 = one player, 1 = two players */
    int gamemode;
    /** index of button */
    int choice;
    /** index of highlighted button */
    size_t highlight;
    /** vector with text for menu button */
    vector<std::string> choices;
public:
    /**
     * Initialize menu window and all ncurses values
     */
    void init();

    /**
     * First menu page where user choose gamemode
     */
    void chooseGamemode();

    /**
     * Second menu page where user choose map
     */
    void chooseMap();

    /**
     * Prints menu and highlights the chosen one
     */
    void print();

    /**
     * Takes input from user
     */
    void arrows();
};


#endif //BOMBERMAN_CMENU_H
