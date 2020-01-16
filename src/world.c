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

    c->velocity = 1.75;
    c->time = 0;

    c->location[Y] = CLOUD_HEIGHT;
    c->location[X] = rand()%(100-CLOUD_WIDTH);
    c->location[Z] = rand()%(100-CLOUD_LENGTH);
    memset(c->shape, 0, sizeof(c->shape[0][0])*CLOUD_WIDTH*CLOUD_LENGTH);

    //Generate shape
    int width = rand()%(CLOUD_WIDTH-3) + 3;
    int length = rand()%(CLOUD_LENGTH-5) + 5;

    for (int x = 0; x < width; x++)
        for (int y = 0; y < length; y++)
            c->shape[x][y] = 1;

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
    generateBase(x, z, world);
    generateBase(100-x-1, 99-z, world);

}

void initWorld(GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    generateWorld(world);
    //Add clouds
    clouds = initList();
    int numClouds = rand()%15 + 10;
    for (int i = 0; i < numClouds; i++)
        listAdd(clouds, generateCloud());
}

void updateWorld(GLubyte world[WORLDX][WORLDY][WORLDZ], double deltaTime)
{
    //Remove old cloud position
    for (int x = 0; x < WORLDX; x++)
        for (int z = 0; z < WORLDZ; z++)
            world[x][CLOUD_HEIGHT][z] = 0;

    //Update each cloud
    for (int i = 0; i < listSize(clouds); i++)
    {
        Cloud* c = listGet(clouds, i);

        //Update cloud velocity timing, (move every X amt of time)
        c->time += deltaTime;
        if (c->time > c->velocity) 
        {
            c->time = 0;
            //Check if the cloud needs to be wrapped
            c->location[Z] = c->location[Z] + 1;
            if (c->location[Z] > WORLDZ)
                c->location[Z] = 0;
        }

        //Add new cloud position to world
        for (int cloudX = 0; cloudX < CLOUD_WIDTH; cloudX++)
            for (int cloudZ = 0; cloudZ < CLOUD_LENGTH; cloudZ++)
                if (c->shape[cloudX][cloudZ])
                    if (c->location[Z] + cloudZ >= WORLDZ) 
                        world[c->location[X]+cloudX][CLOUD_HEIGHT][c->location[Z]+cloudZ-WORLDZ] = CLOUD;
                    else
                        world[c->location[X]+cloudX][CLOUD_HEIGHT][c->location[Z]+cloudZ] = CLOUD;
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