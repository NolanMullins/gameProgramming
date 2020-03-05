/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Utils
 ****************************************/

#include <math.h>
#include <stdio.h>

#include "graphics.h"
#include "utils.h"

extern void draw2Dline(int, int, int, int, int);
extern void draw2Dbox(int, int, int, int);
extern void draw2Dtriangle(int, int, int, int, int, int);
extern void set2Dcolour(float[]);


void printVector(float vec[3])
{
    printf("%.1f %.1f %.1f\n", vec[0], vec[1], vec[2]);
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

void setWorldBlockF(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block)
{
    world[(int)loc[X]][(int)loc[Y]][(int)loc[Z]] = block;
}

void setWorldBlock(int loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int block)
{
    world[loc[X]][loc[Y]][loc[Z]] = block;
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
