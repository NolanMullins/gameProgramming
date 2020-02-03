/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing -  Projectile class
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
#include "projectile.h"

List* objs;

extern void createMob(int, float, float, float, float);
extern void setMobPosition(int, float, float, float, float);
extern void hideMob(int);
extern void showMob(int);

void initProjectiles()
{
    objs = initList();
}

bool collision(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    if (x < 0 || x > WORLDX || y < 0 || y > WORLDY || z < 0 || z > WORLDZ)
        return false;
    if (world[(int)x][(int)y][(int)z] > 0)
        return true;
    return false;
}

bool collisionHelper(float pos[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    if (collision(pos[X], pos[Y], pos[Z], world))
        return true;
    if (collision(pos[X], pos[Y]+0.5, pos[Z], world)) {
        pos[Y]+=0.5;
        return true;
    }
    if (collision(pos[X], pos[Y]-0.5, pos[Z], world)) {
        pos[Y]-=0.5;
        return true;
    }
    if (collision(pos[X]+0.5, pos[Y], pos[Z], world)) {
        pos[X]+=0.5;
        return true;
    }
    if (collision(pos[X]-0.5, pos[Y], pos[Z], world)) {
        pos[X]-=0.5;
        return true;
    }
    if (collision(pos[X], pos[Y], pos[Z]+0.5, world)) {
        pos[Z]+=0.5;
        return true;
    }
    if (collision(pos[X], pos[Y], pos[Z]-0.5, world)) {
        pos[Z]-=0.5;
        return true;
    }
    return false;
}

#define RES 4
bool getCollision(float pos[3], float vel[3], float collision[3], GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    memset(collision, 0, sizeof(int)*3);
    memcpy(collision, pos, sizeof(float)*3);
    //Check if the block is empty
    //if (collisionHelper(collision, world))
        //return true;
    for (int i = 0; i < 3; i++)
        collision[i] -= vel[i]*deltaTime - 0.5;
    for (int a = 0; a < RES; a++)
    {
        for (int i = 0; i < 3; i++)
            collision[i] += vel[i] * deltaTime / RES;
        if (collisionHelper(collision, world))
            return true;
    }
    /*#define rad 0.25
    for (int i = 0; i < 3; i++)
    {
        collision[i] += rad;
        if (collisionHelper(collision, world))
            return true;
        collision[i] -= 2*rad;
        if (collisionHelper(collision, world))
            return true;
        collision[i] += rad;
    }*/
    return false;
}

int getAvailableID()
{
    int id = -1; 
    bool inUse = true;
    while (inUse && id < MOB_COUNT)
    {
        id++;
        inUse = false;
        for (int i = 0; i < listSize(objs); i++)
        {
            if (((Projectile*)listGet(objs, i))->id == id)
            {
                inUse = true;
                break;
            }
        }
    }
    if (id == MOB_COUNT)
        return -1;
    return id;
}

void createProjectile(int type, float spawnLocation[3], float velocity[3])
{
    int id = getAvailableID();
    if (id == -1)
    {
        printf("No space for more projectiles\n");
        return;
    }
    Projectile* obj = malloc(sizeof(Projectile));
    memcpy(obj->pos, spawnLocation, sizeof(float)*3);
    memcpy(obj->velocity, velocity, sizeof(float)*3);
    obj->type = type;
    obj->id = id;
    createMob(id, obj->pos[X], obj->pos[Y], obj->pos[Z], 0);
    showMob(id);
    listAdd(objs, obj);
}

void updateProjectiles(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    int i = -1;
    while (++i < listSize(objs))
    {
        Projectile* obj = (Projectile*)listGet(objs, i);
        obj->pos[X] += obj->velocity[X] * deltaTime;
        obj->pos[Y] += obj->velocity[Y] * deltaTime;
        obj->pos[Z] += obj->velocity[Z] * deltaTime;
        obj->velocity[Y] -= GRAVITY * deltaTime;

        //Check to see if its in the world bounds
        float c[3];
        if (obj->pos[X] >= WORLDX || obj->pos[Y] >= WORLDY || obj->pos[Z] >= WORLDZ ||
        obj->pos[X] < 0 || obj->pos[Y] < 0 || obj->pos[Z] < 0)
        {
            hideMob(obj->id);
            free(listRemove(objs, i));
            i--;
        }
        else if (getCollision(obj->pos, obj->velocity, c, world, deltaTime))
        {
            int block = world[(int)c[X]][(int)c[Y]][(int)c[Z]];
            if (block != BASEA && block != BASEB)
                world[(int)c[X]][(int)c[Y]][(int)c[Z]] = 0;
            hideMob(obj->id);
            free(listRemove(objs, i));
            i--;
        }
        else
        {
            setMobPosition(obj->id, obj->pos[X], obj->pos[Y], obj->pos[Z], 0);
        }
        
    }
}

List* getProjectiles()
{
    return objs;
}

int getNumberOfActiveProjectiles()
{
    return listSize(objs);
}

void freeProjectile(void* obj)
{
    free(obj);
}

void endGameProjectiles()
{
    objs = listClear(objs, &freeProjectile);
}
