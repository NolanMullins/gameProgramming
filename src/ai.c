/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-07
 * Game programing - AI Class
 ****************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#include "vehicle.h"
#include "tower.h"
#include "score.h"
#include "utils.h"

float aiTimer;
float lastTowerPos[3];
int numTowers;
int towerProb;
int buildProb;
float aiThinkTime;

void initAI() 
{
    aiThinkTime = 0.5;
    aiTimer = aiThinkTime;
    numTowers = 0;
    towerProb = 105;
    memcpy(lastTowerPos, getBasePos(AI), sizeof(float)*3);
    buildProb = -50;
}

bool aiSpawnTower(GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    int attempts = 50, x, z, offset = 0;
    if (numTowers > 3)
        offset = 2;
    int randX[4] = {40,lastTowerPos[X]+10, 30, 50};
    //int randZ[4] = {30,lastTowerPos[Z]-15, WORLDZ-30, 15};
    int randZ[4] = {30,35, WORLDZ-30, 15};
    do {
        x = rand()%randX[offset] + randX[offset+1];
        z = rand()%randZ[offset] + randZ[offset+1];
    } while(attempts-- > 0 && !createTower(AI, x, z, world));
    if (attempts <= 0)
        return false;
    numTowers++;
    lastTowerPos[X] = x;
    lastTowerPos[Z] = z;
    return true;
}

void updateAI(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime)
{
    aiTimer -= deltaTime;
    if (aiTimer > 0)
        return;
    aiTimer = aiThinkTime;
    if (getScore(AI) <= 0 || getScore(AI) > 20)
        return;
    if (numTowers > 6)
        if ((int)(rand()%100)<buildProb)
            return;
    if (numTowers > 6) {
        int numV = 0;
        Node* vics = getVehicles()->list;
        if (((Vehicle*)vics->data)->team == AI)
            numV++;
        while ((vics = vics->next) != NULL)
            if (((Vehicle*)vics->data)->team == AI)
                numV++;
        if (numV < 5) {
            createVehicle(AI);
            return;
        }
        aiThinkTime += 1;
    }
    int num = (int)(rand()%100)+1;
    if (num > towerProb)
        aiSpawnTower(world);
    else {
        createVehicle(AI);
        if (towerProb > 20)
            towerProb -= 5;
    }
    buildProb += 5;
}

void endGameAI()
{

}
