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
#include "utils.h"
#include "projectile.h"
#include "vehicle.h"
#include "tower.h"
#include "score.h"
#include "tank.h"
#include "heli.h"

List* projectiles;

extern void createMob(int, float, float, float, float);
extern void setMobPosition(int, float, float, float, float);
extern void hideMob(int);
extern void showMob(int);

#define TOWER_RAD 2.5

void initProjectiles()
{
    projectiles = initList();
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

    for (int i = 0; i < 3; i++)
        collision[i] -= vel[i]*deltaTime - 0.5;
    for (int a = 0; a < RES; a++)
    {
        for (int i = 0; i < 3; i++)
            collision[i] += vel[i] * deltaTime / RES;
        if (collisionHelper(collision, world))
            return true;
    }
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
        for (int i = 0; i < listSize(projectiles); i++)
        {
            if (((Projectile*)listGet(projectiles, i))->id == id)
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

bool createProjectile(int type, int team, float spawnLocation[3], float velocity[3])
{
    int id = getAvailableID();
    if (id == -1)
    {
        printf("No space for more projectiles\n");
        return false;
    }
    Projectile* obj = malloc(sizeof(Projectile));
    memcpy(obj->pos, spawnLocation, sizeof(float)*3);
    memcpy(obj->velocity, velocity, sizeof(float)*3);
    obj->type = type;
    obj->id = id;
    obj->team = team;
    createMob(id, obj->pos[X], obj->pos[Y], obj->pos[Z], 0);
    showMob(id);
    listAdd(projectiles, obj);
    return true;
}

bool checkVehicleCollision(Vehicle* v, float c[3])
{
    return (occupySameBlock(v->front, c) || occupySameBlock(v->mid, c) || occupySameBlock(v->back, c));
}

bool checkTankCollision(Tank* t, float c[3])
{
    //check front 
    for (int a = 0; a < 2; a++) {
        if (occupySameBlockCustom(t->front[X], t->front[Y]+a, t->front[Z], c))
            return true;
        if (occupySameBlockCustom(t->front[X]-isNegUtil(t->currDirection[X]), t->front[Y]+a, t->front[Z], c))
            return true;
        if (occupySameBlockCustom(t->front[X], t->front[Y]+a, t->front[Z]-isNegUtil(t->currDirection[Z]), c))
            return true;
        if (occupySameBlockCustom(t->front[X]-isNegUtil(t->currDirection[X]), t->front[Y]+a, t->front[Z]-isNegUtil(t->currDirection[Z]), c))
            return true;

        if (inBoundsV(t->back)) {
            if (occupySameBlockCustom(t->back[X], t->back[Y], t->back[Z], c))
                return true;
            if (occupySameBlockCustom(t->back[X]-isNegUtil(t->currDirection[X]), t->back[Y], t->back[Z], c))
                return true;
            if (occupySameBlockCustom(t->back[X], t->back[Y], t->back[Z]-isNegUtil(t->currDirection[Z]), c))
                return true;
            if (occupySameBlockCustom(t->back[X]-isNegUtil(t->currDirection[X]), t->back[Y], t->back[Z]-isNegUtil(t->currDirection[Z]), c))
                return true;
        }
    }
    return false;
}

bool checkHeliCollision(Heli* h, float c[3])
{
    return (distanceVector2D(h->pos, c) < 2.5);
}

void collideWithTower(int block, Projectile* obj, float c[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    Node* towerNode = getTowers()->list;
    if (towerNode == NULL)
        return;
    Tower* t = (Tower*)towerNode->data;
    if (block == TOWER_A+obj->team)
        return;

    float tmp[3] = {t->pos[X], t->pos[Y], t->pos[Z]};
    int a = 0;
    if (distanceVector2D(tmp, c) < TOWER_RAD) {
        damageTower(a, t, ProjectileDMG, world);
    } else  {
        while ((towerNode = towerNode->next) != NULL) {
            t = (Tower*)towerNode->data;
            a++;
            float tPos[3] = {t->pos[X], t->pos[Y], t->pos[Z]};
            if (distanceVector2D(tPos, c) < TOWER_RAD) {
                damageTower(a, t, ProjectileDMG, world);
                break;
            }
        }
    }
}

void collideWithVehicle(int block, Projectile* obj, float c[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    Node* vics = getVehicles()->list;
    if (vics == NULL)
        return;
    Vehicle* v = (Vehicle*)vics->data;
    if (block == VEHICLE_A+obj->team)
        return;
    int a = 0;
    if (checkVehicleCollision(v, c)) 
        damageVehicle(a, v, ProjectileDMG, world);
    else 
        while ((vics = vics->next) != NULL) {
            v = (Vehicle*)vics->data;
            a++;
            if (checkVehicleCollision(v, c)) {
                damageVehicle(a, v, ProjectileDMG, world);
                break;
            }
        }
}

void collideWithTank(int block, Projectile* obj, float c[3], GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    Node* tanks = getTanks()->list;
    if (tanks == NULL)
        return;
    Tank* t = (Tank*)tanks->data;
    if (block == TANK_A+obj->team)
        return;
    int a = 0;
    if (checkTankCollision(t, c)) 
        damageTank(a, t, ProjectileDMG, world);
    else 
        while ((tanks = tanks->next) != NULL) {
            t = (Tank*)tanks->data;
            a++;
            if (checkTankCollision(t, c)) {
                damageTank(a, t, ProjectileDMG, world);
                break;
            }
        }
}

void collideWithHeli(int block, Projectile* obj, float c[3], GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    Node* helis = getHeli()->list;
    if (helis == NULL)
        return;
    Heli* h = (Heli*)helis->data;
    if (block == HELI_A+obj->team)
        return;
    int a = 0;
    if (checkHeliCollision(h, c)) 
        damageHeli(a, h, ProjectileDMG, world);
    else 
        while ((helis = helis->next) != NULL) {
            h = (Heli*)helis->data;
            a++;
            if (checkHeliCollision(h, c)) {
                damageHeli(a, h, ProjectileDMG, world);
                break;
            }
        }
}

void updateProjectiles(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    int i = -1;
    while (++i < listSize(projectiles))
    {
        Projectile* obj = (Projectile*)listGet(projectiles, i);
        obj->pos[X] += obj->velocity[X] * deltaTime;
        obj->pos[Y] += obj->velocity[Y] * deltaTime;
        obj->pos[Z] += obj->velocity[Z] * deltaTime;
        if (APPLY_GRAVITY)
            obj->velocity[Y] -= GRAVITY * deltaTime;

        //Check to see if its in the world bounds
        float c[3];
        if (!inBoundsV(obj->pos))
        {
            hideMob(obj->id);
            free(listRemove(projectiles, i));
            i--;
        }
        else if (getCollision(obj->pos, obj->velocity, c, world, deltaTime))
        {
            int block = getWorldBlockF(c, world);
            if (block == TOWER_A || block == TOWER_B) {
                collideWithTower(block, obj, c, world);
            }
            else if (block == VEHICLE_A || block == VEHICLE_B) {
                collideWithVehicle(block, obj, c, world);
            }
            else if (block == TANK_A || block == TANK_B) {
                collideWithTank(block, obj, c, world);
            }
            else if (block == HELI_A || block == HELI_B) {
                collideWithHeli(block, obj, c, world);
            }
            //Remove block from the world
            //int block = world[(int)c[X]][(int)c[Y]][(int)c[Z]];
            //if (block != BASEA && block != BASEB)
                //world[(int)c[X]][(int)c[Y]][(int)c[Z]] = 0;
            hideMob(obj->id);
            free(listRemove(projectiles, i));
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
    return projectiles;
}

int getNumberOfActiveProjectiles()
{
    return listSize(projectiles);
}

void freeProjectile(void* obj)
{
    free(obj);
}

void endGameProjectiles()
{
    projectiles = listClear(projectiles, &freeProjectile);
}
