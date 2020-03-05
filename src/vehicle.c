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
#include "meteor.h"

List* vehicles;
//todo
void (*stateFunctions[4]) (Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);

void initVehicles()
{
    //set fnc pointers
    stateFunctions[0] = state0Update;
    stateFunctions[1] = state1Update;
    stateFunctions[2] = state2Update;
    stateFunctions[3] = state3Update;

    vehicles = initList();
    createVehicle(0);
    createVehicle(1);
    createVehicle(0);
    createVehicle(1);
    createVehicle(0);
    createVehicle(1);
    createVehicle(0);
    createVehicle(1);
    createVehicle(0);
    createVehicle(1);
    createVehicle(0);
    createVehicle(1);
    createVehicle(0);
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
    v->health = MaxVehicleHealth;
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
    listAdd(vehicles, v);
}

void damageVehicle(int index, Vehicle* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    v->health -= dmg;
    if (v->health <= 0) {
        //undraw the vehicle
        setWorldBlockF(v->front, world, 0);
        if (inBoundsV(v->mid))
            setWorldBlockF(v->mid, world, 0);
        if (inBoundsV(v->back))
            setWorldBlockF(v->back, world, 0); 
        if (v->hasBlock) {
            world[(int)v->mid[X]][(int)v->mid[Y]+1][(int)v->mid[Z]] = 0;
            List* gms = getGroundedMeteors();
            GroundedMeteor* gm = malloc(sizeof(GroundedMeteor));
            memcpy(gm->pos, v->mid, sizeof(float)*3);
            gm->pos[Y] -= 1;
            setWorldBlockF(gm->pos, world, METEOR);
            listAdd(gms, gm);
        }
        //Create crater
        for (int x = (int)v->mid[X]-1; x <= (int)v->mid[X]+1; x++)
            for (int y = (int)v->mid[Y]-1; y <= (int)v->mid[Y]; y++)
                for (int z = (int)v->mid[Z]-1; z <= (int)v->mid[Z]+1; z++)
                    if (inBounds(x,y,z))
                        world[x][y][z] = 0;

        //remove it from the world
        listRemove(vehicles, index);
        //Spawn a new one
        createVehicle(v->team);
        //Clean it up
        free(v);
    }
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
    memcpy(v->currDirection, unitDirection, sizeof(float)*3);
    unitDirection[X] *= VEHICLE_VEL * deltaTime;
    unitDirection[Z] *= VEHICLE_VEL * deltaTime;
    
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

void generateRandomCord(float dest[3])
{
    dest[X] = rand()%(WORLDX-10)+5;
    dest[Z] = rand()%(WORLDZ-10)+5;
}

//Search for meteors
void state0Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    List* grounded = getGroundedMeteors();
    int numMeteors = listSize(grounded);
    for (int a = 0; a < numMeteors; a++) {
        GroundedMeteor* m = (GroundedMeteor*)listGet(grounded, a);
        if (getWorldBlockF(m->pos, world) == METEOR) {
            if (distanceVector(v->front, m->pos) <= 10) {
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
    float dist = moveVehicleToDest(v, world, deltaTime);
    //Move vehicle
    if (dist < 0.5)
        generateRandomCord(v->dest);
}

//Move towards meteors
void state1Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    //Check if meteors is still there
    if (getWorldBlockF(v->dest, world) != METEOR) {
        v->state = 0;
        generateRandomCord(v->dest);
        return;
    }

    //Move vehicle towards meteor
    float dist = moveVehicleToDest(v, world, deltaTime);

    //Is it at meteor?
    if (dist < 1.51) {
        v->state = 2;
        v->loadTime = VEHICLE_LOAD_TIME;
    }
}

//Load meteor
void state2Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    //Check if meteor is still there
    if (getWorldBlockF(v->dest, world) != METEOR) {
        v->state = 0;
        generateRandomCord(v->dest);
        return;
    }

    //Update load clock
    v->loadTime -= deltaTime;

    //Are we done loading?
    if (v->loadTime <= 0) {
        List* grounded = getGroundedMeteors();
        int numMeteors = listSize(grounded);
        for (int a = 0; a < numMeteors; a++) {
            GroundedMeteor* m = (GroundedMeteor*)listGet(grounded, a);
            if ((int)m->pos[X] == (int)v->dest[X] && (int)m->pos[Z] == (int)v->dest[Z]) {
                free(listRemove(grounded, a));
                break;
            }
        }
        setWorldBlockF(v->dest, world, 0);
        memcpy(v->dest, getBasePos(v->team), sizeof(float)*3);
        v->state = 3;
        v->hasBlock = true;
    }
}

//Return to base
void state3Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    //move towards base
    float dist = moveVehicleToDest(v, world, deltaTime);
    float adjacent[3];
    memcpy(adjacent, v->front, sizeof(float)*3);
    adjacent[X] += v->currDirection[X];
    adjacent[Z] += v->currDirection[Z];
    if (inBoundsV(adjacent)) {
        int adjacentBlock = world[(int)adjacent[X]][(int)adjacent[Y]][(int)adjacent[Z]];
        //Check if vehicle is on top of the base
        if (adjacentBlock == 0)
            adjacentBlock = world[(int)adjacent[X]][(int)adjacent[Y]-1][(int)adjacent[Z]];
        if (v->team == 0 && adjacentBlock == BASEA) {
            v->hasBlock = false;
            v->state = 0;
            generateRandomCord(v->dest);
        } else if (v->team == 1 & adjacentBlock == BASEB) {
            v->hasBlock = false;
            v->state = 0;
            generateRandomCord(v->dest);
        }
    }
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
        if (v->hasBlock)
            world[(int)v->mid[X]][(int)v->mid[Y]+1][(int)v->mid[Z]] = 0;

        //Update state
        (*stateFunctions[v->state])(v, world, deltaTime);

        //Draw vehicle
        setWorldBlockF(v->front, world, blockType);
        if (inBoundsV(v->mid))
            setWorldBlockF(v->mid, world, blockType);
        if (inBoundsV(v->back))
            setWorldBlockF(v->back, world, blockType);
        if (v->hasBlock)
            world[(int)v->mid[X]][(int)v->mid[Y]+1][(int)v->mid[Z]] = METEOR;
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
