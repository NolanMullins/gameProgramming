/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-01
 * Game programing -  Vehicle class
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

List* vehicles;

void initVehicles()
{
    vehicles = initList();
    createVehicle(0);
    createVehicle(0);
    createVehicle(0);
    createVehicle(0);
    createVehicle(0);
    createVehicle(0);
    createVehicle(1);
    createVehicle(1);
    createVehicle(1);
    createVehicle(1);
    createVehicle(1);
    createVehicle(1);
}

void createVehicle(int team)
{
    Vehicle* v = malloc(sizeof(Vehicle));
    memcpy(v->front, getBasePos(team), sizeof(float)*3);
    memcpy(v->dest, v->front, sizeof(float)*3);
    memset(v->mid, 0, sizeof(float)*3);
    memset(v->back, 0, sizeof(float)*3);
    memset(v->move, 0, sizeof(float)*3);
    v->mid[X]=-1;
    v->back[X]=-1;
    v->state = -1;
    int direction = 1;
    if (team==1)
        direction = -1;
    v->front[X] += 3*direction;
    v->front[Y] += 0.5f;
    v->front[Z] += 0;
    //6 blocks away from center of 
    v->dest[X] += 6*direction;
    v->team = team;
    listAdd(vehicles, v);
}

float moveVehicleToDest(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    float direction[3], unitDirection[3];
    memcpy(direction, v->dest, sizeof(float)*3);
    direction[X] -= v->front[X];
    direction[Y] = 0;
    direction[Z] -= v->front[Z];

    if (vectorLength(direction) < 0.01)
        return 0;

    getUnitVector(unitDirection, direction);
    unitDirection[X] *= VEHICLE_VEL * deltaTime;
    unitDirection[Z] *= VEHICLE_VEL * deltaTime;
    
    v->move[X] += unitDirection[X]; 
    v->move[Z] += unitDirection[Z]; 

    if (vectorLength(v->move) >= 1) {
        getUnitVector(v->move, v->move);
        memcpy(v->back, v->mid, sizeof(float)*3);
        memcpy(v->mid, v->front, sizeof(float)*3);

        //Check if 2 blocks in front are occupied, start climbing
        if (world[(int)(v->front[X] + v->move[X])][(int)(v->front[Y])][(int)(v->front[Z]+v->move[Z])] != 0) {
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

void generateRandomCord(float dest[3])
{
    dest[X] = rand()%(WORLDX-10)+5;
    dest[Z] = rand()%(WORLDZ-10)+5;
}

void updateVehicles(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    int i = -1;
    while (++i < listSize(vehicles))
    {
        Vehicle* v = (Vehicle*)listGet(vehicles, i);
        int blockType = VEHICLE_A;
        if (v->team == 1)
            blockType = VEHICLE_B;

        //Undraw vehicle
        setWorldBlockF(v->front, world, 0);
        if (inBoundsV(v->mid))
            setWorldBlockF(v->mid, world, 0);
        if (inBoundsV(v->back))
            setWorldBlockF(v->back, world, 0);

        //Move vehicle
        float dist = moveVehicleToDest(v, world, deltaTime);
        if (dist < 0.5)
            generateRandomCord(v->dest);

        //Draw vehicle
        setWorldBlockF(v->front, world, blockType);
        if (inBoundsV(v->mid))
            setWorldBlockF(v->mid, world, blockType);
        if (inBoundsV(v->back))
            setWorldBlockF(v->back, world, blockType);
        /*
        obj->pos[X] += obj->velocity[X] * deltaTime;
        obj->pos[Y] += obj->velocity[Y] * deltaTime;
        obj->pos[Z] += obj->velocity[Z] * deltaTime;
        */
    }
}

List* getVehicles()
{
    return vehicles;
}

int getNumberOfActiveVehicles()
{
    return listSize(vehicles);
}

void freeVehicle(void* obj)
{
    free(obj);
}

void endGameVehicles()
{
    vehicles = listClear(vehicles, &freeVehicle);
}
