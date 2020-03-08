/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-07
 * Game programing - AI Class
 ****************************************/

#ifndef __MULLINSN_AI_H__
#define __MULLINSN_AI_H__

#include "graphics.h"
#include "world.h"

void initAI();
void updateAI(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void endGameAI();

#endif