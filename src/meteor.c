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
float timer;

void initMeteors()
{
    meteors = initList();
    for (int a = 0; a < INIT_METEORS; a++)
        createMeteor();
    timer = 0;
}

#define H_RANGE 12
void createMeteor()
{
    Meteor* m = malloc(sizeof(Meteor));
    m->pos[X] = rand()%WORLDX;
    m->pos[Y] = WORLDY-TAIL_SIZE-1;
    m->pos[Z] = rand()%WORLDZ;
    m->velocity[X] = rand()%H_RANGE-H_RANGE/2.0;
    m->velocity[Y] = rand()%5-8.0;
    m->velocity[Z] = rand()%H_RANGE-H_RANGE/2.0;
    float unit[3];
    getUnitVector(unit, m->velocity);
    for (int a = 0; a < TAIL_SIZE; a++)
        for (int b = 0; b < 3; b++)
            m->prevPos[a][b] = m->pos[b] - (a+1)*unit[b];
    listAdd(meteors, m);
}

bool checkBoundsMeteor(float pos[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
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
        if (checkBoundsMeteor(newPos, world))
        {
            free(listRemove(meteors, a));
            a--;
        }
        else if (checkCollisionMeteor(newPos, world))
        {
            //Need to check 1 block down to see if it can be moved down after a collision detection
            //This stems from my laggy tablet getting 4fps and sometimes moving the blocks slightly
            //more than 1 block in an update
            if (world[(int)m->pos[X]][(int)m->pos[Y]-1][(int)m->pos[Z]] == 0)
            {
                world[(int)m->pos[X]][(int)m->pos[Y]-1][(int)m->pos[Z]] = METEOR;
            }
            else
            {
                world[(int)m->pos[X]][(int)m->pos[Y]][(int)m->pos[Z]] = METEOR;
            }
            
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
        world[(int)m->pos[X]][(int)m->pos[Y]][(int)m->pos[Z]] = METEOR;
        for (int a = 0; a < 3; a++)
            world[(int)m->prevPos[a][X]][(int)m->prevPos[a][Y]][(int)m->prevPos[a][Z]] = TAIL+a;
    }
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
}
