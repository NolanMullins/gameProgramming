/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-16
 * Game programing - Tank class
 ***************************************/

#ifndef __MULLINSN_TANK_H__
#define __MULLINSN_TANK_H__

#include "graphics.h"
#include "world.h"
#include "list.h"

#define TANK_VEL 1.5

#define MaxTankHealth 4
#define ProjectileDMG 1

typedef struct tank
{
    int team;
    int health;
    float dest[3];
    float currDirection[3];
    float front[3];
    float mid[3];
    float back[3];
    float move[3];
} Tank;

void initTanks();
void createTank(int team);
void updateTanks(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void damageTank(int index, Tank* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ]);
void drawTank(Tank* t, GLubyte world[WORLDX][WORLDY][WORLDZ], int blockType);
List* getTanks();
int getNumberOfActiveTanks();
void endGameTanks();

#endif