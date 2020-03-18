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
#include "score.h"

float towerBuffer[2];
List* towerList;
void initTowers(GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    towerList = initList();
    towerBuffer[AI] = 10;
    towerBuffer[PLAYER] = 5;
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

bool validTowerLoc(int team, int x, int z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    float towerPos[3];
    towerPos[X] = x;
    towerPos[Y] = 0;
    towerPos[Z] = z;
    if (!inBoundsV(towerPos))
        return false;

    int baseBuff[2] = {5, 5};
    baseBuff[1-team] *= 4;
    for (int a = 0; a < 2; a++)
        if (distanceVector2D(getBasePos(a), towerPos) < baseBuff[a])
            return false;
    //Check if occupied
    for (int a = 0; a < listSize(towerList); a++)
    {
        Tower* other = (Tower*)listGet(towerList, a);
        float otherPos[3];
        for (int b = 0; b < 3; b++)
            otherPos[b] = other->pos[b];
        float dist = distanceVector2D(towerPos, otherPos);
        if (dist < towerBuffer[team])
        {
            //printf("Tower already in that pos\n");
            return false;
        }

    }
    //Dont spawn on top of meteors
    int h = 0, block = 0;
    while ((block = world[x][h++][z]) != 0)
        if (block == METEOR)
            return false;
    //Check if close to base
    if (distanceVector2D(towerPos, getBasePos(team)) <= 15)
        return true;

    for (int a = 0; a < listSize(towerList); a++)
    {
        Tower* other = (Tower*)listGet(towerList, a);
        float otherPos[3];
        for (int b = 0; b < 3; b++)
            otherPos[b] = other->pos[b];
        float dist = distanceVector2D(towerPos, otherPos);
        if (other->team == team) 
            if (dist >= 5 && dist < 15)
                return true;
    }
    return false;
}

bool createTower(int team, int x, int z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{

    if (!validTowerLoc(team, x, z, world))
        return false;
    if (!usePoints(team, TOWER_COST))
        return false;
    //init
    Tower* t = malloc(sizeof(Tower));
    memset(t->aim, 0, sizeof(int)*3);
    t->aim[Z] = 1;
    t->coolDown = TOWER_COOL_DOWN;
    t->team = team;
    t->health = TOWER_HEALTH;

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
    //Redraw stack
    for (int h=t->pos[Y]-TOWER_HEIGHT-3; h < t->pos[Y]; h++)
        world[t->pos[X]][h][t->pos[Z]] = TOWER_A+t->team; 


    float towerPos[3] = {t->pos[0], t->pos[1]-1, t->pos[2]};
    float closestTargetPos[3] = {-1,-1,-1};
    //Look for targets
    getClosestTarget(t->team, TOWER_RANGE, towerPos, closestTargetPos);

    //shoot at target
    if (inBoundsV(closestTargetPos)) {
        if (closestTargetPos[Y] > GROUND_LEVEL+12)
            towerPos[Y] += 3;
        float aim[3];
        memcpy(aim, closestTargetPos, sizeof(float)*3);
        for (int a = 0; a < 3; a++)
            aim[a] -= towerPos[a];
        getUnitVector(aim, aim);
        float proj[3];
        memcpy(proj, towerPos, sizeof(float)*3);
        for (int a = 0; a < 3; a++) {
            proj[a] += aim[a]*2;
            aim[a] *= PROJECTILE_SPEED;
        }
        if (createProjectile(0,t->team, proj, aim))
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

void damageTower(int index, Tower* t, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    t->health -= dmg;
    if (t->health <= 0) {
        //undraw the vehicle
        int c[3];
        memcpy(c, t->pos, sizeof(int)*3);
        for (int h = t->pos[Y]; h >= t->pos[Y]-TOWER_HEIGHT; h--) {
            setWorldBlock(c, world, 0);
            c[Y]-=1;
        }
        for (int h = t->pos[Y]-TOWER_HEIGHT-1; h >= GROUND_LEVEL; h--) {
            setWorldBlock(c, world, HILL+c[Y]-GROUND_LEVEL);
            c[Y]-=1;
        }
        for (int h = GROUND_LEVEL-1; h >= 0; h--) {
            setWorldBlock(c, world, DIRT+c[Y]);
            c[Y]-=1;
        }
        //Create crater
        for (int x = t->pos[X]-1; x <= t->pos[X]+1; x++)
            for (int y = t->pos[Y]; y < t->pos[Y]+3; y++)
                for (int z = t->pos[Z]-1; z <= t->pos[Z]+1; z++)
                        if (inBounds(x,y,z))
                            world[x][y][z] = 0;

        //remove it from the world
        free(listRemove(towerList, index));
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
