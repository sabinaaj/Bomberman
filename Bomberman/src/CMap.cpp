
#include "CMap.h"

void CMap::loadMap(int mapNum) {
    game_win = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);
    keypad(game_win, TRUE);
    bonus_win = newwin(1, WIN_WIDTH, 24, 0);

    ifstream in;
    string c;
    string mapName = string("../maps/map0").append(to_string(mapNum));
    in.open(mapName, ifstream::in);
    if (in.is_open()) {
        for (int i = 0; i < MAP_HEIGHT; i++){
            for (int j = 0; j < MAP_WIDTH; j++){
                c = in.get();
                mvwprintw(game_win, i, j, c.c_str());
                map_arr[j][i] = c;
            }
        }
        in.close();
    }
    wrefresh(game_win);
}

void CMap::redraw(){
    for (int i = 0; i < MAP_HEIGHT; i++){
        for (int j = 0; j < MAP_WIDTH; j++){
            mvwprintw(game_win, i, j, map_arr[j][i].c_str());
        }
    }
    wrefresh(game_win);
}

void CMap::changeMap(int x, int y, string a) {
    map_arr[x][y] = a;
    mvwprintw(game_win, y, x, a.c_str());
    wrefresh(game_win);
}

string CMap::getMap(int x, int y) {
    return map_arr[x][y];
}

int CMap::isEmpty(int x, int y){
    if (map_arr[x][y] == " ") return 1;
    if (map_arr[x][y] == "?") return 2;
    return 0;
}

bool CMap::isInRange(int x, int y, string a) {
    if (map_arr[x + 2][y] == a) return true;
    if (map_arr[x - 2][y] == a) return true;
    if (map_arr[x][y + 1] == a) return true;
    if (map_arr[x][y - 1] == a) return true;
    return false;
}

void CMap::explode(int x, int y, int flameSize, string a) {
    map_arr[x][y] = a;
    bool exp0 = true, exp1 = true, exp2 = true, exp3 = true;

    for (int i = 1; i < flameSize; i++) {
        if (explodeEmpty(x + 2 * i, y, exp0) && exp0) map_arr[x + 2 * i][y] = a;
        if (explodeEmpty(x - 2 * i, y, exp1) && exp1) map_arr[x - 2 * i][y] = a;
        if (explodeEmpty(x, y + i, exp2) && exp2) map_arr[x][y + i] = a;
        if (explodeEmpty(x, y - i, exp3) && exp3) map_arr[x][y - i] = a;
    }
    redraw();
}

bool CMap::explodeEmpty(int x, int y, bool & exp) {
    if (!exp) return false;
    if (x > MAP_WIDTH || x < 0 || y > MAP_HEIGHT || y < 0) {
        exp = false;
        return false;
    }
    if (map_arr[x][y] == " " || map_arr[x][y] == "A" || map_arr[x][y] == "B"
     || map_arr[x][y] == "?" || map_arr[x][y] == "0" || map_arr[x][y] == "X") return true;
    if (map_arr[x][y] == "."){
        flame.push_back(make_pair(x, y));
        return true;
    }
    exp = false;
    return false;
}

void CMap::drawStats(int lives, int bombs, int player) {
    string liveStr = string("Lives: ").append(to_string(lives));
    string bombStr = string ("Bombs: ").append(to_string(bombs));
    if(player){
        mvwprintw(game_win, 23, WIN_WIDTH - 20, liveStr.c_str());
        mvwprintw(game_win, 23, WIN_WIDTH - 10, bombStr.c_str());
    }
    else{
        mvwprintw(game_win, 23, 0, liveStr.c_str());
        mvwprintw(game_win, 23, 10, bombStr.c_str());
    }
}

void CMap::drawBonus() {
   int flameSize = flame.size();
   if (flameSize >= 2){
       srand(time(nullptr));
       int randPlace = rand() % flameSize;
       pair<int, int> coords = flame[randPlace];
       map_arr[coords.first][coords.second] = "?";
       mvwprintw(game_win, coords.second, coords.first, "?");
       wrefresh(game_win);
   }
   flame.clear();
}

void CMap::bonusText(string text) {
    wprintw(bonus_win, text.c_str());
    wrefresh(bonus_win);

    sleep(3);
    wclear(bonus_win);
    wrefresh(bonus_win);
}

void CMap::endGame(string player) {
    delwin(bonus_win);
    wclear(game_win);

    string msg = "Player " + player + " lost the game";
    mvwprintw(game_win, 10, 10, msg.c_str());
    wrefresh(game_win);
    wgetch(game_win);

    delwin(game_win);
    endwin();
}