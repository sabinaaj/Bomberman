
#include "CGame.h"
#include "CMap.h"

CGame::CGame(int gm, int map)
        : gamemode(gm), mapNum(map) {
    CPlayer A(0);
    CPlayer B(1);
    players.push_back(A);
    players.push_back(B);
    game_win = newwin(HEIGHT, WIDTH, 0, 0);
    keypad(game_win, TRUE);
    Map.loadMap(gm, map, game_win );
    gameloop();
}

void CGame::gameloop() {
    thread t1 (&CGame::input, this);
    t1.detach();

    while (true){
        Map.redraw(game_win);
        usleep(50000);
    }

}


void CGame::input() {
    int ch;
    bool end_game = false;

    while(!end_game){
        ch = wgetch(game_win);
        switch(ch){
            case 'w':
                players[0].step('U', &Map);
                break;
            case 'a':
                players[0].step('L', &Map);
                break;
            case 'd':
                players[0].step('R', &Map);
                break;
            case 's':
                players[0].step('D', &Map);
                break;
            case 'e':
                players[0].placeBomb(&Map);
                break;
            case KEY_UP:
                players[1].step('U', &Map);
                break;
            case KEY_LEFT:
                players[1].step('L', &Map);
                break;
            case KEY_RIGHT:
                players[1].step('R', &Map);
                break;
            case KEY_DOWN:
                players[1].step('D', &Map);
                break;
            case ' ':
                players[1].placeBomb(&Map);
                break;
            case 10:
                end_game = true;
                break;
        }
    }
    endwin();
}

