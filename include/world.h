/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - world utility functions
 ****************************************/

#ifndef __MULLINSN_WORLD_H__
#define __MULLINSN_WORLD_H__

#include "graphics.h"

#define X 0
#define Y 1
#define Z 2
#define CLOUD_HEIGHT 49
#define CLOUD_WIDTH 10 
#define CLOUD_LENGTH 20
#define GROUND_LEVEL 7

#define GRAVITY 9.8

typedef struct cloud
{
	int location[3];
    double velocity;
    double time;
    int shape[CLOUD_WIDTH][CLOUD_LENGTH];
} Cloud;

float baseAPos[3];
float baseBPos[3];

void initWorld(GLubyte world[WORLDX][WORLDY][WORLDZ]);
void updateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ], double deltaTime);
void destroyWorld();
void generateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ]);
float* getBasePos(int team);


#endif
