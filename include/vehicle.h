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

#define VEHICLE_VEL 5
#define VEHICLE_LOAD_TIME 3.0

#define MaxVehicleHealth 2
#define ProjectileDMG 1

typedef struct vehicle
{
    int team;
    int health;
    int state;
    float dest[3];
    float currDirection[3];
    float front[3];
    float mid[3];
    float back[3];
    float move[3];
    float loadTime;
    bool hasBlock;
} Vehicle;

void initVehicles();
void createVehicle(int team);
void updateVehicles(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void damageVehicle(int index, Vehicle* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ]);
void drawVehicle(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], int blockType);
List* getVehicles();
int getNumberOfActiveVehicles();
void endGameVehicles();

void state0Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void state1Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void state2Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void state3Update(Vehicle* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);

#endif