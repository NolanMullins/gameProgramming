/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Utils
 ****************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "utils.h"
#include "tower.h"
#include "vehicle.h"
#include "tank.h"
#include "heli.h"

extern void draw2Dline(int, int, int, int, int);
extern void draw2Dbox(int, int, int, int);
extern void draw2Dtriangle(int, int, int, int, int, int);
extern void set2Dcolour(float[]);


void printVector(float vec[3])
{
    printf("%.1f %.1f %.1f\n", vec[0], vec[1], vec[2]);
}

void printVectorCustom(float x, float y, float z)
{
    printf("%.1f %.1f %.1f\n", x, y, z);
}

void rotateVector(float v[3], float radians)
{
    //double angle = atan(v[Z]/v[X]);
    //angle += radians;
    v[X] = v[X]*cos(radians) - v[Z]*sin(radians);
    v[Z] = v[X]*sin(radians) + v[Z]*cos(radians);
}

float vectorLength(float vec[3])
{
    return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

void getUnitVector(float unitVec[3], float vec[3])
{
    float l = vectorLength(vec);
    for (int i = 0; i < 3; i++)
        unitVec[i] = vec[i]/l; 
}

void setVector(float x, float y, float z, float vec[3])
{
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

float distanceVector(float a[3], float b[3])
{
    float c[3];
    for (int i = 0; i < 3; i++)
        c[i] = a[i] - b[i];
    return vectorLength(c);
}

float distanceVector2D(float a[3], float b[3])
{
    float c[3];
    c[0] = a[0]-b[0];
    c[1] = 0;
    c[2] = a[2]-b[2];
    return vectorLength(c);
}

float isNegUtil(float tmp)
{
    if (tmp>=0)
        return 1.0;
    return -1.0;
}

bool occupySameBlockCustom(float x, float y, float z, float v2[3])
{
    return ((int)x == (int)v2[X] && (int)y == (int)v2[Y] && (int)z == (int)v2[Z]);
}

bool occupySameBlock(float v1[3], float v2[3])
{
    for (int a = 0; a < 3; a ++)
        if ((int)v1[a] != (int)v2[a])
            return false;
    return true;
}

bool inBoundsV(float pos[3])
{
    return inBounds(pos[X], pos[Y], pos[Z]);
}

bool inBounds(float x, float y, float z)
{
    if ((int)x < 0 || (int)x > WORLDX || (int)y < 0 || (int)y > WORLDY || (int)z < 0 || (int)z > WORLDZ)
        return false;
    return true;
}

bool collisionV(float pos[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    return collision(pos[X], pos[Y], pos[Z], world);
}

bool collision(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    if (!inBounds(x,y,z))
        return false;
    if (world[(int)x][(int)y][(int)z] > 0)
        return true;
    return false;
}

int getWorldBlockF(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    return world[(int)loc[X]][(int)loc[Y]][(int)loc[Z]];
}

int getWorldBlock(int loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    return world[loc[X]][loc[Y]][loc[Z]];
}

int getWorldBlockCustom(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    return world[(int)x][(int)y][(int)z];
}

void setWorldBlockF(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block)
{
    world[(int)loc[X]][(int)loc[Y]][(int)loc[Z]] = block;
}

void setWorldBlock(int loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block)
{
    world[loc[X]][loc[Y]][loc[Z]] = block;
}

void setWorldBlockCustom(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ], int block)
{
    if (inBounds(x,y,z))
        world[(int)x][(int)y][(int)z] = block;
    else 
        printVectorCustom(x,y,z);
}

void generateRandomCord(float dest[3])
{
    dest[X] = rand()%(WORLDX-10)+5;
    dest[Z] = rand()%(WORLDZ-10)+5;
}

void pauseGame(bool p)
{
    paused = p;
}

bool isPaused()
{
    return paused;
}

void getClosestTarget(int team, float range, float pos[3], float closestTargetPos[3]) 
{
    //Look for targets
    List* targetList = getVehicles();
    for (int a = 0; a < 3; a++)
        closestTargetPos[a] = -1;
    float targetDist = 100;
    //float towerPos[3] = {t->pos[0], t->pos[1]-1, t->pos[2]};

    Node* towerNode = getTowers()->list;
    if (towerNode != NULL) {
        Tower* tNode = (Tower*)towerNode->data;
        float tmp[3] = {tNode->pos[X], tNode->pos[Y], tNode->pos[Z]};
        int a = 0;
        if (tNode->team != team && distanceVector2D(tmp, pos) < range) {
            targetDist = distanceVector2D(tmp, pos);
            memcpy(closestTargetPos, tmp, sizeof(float)*3);
        }
        while ((towerNode = towerNode->next) != NULL) {
            tNode = (Tower*)towerNode->data;
            if (tNode->team == team)
                continue;
            float tPos[3] = {tNode->pos[X], tNode->pos[Y], tNode->pos[Z]};
            float dist = distanceVector2D(tPos, pos);
            if (dist < range && dist < targetDist) {
                targetDist = dist;
                memcpy(closestTargetPos, tPos, sizeof(float)*3);
            }
        }
    }
    //if (!inBoundsV(closestTargetPos))
    for (int a = 0; a < listSize(targetList); a++) {
        Vehicle* v = (Vehicle*)listGet(targetList, a);
        if (v->team == team)
            continue;
        float dist = distanceVector2D(pos, v->front);
        if (dist < range && dist < targetDist) {
            memcpy(closestTargetPos, v->front, sizeof(float)*3);
            targetDist = dist;
        }
    }

    targetList = getTanks();
    for (int a = 0; a < listSize(targetList); a++) {
        Tank* t = (Tank*)listGet(targetList, a);
        if (t->team == team)
            continue;
        float dist = distanceVector2D(pos, t->front);
        if (dist < range && dist < targetDist) {
            memcpy(closestTargetPos, t->front, sizeof(float)*3);
            targetDist = dist;
        }
    }

    targetList = getHeli();
    for (int a = 0; a < listSize(targetList); a++) {
        Heli* h = (Heli*)listGet(targetList, a);
        if (h->team == team)
            continue;
        float dist = distanceVector2D(pos, h->pos);
        if (dist < range && dist < targetDist) {
            memcpy(closestTargetPos, h->pos, sizeof(float)*3);
            targetDist = dist;
        }
    }
}

void drawPixel(int x, int y, int size)
{
    draw2Dbox(x,y,x+size,y+size);
}

int zero[15] = {1,1,1,
                1,0,1,
                1,0,1,
                1,0,1,
                1,1,1};
int one[15] = { 0,0,1,
                0,0,1,
                0,0,1,
                0,0,1,
                0,0,1};
int two[15] = { 1,1,1,
                1,0,0,
                1,1,1,
                0,0,1,
                1,1,1};
int three[15] = { 1,1,1,
                0,0,1,
                1,1,1,
                0,0,1,
                1,1,1};
int four[15] = { 0,0,1,
                0,0,1,
                1,1,1,
                1,0,1,
                1,0,1};
int five[15] = { 1,1,1,
                0,0,1,
                1,1,1,
                1,0,0,
                1,1,1};
int six[15] = { 1,1,1,
                1,0,1,
                1,1,1,
                1,0,0,
                1,1,1};
int seven[15] = { 0,0,1,
                0,0,1,
                0,0,1,
                0,0,1,
                1,1,1};
int eight[15] = { 1,1,1,
                1,0,1,
                1,1,1,
                1,0,1,
                1,1,1};
int nine[15] = { 0,0,1,
                0,0,1,
                1,1,1,
                1,0,1,
                1,1,1};
int* digits[] ={zero, one, two, three, four, five, six, seven, eight, nine};
void drawNumber(int digit, int screenX, int screenY, int pixelSize)
{
    if (digit > 9 || digit < 0)
        return;
    for (int a = 0; a < 5; a++)
        for (int b = 0; b < 3; b++)
            if (digits[digit][a*3+b]==1)
                drawPixel(screenX+b*pixelSize, screenY+a*pixelSize, pixelSize);
}
