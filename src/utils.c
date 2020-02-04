/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Utils
 ****************************************/

#include <math.h>
#include "graphics.h"

extern void draw2Dline(int, int, int, int, int);
extern void draw2Dbox(int, int, int, int);
extern void draw2Dtriangle(int, int, int, int, int, int);
extern void set2Dcolour(float[]);

void getUnitVector(float unitVec[3], float vec[3])
{
    float l = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    for (int i = 0; i < 3; i++)
        unitVec[i] = vec[i]/l; 
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
