/*********************************
 * fireworks.h
 * Contains the structs for the main program, and some MACROs.
 * Also contains function prottypes, after the structs.
 * Heavily commented, but also pretty tentative
 */

#ifndef _FIREWORKS_H_
#define _FIREWORKS_H_
#endif

// platform specific includes
#ifdef __APPLE__
	#include <GLUT/glut.h>
 	#include <OpenGL/gl.h>
 	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 640

// some MACROs for various sound types
#define BANG 0
#define BOOM 1
#define SIZZLE 2
#define CRACK 3
#define POP 4
#define SONG 5
#define etc 6

// macros for shape_id's
#define SPHERE 0
#define CUBE 1
#define CYLINDER 2
#define CONE 3
#define TORUS 4
#define HOUSE lololololjk

// macros for explosion shapes
#define WILLOW 0
#define BURST 1
#define SPARKLER 2
#define you get the idea

// because why not
#define min(a,b) ((a) < (b)? a:b)
#define max(a,b) ((a) > (b)? a:b)

// a generic way to describe the various 'bang', 'sizzle', and 'boom's
// as well as the music (1812 overture, etc)
typedef struct _Sound {
	short type;						// see the MACROs above
	GLfloat duration;			// how long should the sound last
	GLfloat volume;				// because some will be louder than others

}SOUND;

// this is the basic cube/sphere/cone/cylinder/torus shape class
// it may turn out that we use the same shape for everything,
// (ie, all are cubes or all are spheres) and then we won't need this
typedef struct _Shape {
	short shape_id;										// to distinguish between shapes
	
	GLfloat shape_verts[RS][VS][4];		// vertices for the shape

}SHAPE;

// a wrapper around shape that includes info about color, brightness, etc
typedef struct _Particle {
	GLfloat init_color[4];					// the starting/base color
	GLfloat init_brightness[4];			// initial value/brightness
	short single_color;							// if true, then the color doesn't change
	GLfloat end_color[4];						// otherwise, this is the end color
	GLfloat end_brightness[4];			// and end value/brightness
	short interpolate_how;					// a flag to determine how to go init->end

	GLfloat duration;								// how long the particle is visible

	SHAPE shape;										// which shape is it?

}PARTICLE;

// the component of a firework between launch and explosion
typedef struct _Fuse {
	GLfloat hangtime;			// time between ignition and explosion
	GLfloat velocity;			// initial velocity out of the nozzle
	// note, the above two could easily be consoldiated together

	short trail;							// a boolean to determine if there is a trail
	GLfloat trail_color[4];		// if there is a trail, then what color is it?

	GLfloat direction;		// determines the initial angle the fireowrk is shot

	SOUND sound;					// the sound that the fuse will make (if any)

}FUSE;

// the factors that determine the actual explosion
typedef struct _Explosion {
	GLfloat radius;			// the distance the farthest particle will travel
	GLfloat density;		// a measure of how close particles will be to each other
	GLfloat quantity;		// how many particles comprise the explosion

	PARTICLE *particles;	// the array of particles, to be malloc'd later
												// once we know 'quantity'

	short shape;				// willow, burst, boom, sparkler, etc

	SOUND sound;				// the sound that the explosion will make

}EXPLOSION;

// the firework!
typedef struct _Firework {
	short id;							// because we might want to keep track of which one it is
	short nozzle;					// which nozzle it comes out of (probably not necessary)

	FUSE fuse;						// the fuse!
	EXPLOSION explosion;	// the explosion!

}FIREWORK;

// nozzle class for launch point of firework
typedef struct _Nozzle {
	short cur_firework;				//the type of firework
	short nozzle_id;				//nozzle id to specify
	int x;
	int y;							//position of nozzle/ start of firework
	
	FIREWORK *all_fireworks;	//array of the fireworks for that nozzle
}NOZZLE;



void initGL(void);
void my_display(void);
void my_keyboard(unsigned char key, int x, int y);
