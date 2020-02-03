
/* Derived from scene.c in the The OpenGL Programming Guide */
/* Keyboard and mouse rotation taken from Swiftless Tutorials #23 Part 2 */
/* http://www.swiftless.com/tutorials/opengl/camera2.html */

/* Frames per second code taken from : */
/* http://www.lighthouse3d.com/opengl/glut/index.php?fps */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "graphics.h"
#include "world.h"
#include "playerController.h"
#include "projectile.h"
#include "meteor.h"

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
struct timespec currentTime;

/* mouse function called by GLUT when a button is pressed or released */
void mouse(int, int, int, int);

/* initialize graphics library */
extern void graphicsInit(int *, char **);

/* lighting control */
extern void setLightPosition(GLfloat, GLfloat, GLfloat);
extern GLfloat *getLightPosition();

/* viewpoint control */
extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);
extern void setOldViewPosition(float, float, float);
extern void setViewOrientation(float, float, float);
extern void getViewOrientation(float *, float *, float *);
extern void getMovementKeys(bool* f, bool* l, bool* r, bool* b);

/* add cube to display list so it will be drawn */
extern void addDisplayList(int, int, int);

/* mob controls */
extern void createMob(int, float, float, float, float);
extern void setMobPosition(int, float, float, float, float);
extern void hideMob(int);
extern void showMob(int);

/* player controls */
extern void createPlayer(int, float, float, float, float);
extern void setPlayerPosition(int, float, float, float, float);
extern void hidePlayer(int);
extern void showPlayer(int);

/* tube controls */
extern void createTube(int, float, float, float, float, float, float, int);
extern void hideTube(int);
extern void showTube(int);

/* 2D drawing functions */
extern void draw2Dline(int, int, int, int, int);
extern void draw2Dbox(int, int, int, int);
extern void draw2Dtriangle(int, int, int, int, int, int);
extern void set2Dcolour(float[]);

/* flag which is set to 1 when flying behaviour is desired */
extern int flycontrol;
/* flag used to indicate that the test world should be used */
extern int testWorld;
/* flag to print out frames per second */
extern int fps;
/* flag to indicate the space bar has been pressed */
extern int space;
/* flag indicates the program is a client when set = 1 */
extern int netClient;
/* flag indicates the program is a server when set = 1 */
extern int netServer;
/* size of the window in pixels */
extern int screenWidth, screenHeight;
/* flag indicates if map is to be printed */
extern int displayMap;
/* flag indicates use of a fixed viewpoint */
extern int fixedVP;

/* frustum corner coordinates, used for visibility determination  */
extern float corners[4][3];

/* determine which cubes are visible e.g. in view frustum */
extern void ExtractFrustum();
extern void tree(float, float, float, float, float, float, int);

/* allows users to define colours */
extern int setUserColour(int, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat,
                         GLfloat, GLfloat, GLfloat);
void unsetUserColour(int);
extern void getUserColour(int, GLfloat *, GLfloat *, GLfloat *, GLfloat *,
                          GLfloat *, GLfloat *, GLfloat *, GLfloat *);

/********* end of extern variable declarations **************/

/*** collisionResponse() ***/
/* -performs collision detection and response */
/*  sets new xyz  to position of the viewpoint after collision */
/* -can also be used to implement gravity by updating y position of vp*/
/* note that the world coordinates returned from getViewPosition()
       will be the negative value of the array indices */
void collisionResponse()
{

    /* your code for collisions goes here */
}

void computeMiniMap(int map[WORLDX][WORLDZ], GLubyte world[WORLDX][WORLDY][WORLDZ])
{
    memset(map, 0, sizeof(int)*WORLDX*WORLDZ);
    for (int y = GROUND_LEVEL+5; y >= 0; y--)
    {
        bool flag = false;
        for (int x = 0; x < WORLDX; x++)
        {
            for (int z = 0; z < WORLDZ; z++)
            {
                if (map[x][z] == 0)
                {
                    flag = true;
                    map[x][z] = world[x][y][z];
                }
            }
        }
        if (!flag)
            break;
    }
}

