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

void damageHeli(int index, Heli* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    v->health -= dmg;
    if (v->health <= 0) {
        //undraw the vehicle
        /*
        setWorldBlockF(v->front, world, 0);
        if (inBoundsV(v->mid))
            setWorldBlockF(v->mid, world, 0);
        if (inBoundsV(v->back))
            setWorldBlockF(v->back, world, 0); 

        //Create crater
        if ((int)v->mid[Y]>1)
            for (int x = (int)v->mid[X]-1; x <= (int)v->mid[X]+1; x++)
                for (int y = (int)v->mid[Y]-1; y <= (int)v->mid[Y]; y++)
                    for (int z = (int)v->mid[Z]-1; z <= (int)v->mid[Z]+1; z++)
                        if (inBounds(x,y,z))
                            world[x][y][z] = 0;
        //Place meteor back in world
        if (v->hasBlock) {
            world[(int)v->mid[X]][(int)v->mid[Y]+1][(int)v->mid[Z]] = 0;
            List* gms = getGroundedMeteors();
            GroundedMeteor* gm = malloc(sizeof(GroundedMeteor));
            memcpy(gm->pos, v->mid, sizeof(float)*3);
            if ((int)v->mid[Y]>1)
                gm->pos[Y] -= 1;
            setWorldBlockF(gm->pos, world, METEOR);
            listAdd(gms, gm);
        }


        //remove it from the world
        free(listRemove(helicopters, index));
        */
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

        //int nextBlock = world[(int)(v->front[X] + v->move[X])][(int)(v->front[Y])][(int)(v->front[Z]+v->move[Z])];

        /*
        //Check if 2 blocks in front are occupied, start climbing
        if (nextBlock != 0) {
            //If we can move on top of block in front 
            if (world[(int)(v->front[X] + v->move[X])][(int)(v->front[Y]+1)][(int)(v->front[Z]+v->move[Z])] == 0) {
                v->front[X] += v->move[X]; 
                v->front[Z] += v->move[Z]; 
            }
            v->front[Y] += 1.0;
        } else {
            //Check if block below is empty (needs to fall down)
            if (world[(int)v->front[X]][(int)v->front[Y]-1][(int)v->front[Z]]==0) {
                v->front[Y] -= 1.0;
            } else {
                v->front[X] += v->move[X]; 
                v->front[Z] += v->move[Z]; 
                if (world[(int)v->front[X]][(int)(v->front[Y]-1.0)][(int)v->front[Z]]==0)
                    v->front[Y] -= 1.0;
            }
        }
        */
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

void moveHeli(Heli* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    List* grounded = getGroundedMeteors();
    int numMeteors = listSize(grounded);
    //TODO look for targets
    /*
    for (int a = 0; a < numMeteors; a++) {
        GroundedMeteor* m = (GroundedMeteor*)listGet(grounded, a);
        if (getWorldBlockF(m->pos, world) == METEOR) {
            if (distanceVector(v->front, m->pos) <= 10 && inBoundsV(m->pos)) {
                memcpy(v->dest, m->pos, sizeof(float)*3);
                v->state = 1;
                return;
            }
        } else {
            free(listRemove(grounded, a));
            a--;
            numMeteors--;
        }
    }
    */
    float dist = moveHeliToDest(v, world, deltaTime);
    //Move vehicle
    if (dist < 0.5)
        generateRandomCord(v->dest);
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
