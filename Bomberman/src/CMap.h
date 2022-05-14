
#ifndef BOMBERMAN_CMAPA_H
#define BOMBERMAN_CMAPA_H
#include <string>
#include <vector>
#include <fstream>
#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include "Constants.h"
using namespace std;

/**
 * Class handle map and draws map in terminal
 */
class CMap {
private:
    /** pointer to window with bonus text */
    WINDOW * bonus_win;
    /** pointer to game win  */
    WINDOW * game_win;
    /** vector stores chars in map in theirs coords */
    string map_arr[MAP_WIDTH][MAP_HEIGHT];
    /** vector store coords of places where cn be placed bonus*/
    vector<pair<int, int>> flame;
public:
    /**
     * Load map from file
     * @param mapNum number of chosen map
     */
    void loadMap(int mapNum);

    /**
     * Draw changes in map
     */
    void redraw();

    /**
     * Checks if player can make step
     * @param x next step coords
     * @param y next step coords
     * @return 1 if coords are empty, 0 if is not and 2 if there is a bonus
     */
    int isEmpty(int x, int y);

    /**
     * Assigns string a to given coord
     * @param x
     * @param y
     * @param a string which we want to have at given coords
     */
    void changeMap(int x, int y, string a);

    /**
     * Gets string which is on given coords
     * @param x
     * @param y
     * @return what is on given coords
     */
    string getMap(int x, int y);

    /**
     * Draws flame and turn map back to normal
     * @param x bomb x
     * @param y bomb y
     * @param flameSize size of the flame
     * @param a what it draws
     */
    void explode(int x, int y, int flameSize, string a);

    /**
     * Checks if bomb can explode at given coords
     * @param x of bomb
     * @param y of bomb
     * @param exp if there can be flame or not
     * @return true if coords are empty, false if is not
     */
    bool explodeEmpty (int x, int y, bool & exp);

    /**
     * Draws live and bomb counters on the bottom of game window
     * @param lives number of lives
     * @param bombs number of lives
     * @param player player whose stats it is
     */
    void drawStats(int lives, int bombs, int player);

    bool isInRange(int x, int y, string a);

    /**
     * Draws bonus at random coords
     */
    void drawBonus();

    /**
     * Draws text which says who get which bonus
     * @param text
     */
    void bonusText(string text);

    /**
     * show end text and ends game
     * show end text and ends game
     * @param player image of player who lost
     */
    void endGame(string player);

    inline void refresh (){wrefresh(game_win);}

    /**
     * Gets player input
     * @return player input
     */
    inline int input() { int ch = wgetch(game_win);
                         return ch;}

};


#endif //BOMBERMAN_CMAPA_H


