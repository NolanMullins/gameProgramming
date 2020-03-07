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

#define PLAYER_START 3
#define AI_START 3

#define AI 0
#define PLAYER 1

int score[2];

void initScore();
int getScore(int team);
bool usePoints(int team, int score);
void depositPoints(int team, int score);

#endif

