
#include "CGame.h"

CGame::CGame(int gm, int map)
        : gamemode(gm), mapNum(map) {
    endGame = 0;
    players.push_back(new CPlayerAI(0));
    if (gamemode == 2) {
        players.push_back(new CPlayerNotAI(1));
    } else {
        players.push_back(new CPlayerAI(1));
    }
    Map.loadMap(map);
    gameloop();
}

CGame::~CGame() {
    for (auto it = players.begin(); it != players.end(); it++){
        delete *it;
    }
}

void CGame::gameloop() {
    thread t1(&CGame::input, this);
    t1.detach();
    while (true) {
        if (players[0]->getLives() == 0) endGame = 1;
        if (players[1]->getLives() == 0) endGame = 2;

        if (endGame) break;

        players[1]->control(&Map, bombs);
        players[0]->control(&Map, bombs);

        for (auto it = bombs.begin(); it != bombs.end(); it++) {
            if (it->timer()) {
                thread t2(&CGame::explode, this, it);
                t2.detach();
            }
        }
        Map.redraw();
        players[0]->drawStats(&Map);
        players[1]->drawStats(&Map);
        Map.refresh();
        usleep(50000 * 3);
    }

    fstream score;
    int maxscore = 0;
    score.open("score", fstream::in);
    if (score.is_open()) {
        score >> maxscore;
        for (size_t i = 0; i < players.size(); i++) {
            if (maxscore < players[i]->getScore()) {
                maxscore = players[i]->getScore();
                score.close(); //???
                score.open("score", fstream::out | fstream::trunc);
                score << maxscore;
            }
        }
    }
    score.close();
    string image = players[endGame - 1]->getImage();
    Map.endGame(image, maxscore);
}

void CGame::explode(vector<CBomb>::iterator bomb) {
    pair<int, int> coords = bomb->getCoords();
    int player = bomb->getPlayer();
    int flame = players[player]->getFlame();
    int x = coords.first;
    int y = coords.second;
    vector<int> hitPlayers;

    if (players[0]->hitPlayer(x, y, flame)) {
        if (player == 1) players[1]->addScore(20);
        players[0]->gotHit1();
        hitPlayers.push_back(0);
    }
    if (players[1]->hitPlayer(x, y, flame)) {
        if (player == 0) players[0]->addScore(20);
        players[1]->gotHit1();
        hitPlayers.push_back(1);
    }

    players[player]->explode(x, y, &Map);
    players[player]->addScore(Map.getFlameSize());

    for (auto a: hitPlayers) {
        players[a]->gotHit2(&Map);
    }
    Map.drawBonus();
    bombs.erase(bombs.begin());
}

void CGame::input() {
    int ch;

    while (!endGame) {
        ch = Map.input();
        pair<int, int> coords0 = players[0]->getCoords();
        pair<int, int> coords1 = players[1]->getCoords();
        switch (ch) {
            case 'w':
                players[0]->step('U', &Map);
                break;
            case 'a':
                players[0]->step('L', &Map);
                break;
            case 'd':
                players[0]->step('R', &Map);
                break;
            case 's':
                players[0]->step('D', &Map);
                break;
            case 'e':
                if (players[0]->placeBomb())
                    bombs.push_back(CBomb(coords0.first, coords0.second, 0));
                break;
            case KEY_UP:
                if (gamemode == 2)
                    players[1]->step('U', &Map);
                break;
            case KEY_LEFT:
                if (gamemode == 2)
                    players[1]->step('L', &Map);
                break;
            case KEY_RIGHT:
                if (gamemode == 2)
                    players[1]->step('R', &Map);
                break;
            case KEY_DOWN:
                if (gamemode == 2)
                    players[1]->step('D', &Map);
                break;
            case ' ':
                if (players[1]->placeBomb() && gamemode == 2)
                    bombs.push_back(CBomb(coords1.first, coords1.second, 1));
                break;
        }
    }
}

