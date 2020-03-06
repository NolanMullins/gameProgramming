/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Meteor
 ****************************************/

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
#include "meteor.h"

List* meteors;
List* grounded;
float timer;

void initMeteors()
{
    meteors = initList();
    grounded = initList();
    for (int a = 0; a < INIT_METEORS; a++)
        createMeteor();
    timer = 0;
}

bool checkBoundsMeteor(float pos[3])
{
    static int sizes[3] = {WORLDX, WORLDY, WORLDZ};
    for (int a = 0; a < 3; a++)
        if ((int)pos[a] < 0 || (int)pos[a] >= sizes[a])
            return true;
    return false;
}

bool checkCollisionMeteor(float pos[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    int block = world[(int)pos[X]][(int)pos[Y]][(int)pos[Z]];
    if (block == CLOUD)
        return false;
    if (block != 0)
        return true;
    return false;
}

#define H_RANGE 16
void createMeteor()
{
    Meteor* m = malloc(sizeof(Meteor));
    bool found = false;
    int i = 0; 
    float x,y,z,vx,vy,vz;
    while (!found && i++ < 50)
    {
        x = rand()%WORLDX;
        y = WORLDY-TAIL_SIZE-1;
        z = rand()%WORLDZ;
        vx = rand()%H_RANGE-H_RANGE/2.0;
        vy = rand()%5-8.0;
        vz = rand()%H_RANGE-H_RANGE/2.0;
        float intercept[3];
        intercept[X] = x;
        intercept[Y] = y;
        intercept[Z] = z;
        float scale = (y - GROUND_LEVEL - 2) / (-1.0*vy);
        intercept[X] += vx*scale;
        intercept[Y] += vy*scale;
        intercept[Z] += vz*scale;
        if (!checkBoundsMeteor(intercept))
            break;
    }
    m->pos[X] = x;
    m->pos[Y] = y;
    m->pos[Z] = z;
    m->velocity[X] = vx;
    m->velocity[Y] = vy;
    m->velocity[Z] = vz;
    float unit[3];
    getUnitVector(unit, m->velocity);
    for (int a = 0; a < TAIL_SIZE; a++)
        for (int b = 0; b < 3; b++)
            m->prevPos[a][b] = m->pos[b] - (a+1)*unit[b];
    listAdd(meteors, m);
}

void updateMeteors(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    timer += deltaTime;
    if (timer >= METEOR_WAVE)
    {
        for (int a = 0; a < METEOR_WAVE_SIZE; a++)
            createMeteor();
        timer = 0;
    }
    //Clear previous meteor positions
    for (int a = 0; a < listSize(meteors); a++)
    {
        Meteor* m = (Meteor*)listGet(meteors, a);
        world[(int)m->pos[X]][(int)m->pos[Y]][(int)m->pos[Z]] = 0;
        for (int a = 0; a < 3; a++)
            if (inBoundsV(m->prevPos[a]))
                world[(int)m->prevPos[a][X]][(int)m->prevPos[a][Y]][(int)m->prevPos[a][Z]] = 0;
    }
    //Update position
    int a = -1;
    while (++a < listSize(meteors))
    {
        Meteor* m = (Meteor*)listGet(meteors, a);
        float newPos[3];
        memcpy(newPos, m->pos, sizeof(float)*3);
        newPos[X] += m->velocity[X] * deltaTime;
        newPos[Y] += m->velocity[Y] * deltaTime;
        newPos[Z] += m->velocity[Z] * deltaTime;
        if (checkBoundsMeteor(newPos))
        {
            free(listRemove(meteors, a));
            a--;
        }
        else if (checkCollisionMeteor(newPos, world))
        {
            //Need to check 1 block down to see if it can be moved down after a collision detection
            //This stems from my laggy tablet getting 4fps and sometimes moving the blocks slightly
            //more than 1 block in an update
            int x = (int)m->pos[X];
            int y = (int)m->pos[Y];
            int z = (int)m->pos[Z];
            if (world[x][y-1][z] == 0)
                y--;
            world[x][y][z] = METEOR;
            GroundedMeteor* gm = malloc(sizeof(GroundedMeteor));
            setVector(x+0.5f,y+0.5f,z+0.5f, gm->pos);
            listAdd(grounded, gm);
            free(listRemove(meteors, a));
            a--;
        } 
        else
        {
            memcpy(m->pos, newPos, sizeof(float)*3);
        }
    }
    //write to world
    for (int a = 0; a < listSize(meteors); a++)
    {
        Meteor* m = (Meteor*)listGet(meteors, a);
        float unit[3];
        getUnitVector(unit, m->velocity);
        for (int a = 0; a < TAIL_SIZE; a++)
            for (int b = 0; b < 3; b++)
                m->prevPos[a][b] = m->pos[b] - (a+1)*unit[b];
        for (int a = 0; a < 3; a++)
            world[(int)m->prevPos[a][X]][(int)m->prevPos[a][Y]][(int)m->prevPos[a][Z]] = TAIL+a;
        world[(int)m->pos[X]][(int)m->pos[Y]][(int)m->pos[Z]] = METEOR;
    }
}

List* getGroundedMeteors()
{
    return grounded;
}

List* getMeteors()
{
    return meteors;
}

int getNumberOfActiveMeteors()
{
    return listSize(meteors);
}

void freeMeteor(void* obj)
{
    free(obj);
}

void endGameMeteors()
{
    meteors = listClear(meteors, &freeMeteor);
    grounded = listClear(grounded, &freeMeteor);
}
