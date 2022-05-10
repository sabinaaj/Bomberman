//
// Created by sabina on 19.4.22.
//

#include "CMenu.h"


void CMenu::print() {
    int x, y;
    x = 2;
    y = 2;

    for(size_t i = 0; i < choices.size(); ++i)
    {	if(highlight == i + 1) /* High light the present choice */
        {	wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, choices[i].c_str());
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, choices[i].c_str());
        ++y;
    }
    wrefresh(menu_win);
}

void CMenu::init(){
    int start_x = (80 - WIN_WIDTH) / 2;
    int start_y = (30 - WIN_HEIGHT) / 2;
    highlight = 1;

    initscr();
    clear();
    noecho();
    curs_set(0);
    cbreak();
    refresh();

    menu_win = newwin(WIN_HEIGHT, WIN_WIDTH, start_y, start_x);
    keypad(menu_win, TRUE);
}

void CMenu::chooseGamemode(){
    highlight = 1;
    choices = {"1 player", "2 players", "Exit"};
    print();

    arrows();

    if(choice == 3){
        delwin(menu_win);
        endwin();
    }
    else {
        gamemode = choice;
        chooseMap();
    }
}

void CMenu::chooseMap() {
    highlight = 1;
    choices = {"Map 1", "Map 2", "Map 3", "Back"};
    print();

    arrows();

    if(choice == 4){
        chooseGamemode();
    }
    else {
        delwin(menu_win);
        CGame(gamemode, choice);
    }


}

void CMenu::arrows() {
    choice = 0;
    int ch;
    while (true){
        ch = wgetch(menu_win);
        switch(ch)
        {	case KEY_UP:
                if(highlight == 1)
                    highlight = choices.size();
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == choices.size())
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                refresh();
                break;
        }
        print();


        if(choice != 0)	{
            choices.clear();
            wclear(menu_win);
            break;
        }

    }
}