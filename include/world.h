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
#define CLOUD_HEIGHT 45

typedef struct cloud
{
	int location[3];
    int shape[5][10];
} Cloud;

void initWorld(GLubyte world[WORLDX][WORLDY][WORLDZ]);
void updateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ]);
void destroyWorld();
void generateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ]);


#endif
