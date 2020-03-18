/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-016
 * Game programing - Heli class
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
#include "heli.h"
#include "meteor.h"
#include "score.h"
#include "projectile.h"
#include "projectile.h"

List* helicopters;

void initHeli()
{
    helicopters = initList();
}

void createHeli(int team)
{
    if (!usePoints(team, HELI_COST))
        return;
    Heli* h = malloc(sizeof(Heli));
    memcpy(h->pos, getBasePos(team), sizeof(float)*3);
    memcpy(h->dest, h->pos, sizeof(float)*3);
    memset(h->move, 0, sizeof(float)*3);
    h->health = MaxHeliHealth;
    h->cooldown =HELI_COOL_DOWN;
    int direction = 1;
    if (team==1)
        direction = -1;
    h->pos[X] += 0;
    h->pos[Y] += 4;
    h->pos[Z] += 0;
    //6 blocks away from center of 
    h->dest[X] += 6*direction;
    h->team = team;
    listAdd(helicopters, h);
}

void drawHeli(Heli* h, GLubyte world[WORLDX][WORLDY][WORLDZ], int blockType)
{
    //Draw vehicle
    setWorldBlockF(h->pos, world, blockType);
    setWorldBlockCustom(h->pos[X]+1, h->pos[Y], h->pos[Z]+1, world, blockType);
    setWorldBlockCustom(h->pos[X]+1, h->pos[Y], h->pos[Z]-1, world, blockType);
    setWorldBlockCustom(h->pos[X]-1, h->pos[Y], h->pos[Z]+1, world, blockType);
    setWorldBlockCustom(h->pos[X]-1, h->pos[Y], h->pos[Z]-1, world, blockType);
}

void damageHeli(int index, Heli* h, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    h->health -= dmg;
    if (h->health <= 0) {
        //undraw the vehicle
        drawHeli(h, world, 0);

        //remove it from the world
        free(listRemove(helicopters, index));
    }
}

float moveHeliToDest(Heli* h, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    float direction[3], unitDirection[3];
    memcpy(direction, h->dest, sizeof(float)*3);
    direction[X] -= h->pos[X];
    direction[Y] = 0;
    direction[Z] -= h->pos[Z];

    if (vectorLength(direction) < 0.01)
        return 0;

    getUnitVector(unitDirection, direction);
    memcpy(h->currDirection, unitDirection, sizeof(float)*3);
    unitDirection[X] *= HELI_VEL * deltaTime;
    unitDirection[Z] *= HELI_VEL * deltaTime;
    
    h->move[X] += unitDirection[X]; 
    h->move[Z] += unitDirection[Z]; 

    //Move
    if (vectorLength(h->move) >= 1) {
        getUnitVector(h->move, h->move);

        if (h->pos[Y] < HELI_HEIGHT) {
            h->pos[Y] += 1;
        } else {
            h->pos[X] += h->move[X]; 
            h->pos[Z] += h->move[Z]; 

        }
        memset(h->move, 0, sizeof(float)*3);
    }
    return vectorLength(direction);
}

void moveHeli(Heli* h, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    float dist = moveHeliToDest(h, world, deltaTime);
    //Move vehicle
    if (dist < 0.5)
        generateRandomCord(h->dest);

    if (h->cooldown > 0) {
        h->cooldown -= deltaTime;
        return;
    }
    float heliPos[3] = {h->pos[0], h->pos[1]-1, h->pos[2]};
    float closestTargetPos[3] = {-1,-1,-1};
    //Look for targets
    getClosestTarget(h->team, HELI_RANGE, heliPos, closestTargetPos);

    //shoot at target
    if (inBoundsV(closestTargetPos)) {
        float aim[3];
        memcpy(aim, closestTargetPos, sizeof(float)*3);
        for (int a = 0; a < 3; a++)
            aim[a] -= heliPos[a];
        getUnitVector(aim, aim);
        float proj[3];
        memcpy(proj, heliPos, sizeof(float)*3);
        for (int a = 0; a < 3; a++) {
            proj[a] += aim[a]*2;
            aim[a] *= PROJECTILE_SPEED;
        }
        if (createProjectile(0,h->team, proj, aim))
            h->cooldown = HELI_COOL_DOWN;
    }
}

void updateHeli(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    int i = -1;
    int listsize = listSize(helicopters);
    while (++i < listsize)
    {
        Heli* h = (Heli*)listGet(helicopters, i);
        int blockType = HELI_A;
        if (h->team == 1)
            blockType = HELI_B;

        //Undraw vehicle
        drawHeli(h, world, 0);

        //Update state
        moveHeli(h, world, deltaTime);

        //Safety check
        if (!inBoundsV(h->pos)) {
            //remove it from the world
            listRemove(helicopters, i);
            //Spawn a new one
            createHeli(h->team);
            depositPoints(h->team, HELI_COST);
            //Clean it up
            free(h); 
            listsize--;
            i--;
        }

        //Draw vehicle
        drawHeli(h, world, blockType);
    }
}

List* getHeli()
{
    return helicopters;
}

int getNumberOfActiveHeli()
{
    return listSize(helicopters);
}

void freeHeli(void* obj)
{
    free(obj);
}

void endGameHeli()
{
    helicopters = listClear(helicopters, &freeHeli);
}
