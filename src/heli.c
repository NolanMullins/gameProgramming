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
    Heli* v = malloc(sizeof(Heli));
    memcpy(v->front, getBasePos(team), sizeof(float)*3);
    memcpy(v->dest, v->front, sizeof(float)*3);
    memset(v->mid, 0, sizeof(float)*3);
    memset(v->back, 0, sizeof(float)*3);
    memset(v->move, 0, sizeof(float)*3);
    v->health = MaxHeliHealth;
    v->hasBlock = false;
    v->mid[X]=-1;
    v->back[X]=-1;
    v->state = 0;
    int direction = 1;
    if (team==1)
        direction = -1;
    v->front[X] += 3*direction;
    v->front[Y] += 0;
    v->front[Z] += 0;
    //6 blocks away from center of 
    v->dest[X] += 6*direction;
    v->team = team;
    listAdd(helicopters, v);
}

void damageHeli(int index, Heli* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    v->health -= dmg;
    if (v->health <= 0) {
        //undraw the vehicle
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
    }
}

float moveHeliToDest(Heli* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    float direction[3], unitDirection[3];
    memcpy(direction, v->dest, sizeof(float)*3);
    direction[X] -= v->front[X];
    direction[Y] = 0;
    direction[Z] -= v->front[Z];

    if (vectorLength(direction) < 0.01)
        return 0;

    getUnitVector(unitDirection, direction);
    memcpy(v->currDirection, unitDirection, sizeof(float)*3);
    unitDirection[X] *= HELI_VEL * deltaTime;
    unitDirection[Z] *= HELI_VEL * deltaTime;
    
    v->move[X] += unitDirection[X]; 
    v->move[Z] += unitDirection[Z]; 

    if (vectorLength(v->move) >= 1) {
        getUnitVector(v->move, v->move);
        memcpy(v->back, v->mid, sizeof(float)*3);
        memcpy(v->mid, v->front, sizeof(float)*3);

        int nextBlock = world[(int)(v->front[X] + v->move[X])][(int)(v->front[Y])][(int)(v->front[Z]+v->move[Z])];

        //Check if running into a tower
        if (nextBlock == TOWER_A || nextBlock == TOWER_B) {
            //Side step block
            double angle = atan(v->move[Z]/v->move[X]);
            angle += 3.14159265/2.0;
            v->move[X] = 1.0/tan(angle);
            v->move[Z] = tan(angle);
            getUnitVector(v->move, v->move);
            v->front[X] += v->move[X];
            v->front[Z] += v->move[Z];
        }
        //Check if 2 blocks in front are occupied, start climbing
        else if (nextBlock != 0) {
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
        memset(v->move, 0, sizeof(float)*3);
    }
    return vectorLength(direction);
}

void moveHeli(Heli* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    List* grounded = getGroundedMeteors();
    int numMeteors = listSize(grounded);
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
        Heli* v = (Heli*)listGet(helicopters, i);
        int blockType = HELI_A;
        if (v->team == 1)
            blockType = HELI_B;

        //Undraw vehicle
        setWorldBlockF(v->front, world, 0);
        if (inBoundsV(v->mid))
            setWorldBlockF(v->mid, world, 0);
        if (inBoundsV(v->back))
            setWorldBlockF(v->back, world, 0);
        if (v->hasBlock)
            if (inBoundsV(v->mid))
                world[(int)v->mid[X]][(int)v->mid[Y]+1][(int)v->mid[Z]] = 0;

        //Update state
        moveHeli(v, world, deltaTime);

        //Safety check
        if (!inBoundsV(v->front)) {
            //remove it from the world
            listRemove(helicopters, i);
            //Spawn a new one
            createHeli(v->team);
            //Clean it up
            free(v); 
            listsize--;
            i--;
        }

        //Draw vehicle
        setWorldBlockF(v->front, world, blockType);
        if (inBoundsV(v->mid))
            setWorldBlockF(v->mid, world, blockType);
        if (inBoundsV(v->back))
            setWorldBlockF(v->back, world, blockType);
        if (v->hasBlock)
            if (inBoundsV(v->mid))
                world[(int)v->mid[X]][(int)v->mid[Y]+1][(int)v->mid[Z]] = METEOR;
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
