
#ifndef BOMBERMAN_EXCEPTION_H
#define BOMBERMAN_EXCEPTION_H
#include <string>
#include <ncurses.h>
#include "Constants.h"
using namespace std;

class CException{
private:
    string text;
public:
    CException(const string & text)
    :text(text)
    {}

    void print() const {
        WINDOW * error_win = newwin(MAP_HEIGHT, MAP_WIDTH, 0, 0);
        mvwprintw(error_win, 8, 10, text.c_str());
        mvwprintw(error_win, 10, 10, "Press enter to exit");

        wrefresh(error_win);
        while (wgetch(error_win) != 10) {}
        delwin(error_win);
        endwin();
    }
};

#endif //BOMBERMAN_EXCEPTION_H
