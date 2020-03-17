/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-016
 * Game programing - Heli class
 ***************************************/

#ifndef __MULLINSN_HELI_H__
#define __MULLINSN_HELI_H__

#include "graphics.h"
#include "world.h"
#include "list.h"

#define HELI_VEL 8

#define MaxHeliHealth 2
#define ProjectileDMG 1

typedef struct heli
{
    int team;
    int health;
    int state;
    float dest[3];
    float currDirection[3];
    float front[3];
    float mid[3];
    float back[3];
    float move[3];
    float loadTime;
    bool hasBlock;
} Heli;

void initHeli();
void createHeli(int team);
void updateHeli(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void damageHeli(int index, Heli* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ]);
List* getHeli();
int getNumberOfActiveHeli();
void endGameHeli();

#endif