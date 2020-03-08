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

#define TOWER_COOL_DOWN 2.0
#define TOWER_HEIGHT 7
#define TOWER_RANGE 20.0
#define TOWER_PROJECTILE_SPEED 30
#define TOWER_HEALTH 10

typedef struct tower
{
    int team;
    float coolDown;
    int pos[3];
    int aim[3];
    int health;
} Tower;

void initTowers(GLubyte world[WORLDX][WORLDY][WORLDZ]);
bool validTowerLoc(int team, int x, int z, GLubyte world[WORLDX][WORLDY][WORLDZ]);
void createRandomTower(int team, GLubyte world[WORLDX][WORLDY][WORLDZ]);
bool createTower(int team, int x, int z,GLubyte world[WORLDX][WORLDY][WORLDZ]);
void updateTowers(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void damageTower(int index, Tower* t, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ]);
List* getTowers();
void endGameTowers();

#endif