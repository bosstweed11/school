/*********************************
 * fireworks.c
 * The main program for the firework display.
 * For now, just some basic setup copied from glhello.c (hw1 support)
 * And a little from ray_trace.c
 * For now, it displays a quad and you can press 'q' to quit.
 */

#include "fireworks.h"

FIREWORK test;

int main(int argc, char **argv)
{
	// see if the structs all compiled and work together
	test.id = 3;
	printf("test.id is %d\nHeck yes!\n", test.id);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("Fireworks");

	initGL();

	glutDisplayFunc(my_display);
  glutKeyboardFunc(my_keyboard);

	glutMainLoop();

	return 0;
}

void initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
}

void my_keyboard( unsigned char key, int x, int y ) 
{
  switch( key ) {
  case 'q': 
  case 'Q':
    exit(0) ;
    break ; 
  default: break;
  }
}

void my_display()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // make it purple
  glColor3f(.5,0,.5) ;

  // draw a quad in the center
  glBegin(GL_POLYGON);
  glVertex2f(.25, .75);
  glVertex2f(.75, .75);
  glVertex2f(.75, .25);
  glVertex2f(.25, .25);
  glEnd();

  // restore projection
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glutSwapBuffers();
}
