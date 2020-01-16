
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

#define BROWN 10
#define GREEN 11
#define PURPLE 12
#define CLOUD 13

/* list of cubes to draw with each screen update */
#define MAX_DISPLAY_LIST 500000

/* maximum number of user defined colours */
#define NUMBERCOLOURS 100