/******* draw2D() *******/
/* draws 2D shapes on screen */
/* use the following functions: 			*/
/*	draw2Dline(int, int, int, int, int);		*/
/*	draw2Dbox(int, int, int, int);			*/
/*	draw2Dtriangle(int, int, int, int, int, int);	*/
/*	set2Dcolour(float []); 				*/
/* colour must be set before other functions are called	*/
void draw2D()
{

    if (testWorld)
    {
        /* draw some sample 2d shapes */
        if (displayMap == 1)
        {
            GLfloat green[] = {0.0, 0.5, 0.0, 0.5};
            set2Dcolour(green);
            draw2Dline(0, 0, 500, 500, 15);
            draw2Dtriangle(0, 0, 200, 200, 0, 200);

            GLfloat black[] = {0.0, 0.0, 0.0, 0.5};
            set2Dcolour(black);
            draw2Dbox(500, 380, 524, 388);
        }
    }
    else
    {
        //screenWidth, screenHeight;
        drawUI();

        #define PIXEL 2
        #define MAP_BUF PIXEL*10
        int map[WORLDX][WORLDZ];
        float player[3];
        getPlayerPos(player);
        computeMiniMap(map, world);
        //Draw minimap
        for (int x = 0; x < WORLDX; x++)
        {
            int screenX = screenWidth - MAP_BUF - WORLDX*PIXEL + x*PIXEL;
            for(int y = 0; y < WORLDZ; y++)
            {
                int screenY = screenHeight - MAP_BUF - WORLDZ*PIXEL + y*PIXEL;
                float r,g,b,a,dr,dg,db,da;
                r=g=b=dr=dg=db=0;
                a=1.0;
                if (abs((int)player[X]+x) > 1 || abs((int)player[Z]+y) > 1)
                    getUserColour(map[x][y], &r, &g, &b, &a, &dr, &dg, &db, &da);
                GLfloat col[] = {r,g,b,a};
                set2Dcolour(col);
                draw2Dbox(screenX,screenY,screenX+PIXEL,screenY+PIXEL);
            }
        }
        //draw2Dbox(screenWidth-100-10, screenHeight-100-10, screenWidth-10, screenHeight-10);
    }
}

