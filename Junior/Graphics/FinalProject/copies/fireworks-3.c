/*********************************
 * fireworks.c
 * The main program for the firework display.
 * For now, just some basic setup copied from glhello.c (hw1 support)
 * And a little from ray_trace.c
 * For now, it displays a quad and you can press 'q' to quit.
 */

#include "fireworks.h"

FIREWORK test;
NOZZLE nozzles[NUM_NOZZLES];


//global vars
GLfloat Rz[4][4];
GLfloat RzT[4][4];
GLfloat Ry[4][4];
GLfloat RyT[4][4];
GLfloat Rx[4][4];
GLfloat RxT[4][4];
GLfloat RR[4][4];
GLfloat RT[4][4];
GLfloat S[4][4];
GLfloat T[4][4];
GLfloat R[4][4];
GLfloat result[4];
GLfloat result2[4];





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
	my_setup();


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


void my_setup(){
	int i;
	
	NOZZLE temp;
	
	
	create_nozzles();		//fill arrays of nozzles
	

	//read through array to show x,y coords of where they "would be"
	for(i=0;i<NUM_NOZZLES;i++){
		temp = nozzles[i];
		printf("x: %d\ny: %d\n",temp.x,temp.y);
	}

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




void create_nozzles(){
	int i;
	int x;
	int y;
	NOZZLE temp;			// this may need to be a pointer, im confused on how to do the shape pointer inside
							// of the nozzle pointer to fill the array of verts, if so then replace . with -> below
							
	SHAPE* sh_temp;
	
	
	for(i=0;i<NUM_NOZZLES;i++){
		//make_cylinder(sh_temp); // cant go in here because changing values of v_start in SHAPE
									// cause for a seg fault, i used similar syntax for assign2-5 but idk
		
		temp.cyl = sh_temp;			//assign shape pointer to the shape pointer in nozzle (nozzle contains a cylinder)
		
		
		temp.x = WINDOW_WIDTH/(NUM_NOZZLES+1)*(i+1);	//place the nozzles equidistant.
		temp.y = WINDOW_HEIGHT*NOZZLE_HEIGHT;						//arbitrarily low y value
		
		
		//translate cyl by temp.x and temp.y, maybe a set z value? where is the camera?
		
		nozzles[i] = temp;			//add the nozzle to the array
		
		
		
	}
	


}

void matrix_mult(GLfloat M[][4],GLfloat V[]){


	double ans=0;
	int i;
	int j;
	
	GLfloat stored[4];
	stored[0] = V[0];
	stored[1] = V[1];
	stored[2] = V[2];
	stored[3] = V[3];
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			ans+=M[i][j]*stored[j];
			
		}
		result[i]=ans;
		result2[i]=ans;
		
		ans=0;
		
	}
	
	return;
}

//set up the x rotation matrix
void set_Rx(GLfloat theta){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			Rx[i][j]=0;
		}
	}
	Rx[0][0] =1 ;
	Rx[1][1] =cos(theta);
	Rx[2][1] = sin(theta);
	Rx[1][2] = -sin(theta);
	Rx[2][2] = cos(theta);
	Rx[3][3] =1;

}

void set_RxT(GLfloat theta){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			RxT[i][j]=0;
		}
	}
	RxT[0][0] =1 ;
	RxT[1][1] =cos(theta);
	RxT[1][2] = sin(theta);
	RxT[2][1] = -sin(theta);
	RxT[2][2] = cos(theta);
	RxT[3][3] =1;

}

//set up the y rotation matrix
void set_Ry(GLfloat theta){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			Ry[i][j]=0;
		}
	}
	Ry[0][0] = cos(theta);
	Ry[1][1] =1;
	Ry[0][2] = sin(theta);
	Ry[2][0] = -sin(theta);
	Ry[2][2] = cos(theta);
	Ry[3][3] =1;

}

//set up the y rotation matrix
void set_RyT(GLfloat theta){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			RyT[i][j]=0;
		}
	}
	RyT[0][0] = cos(theta);
	RyT[1][1] =1;
	RyT[2][0] = sin(theta);
	RyT[0][2] = -sin(theta);
	RyT[2][2] = cos(theta);
	RyT[3][3] =1;

}
// set up the z rotation matrix
void set_Rz(GLfloat theta){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			Rz[i][j]=0;
		}
	}
	Rz[0][0] = cos(theta);
	Rz[2][2] =1;
	Rz[0][1] = -sin(theta);
	Rz[1][0] = sin(theta);
	Rz[1][1] = cos(theta);
	Rz[3][3] =1;

}

void set_RzT(GLfloat theta){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			RzT[i][j]=0;
		}
	}
	RzT[0][0] = cos(theta);
	RzT[2][2] =1;
	RzT[1][0] = -sin(theta);
	RzT[0][1] = sin(theta);
	RzT[1][1] = cos(theta);
	RzT[3][3] =1;

}
// set up the translation matrix
void set_T(GLfloat tx,GLfloat ty, GLfloat tz){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			T[i][j]=0;
		}
	} 
	T[0][0] = 1;
	T[1][1] = 1;
	T[2][2] =1;
	T[3][3] =1;
	T[0][3] =tx;
	T[1][3] =ty;
	T[2][3] =tz;
}
// set up the scaling transformation matrix
void set_S(GLfloat sx,GLfloat sy, GLfloat sz){
	int i;
	int j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			S[i][j]=0;
		}
	}
	S[0][0] = sx;
	S[1][1] = sy;
	S[2][2] =sz;
	S[3][3] =1;

}


void make_cylinder(SHAPE *temp){
    GLfloat ray =1;
    GLfloat height =2;
    
    temp->v_start[0]=ray;
    temp->v_start[1] =-height/2;
    temp->v_start[2] =0;
    temp->v_start[3] =1;
  
    int i;
    int j;
    double theta;
    double phi;
    double h;
    double half = height/2;
    for(i=0,theta =0;i<VS;i++,theta+=2*M_PI/VS){
  		for(j=0,h=0;j<RS;h+=height/RS,j++){
  			set_Ry(theta);
  			set_T(0,h,0);
  			
  			matrix_mult(T,temp->v_start);
  			matrix_mult(Ry,result);
	
	  		temp->shape_verts[i][j][0] = result[0];
  			temp->shape_verts[i][j][1] = result[1];
  			temp->shape_verts[i][j][2] = result[2];
  			temp->shape_verts[i][j][3] = result[3];

  			result[0]=0;
  			result[1]=0;
  			result[2]=0;
  			result[3]=0;
  		
  		}
    }


}


