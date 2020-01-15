/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - utility functions
 ****************************************/

#include <stdio.h>

#include "graphics.h"
#include "utils.h"

void generateWorld(GLubyte (*world)[WORLDX][WORLDY][WORLDZ])
{
    int i,j,k;
    /* initialize world to empty */
    for (i = 0; i < WORLDX; i++)
        for (j = 0; j < WORLDY; j++)
            for (k = 0; k < WORLDZ; k++)
                *world[i][j][k] = 0;

    /* some sample objects */
    /* build a red platform */
    for (i = 0; i < WORLDX; i++)
    {
        for (j = 0; j < WORLDZ; j++)
        {
            *world[i][24][j] = 3;
        }
    }
}
