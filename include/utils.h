/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Utils
 ****************************************/

#ifndef __MULLINSN_UTILS_H__
#define __MULLINSN_UTILS_H__

#include <stdbool.h>
#include "graphics.h"
#include "world.h"

void getUnitVector(float unitVec[3], float vec[3]);
void setVector(float x, float y, float z, float vec[3]);

bool inBoundsV(float pos[3]);
bool inBounds(float x, float y, float z);
bool collisionV(float pos[3], GLubyte world[WORLDX][WORLDY][WORLDZ]);
bool collision(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ]);

int getWorldBlockF(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ]);
int getWorldBlock(int loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ]);
void setWorldBlockF(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block);
void setWorldBlock(int loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block);

void drawNumber(int digit, int screenX, int screenY, int pixelSize);

#endif