/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-07
 * Game programing - Score helper class
 ****************************************/

#ifndef __MULLINSN_SCORE_H__
#define __MULLINSN_SCORE_H__

#include <stdbool.h>

#include "graphics.h"
#include "world.h"

#define PLAYER_START 18
#define AI_START 4

#define AI 0
#define PLAYER 1

#define TOWER_COST 2
#define VEHICLE_COST 1
#define HELI_COST 2
#define TANK_COST 3

int score[2];

void initScore();
int getScore(int team);
bool usePoints(int team, int score);
void depositPoints(int team, int score);

#endif

