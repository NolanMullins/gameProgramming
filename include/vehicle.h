/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-01
 * Game programing -  Vehicle class
 ***************************************/

#ifndef __MULLINSN_VEHICLE_H__
#define __MULLINSN_VEHICLE_H__

#include "graphics.h"
#include "world.h"
#include "list.h"

#define VEHICLE_VEL 3

typedef struct vehicle
{
    int team;
    int id;
    int state;
    int dest[3];
    float front[3];
    float mid[3];
    float back[3];
    //float velocity[3];
} Vehicle;

void initVehicles();
void createVehicle(int team);
void updateVehicles(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
List* getVehicles();
int getNumberOfActiveVehicles();
void endGameVehicles();

#endif