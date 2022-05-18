
#include "CMenu.h"

/**
 * @mainpage Bomberman
 *
 * Bomberman is an action game with 2 gamemodes:
 *   - 1 player
 *   - 2 players
 *
 * You can choose from 3 maps from directory /examples.
 *
 * Controls:
 *   - player 'A' : W S A D to move and place bomb with E
 *   - player 'B' : arrows keys to move and space to place bomb
 *
 * Game description:
 *   - '#' is a wall that cannot be destroyed
 *   - '.' is a wall that can be destroyed by a bomb
 *   - '?' is a bonus that can be +1 bomb or bigger flame
 *   - '0' is a bomb
 *   - 'X' is a flame
 */

int main() {
    CMenu menu;
    menu.init();
    menu.chooseGamemode();
    return 0;
}