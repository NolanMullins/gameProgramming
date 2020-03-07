/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-03-07
 * Game programing - Score helper class
 ****************************************/

#include <stdbool.h>

#include "score.h"
#include "graphics.h"
#include "world.h"

void initScore()
{
    score[PLAYER] = PLAYER_START;
    score[AI] = AI_START;
}

int getScore(int team)
{
    return score[team];
}

bool usePoints(int team, int points)
{
    if (score[team] < points)
        return false;
    score[team] -= points;
    return true;
}

void depositPoints(int team, int points)
{
    score[team] += points;
}
