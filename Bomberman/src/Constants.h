
#ifndef BOMBERMAN_CONSTANTS_H
#define BOMBERMAN_CONSTANTS_H

/** width of game window */
#define MAP_WIDTH 51
/** height of game window */
#define MAP_HEIGHT 25
/** number of loops before bomb explode */
#define BOMB_TIME 15
/** max number of bombs that can one player have */
#define MAX_BOMBS 4
/** max number of flame size */
#define MAX_FLAME 4
/** sleep time in gameloop in microseconds*/
#define SLEEP_TIME 150000
/** time for how long there will be flames*/
#define EXPLODE_SLEEP 1
/** time for how long there will be bonus text*/
#define BONUS_SLEEP 3
/** score for hitting a player*/
#define HIT_SCORE 20
/** score for getting a bonus*/
#define BONUS_SCORE 10
/** number of lives player starts with*/
#define LIVES 3
/** number of bombs player starts with*/
#define BOMBS 1
/** initial size of flame*/
#define FLAME 2
/** start x and y coord of A*/
#define A_X 2
#define A_Y 1
/** start x and y coord of A*/
#define B_X 46
#define B_Y 21

#endif //BOMBERMAN_CONSTANTS_H
