/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-05
 * Game programing -  tower class
 ***************************************/

#ifndef __MULLINSN_TOWER_H__
#define __MULLINSN_TOWER_H__

#include <stdbool.h>

#include "graphics.h"
#include "world.h"
#include "list.h"
#include "vehicle.h"

#define TOWER_COOL_DOWN 2
#define TOWER_HEIGHT 7

typedef struct tower
{
    int team;
    int coolDown;
    int pos[3];
    int aim[3];
} Tower;

void initTowers(GLubyte world[WORLDX][WORLDY][WORLDZ]);
void createRandomTower(int team, GLubyte world[WORLDX][WORLDY][WORLDZ]);
bool createTower(int team, int x, int z,GLubyte world[WORLDX][WORLDY][WORLDZ]);
void updateTowers(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
List* getTowers();
void endGameTowers();

#endif