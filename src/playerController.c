/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - player utility functions
 ****************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#include "graphics.h"
#include "projectile.h"
#include "playerController.h"

void initPlayer(float spawnLocation[3]) 
{
    spawnLocation[0] = -50;
    spawnLocation[1] = -20;
    spawnLocation[2] = -50;
    memcpy(playerLocation, spawnLocation, sizeof(float)*3);
    playerOrientation[X] = 0;
    playerOrientation[Y] = 0;
}

bool checkCollision(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    //Check if the player is inbounds
    if ((int)x <= 0 || (int)y <= 0 || (int)z <= 0)
        return true;
    if ((int)x >= WORLDX || (int)y >= CLOUD_HEIGHT-2 || (int)z >= WORLDZ)
        return true;

    //Check if the block is empty
    if (world[(int)x][(int)y][(int)z]!=0)
        return true;
    return false;
}

void findEmpty(float loc[3], GLubyte world[WORLDX][WORLDY][WORLDZ], int radius) 
{
    //For some reason the player location is stored backwards...
    loc[X] = -loc[X];
    loc[Y] = -loc[Y];
    loc[Z] = -loc[Z];
    //Look for a close by empty cube to put the player
    for (int i = (int)loc[X]-radius; i <= (int)loc[X]+radius; i++)
    {
        for (int j = (int)loc[Y]; j <= (int)loc[Y]+radius; j++)
        {
            for (int k = (int)loc[Z]-radius; k <= (int)loc[Z]+radius; k++)
            {
                if (i >= WORLDX || j >= WORLDY || k >= WORLDZ)
                    continue;
                if (!checkCollision(i, j, k, world))
                {
                    loc[X] = -i-.5;
                    loc[Y] = -j-.5;
                    loc[Z] = -k-.5;
                    return;
                }
            }
        }
    }
    //flip cord back
    loc[X] = -loc[X];
    loc[Y] = -loc[Y];
    loc[Z] = -loc[Z];
    //If nothing was found increase the search radius, allow max of 3 blocks away
    if (radius <= 2)
        findEmpty(loc, world, radius+1);
}

void updatePlayerPosition(float pos[3], float view[3], bool f, bool l, bool r, bool b, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    float rotx = (view[0]/ 180.0 * 3.141592);
    float roty = (view[1] / 180.0 * 3.141592);
    float newLoc[3] = {pos[X], pos[Y], pos[Z]};
    if (f)
    {
        newLoc[X] -= sin(roty) * deltaTime * SPEED_MULT;
        //if (flycontrol == 1)
        newLoc[Y] += sin(rotx) * deltaTime * SPEED_MULT;
        newLoc[Z] += cos(roty) * deltaTime * SPEED_MULT;
    }
    if (l)
    {
        newLoc[X] += cos(roty) * deltaTime * SPEED_MULT;
        newLoc[Z] += sin(roty) * deltaTime * SPEED_MULT;
    }
    if (r)
    {
        newLoc[X] -= cos(roty) * deltaTime * SPEED_MULT;
        newLoc[Z] -= sin(roty) * deltaTime * SPEED_MULT;
    }
    if (b)
    {
        newLoc[X] += sin(roty) * deltaTime * SPEED_MULT;
        //if (flycontrol == 1)
        newLoc[Y] -= sin(rotx) * deltaTime * SPEED_MULT;
        newLoc[Z] -= cos(roty) * deltaTime * SPEED_MULT;
    }


    //Check direction player is moving in for clear path
    float direction[3] = {newLoc[0]-pos[0], newLoc[1]-pos[1], newLoc[2]-pos[2]};
    float len = sqrt(direction[X]*direction[X] + direction[Y]*direction[Y] + direction[Z]*direction[Z]);
    direction[X] = direction[X] / (len);
    direction[Y] = direction[Y] / (len);
    direction[Z] = direction[Z] / (len);

    int flag = 0;
    if (checkCollision(-newLoc[X],-newLoc[Y],-newLoc[Z], world))
        flag=1;
    if (len > 0 && checkCollision(-(newLoc[X] + direction[X]),-(newLoc[Y] + direction[Y]),-(newLoc[Z] + direction[Z]), world)) 
        flag=1;

    if (flag == 0)
    {
        pos[X] = newLoc[X];
        pos[Y] = newLoc[Y];
        pos[Z] = newLoc[Z];
    }

    memcpy(playerLocation, pos, sizeof(float)*3);
    playerOrientation[0] = rotx;
    playerOrientation[1] = roty;
}

void playerInput(int button, int state, int x, int y)
{
    /*
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        float x = sin(playerOrientation[1])*cos(playerOrientation[0]);
        float y = -sin(playerOrientation[0]);
        float z = -cos(playerOrientation[1])*cos(playerOrientation[0]);

        #define SPEED 40
        float velocity[3] = {x*SPEED, y*SPEED, z*SPEED};
        float location[3] = {-playerLocation[X], -playerLocation[Y], -playerLocation[Z]};
        createProjectile(0, location, velocity);
    }
    */
    /*
    else if (button == GLUT_MIDDLE_BUTTON)
        printf("middle button - ");
    else
        printf("right button - ");

    if (state == GLUT_UP)
        printf("up - ");
    else
        printf("down - ");

    printf("%d %d\n", x, y);
    */
}

void getPlayerPos(float pos[3])
{
    memcpy(pos, playerLocation, sizeof(float)*3);
}

void drawUI()
{
    //Draw crosshair
}

void endGamePlayer()
{

}
