/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-16
 * Game programing - Tank class
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
#include "tank.h"
#include "meteor.h"
#include "score.h"

List* tanks;

void initTanks()
{
    tanks = initList();
}

void createTank(int team)
{
    if (!usePoints(team, TANK_COST))
        return;
    Tank* t = malloc(sizeof(Tank));
    memcpy(t->front, getBasePos(team), sizeof(float)*3);
    memcpy(t->dest, t->front, sizeof(float)*3);
    memset(t->mid, 0, sizeof(float)*3);
    memset(t->back, 0, sizeof(float)*3);
    memset(t->move, 0, sizeof(float)*3);
    t->health = MaxTankHealth;
    t->mid[X]=-1;
    t->back[X]=-1;
    int direction = 1;
    if (team==1)
        direction = -1;
    t->front[X] += 4*direction;
    t->front[Y] += 0;
    t->front[Z] += 0;
    //6 blocks away from center of 
    t->dest[X] += 6*direction;
    t->team = team;
    listAdd(tanks, t);
}

void damageTank(int index, Tank* v, int dmg, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    v->health -= dmg;
    if (v->health <= 0) {
        /*undraw the vehicle
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
        free(listRemove(tanks, index));
        */
    }
}

float isNegUtil(float tmp)
{
    if (tmp>=0)
        return 1.0;
    return -1.0;
}

bool isGroundBelow(float x, float y, float z, float dir[3], GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    if (getWorldBlockCustom(x, y-1, z, world)!=0)
        return true;
    if (getWorldBlockCustom(x-isNegUtil(dir[X]), y-1, z, world)!=0)
        return true;
    if (getWorldBlockCustom(x, y-1, z-isNegUtil(dir[Z]), world)!=0)
        return true;
    if (getWorldBlockCustom(x-isNegUtil(dir[X]), y-1, z-isNegUtil(dir[Z]), world)!=0)
        return true;
    return false;
}

bool isGroundBelowV(float pos[3], float dir[3], GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    return isGroundBelow(pos[X], pos[Y], pos[Z], dir, world);
}

float moveTankToDest(Tank* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
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
    unitDirection[X] *= TANK_VEL * deltaTime;
    unitDirection[Z] *= TANK_VEL * deltaTime;
    
    v->move[X] += unitDirection[X]; 
    v->move[Z] += unitDirection[Z]; 

    if (vectorLength(v->move) >= 1) {
        getUnitVector(v->move, v->move);
        memcpy(v->back, v->mid, sizeof(float)*3);
        memcpy(v->mid, v->front, sizeof(float)*3);
        int diff[3];
        diff[Z] = ((int)(v->front[X]+v->move[X]) - (int)v->front[X])*(-1);
        diff[X] = ((int)(v->front[Z]+v->move[Z]) - (int)v->front[Z])*(-1);

        int nextBlock = world[(int)(v->front[X] + v->move[X])][(int)(v->front[Y])][(int)(v->front[Z]+v->move[Z])];
        int nextBlock2 = world[(int)(v->front[X] + v->move[X])+diff[X]][(int)(v->front[Y])][(int)(v->front[Z]+v->move[Z])+diff[Z]];

//TODO, modify for double block

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
        } else if (nextBlock2 == TOWER_A || nextBlock2 == TOWER_B) {
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
        else if (nextBlock != 0 || nextBlock2 != 0) {
            //If we can move on top of block in front 
            if (world[(int)(v->front[X] + v->move[X])][(int)(v->front[Y]+1)][(int)(v->front[Z]+v->move[Z])] == 0) {
                v->front[X] += v->move[X]; 
                v->front[Z] += v->move[Z]; 
            }
            v->front[Y] += 1.0;
        } else {
            //TODO check all 4 blocks below
            //Check if block below is empty (needs to fall down)
            if (!isGroundBelowV(v->front, v->currDirection, world)) {
                v->front[Y] -= 1.0;
            } else {
                v->front[X] += v->move[X]; 
                v->front[Z] += v->move[Z]; 
                if (!isGroundBelowV(v->front, v->currDirection, world))
                    v->front[Y] -= 1.0;
            }
        }
        memset(v->move, 0, sizeof(float)*3);
    }
    return vectorLength(direction);
}

