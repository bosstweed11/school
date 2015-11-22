/**************************************************************************
File: hellomouse.c
Does: basic drawing and interaction in OpenGL/GLUT
Author: gem, based on hwa's
Date: 01/08/08
**************************************************************************/
/**********
notes:
VS users: include opengl/glut.h below instead of GL/glut.h
OSX users: include glut/glut.h below instead of GL/glut.h
**********/



#include <GLUT/glut.h>
#include <stdio.h>
#include <math.h>
#include "hellomouse.h"




/* define and initialize some global variables */
double my_x = 0, my_y = 0,prev_x,delta_x,prev_y,delta_y; /* current position */
int window_w = 400, window_h = 400; /* window height and width */
double my_rect_w = 0.1, my_rect_h = 0.1; /* rectangle width and height */




double radius =.05;
double gravity=1.2;
double velocity =1;
double dt=.1;
int shape =0;
int index;
double this_y;
int changing_size=1;


/*Create Shape class */

class Shape {

public:

	int type,dir;//0 down 1 up
	
	Shape(){}
	~Shape() {}
	//main constuctor
	void set_shape(double &x,double &y, double &h, double &w, double type, double side_v,double v,int dir, double &x_dir){
		this->initial=0;
		if(type==0){
			set_x(x);
			set_y(y);
			set_w(w);
			set_h(h);
			set_type(type);
			set_side_v(side_v);
			set_v(v);
			set_dir(dir);
			set_x_dir(x_dir);
		}
		else{
			set_x(x);
			set_y(y);
			set_r(h);
			set_type(type);
			set_side_v(side_v);
			set_v(v);
			set_dir(dir);
			set_x_dir(x_dir);
		}
		this->initial=1;
	
	}
	
	/* Setters and Getters*/
	double get_x() {
		return x;
	}
	double get_y() {
		return y;
	}
	double get_w() {
		return w;
	}
	double get_h() {
		return h;
	}
	double get_v() {
		return v;
	}
	double get_side_v() {
		return side_v;
	}
	int get_dir() {
		return dir;
	}
	int get_x_dir() {
		return x_dir;
	}
	void set_x(double new_x) {
		this-> x = new_x;
		
		return;
	}
	void set_y(double new_y) {
		this-> y = new_y;
		
		return;
	}
	void set_w(double new_w) {
		if(this->type==0)
			this-> w = new_w;
		
		return;
	}
	
	void set_h(double new_h) {
		if(this->type==0)
			this-> h = new_h;
			
			return;
	}
	void set_v(double new_v) {
		if(new_v<.4&&new_v>-.4&&this->initial==0){
			new_v =1;
		}
		this-> v = new_v;
		
		return;
	}
	void set_side_v(double new_v) {
		this-> side_v = new_v;
		
		return;
	}
	void set_dir(int new_dir) {
		this-> dir = new_dir;
		
		return;
	}
	void set_x_dir(double new_dir) {
		if(new_dir>0)
		{
			this->x_dir =1;
		}
		else{
			this->x_dir=0;
		}
		
		return;
	}
	void set_r(double new_r) {
		if(this->type==1)
			this-> r = new_r;
			
		return;
	}
	void set_type(int new_type) {
		this-> type = new_type;
		
		return;
	}
	/* Main drawing method */
	void draw(){
		if(this->type ==0){
			glColor3f(0,0,1) ; /* (Red, Green, Blue); so here we ask for Blue */
			glRectf( this->x/window_w, this->y/window_h, this->x/window_w + this->w, this->y/window_h + this->h);
			
		}
		
		if(this->type ==1){
			glColor3f(1,0,0);
			glBegin(GL_TRIANGLE_FAN);     	// draw triangle
			glVertex2d(this->x/window_w,this->y/window_h);
				int i;
				for(i=0;i<360;i+=6){
					glVertex2d(this->x/window_w+radius*cos(i),  this->y/window_h+radius*sin(i));
				}
			glEnd();	
		}	
	}

/*private variables*/
private:
	double x,y,w,h,r,v,side_v;
	int x_dir,initial;
};



// make shapes array
Shape shapes[25];

