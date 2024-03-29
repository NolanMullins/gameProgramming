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
#include "list.h"

bool paused;

float vectorLength(float vec[3]);
void getUnitVector(float unitVec[3], float vec[3]);
void setVector(float x, float y, float z, float vec[3]);
float distanceVector(float a[3], float b[3]);
float distanceVector2D(float a[3], float b[3]);
bool occupySameBlock(float v1[3], float v2[3]);
bool occupySameBlockCustom(float x, float y, float z, float v2[3]);

void rotateVector(float v[3], float radians);

void printVector(float vec[3]);
void printVectorCustom(float x, float y, float z);

bool inBoundsV(float pos[3]);
bool inBounds(float x, float y, float z);
bool collisionV(float pos[3], GLubyte world[WORLDX][WORLDY][WORLDZ]);
bool collision(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ]);
float isNegUtil(float tmp);

void generateRandomCord(float dest[3]);

int getWorldBlockF(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ]);
int getWorldBlock(int loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ]);
int getWorldBlockCustom(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ]);
void setWorldBlockF(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block);
void setWorldBlock(int loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block);
void setWorldBlockCustom(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ], int block);

void drawNumber(int digit, int screenX, int screenY, int pixelSize);

void getClosestTarget(int team, float range, float pos[3], float closestTargetPos[3]);

void pauseGame(bool p);
bool isPaused();

#endif