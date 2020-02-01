/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - world utility functions
 ****************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "graphics.h"
#include "world.h"
#include "list.h"

#define BASE_PADDING 10
//30 hills, 20 valley
#define HILL_COUNT 15
#define VALLEY_COUNT 30

List* clouds;

void generateBase(int x, int z, GLubyte world[WORLDX][WORLDY][WORLDZ], int block) {
    for (int y = GROUND_LEVEL+1; y < GROUND_LEVEL+4; y++)
        for (int i = x-2; i <= x+2; i++)
            for (int j = z-2; j <= z+2; j++)
                world[i][y][j] = block;
}

bool isSpaceOccupied(int x, int yLayer, int z, int w, int l, GLubyte world[WORLDX][WORLDY][WORLDZ], bool lookForBase)
{
    if (x < 0 || x+w >= WORLDX)
        return true; 
    if (z < 0 || z+l >= WORLDZ)
        return true;
    
    for (int xCord = x; xCord < x+w; xCord++)
    {
        for (int zCord = z; zCord < z+l; zCord++) {
            int block = world[xCord][yLayer][zCord];
            if (block > 0)
                if (lookForBase && (block == BASEA || block == BASEB))
                    return true;
                else if (!lookForBase)
                    return true;
        }
    }
    return false;
}

void fillSpace(int x, int yLayer, int z, int w, int l, int blockType, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    for (int xCord = x; xCord < x+w; xCord++)
        for (int zCord = z; zCord < z+l; zCord++)
            world[xCord][yLayer][zCord] = blockType;
}

void generateHill(GLubyte world[WORLDX][WORLDY][WORLDZ]) 
{
    int attempts = 0;
    while (attempts < 50)
    {
        int w = rand()%15+5;
        int l = rand()%15+5;
        int x = rand()%(100-w-2)+2;
        int z = rand()%(100-l-2)+2;
        if (isSpaceOccupied(x-BASE_PADDING, GROUND_LEVEL+1, z-BASE_PADDING, w+(2*BASE_PADDING), l+(2*BASE_PADDING), world, true))
        {
            attempts++;
            continue;
        }
        int smSide = w;
        if (w > l)
            smSide = l;
        int layers = rand()%(smSide/3)+1;
        for (int h = 0; h < layers; h++) {
            fillSpace(x, GROUND_LEVEL+h+1, z, w, l, HILL+h+1, world);
            int scale = 1;
            if (rand()%10 > 5)
                scale = 2;
            x+=1*scale;
            z+=1*scale;
            w-=2*scale;
            l-=2*scale;
            if (w<3 || l<3)
                break;
        }
        break;
    }
}

void generateValley(GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    int attempts = 0;
    while (attempts < 50)
    {
        int w = rand()%15+5;
        int l = rand()%15+5;
        int x = rand()%(100-w-2)+2;
        int z = rand()%(100-l-2)+2;
        //Look for bases within 5 blocks and hills within 2 blocks
        if (isSpaceOccupied(x-BASE_PADDING, GROUND_LEVEL+1, z-BASE_PADDING, w+(2*BASE_PADDING), l+(2*BASE_PADDING), world, true) ||
            isSpaceOccupied(x-2, GROUND_LEVEL+1, z-2, w+4, l+4, world, false))
        {
            attempts++;
            continue;
        }

        int smSide = w;
        if (w > l)
            smSide = l;
        int depthModifier = 0;
        if (smSide >= 12)
            depthModifier = 1;
        int layers = rand()%2+2+depthModifier;
        for (int h = 0; h > -layers; h--) {
            fillSpace(x, GROUND_LEVEL+h, z, w, l, 0, world);
            x++;
            z++;
            w-=2;
            l-=2;
            if (w<3 || l<3)
                break;
        }
        break;
    }
}

Cloud* generateCloud()
{
    Cloud* c = malloc(sizeof(Cloud));

    //c->velocity = (float)rand()/(float)(RAND_MAX/2.0);
    c->velocity = .5;
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
        for (j = 0; j < GROUND_LEVEL; j++)
            for (k = 0; k < WORLDZ; k++)
                world[i][j][k] = DIRT+j+1;
    
    for (i = 0; i < WORLDX; i++)
            for (k = 0; k < WORLDZ; k++)
                world[i][GROUND_LEVEL][k] = GREEN;
    

    //Randomly place the bases in opposite corners
    int x = rand()%10+8;
    int z = rand()%20+30;
    generateBase(x, z, world, BASEA);
    generateBase(100-x-1, 99-z, world, BASEB);

    int numHills = rand()%15+HILL_COUNT;
    int numValley = rand()%10+VALLEY_COUNT;
    for (int i = 0; i < numHills; i++)
            generateHill(world);
    for (int i = 0; i < numValley; i++)
            generateValley(world);
    
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