/*Typical OpenGL/GLUT Main function */ 
int main(int argc, char **argv) { /* program arguments */

  /* initialize GLUT and OpenGL; Must be called first */
  glutInit( &argc, argv ) ;
  
  /* our own initializations; we'll define these setup procedures */
  glut_setup() ;  
  gl_setup() ;
  my_setup();

  /* turn control over to GLUT */
  glutMainLoop() ;

  return(0) ; /* make the compiler happy */
}



/* Typical function to set up GLUT*/
void glut_setup(void) {

  /* specify display mode -- here we ask for double buffering and RGB coloring */
  glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB);

  /* make a window of size window_w by window_h; title of "GLUT Basic Interaction" placed at top left corner */
  glutInitWindowSize(window_w, window_h);
  glutInitWindowPosition(0,0);
  glutCreateWindow("GLUT Basic Interaction");

  /*initialize typical callback functions */
  glutDisplayFunc( my_display );
  glutReshapeFunc( my_reshape ); 
  glutIdleFunc( my_idle );
  glutMouseFunc( my_mouse );	
  glutKeyboardFunc( my_keyboard );	
  glutTimerFunc( dt*10, my_TimeOut, 0);/* schedule a my_TimeOut call with the ID 0 in 20 seconds from now */
  return ;
}

/* Typical function to set up OpenGL */
/* For now, ignore the contents of function */
void gl_setup(void) {

  /* specifies a background color: black in this case */
  glClearColor(0,0,0,0) ;

  /* setup for simple 2d projection */
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  /* map unit square to viewport window */
  gluOrtho2D(0.0, 1.0, 0.0, 1.0); 

  return ;
}


void my_idle(void) {
	
  return ;
}


void my_setup(void) {
/* set up initialization of circle coordinates*/

  return;
}




/* drawing function */
/* TODO: draw some other shape, use some other colors */
/* TODO part 2: draw at the location of the mouse click */
void my_display(void) {
  /* clear the buffer */
  glClear(GL_COLOR_BUFFER_BIT) ;

  /* draw small rectangle; specify two opposing corners as arguments*/
  /* note how the rectangle coordinates we pass as arguments are mapped to the window coordinates */
  /* basically, we need to map x values between (0,window_w) to the (0,1) interval etc. */
	
	//draw the shapes
	int i;
	for(i=0;i<25;i++)
 		shapes[i].draw();
 		
  /* buffer is ready; show me the money! */

  glutSwapBuffers();
	
  return ;
}

	



/* called if resize event (i.e., when we create the window and later whenever we resize the window) */ 
void my_reshape(int w, int h) {
  /* define the viewport to be the entire window */
  glViewport (0, 0, w, h); 
  window_w = w;
  window_h = h;
  
  
  return;
}