//Search for meteors
void moveTank(Tank* v, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    //TODO look for things to shoot
    /*
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
    }*/
    float dist = moveTankToDest(v, world, deltaTime);
    //Move vehicle
    if (dist < 0.5)
        generateRandomCord(v->dest);
}

void setBlockHelper(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ], int blockType)
{
    int block = getWorldBlockCustom(x,y,z,world);
    if (blockType > 0) {
        if (block > 0)
            if (block != TANK_A && block != TANK_B)
                return;
    } else {
        if (block != TANK_A && block != TANK_B && block != SELECTED)
            return;
    }
    if (inBounds(x,y,z))
        setWorldBlockCustom(x,y,z,world,blockType);
}

void drawTank(Tank* t, GLubyte world[WORLDX][WORLDY][WORLDZ], int blockType)
{
    //Draw vehicle
    setBlockHelper(t->front[X], t->front[Y], t->front[Z], world, blockType);
    setBlockHelper(t->front[X]-isNegUtil(t->currDirection[X]), t->front[Y], t->front[Z], world, blockType);
    setBlockHelper(t->front[X], t->front[Y], t->front[Z]-isNegUtil(t->currDirection[Z]), world, blockType);
    setBlockHelper(t->front[X]-isNegUtil(t->currDirection[X]), t->front[Y], t->front[Z]-isNegUtil(t->currDirection[Z]), world, blockType);

    setBlockHelper(t->front[X], t->front[Y]+1, t->front[Z], world, blockType);
    setBlockHelper(t->front[X]-isNegUtil(t->currDirection[X]), t->front[Y]+1, t->front[Z], world, blockType);
    setBlockHelper(t->front[X], t->front[Y]+1, t->front[Z]-isNegUtil(t->currDirection[Z]), world, blockType);
    setBlockHelper(t->front[X]-isNegUtil(t->currDirection[X]), t->front[Y]+1, t->front[Z]-isNegUtil(t->currDirection[Z]), world, blockType);
    //Fin
    //setWorldBlockCustom(t->front[X]-isNegUtil(t->currDirection[X]), t->front[Y]+1, t->front[Z]-isNegUtil(t->currDirection[Z]), world, blockType);
    if (inBoundsV(t->back)) {
        setBlockHelper(t->back[X], t->back[Y], t->back[Z], world, blockType);
        setBlockHelper(t->back[X]-isNegUtil(t->currDirection[X]), t->back[Y], t->back[Z], world, blockType);
        setBlockHelper(t->back[X], t->back[Y], t->back[Z]-isNegUtil(t->currDirection[Z]), world, blockType);
        setBlockHelper(t->back[X]-isNegUtil(t->currDirection[X]), t->back[Y], t->back[Z]-isNegUtil(t->currDirection[Z]), world, blockType);

        setBlockHelper(t->back[X], t->back[Y]+1, t->back[Z], world, blockType);
        setBlockHelper(t->back[X]-isNegUtil(t->currDirection[X]), t->back[Y]+1, t->back[Z], world, blockType);
        setBlockHelper(t->back[X], t->back[Y]+1, t->back[Z]-isNegUtil(t->currDirection[Z]), world, blockType);
        setBlockHelper(t->back[X]-isNegUtil(t->currDirection[X]), t->back[Y]+1, t->back[Z]-isNegUtil(t->currDirection[Z]), world, blockType);

        //Fin
        //setWorldBlockCustom(t->back[X], t->back[Y]+1, t->back[Z], world, blockType);
    }
}

void updateTanks(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    int i = -1;
    int listsize = listSize(tanks);
    while (++i < listsize)
    {
        Tank* v = (Tank*)listGet(tanks, i);
        int blockType = TANK_A;
        if (v->team == 1)
            blockType = TANK_B;

        //Undraw vehicle
        drawTank(v, world, 0);

        //Update state
        moveTank(v, world, deltaTime);

        //Safety check
        if (!inBoundsV(v->front)) {
            //remove it from the world
            listRemove(tanks, i);
            //Spawn a new one
            createTank(v->team);
            depositPoints(v->team, TANK_COST);
            //Clean it up
            free(v); 
            listsize--;
            i--;
        }

        drawTank(v, world, blockType);
    }
}

List* getTanks()
{
    return tanks;
}

int getNumberOfActiveTanks()
{
    return listSize(tanks);
}

void freeTank(void* obj)
{
    free(obj);
}

void endGameTanks()
{
    tanks = listClear(tanks, &freeTank);
}
