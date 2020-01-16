/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - utility functions
 ****************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "world.h"
#include "list.h"

List* clouds;

void generateBase(int x, int z, GLubyte world[WORLDX][WORLDY][WORLDZ]) {
    for (int y = 5; y < 8; y++)
        for (int i = x-2; i <= x+2; i++)
            for (int j = z-2; j <= z+2; j++)
                world[i][y][j] = PURPLE;
}

Cloud* generateCloud()
{
    Cloud* c = malloc(sizeof(Cloud));
    c->location[Y] = CLOUD_HEIGHT;
    c->location[X] = 20;
    c->location[Z] = 49;
    memset(c->shape, 0, sizeof(c->shape[0][0])*5*10);
    for (int i = 0; i < 5; i++)
        c->shape[0][i] = CLOUD;
    return c;
}

void generateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    int i,j,k;
    /* initialize world to empty */
    memset(world, 0, sizeof(world[0][0][0]*WORLDX*WORLDY*WORLDZ));

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

void initWorld(GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    generateWorld(world);
    //Add clouds
    clouds = initList();
    listAdd(clouds, generateCloud());
    listAdd(clouds, generateCloud());
}

void updateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    //Need to update clouds here
    for (int i = 0; i < listSize(clouds); i++)
    {
        Cloud* c = listGet(clouds, i);
        //Remove old cloud position
        //Add new cloud position
        for (int x = c->location[X]; x < c->location[X]+5; x++)
        {

        }
    }
}

void freeCloud(void* cloud)
{
    free(cloud);
}

void destroyWorld() 
{
    //free clouds
    clouds = listClear(clouds, &freeCloud);
}