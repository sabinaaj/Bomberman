#include "CMap.h"

void CMap::loadMap(int gamemode, int mapNum, WINDOW * game_win) {
    ifstream in;
    string c;
    string mapName = string("map0").append(to_string(mapNum));
    in.open("map01", ifstream::in);
    if (in.is_open()) {
        for (int i = 0; i < HEIGHT; i++){
            for (int j = 0; j < WIDTH; j++){
                c = in.get();
                mvwprintw(game_win, i, j, c.c_str());
                map_arr[j][i] = c;
            }
        }
        in.close();
    }
    wrefresh(game_win);
}

void CMap::redraw(WINDOW * game_win){
    wclear(game_win);

    for(auto & x: bombs){
        if(x.timer()){
            thread t (&CMap::explode, this);
            t.detach();
        }
    }

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            mvwprintw(game_win, i, j, map_arr[j][i].c_str());
        }
    }
    wrefresh(game_win);
}

void CMap::changeMap(int x, int y, string a) {
    map_arr[x][y] = a;
}

string CMap::getMap(int x, int y) {
    return map_arr[x][y];
}

bool CMap::isEmpty(int x, int y){
    if (map_arr[x][y] == " " || map_arr[x][y] == "0") return true;
    return false;
}

void CMap::placeBomb(int x, int y) {
    map_arr[x][y] = "0";
    bombs.push_back(CBomb(x, y));
}

void CMap::explode() {
    pair<int, int> coords =  bombs[0].getCoords();
    int x = coords.first;
    int y = coords.second;
    map_arr[x][y] = "X";
    for (int i = 1; i < 3; i++){
        if (explodeEmpty(x+2*i, y)) map_arr[x+2*i][y] = "X";
        if (explodeEmpty(x-2*i, y)) map_arr[x-2*i][y] = "X";
        if (explodeEmpty(x, y+i)) map_arr[x][y+i] = "X";
        if (explodeEmpty(x, y-i)) map_arr[x][y-i] = "X";
    }
    sleep(1);
    map_arr[x][y] = " ";
    for (int i = 1; i < SIZE_EXPLODE; i++){
        if (map_arr[x+2*i][y] == "X") map_arr[x+2*i][y] = " ";
        if (map_arr[x-2*i][y] == "X") map_arr[x-2*i][y] = " ";
        if (map_arr[x][y+i] == "X") map_arr[x][y+i] = " ";
        if (map_arr[x][y-i] == "X") map_arr[x][y-i] = " ";
    }


    bombs.erase(bombs.begin());
}

bool CMap::explodeEmpty(int x, int y) {
    if (map_arr[x][y] == " " || map_arr[x][y] == ".") return true;
    if (map_arr[x][y] == "A" || map_arr[x][y] == "B")return false;
    return false;
}