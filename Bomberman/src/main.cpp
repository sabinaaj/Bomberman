#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <bits/stdc++.h>
#include <ncurses.h>
#include "Constants.h"
#include "CMenu.h"
#include "CPlayer.h"
#include "CGame.h"
using namespace std;


int main() {
    CMenu menu;
    menu.init();
    menu.chooseGamemode();

    return 0;
}

//bomba je zed
// omezeny pocet bomb