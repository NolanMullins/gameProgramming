/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - utility functions
 ****************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "graphics.h"
#include "utils.h"


void generateBase(int x, int z, GLubyte world[WORLDX][WORLDY][WORLDZ]) {
    for (int y = 5; y < 8; y++)
        for (int i = x-2; i <= x+2; i++)
            for (int j = z-2; j <= z+2; j++)
                world[i][y][j] = PURPLE;
}

void generateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    int i,j,k;
    /* initialize world to empty */
    for (i = 0; i < WORLDX; i++)
        for (j = 5; j < WORLDY; j++)
            for (k = 0; k < WORLDZ; k++)
                world[i][j][k] = 0;

    for (i = 0; i < WORLDX; i++)
        for (j = 0; j < 4; j++)
            for (k = 0; k < WORLDZ; k++)
                world[i][j][k] = BROWN;

    for (i = 0; i < WORLDX; i++)
        for (j = 4; j < 5; j++)
            for (k = 0; k < WORLDZ; k++)
                world[i][j][k] = GREEN;

    int x = rand()%10+8;
    int z = rand()%20+30;
    printf("%d\n", x);
    printf("%d\n", z);
    generateBase(x, z, world);
    generateBase(100-x-1, 99-z, world);

}
