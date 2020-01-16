/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - world utility functions
 ****************************************/

#ifndef __MULLINSN_PLAYER_H__
#define __MULLINSN_PLAYER_H__

#include "graphics.h"
#include "world.h"

#define MAX_PLAYER_HEIGHT CLOUD-2
#define PLAYER_BUFFER 0.5

void initPlayer(float spawnLocation[3]);
void updatePlayer(float prev[3], float curr[3], GLubyte world[WORLDX][WORLDY][WORLDZ]);
void endGamePlayer();

#endif