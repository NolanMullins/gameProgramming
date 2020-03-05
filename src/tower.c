/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-05
 * Game programing -  tower class
 ***************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#include "graphics.h"
#include "world.h"
#include "utils.h"
#include "vehicle.h"
#include "projectile.h"
#include "meteor.h"
#include "tower.h"

List* towerList;
void initTowers(GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    towerList = initList();
    for (int a = 0; a < 5; a++)
        createRandomTower(0, world);
    for (int a = 0; a < 5; a++)
        createRandomTower(1, world);
}

void createRandomTower(int team, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    bool created = false;
    do {
        int x = rand()%(WORLDX-10)+5;
        int z = rand()%(WORLDZ-10)+5;
        created = createTower(team, x, z, world);
    } while(!created);
}

bool createTower(int team, int x, int z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    //check if near a base
    float posA[3];
    memcpy(posA, getBasePos(0), sizeof(float)*3);
    posA[Y] = 0;
    float posB[3];
    memcpy(posB, getBasePos(1), sizeof(float)*3);
    posB[Y] = 0;
    float basePos[3];
    basePos[X] = x;
    basePos[Y] = 0;
    basePos[Z] = z;
    if (distanceVector(posA, basePos) < 20 || distanceVector(posB, basePos) < 20) {
        //printf("Too close to base\n");
        return false;
    }
    //Check if occupied
    for (int a = 0; a < listSize(towerList); a++)
    {
        Tower* other = (Tower*)listGet(towerList, a);
        if (abs(other->pos[X]-x) + abs(other->pos[Z] - z) < 5) {
            //printf("Tower already in that pos\n");
            return false;
        }
    }
    //init
    Tower* t = malloc(sizeof(Tower));
    memset(t->aim, 0, sizeof(int)*3);
    t->aim[Z] = 1;
    t->coolDown = TOWER_COOL_DOWN;
    t->team = team;

    //Determine height
    int h;
    for (h = 0; h < WORLDY-20; h++)
        if (world[x][h][z] == 0)
            break;
    t->pos[X] = x;
    t->pos[Y] = h+TOWER_HEIGHT;
    t->pos[Z] = z;

    int top = t->pos[Y];
    //Draw main tower stack
    for (h=t->pos[Y]-TOWER_HEIGHT; h < top; h++)
        world[t->pos[X]][h][t->pos[Z]] = TOWER_A+t->team;

    //Draw hat
    for (int hatX = x-1; hatX <= x+1; hatX++)
        for (int hatZ = z-1; hatZ <= z+1; hatZ++)
            world[hatX][top][hatZ] = TOWER_A+t->team;
    world[x][top+1][z] = TOWER_A+t->team;

    listAdd(towerList, t);
    return true;
}

void updateTower(Tower* t, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    if (t->coolDown > 0) {
        t->coolDown -= deltaTime;
        return;
    }
    //Look for targets
    List* targetList = getVehicles();
    Vehicle* closestTarget = NULL;
    float targetDist = 100;
    float towerPos[3] = {t->pos[0], t->pos[1]-1, t->pos[2]};
    for (int a = 0; a < listSize(targetList); a++) {
        Vehicle* v = (Vehicle*)listGet(targetList, a);
        if (v->team == t->team)
            continue;
        float dist = distanceVector2D(towerPos, v->front);
        if (dist < TOWER_RANGE && dist < targetDist) {
            closestTarget = v;
            targetDist = dist;
        }
    }
    //shoot at target
    if (closestTarget != NULL) {
        float aim[3];
        memcpy(aim, closestTarget->front, sizeof(float)*3);
        for (int a = 0; a < 3; a++)
            aim[a] -= towerPos[a];
        getUnitVector(aim, aim);
        float proj[3];
        memcpy(proj, towerPos, sizeof(float)*3);
        for (int a = 0; a < 3; a++) {
            proj[a] += aim[a]*2;
            aim[a] *= TOWER_PROJECTILE_SPEED;
        }
        if (createProjectile(0, proj, aim))
            t->coolDown = TOWER_COOL_DOWN;
    }
}

void updateTowers(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    for (int a = 0; a < listSize(towerList); a++)
    {
        Tower* t = (Tower*)listGet(towerList, a);
        updateTower(t, world, deltaTime);
    }
}

List* getTowers()
{
    return towerList;
}

void freeTower(void* obj)
{
    free(obj);
}

void endGameTowers()
{
    towerList = listClear(towerList, &freeTower);
}
