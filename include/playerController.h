/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - world utility functions
 ****************************************/

#ifndef __MULLINSN_PLAYER_H__
#define __MULLINSN_PLAYER_H__

#include "graphics.h"
#include "world.h"

#define MAX_PLAYER_HEIGHT CLOUD-2
#define PLAYER_BUFFER 0.5
#define SPEED_MULT 5

float playerLocation[3];
float playerOrientation[2];

void initPlayer(float spawnLocation[3]);
void preUpdatePlayer(GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void updatePlayerPosition(float pos[3], float view[3], bool f, bool l, bool r, bool b, GLubyte world[WORLDX][WORLDY][WORLDZ], float deltaTime);
void playerInput(int button, int state, int x, int y);
void playerKeyboardInput(unsigned char key, GLubyte world[WORLDX][WORLDY][WORLDZ]);
void drawUI();
void endGamePlayer();
void getPlayerPos(float pos[3]);

#endif