/*** update() ***/
/* background process, it is called when there are no other events */
/* -used to control animations and perform calculations while the  */
/*  system is running */
/* -gravity must also implemented here, duplicate collisionResponse */
void update()
{
    int i, j, k;
    float *la;
    float x, y, z;

    //Solve for frame delta
    struct timespec timeStruct;
    double delta;
    clock_gettime(CLOCK_MONOTONIC, &timeStruct);
    delta = (timeStruct.tv_sec - currentTime.tv_sec);
    delta += (timeStruct.tv_nsec - currentTime.tv_nsec) / 1000000000.0;
    currentTime = timeStruct;

    /* sample animation for the testworld, don't remove this code */
    /* demo of animating mobs */
    if (testWorld)
    {

        /* update old position so it contains the correct value */
        /* -otherwise view position is only correct after a key is */
        /*  pressed and keyboard() executes. */
#if 0
// Fire a ray in the direction of forward motion
float xx, yy, zz;
getViewPosition(&x, &y, &z);
getOldViewPosition(&xx, &yy, &zz);
printf("%f %f %f %f %f %f\n", xx, yy, zz, x, y, z);
printf("%f %f %f\n",  -xx+((x-xx)*25.0), -yy+((y-yy)*25.0), -zz+((z-zz)*25.0));
createTube(2, -xx, -yy, -zz, -xx-((x-xx)*25.0), -yy-((y-yy)*25.0), -zz-((z-zz)*25.0), 5);
#endif

        getViewPosition(&x, &y, &z);
        setOldViewPosition(x, y, z);

        /* sample of rotation and positioning of mob */
        /* coordinates for mob 0 */
        static float mob0x = 50.0, mob0y = 25.0, mob0z = 52.0;
        static float mob0ry = 0.0;
        static int increasingmob0 = 1;
        /* coordinates for mob 1 */
        static float mob1x = 50.0, mob1y = 25.0, mob1z = 52.0;
        static float mob1ry = 0.0;
        static int increasingmob1 = 1;
        /* counter for user defined colour changes */
        static int colourCount = 0;
        static GLfloat offset = 0.0;

        /* move mob 0 and rotate */
        /* set mob 0 position */
        setMobPosition(0, mob0x, mob0y, mob0z, mob0ry);

        /* move mob 0 in the x axis */
        if (increasingmob0 == 1)
            mob0x += 0.2;
        else
            mob0x -= 0.2;
        if (mob0x > 50)
            increasingmob0 = 0;
        if (mob0x < 30)
            increasingmob0 = 1;

        /* rotate mob 0 around the y axis */
        mob0ry += 1.0;
        if (mob0ry > 360.0)
            mob0ry -= 360.0;

        /* move mob 1 and rotate */
        setMobPosition(1, mob1x, mob1y, mob1z, mob1ry);

        /* move mob 1 in the z axis */
        /* when mob is moving away it is visible, when moving back it */
        /* is hidden */
        if (increasingmob1 == 1)
        {
            mob1z += 0.2;
            showMob(1);
        }
        else
        {
            mob1z -= 0.2;
            hideMob(1);
        }
        if (mob1z > 72)
            increasingmob1 = 0;
        if (mob1z < 52)
            increasingmob1 = 1;

        /* rotate mob 1 around the y axis */
        mob1ry += 1.0;
        if (mob1ry > 360.0)
            mob1ry -= 360.0;

        /* change user defined colour over time */
        if (colourCount == 1)
            offset += 0.05;
        else
            offset -= 0.01;
        if (offset >= 0.5)
            colourCount = 0;
        if (offset <= 0.0)
            colourCount = 1;
        setUserColour(9, 0.7, 0.3 + offset, 0.7, 1.0, 0.3, 0.15 + offset, 0.3, 1.0);

        /* sample tube creation  */
        /* draws a purple tube above the other sample objects */
        createTube(1, 45.0, 30.0, 45.0, 50.0, 30.0, 50.0, 6);

        /* end testworld animation */
    }
    else
    {
        updateWorld(world, delta);
        getViewPosition(&x, &y, &z);
        float curr[3] = {x,y,z};
        getOldViewPosition(&x, &y, &z);
        float prev[3] = {x,y,z};
        getViewOrientation(&x, &y, &z);
        float view[3] = {x,y,z};
        bool f,l,r,b;
        getMovementKeys(&f, &l, &r, &b);
        updatePlayerPosition(curr, view, f, l, r, b, world, delta);
        updateMeteors(world, delta);
        updateProjectiles(world, delta);
        //updatePlayer(prev, curr, world);
        setViewPosition(curr[0], curr[1], curr[2]);
    }
}

