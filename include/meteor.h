/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Meteor
 ****************************************/

#ifndef __MULLINSN_METEOR_H__
#define __MULLINSN_METEOR_H__

#include "graphics.h"
#include "world.h"
#include "list.h"

#define INIT_METEORS 30
#define METEOR_WAVE 30
#define METEOR_WAVE_SIZE 10
#define TAIL_SIZE 3

typedef struct meteor
{
    float pos[3];
    float prevPos[TAIL_SIZE][3];
    float velocity[3];
} Meteor;

void initMeteors();
void createMeteor();
void updateMeteors(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
List* getMeteors();
int getNumberOfActiveMeteors();
void endGameMeteors();

#endif