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

#define HELI_VEL 10
#define HELI_HEIGHT GROUND_LEVEL+15
#define HELI_COOL_DOWN 1
#define HELI_RANGE 30

#define MaxHeliHealth 2
#define ProjectileDMG 1

typedef struct heli
{
    int team;
    int health;
    float dest[3];
    float currDirection[3];
    float pos[3];
    float move[3];
    float cooldown;
} Heli;

void initHeli();
void createHeli(int team);
void updateHeli(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void damageHeli(int index, Heli* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ]);
void drawHeli(Heli* h, GLubyte world[WORLDX][WORLDY][WORLDZ], int blockType);
List* getHeli();
int getNumberOfActiveHeli();
void endGameHeli();

#endif