/* called if mouse event (i.e., mouse click) */
/* TODO: correct my_y so we draw exactly where the mouse clicked */
/* TODO for EC: have more fun in here */
void my_mouse(int b, int s, int x, int y) {
  switch (b) {            /* b indicates the button */
  case GLUT_LEFT_BUTTON:
    if (s == GLUT_DOWN)  {    /* button pressed */

      
      prev_x= my_x;// hold previous value of the mouse x
      prev_y = my_y;
      my_x = x; 
      my_y = window_h-y;//change my_y to correspond to where the click actually was
      
      delta_x= my_x-prev_x; //save delta x for the side velocity
      delta_y= my_y-prev_y;
      if(index%2 ==0){
      
      	// create rectangle
      	shapes[index].set_shape(my_x,my_y,my_rect_h,my_rect_w,0.0,delta_x*.2,-delta_y*.2,0,delta_x);	
      }
      else{
      	// create circle
      	shapes[index].set_shape(my_x,my_y,radius,my_rect_w,1.0,delta_x*.2,-delta_y*.2,0,delta_x);
      }
      
      index++;
      
      if(index==25){
      		index=0;
      }
      

    }
    else if (s == GLUT_UP) { /* button released */

    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (s == GLUT_DOWN)  {    /* button pressed */

    }
    else if (s == GLUT_UP) { /* button released */

    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if (s == GLUT_DOWN)  {    /* button pressed */

    }
    else if (s == GLUT_UP) { /* button released */

    }
    break;

  }
}


/* called if keyboard event (keyboard key hit) */
/* TODO for EC: do something fun in here */
void my_keyboard(unsigned char c, int x, int y) {
  switch (c) {  /* c is the key that is being hit */
  case 'c': {
    


  }
    break;
   case 'y': {
    

  }
    break;
    /* and so on*/
  }

	return;
}


/* called if timer event */
/* TODO for EC: do something fun in here */
void my_TimeOut(int id) { 
  /* id is the id of the timer; you can schedule as many timer events as you want in the callback setup section of glut_setup */

  /*...you could use this timer event to advance the state of animation incrementally */
  /* if animating you shd replace the 20 secs with something smaller, like 20 msec */
  /* but anyway, right now it only displays a benign message */
 
 
  glutTimerFunc(dt*10, my_TimeOut, 0);/* schedule next timer event, 20 secs from now */
  
  
  //update v 
  //update x
  int i;
  
  //UPDATE V;
  for(i=0;i<25;i++){
  	if(!shapes[i].get_v()==0){
  		shapes[i].set_v(shapes[i].get_v() + gravity*dt);
  	}
  	//UPDATE X,Y
  	if(shapes[i].get_dir()==0){
 			
 		/*Huge conditional which means if the the box has not hit the ground, or the circle has not hit the ground, go down*/
 		if((shapes[i].get_y()>=0&& shapes[i].type==0&&!shapes[i].get_v()==0)||((shapes[i].get_y()-radius*window_h)>=0 && shapes[i].type==1&&!shapes[i].get_v()==0)){
 			shapes[i].set_y(shapes[i].get_y()-shapes[i].get_v()*dt-gravity*gravity*dt/2); 
 		}
 		/*if we hit the ground, flip the value if velocity decreasing it by 20% each time it hits the ground*/
 		else{
 			shapes[i].set_dir(1);
 			shapes[i].set_v(-shapes[i].get_v()*.8);
 			/*if the velocity gets really low, make it zero to not just bounce really fast at the bottom*/
 			if(shapes[i].get_v()<.4&&shapes[i].get_v()>-.4){
 				shapes[i].set_v(0);
 			}
 		}
 	}
 	/*if the shape is going up, we go up until the velocity goes positive*/
 	else if(shapes[i].get_dir()==1&&!shapes[i].get_v()==0){
 			shapes[i].set_y(shapes[i].get_y()-shapes[i].get_v()*dt+gravity*gravity*dt/2); 
			if(shapes[i].get_v()>=0){
				shapes[i].set_dir(0);
			}
 	}
	/*END Y MOVEMENT*/
	/*START X MOVEMENT*/
		// for blue rectangle
 	if(shapes[i].type==0){
 		// if we hit the wall to the right, flip the side velocity and decrease by 70%
 		if((shapes[i].get_x()+shapes[i].get_w()*window_w)>window_w&&shapes[i].get_x_dir()==1){
 			shapes[i].set_side_v(-shapes[i].get_side_v()*.3);
 			shapes[i].set_x_dir(0);
 			
 		}
 		// if we hit the wall to the left, flip the side velocity and decrease by 70%
 		else if(shapes[i].get_x()<0&&shapes[i].get_x_dir()==0){
 			shapes[i].set_side_v(-shapes[i].get_side_v()*.3);
 			shapes[i].set_x_dir(1);
 		}
 	}
 		
 	//for red circle
 	else if(shapes[i].type==1){
 		// if we hit the wall to the right, flip the side velocity and decrease by 70%
 		if((shapes[i].get_x()+radius*window_w)>=window_w&&shapes[i].get_x_dir()==1) {
 		
 			shapes[i].set_side_v(-shapes[i].get_side_v()*.3);
 			shapes[i].set_x_dir(0);
 		}
 		// if we hit the wall to the left, flip the side velocity and decrease by 70%
 		else if((shapes[i].get_x()-radius*window_w)<0&&shapes[i].get_x_dir()==0){
 			shapes[i].set_side_v(-shapes[i].get_side_v()*.3);
 			shapes[i].set_x_dir(1);
 		}
 	}
 	//move in the direction of the side velocity
 	shapes[i].set_x(shapes[i].get_x()+shapes[i].get_side_v()*dt);
  }
  

	
	

  my_display();
	

	return;
}

