/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Projectile class
 ****************************************/

#ifndef __MULLINSN_PROJECTILE_H__
#define __MULLINSN_PROJECTILE_H__

#include <stdbool.h>

#include "graphics.h"
#include "world.h"
#include "list.h"

#define APPLY_GRAVITY 0
#define PROJECTILE_SPEED 30

typedef struct projectile
{
    int type;
    int id;
    int team;
    float pos[3];
    float velocity[3];
} Projectile;

void initProjectiles();
bool createProjectile(int type, int team, float spawnLocation[3], float velocity[3]);
void updateProjectiles(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
List* getProjectiles();
int getNumberOfActiveProjectiles();
void endGameProjectiles();

#endif