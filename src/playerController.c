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

#include "graphics.h"
#include "playerController.h"

void initPlayer(float spawnLocation[3]) 
{
    spawnLocation[0] = -50;
    spawnLocation[1] = -20;
    spawnLocation[2] = -50;
}

bool checkCollision(float x, float y, float z, GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    if ((int)x > WORLDX || (int)y > WORLDY || (int)z > WORLDZ)
        return true;

    if (world[(int)x][(int)y][(int)z]!=0)
        return true;
    return false;
}

void updatePlayer(float prev[3], float curr[3], GLubyte world[WORLDX][WORLDY][WORLDZ])
{

    float x = -curr[0];
    float y = -curr[1];
    float z = -curr[2];

    int flag = 0;

    if (checkCollision(x,y,z, world))
        flag=1;
    if (checkCollision(x+PLAYER_BUFFER,y,z, world))
        flag=1;
    if (checkCollision(x,y+PLAYER_BUFFER,z, world))
        flag=1;
    if (checkCollision(x,y,z+PLAYER_BUFFER, world))
        flag=1;

    printf("Flag: %d\n", flag);

    if (flag) 
    {
        curr[0] = prev[0];
        curr[1] = prev[1];
        curr[2] = prev[2];

        if (flag)
        {
            float direction[3] = {curr[0]-prev[0], curr[1]-prev[1], curr[2]-prev[2]};
            curr[0]-=direction[0]*2;
            curr[1]-=direction[1]*2;
            curr[2]-=direction[2]*2;
            flag = checkCollision(-curr[0], -curr[1], -curr[2], world);
            printf("%.3f %.3f %.3f\n", direction[0], direction[1], direction[2]);
            printf("Updated flag: %d\n", flag);
        }
    }
}

void drawUI()
{

}

void endGamePlayer()
{

}
