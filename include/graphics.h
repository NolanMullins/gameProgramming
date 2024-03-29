
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifdef __linux__ 
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

/* world size and storage array */
#define WORLDX 100
#define WORLDY 50
#define WORLDZ 100

#define MOB_COUNT 10
#define PLAYER_COUNT 10
#define TUBE_COUNT 10

#define BROWN 10
#define GREEN 11
#define PURPLE 12
#define CLOUD 13
//Hill takes up 10 layers
#define HILL 14
//Dirt  takes up 10 layers
#define DIRT 24
#define BASEA 34
#define BASEB 35
#define METEOR 36
//Tail takes up 3 spaces
#define TAIL 37
#define VEHICLE_A 40
#define VEHICLE_B 41

#define TOWER_A 42
#define TOWER_B 43

#define MARKER 44
#define SELECTED 45

#define HELI_A 46
#define HELI_B 47

#define TANK_A 48
#define TANK_B 49

/* list of cubes to draw with each screen update */
#define MAX_DISPLAY_LIST 500000

/* maximum number of user defined colours */
#define NUMBERCOLOURS 100