/* called by GLUT when a mouse button is pressed or released */
/* -button indicates which button was pressed or released */
/* -state indicates a button down or button up event */
/* -x,y are the screen coordinates when the mouse is pressed or */
/*  released */
void mouse(int button, int state, int x, int y)
{
    playerInput(button, state, x, y);
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    int i, j, k;
    /* initialize the graphics system */
    graphicsInit(&argc, argv);

    /* the first part of this if statement builds a sample */
    /* world which will be used for testing */
    /* DO NOT remove this code. */
    /* Put your code in the else statment below */
    /* The testworld is only guaranteed to work with a world of
        with dimensions of 100,50,100. */
    if (testWorld == 1)
    {
        /* initialize world to empty */
        for (i = 0; i < WORLDX; i++)
            for (j = 0; j < WORLDY; j++)
                for (k = 0; k < WORLDZ; k++)
                    world[i][j][k] = 0;

        /* some sample objects */
        /* build a red platform */
        for (i = 0; i < WORLDX; i++)
        {
            for (j = 0; j < WORLDZ; j++)
            {
                world[i][24][j] = 3;
            }
        }
        /* create some green and blue cubes */
        world[50][25][50] = 1;
        world[49][25][50] = 1;
        world[49][26][50] = 1;
        world[52][25][52] = 2;
        world[52][26][52] = 2;

        /* create user defined colour and draw cube */
        setUserColour(9, 0.7, 0.3, 0.7, 1.0, 0.3, 0.15, 0.3, 1.0);
        world[54][25][50] = 9;

        /* blue box shows xy bounds of the world */
        for (i = 0; i < WORLDX - 1; i++)
        {
            world[i][25][0] = 2;
            world[i][25][WORLDZ - 1] = 2;
        }
        for (i = 0; i < WORLDZ - 1; i++)
        {
            world[0][25][i] = 2;
            world[WORLDX - 1][25][i] = 2;
        }

        /* create two sample mobs */
        /* these are animated in the update() function */
        createMob(0, 50.0, 25.0, 52.0, 0.0);
        createMob(1, 50.0, 25.0, 52.0, 0.0);

        /* create sample player */
        createPlayer(0, 52.0, 27.0, 52.0, 0.0);
    }
    else
    {
        //Brown
        setUserColour(BROWN, 0.61, 0.46, 0.33, 1.0, 0.1, 0.1, 0.1, 1.0);
        //Green 147 163 104
        setUserColour(GREEN, 0.58, 0.64, 0.41, 1.0, 0.1, 0.1, 0.1, 1.0);
        //Purple
        setUserColour(PURPLE, 0.44, 0.24, 0.31, 1.0, 0.2, 0.2, 0.2, 1.0);
        //Cloud
        setUserColour(CLOUD, 0.9, 0.9, 0.8, 0.5, 0.2, 0.2, 0.2, 0.5);

        //setUserColour(BASEA, 214.0/255.0, 84.0/255.0, 99.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);
        //setUserColour(BASEB, 89.0/255.0, 179.0/255.0, 109.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);
        setUserColour(BASEA, 214.0/255.0, 130.0/255.0, 45.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);
        setUserColour(BASEB, 100.0/255.0, 160.0/255.0, 179.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);

        setUserColour(METEOR, 10.0/255.0, 10.0/255.0, 10.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);
        setUserColour(TAIL, 214.0/255.0, 21.0/255.0, 3.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);
        setUserColour(TAIL+1, 252.0/255.0, 94.0/255.0, 1.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);
        setUserColour(TAIL+2, 200.0/255.0, 200.0/255.0, 200.0/255.0, 1.0, 0.2, 0.2, 0.2, 1.0);

        float inc = 0.02;
        //Green hill
        for (int i = 0; i < 10; i++)
            setUserColour(HILL+i, 67.0/255.0+inc*i, 121.0/255.0+inc*i*2, 59.0/255.0+inc*i, 1.0, 75.0/255.0, 96.0/255.0, 47.0/255.0, 0.5);

        inc = 0.025;
        //Ground
        for (int i = 0; i < 10; i++)
            setUserColour(DIRT+i, 79.0/255.0+inc*i*2, 47.0/255.0+inc*i, 29.0/255.0+inc*i, 1.0, 51.0/255.0, 29.0/255.0, 18.0/255.0, 0.5);

        //Build map
        initWorld(world);
        float spawn[3] = {0,0,0};
        initPlayer(spawn);
        setViewPosition(spawn[0], spawn[1], spawn[2]);
        initProjectiles();
        initMeteors();
    }

    //Record initial time
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    /* starts the graphics processing loop */
    /* code after this will not run until the program exits */
    glutMainLoop();
    
    if (testWorld != 1) 
    {
        destroyWorld();
        endGamePlayer();
        endGameProjectiles();
        endGameMeteors();
    }

    return 0;
}
