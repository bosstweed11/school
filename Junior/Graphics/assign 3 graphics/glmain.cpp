//Assignment 3 Graphics cs 1566 Andrew Beers 10/9 2012

/*******************************************************
FILE: glmain.c
AUTHOR: gem, loosely based on some hwa
DATE: 02/09/10
DOES: skeleton for hmkw3
********************************************************/

/**********
notes:
VS users: include gl/glut.h below instead of glut.h
OSX users: include glut/glut.h below instead of glut.j
**********/

//VisS will not let through math.h constants such as M_PI w/out the line below
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
#include <glut/glut.h>
#include <stdio.h>
#include "Shape.cpp"


#include "glmain.h"

double cyl_height=1;
double cyl_ray=1;
double sph_ray=1;
double x_camera = 0, y_camera = 5, z_camera = 30;

int crt_render_mode;
int crt_shape, crt_rs, crt_vs;
int crt_transform;
int spin_val = SPIN_DISABLED;


//global variables
int theta_x;
int theta_y;
int index_ =0;
int draw_norms=0;
int new_shape=0;
GLfloat con_height =1;
GLfloat con_ray =1;
GLfloat v_start[4];
GLfloat Rz[4][4];
GLfloat Ry[4][4];
GLfloat Rx[4][4];
GLfloat RR[4][4];
GLfloat RT[4][4];
GLfloat S[4][4];
GLfloat T[4][4];
double r1=1.2;
double r2=.8;
double pi = atan(1.0)*4;
double normalize;
GLfloat result[4];
GLfloat result2[4];
GLfloat w[4];
GLfloat v[4];
GLfloat u[4];
Shape* cyl;
Shape* sph;
Shape* con;
Shape* tor;
Shape* gen;


 




/*******************************************************
FUNCTION: main
ARGS: argc, argv
RETURN: 0
DOES: main function (duh!); starts GL, GLU, GLUT, then loops 
********************************************************/
int main(int argc, char** argv)

{	

  glutInit(&argc, argv);
  glut_setup();
  gl_setup();
  my_setup();

  glutMainLoop();

  return(0);

}




/*******************************************************
FUNCTION: glut_setup
ARGS: none
RETURN: none
DOES: sets up GLUT; done here because we like our 'main's tidy
********************************************************/
/* This function sets up the windowing related glut calls */
void glut_setup (){

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutInitWindowSize(700,700);
  glutInitWindowPosition(20,20);
  glutCreateWindow("Transformer");


  /* set up callback functions */
  glutDisplayFunc(my_display);
  glutReshapeFunc(my_reshape);
  glutMouseFunc(my_mouse);
  glutKeyboardFunc(my_keyboard);
  glutKeyboardUpFunc( my_keyboard_up );	
  glutTimerFunc( 20, my_TimeOut, 0 );/* schedule a my_TimeOut call with the ID 0 in 20 seconds from now */
  return;

}


/*******************************************************
FUNCTION: gl_setup
ARGS: none
RETURN: none
DOES: sets up OpenGL (name starts with gl but not followed by capital letter, so it's ours)
********************************************************/
void gl_setup(void) {
  // enable depth handling (z-buffer)
  glEnable(GL_DEPTH_TEST);

  // define the background color 
  glClearColor(0,0,0,1);

  glMatrixMode(GL_PROJECTION) ;

  glLoadIdentity() ;

  gluPerspective( 20, 1.0, 15, 100.0);

  glMatrixMode(GL_MODELVIEW) ;

  glLoadIdentity() ;  // init modelview to identity

  return ;

}


/*******************************************************
FUNCTION: my_setup
ARGS: 
RETURN:
DOES: pre-computes stuff and presets some values
********************************************************/
/*TODO add make_cone, make_torus, make_your_shape etc.   */
void my_setup(){
  crt_render_mode = GL_LINE_LOOP;//GL_LINE_LOOP;
  crt_shape = HOUSE;
  crt_transform = NONE_MODE;
  crt_rs = 20;
  crt_vs = 10;
  make_cube_smart(1);
  cyl = new Shape(1,cyl_height,cyl_ray,crt_rs,crt_vs);
  sph = new Shape(2,sph_ray,0,crt_rs,crt_vs);
  con = new Shape(3,con_height,con_ray,crt_rs,crt_vs);
  tor = new Shape(4,r1,r2,crt_rs,crt_vs);
  return;

}



/*******************************************************
FUNCTION: my_reshape
ARGS: new window width and height 
RETURN:
DOES: remaps viewport to the Window Manager's window
********************************************************/
void my_reshape(int w, int h) {
  // ensure a square view port
  glViewport(0,0,min(w,h),min(w,h)) ;
  return ;

}



/*******************************************************
FUNCTION: real_rot_arbitrary_axis_point
ARGS: angle and arbitrary axis ax, ay, az, arbitrary point cx, cy, cz 
RETURN:
DOES: makes shape spin around arbitrary axis passing through arbitrary point
Note: try a diagonal axis going through the back bottom left corner of the house
********************************************************/
/*TODO */
void real_rot_arbitrary_axis_point(GLfloat deg, GLfloat ax, GLfloat ay, GLfloat az, GLfloat cx, GLfloat cy, GLfloat cz ) {
	
	int i;
	int j;
	normalize = sqrt(ax*ax+ay*ay+az*az);
	u[0]=(ax)/normalize;
	u[1]=(ay)/normalize;
	u[2]=(az)/normalize;
	

	//use big rotation matrix
	RR[0][0] = u[0]*u[0]+cos(deg)*(1-u[0]*u[0]);
	RR[0][1] = u[0]*u[1]*(1-cos(deg))-u[2]*sin(deg);
	RR[0][2] = u[2]*u[0]*(1-cos(deg))+u[1]*sin(deg);
	RR[0][3] = 0;
	
	RR[1][0] = u[0]*u[1]*(1-cos(deg))+u[2]*sin(deg);;
	RR[1][1] = u[1]*u[1]+cos(deg)*(1-u[1]*u[1]);
	RR[1][2] = u[2]*u[1]*(1-cos(deg))-u[0]*sin(deg);
	RR[1][3] = 0;
	
	RR[2][0] = u[2]*u[0]*(1-cos(deg))-u[1]*sin(deg);
	RR[2][1] = u[2]*u[1]*(1-cos(deg))+u[0]*sin(deg);
	RR[2][2] = u[2]*u[2]+cos(deg)*(1-u[2]*u[2]);
	RR[2][3] = 0;
	
	RR[3][0] = 0;
	RR[3][1] = 0;
	RR[3][2] = 0;
	RR[3][3] = 1;

	
	set_T(-cx,-cy,-cz);
	
	
	// change my local coordinate system with the object
	//origin
		(*gen).v_start = local_axes[0];
		
		matrix_mult(T,(*gen).v_start);
		matrix_mult(RR,result);
		
		
		//reset T
		
		set_T(cx,cy,cz);
		matrix_mult(T,result);
		
		local_axes[0]= result;
		set_T(-cx,-cy,-cz);
		
		
		//"X" axis
		(*gen).v_start = local_axes[1];
		matrix_mult(T,(*gen).v_start);
		matrix_mult(RR,result);
	
		
		//reset T
		
		set_T(cx,cy,cz);
		matrix_mult(T,result);
		local_axes[1]= result;
		set_T(-cx,-cy,-cz);
		//"Y" axis
		(*gen).v_start = local_axes[2];
		
		matrix_mult(T,(*gen).v_start);
		matrix_mult(RR,result);
		
		
		//reset T
		
		set_T(cx,cy,cz);
		matrix_mult(T,result);
		local_axes[2]= result;
		set_T(-cx,-cy,-cz);
		//"Z" axis
		(*gen).v_start = local_axes[3];
		
		matrix_mult(T,(*gen).v_start);
		matrix_mult(RR,result);
	
		
		//reset T
		
		set_T(cx,cy,cz);
		matrix_mult(T,result);
		local_axes[3]= result;
		
		
		
		set_T(-cx,-cy,-cz);
		
		for(i=0;i<crt_vs;i++){
		for(j=0;j<crt_rs;j++){
		(*gen).v_start = (*gen).vertices[i][j];
		matrix_mult(T,(*gen).v_start);
		matrix_mult(RR,result);
	
		
		//reset T
		
		set_T(cx,cy,cz);
		matrix_mult(T,result);
		(*gen).vertices[i][j] =result;
		
		
		result[0]=0;
		result[1]=0;
		result[2]=0;
		result[3]=0;
		set_T(-cx,-cy,-cz);
		}
	}
	
}

/*******************************************************
FUNCTION: real_rev
ARGS: angle and axis x, y, z 
RETURN:
DOES: makes shape spin around its vertical axis
Note: 'vertical' axis means house keeps revolving around its roof-top
base-center axis, torus spins around axis passing through hole, 
sphere spins around North-South axis etc *at all times*.
********************************************************/
/*TODO */
void real_rev(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) {

	//this method was achieved in using the real rot arbitrary axis point method below

	real_rot_arbitrary_axis_point(deg,x,y,z,0,0,0);

}

/*******************************************************
FUNCTION: real_translation
ARGS: translation amount along x, y, z
RETURN:
DOES: translates shape for real
********************************************************/
/*TODO. Note: Absolutely no gl calls */
/*can assume model-matrix stack contains at this point nothing but viewing transform*/
void real_translation(GLfloat x, GLfloat y, GLfloat z) {

	int i=0;
	int j=0;
	
	set_T(x,y,z);
	//translate local axes
	//origin	
	(*gen).v_start = local_axes[0];
	matrix_mult(T,(*gen).v_start);
	local_axes[0] = result;
		
	//"X" axis
	(*gen).v_start = local_axes[1];
	matrix_mult(T,(*gen).v_start);	
	local_axes[1] = result;
	
	//"Y" axis
	(*gen).v_start = local_axes[2];
	matrix_mult(T,(*gen).v_start);
	local_axes[2] = result;
	
	//"Z" axis
	(*gen).v_start = local_axes[3];
	matrix_mult(T,(*gen).v_start);
	local_axes[3] = result;
		
	for(i=0;i<crt_vs;i++){
		for(j=0;j<crt_rs;j++){
			(*gen).v_start = (*gen).vertices[i][j];
			matrix_mult(T,(*gen).v_start);
			(*gen).vertices[i][j] =result;
			result=0;
	
		}
	}	
}

/*******************************************************
FUNCTION: real_scaling
ARGS: scaling amount along x, y, z
RETURN:
DOES: scales shape, for real
********************************************************/
/*TODO */
/*can assume model-matrix stack contains at this point nothing but viewing transform*/
void real_scaling(GLfloat sx, GLfloat sy, GLfloat sz) {


	int i=0;
	int j=0;

	set_S(sx,sy,sz);
	//scale axes
	//origin
		(*gen).v_start[0] = local_axes[0][0];
		(*gen).v_start[1] = local_axes[0][1];
		(*gen).v_start[2] = local_axes[0][2];
		(*gen).v_start[3] = local_axes[0][3];
		
		matrix_mult(S,(*gen).v_start);
		
		local_axes[0][0]= result[0];
		local_axes[0][1]= result[1];
		local_axes[0][2]= result[2];
		local_axes[0][3]= result[3];
		
		//"X" axis
		(*gen).v_start[0] = local_axes[1][0];
		(*gen).v_start[1] = local_axes[1][1];
		(*gen).v_start[2] = local_axes[1][2];
		(*gen).v_start[3] = local_axes[1][3];
		
		matrix_mult(S,(*gen).v_start);
		result2[0] = result[0]-local_axes[0][0];
		result2[1] = result[1]-local_axes[0][1];
		result2[2] = result[2]-local_axes[0][2];
		result2[3] = result[3]-local_axes[0][3];
		normalize= sqrt(result2[0]*result2[0]+result2[1]*result2[1]+result2[2]*result2[2]+result2[3]*result2[3]);
		local_axes[1][0]= (result2[0]+local_axes[0][0])/normalize;
		local_axes[1][1]= (result2[1]+local_axes[0][1])/normalize;
		local_axes[1][2]= (result2[2]+local_axes[0][2])/normalize;
		local_axes[1][3]= (result2[3]+local_axes[0][3])/normalize;
		
		//"Y" axis
		(*gen).v_start[0] = local_axes[2][0];
		(*gen).v_start[1] = local_axes[2][1];
		(*gen).v_start[2] = local_axes[2][2];
		(*gen).v_start[3] = local_axes[2][3];
		
		matrix_mult(S,(*gen).v_start);
		result2[0] = result[0]-local_axes[0][0];
		result2[1] = result[1]-local_axes[0][1];
		result2[2] = result[2]-local_axes[0][2];
		result2[3] = result[3]-local_axes[0][3];
		normalize= sqrt(result2[0]*result2[0]+result2[1]*result2[1]+result2[2]*result2[2]+result2[3]*result2[3]);
		local_axes[2][0]= (result2[0]+local_axes[0][0])/normalize;
		local_axes[2][1]= (result2[1]+local_axes[0][1])/normalize;
		local_axes[2][2]= (result2[2]+local_axes[0][2])/normalize;
		local_axes[2][3]= (result2[3]+local_axes[0][3])/normalize;
		
		//"Z" axis
		(*gen).v_start[0] = local_axes[3][0];
		(*gen).v_start[1] = local_axes[3][1];
		(*gen).v_start[2] = local_axes[3][2];
		(*gen).v_start[3] = local_axes[3][3];
		
		matrix_mult(S,(*gen).v_start);
		result2[0] = result[0]-local_axes[0][0];
		result2[1] = result[1]-local_axes[0][1];
		result2[2] = result[2]-local_axes[0][2];
		result2[3] = result[3]-local_axes[0][3];
		normalize= sqrt(result2[0]*result2[0]+result2[1]*result2[1]+result2[2]*result2[2]+result2[3]*result2[3]);
		local_axes[3][0]= (result2[0]+local_axes[0][0])/normalize;
		local_axes[3][1]= (result2[1]+local_axes[0][1])/normalize;
		local_axes[3][2]= (result2[2]+local_axes[0][2])/normalize;
		local_axes[3][3]= (result2[3]+local_axes[0][3])/normalize;
	
	
	for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
			(*gen).v_start = (*gen).vertices[i][j];
			matrix_mult(S,(*gen).v_start);
			(*gen).vertices[i][j] =result;
		
			result[0]=0;
			result[1]=0;
			result[2]=0;
			result[3]=0;
			}
		}
	

}

/*******************************************************
FUNCTION: real_rotation
ARGS: angle and axis
RETURN:
DOES: rotates shape, for real 
********************************************************/
/*TODO. Note: Absolutely no gl calls*/
/*can assume model-matrix stack contains at this point nothing but viewing transform */
void real_rotation(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) {
	int i=0;
	int j=0;
	
	
	//use correct rotation for local axes
	if(x==1){
		set_Rx(deg);
		//origin
		(*gen).v_start = local_axes[0];
		matrix_mult(Rx,(*gen).v_start);
		local_axes[0]= result;
		
		//"X" axis
		(*gen).v_start = local_axes[1];
		matrix_mult(Rx,(*gen).v_start);
		local_axes[1]= result;
		
		//"Y" axis
		(*gen).v_start = local_axes[2];
		matrix_mult(Rx,(*gen).v_start);
		local_axes[2]= result;
		
		//"Z" axis
		(*gen).v_start = local_axes[3];
		matrix_mult(Rx,(*gen).v_start);
		local_axes[3]= result;
	}
	else if(y==1){
		set_Ry(deg);
		
		//origin
		(*gen).v_start = local_axes[0];
		matrix_mult(Ry,(*gen).v_start);
		local_axes[0]= result;
		
		//"X" axis
		(*gen).v_start = local_axes[1];
		matrix_mult(Ry,(*gen).v_start);
		local_axes[1]= result;
		
		//"Y" axis
		(*gen).v_start = local_axes[2];
		matrix_mult(Ry,(*gen).v_start);
		local_axes[2]= result;
		
		//"Z" axis
		(*gen).v_start = local_axes[3];
		matrix_mult(Ry,(*gen).v_start);
		local_axes[3]= result;
		
		
	}
	else if(z==1){
		set_Rz(deg);
		
		//origin
		(*gen).v_start = local_axes[0];
		matrix_mult(Rz,(*gen).v_start);
		local_axes[0]= result;
		
		//"X" axis
		(*gen).v_start = local_axes[1];
		matrix_mult(Rz,(*gen).v_start);
		local_axes[1]= result;
		
		//"Y" axis
		(*gen).v_start = local_axes[2];
		matrix_mult(Rz,(*gen).v_start);
		local_axes[2]= result;
		
		//"Z" axis
		(*gen).v_start = local_axes[3];
		matrix_mult(Rz,(*gen).v_start);
		local_axes[3]= result;
		
	}
	
	
	
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
			(*gen).v_start= (*gen).vertices[i][j];
	
			if(x==1){
				matrix_mult(Rx,(*gen).v_start);
			}
			else if(y==1){
				matrix_mult(Ry,(*gen).v_start);
				
			}
			else if(z==1){
				matrix_mult(Rz,(*gen).v_start);
			}
			(*gen).vertices[i][j]=result;

			result=0;
			}
		}

}


/*******************************************************
FUNCTION: reset
ARGS: 
RETURN:
DOES: resets shape and camera
********************************************************/
/*TODO reset shape post real transformations, as well etc*/
void reset()
{
  glMatrixMode(GL_MODELVIEW) ;
  glLoadIdentity() ;
  x_camera = 0; y_camera = 5; z_camera =30;
  
  //remake objects
  cyl = new Shape(1,cyl_height,cyl_ray,crt_rs,crt_vs);
  sph = new Shape(2,sph_ray,0,crt_rs,crt_vs);
  con = new Shape(3,con_height,con_ray,crt_rs,crt_vs);
  tor = new Shape(4,r1,r2,crt_rs,crt_vs);
  
  
  //reset axes
  local_axes[0][0] = 0;
  local_axes[0][1] = 0;
  local_axes[0][2] = 0;
  local_axes[0][3] = 1;
  
  local_axes[1][0] = 1;
  local_axes[1][1] = 0;
  local_axes[1][2] = 0;
  local_axes[1][3] = 1;
  
  local_axes[2][0] = 0;
  local_axes[2][1] = 1;
  local_axes[2][2] = 0;
  local_axes[2][3] = 1;
  
  local_axes[3][0] = 0;
  local_axes[3][1] = 0;
  local_axes[3][2] = 1;
  local_axes[3][3] = 1;
  


}


/*******************************************************
FUNCTION: my_keyboard
ARGS: key id, x, y
RETURN:
DOES: handles keyboard events
********************************************************/
/*TODO: expand to add hmwk 3 keyboard events */
void my_keyboard( unsigned char key, int x, int y ) {
  switch( key ) {
  case 'O':
  case 'o': {
    reset();
    glutPostRedisplay(); 
  }; break;


  case 'y': {
  	
	switch( crt_transform) {
	case ROTATION_MODE: real_rotation(-pi/16,0,1,0); break;
	case TRANSLATION_MODE: real_translation(0,-1,0); break;
	case REAL_TRANSLATION_MODE: real_translation(0,-1,0); break;
	case REAL_SCALING_MODE: real_scaling(1,.5,1); break;
	case REAL_ROTATION_MODE: real_rotation(-pi/16,0,1,0);break;

	
	
	 
	}
    glutPostRedisplay(); 
  }; break;
  case 'Y': {
	switch( crt_transform) {
	case ROTATION_MODE: real_rotation(pi/16,0,1,0); break;
	case TRANSLATION_MODE: real_translation(0,1,0); break;
	case REAL_TRANSLATION_MODE: real_translation(0,1,0); break;
	case REAL_SCALING_MODE: real_scaling(1,2,1); break;
	case REAL_ROTATION_MODE: real_rotation(pi/16,0,1,0);break;
	//... etc: handle real-translation, rotation, scaling
	
	

	}
    glutPostRedisplay(); 
  }; break;
  case 'x': {
  	
	switch( crt_transform) {
	case ROTATION_MODE: real_rotation(-pi/16,0,1,0); break;
	case TRANSLATION_MODE: real_translation(-1,0,0); break;
	case REAL_TRANSLATION_MODE: real_translation(-1,0,0); break;
	case REAL_SCALING_MODE: real_scaling(.5,1,1); break;
	case REAL_ROTATION_MODE: real_rotation(-pi/16,1,0,0);break;

	
	
	 
	}
    glutPostRedisplay(); 
  }; break;
  case 'X': {
	switch( crt_transform) {
	case ROTATION_MODE: real_rotation(pi/16,0,1,0); break;
	case TRANSLATION_MODE: real_translation(1,0,0); break;
	case REAL_TRANSLATION_MODE: real_translation(1,0,0); break;
	case REAL_SCALING_MODE: real_scaling(2,1,1); break;
	case REAL_ROTATION_MODE: real_rotation(pi/16,1,0,0);break;
	//... etc: handle real-translation, rotation, scaling
	

	}
    glutPostRedisplay(); 
  }; break;
  case 'z': {
  	printf("pressing y: crt_trans: %d\n",crt_transform);
	switch( crt_transform) {
	case ROTATION_MODE: real_rotation(-pi/16,0,0,1); break;
	case TRANSLATION_MODE: real_translation(0,0,-1); break;
	case REAL_TRANSLATION_MODE: real_translation(0,0,-1); break;
	case REAL_SCALING_MODE: real_scaling(1,1,.5); break;
	case REAL_ROTATION_MODE: real_rotation(-pi/16,0,0,1);break;

	

	}
    glutPostRedisplay(); 
  }; break;
  case 'Z': {
	switch( crt_transform) {
	case ROTATION_MODE: real_rotation(pi/16,0,0,1); break;
	case TRANSLATION_MODE: real_translation(0,0,1); break;
	case REAL_TRANSLATION_MODE: real_translation(0,0,1); break;
	case REAL_SCALING_MODE: real_scaling(1,1,2); break;
	case REAL_ROTATION_MODE: real_rotation(pi/16,0,0,1);break;
	
	
	
	}
    glutPostRedisplay(); 
  }; break;



  case '1': {
    crt_shape = CUBE;
    reset();
    glutPostRedisplay();
  }; break;
  case '0': {
    crt_shape = HOUSE;
    reset();
    glutPostRedisplay();
  }; break;
  case '2': {
    crt_shape = TORUS;
    gen = tor;
    reset();
    glutPostRedisplay();
  }; break; 
  case '3':{
    crt_shape = CYLINDER;
    gen = cyl;
    reset();
    glutPostRedisplay();
  }; break;
  case '4': {
    crt_shape = SPHERE;
    gen = sph;
    reset();
    glutPostRedisplay();
  }; break;
  case '5': {
    crt_shape = CONE;
    gen = con;
    reset();
    glutPostRedisplay();
  }; break; 

  //etc ... EC shapes from hmwk2

  case 'S':
  case 's': {
    crt_transform = REAL_SCALING_MODE;
  }; break;

  case 'T':
  case 't': {
    crt_transform = REAL_TRANSLATION_MODE;
  
  }; break;

  case 'R':
  case 'r': {
    crt_transform = REAL_ROTATION_MODE;
  }; break;


 


  //add your tessellation code from hmwk2
  case '+': {
  if(crt_vs<50){
    	crt_vs++;
    }
    
  
    cyl = new Shape(1,cyl_height,cyl_ray,crt_rs,crt_vs);
  	sph = new Shape(2,sph_ray,0,crt_rs,crt_vs);
  	con = new Shape(3,con_height,con_ray,crt_rs,crt_vs);
  	tor = new Shape(4,r1,r2,crt_rs,crt_vs);
  	reset();
  	glutPostRedisplay();
  }; break;

  case '-': {
  if(crt_vs>=1){
    	crt_vs--;
    }
  
    cyl = new Shape(1,cyl_height,cyl_ray,crt_rs,crt_vs);
  	sph = new Shape(2,sph_ray,0,crt_rs,crt_vs);
  	con = new Shape(3,con_height,con_ray,crt_rs,crt_vs);
  	tor = new Shape(4,r1,r2,crt_rs,crt_vs);
  	reset();
  	glutPostRedisplay();
  }; break;

  case '>': {
  	if(crt_rs>=1){
    	crt_rs--;
    }
    
    cyl = new Shape(1,cyl_height,cyl_ray,crt_rs,crt_vs);
  	sph = new Shape(2,sph_ray,0,crt_rs,crt_vs);
  	con = new Shape(3,con_height,con_ray,crt_rs,crt_vs);
  	tor = new Shape(4,r1,r2,crt_rs,crt_vs);
  	reset();
   glutPostRedisplay();
  }; break;

  case '<': {
  if(crt_rs<50){
    	crt_rs++;
    }
    
  
    cyl = new Shape(1,cyl_height,cyl_ray,crt_rs,crt_vs);
  	sph = new Shape(2,sph_ray,0,crt_rs,crt_vs);
  	con = new Shape(3,con_height,con_ray,crt_rs,crt_vs);
  	tor = new Shape(4,r1,r2,crt_rs,crt_vs);
  	reset();
  	glutPostRedisplay();
  }; break;

  case 'q': 
  case 'Q':

    exit(0) ;

    break ;	

  default: break;

  }

  

  return ;

}

/*******************************************************
FUNCTION: my_keyboard_up
ARGS: key id, x, y
RETURN:
DOES: handles keyboard up events
********************************************************/
void my_keyboard_up( unsigned char key, int x, int y ) {
	switch( key ) {
	case 'S':
	case 's': 
	case 'T':
	case 't':
	case 'R':
	case 'r': {
		crt_transform = NONE_MODE; break;
		}
	}
}




/*******************************************************
FUNCTION: my_mouse
ARGS: button, state, x, y
RETURN:
DOES: handles mouse events
********************************************************/

void my_mouse(int button, int state, int mousex, int mousey) {
  switch( button ) {	
  case GLUT_LEFT_BUTTON:
    if( state == GLUT_DOWN ) {
 	 if(spin_val == SPIN_ENABLED) spin_val = SPIN_DISABLED;
	  else spin_val = SPIN_ENABLED;
	  glutPostRedisplay();
    }

    if( state == GLUT_UP ) {
    }
    break ;

  case GLUT_RIGHT_BUTTON:
    if( state == GLUT_DOWN ) {
		if(draw_norms==0){
	 		draw_norms=1;
		}
		else if(draw_norms==1){
			draw_norms=0;
        }
      
      
      glutPostRedisplay();
    }
    if( state == GLUT_UP ) {
    }
    break ;
  }
  
  return ;

}



/***********************************
  FUNCTION: draw_quad 
  ARGS: - a vertex array
        - 4 indices into the vertex array defining a quad face
        - an index into the color array.
  RETURN: none
  DOES:  helper drawing function; draws one quad. 
   For the normal to work out, follow left-hand-rule (i.e., counter clock
   wise) 
*************************************/
void draw_quad(GLfloat vertices[][4], int iv1, int iv2, int iv3, int iv4, int ic) {
  glBegin(crt_render_mode); 
  {
    glColor3fv(colors[ic]);
    /*note the explicit use of homogeneous coords below: glVertex4f*/
    glVertex4fv(vertices[iv1]);
    glVertex4fv(vertices[iv2]);
    glVertex4fv(vertices[iv3]);
    glVertex4fv(vertices[iv4]);
  }
  glEnd();
}


/***********************************
  FUNCTION: draw_triangle 
  ARGS: - a vertex array
        - 3 indices into the vertex array defining a triangular face
        - an index into the color array.
  RETURN: none
  DOES:  helper drawing function; draws one triangle. 
   For the normal to work out, follow left-hand-rule (i.e., ccw)
*************************************/
void draw_triangle(GLfloat vertices[][4], int iv1, int iv2, int iv3, int ic) {
  glBegin(crt_render_mode); 
  {
    glColor3fv(colors[ic]);
    /*note the explicit use of homogeneous coords below: glVertex4f*/
    glVertex4fv(vertices[iv1]);
    glVertex4fv(vertices[iv2]);
    glVertex4fv(vertices[iv3]);
  }
  glEnd();
}

/***********************************
  FUNCTION: draw_param_quad 
  ARGS: - a 50 by 50 by 4 vertex array
        - 2 indices into the vertex array defining the top left of a quad face
        - an index into the color array.
  RETURN: none
  DOES:  helper drawing function; draws one quad. 
*************************************/
/*TODO: redefine and implement as needed, according to your data structures */
void draw_param_quad(GLfloat vertices[][50][4], int line, int col, int ic) {
	double v1[4];
	double v2[4];
	GLfloat v3[4];
	GLfloat v4[4];
	
	//take three points and turn them into the normal vector to the points 
	v1[0]=vertices[line][col][0]-vertices[line+1][col][0];
  	v1[1]=vertices[line][col][1]-vertices[line+1][col][1];
  	v1[2]=vertices[line][col][2]-vertices[line+1][col][2];
  	v1[3]=vertices[line][col][3]-vertices[line+1][col][3];
  	
  	v2[0]=vertices[line+1][col+1][0]-vertices[line+1][col][0];
  	v2[1]=vertices[line+1][col+1][1]-vertices[line+1][col][1];
  	v2[2]=vertices[line+1][col+1][2]-vertices[line+1][col][2];
  	v2[3]=vertices[line+1][col+1][3]-vertices[line+1][col][3];
  	
  	v3[0] = (v1[1]*v2[2]) - (v1[2]*v2[1]);
  	v3[1] = (v1[2]*v2[0]) - (v1[0]*v2[2]);
  	v3[2] = (v1[0]*v2[1]) - (v1[1]*v2[0]);
  	v3[3] = 0;
  	
  	//normalize that vector
  	normalize = sqrt(v3[0]*v3[0]+v3[1]*v3[1]+v3[2]*v3[2]+v3[3]*v3[3]);
  	normals[index_][0] = v3[0]/(normalize);
  	normals[index_][1] = v3[1]/(normalize);
  	normals[index_][2] = v3[2]/(normalize);
  	normals[index_][3] = v3[3]/(normalize);
  	index_++;
	
	glBegin(crt_render_mode); 
  	{
   		glColor3fv(colors[ic]);
   		
   		if(line==crt_vs-1&&crt_shape==SPHERE){
   			glVertex4fv(vertices[line][col]);
    		glVertex4fv(vertices[(line+1)][col]);
    		glVertex4fv(vertices[(line+1)][(col+1)]);
    		glVertex4fv(vertices[line][(col+1)]);
   		}
   		/*note the explicit use of homogeneous coords below: glVertex4f*/
    	glVertex4fv(vertices[line][col]);
    	glVertex4fv(vertices[(line+1)%crt_vs][col]);
    	glVertex4fv(vertices[(line+1)%crt_vs][(col+1)%crt_rs]);
    	glVertex4fv(vertices[line][(col+1)%crt_rs]);
    	
    	
    	
  }
  glEnd();
  // find the average of all the face normals
  v4[0] =(vertices[line][(col+1)%crt_rs][0] + vertices[(line+1)%crt_vs][(col+1)%crt_rs][0]+vertices[line][col][0]+vertices[(line+1)%crt_vs][col][0])/4;
  v4[1] =(vertices[line][(col+1)%crt_rs][1] + vertices[(line+1)%crt_vs][(col+1)%crt_rs][1]+vertices[line][col][1]+vertices[(line+1)%crt_vs][col][1])/4;
  v4[2] =(vertices[line][(col+1)%crt_rs][2] + vertices[(line+1)%crt_vs][(col+1)%crt_rs][2]+vertices[line][col][2]+vertices[(line+1)%crt_vs][col][2])/4;
  v4[3] =(vertices[line][(col+1)%crt_rs][3] + vertices[(line+1)%crt_vs][(col+1)%crt_rs][3]+vertices[line][col][3]+vertices[(line+1)%crt_vs][col][3])/4;
  
  //something wrong with the cylinder that i need to negate the normals
  if(crt_shape==CYLINDER){
  	v3[0] = v4[0] -normals[index_][0];
  	v3[1] = v4[1] -normals[index_][1];
  	v3[2] = v4[2] -normals[index_][2];
  	v3[3] = v4[3] -normals[index_][3];
  
  
  }
  else{
  	v3[0] = v4[0] +normals[index_][0];
  	v3[1] = v4[1] +normals[index_][1];
  	v3[2] = v4[2] +normals[index_][2];
  	v3[3] = v4[3] +normals[index_][3];
  }

	// drawing the normals
  if(draw_norms){
  	glBegin(GL_LINES);
  	{
  			 glColor3fv(colors[ic+2]);
  			glVertex4fv(vertices[line][(col+1)%crt_rs]);
  			glVertex4fv(v3);
  	}
  	glEnd();
  }
  
  	
}

/***********************************
  FUNCTION: draw_house 
  ARGS: none
  RETURN: none
  DOES: helper build function; 
        defines a hollow cube with a bottomless pyramid as rooftop
************************************/
void draw_house()
{
  draw_triangle(vertices_house,0,1,2,RED);
  draw_triangle(vertices_house,0,2,3,GREEN);
  draw_triangle(vertices_house,0,3,4,WHITE);
  draw_triangle(vertices_house,0,4,1,GREY);


  draw_quad(vertices_house,2,1,5,6, BLUE);
  draw_triangle(vertices_house,2,6,3, CYAN);
  draw_triangle(vertices_house,3,6,7, CYAN);
  draw_triangle(vertices_house,3,7,8, YELLOW);
  draw_triangle(vertices_house,8,3,4, YELLOW);
  draw_triangle(vertices_house,4,8,1, MAGENTA);
  draw_triangle(vertices_house,1,8,5, MAGENTA);

}

/***********************************
  FUNCTION: draw_cube_brute 
  ARGS: none
  RETURN: none
  DOES: helper build function; 
        defines a hollow cube without a bottom or a top
************************************/
/*TODO: add bottom and top face triangles*/
void draw_cube_brute()
{

  draw_triangle(vertices_cube_brute, 4,5,1,BLUE);
  draw_triangle(vertices_cube_brute, 0,4,1,BLUE);
  draw_triangle(vertices_cube_brute, 5,6,2,CYAN);
  draw_triangle(vertices_cube_brute, 1,5,2,CYAN);
  draw_triangle(vertices_cube_brute, 3,2,6,YELLOW);
  draw_triangle(vertices_cube_brute, 7,3,6,YELLOW);
  draw_triangle(vertices_cube_brute, 0,3,7,MAGENTA);
  draw_triangle(vertices_cube_brute, 4,0,7,MAGENTA);
  draw_triangle(vertices_cube_brute, 1,2,3,WHITE);
  draw_triangle(vertices_cube_brute, 7,6,5,WHITE);
  
  
  //manually calculate the normals for the cube
  if(draw_norms){
  		glBegin(GL_LINES);
  		{
  			glVertex4f(1,1,-1,1);
  			glVertex4f(1+1/sqrt(3),1+1/sqrt(3),-1-1/sqrt(3),1);
  			glVertex4f(1,-1,-1,1);
  			glVertex4f(1+1/sqrt(3),-1-1/sqrt(3),-1-1/sqrt(3),1);
  			glVertex4d(1,-1,1,1);
  			glVertex4f(1+1/sqrt(3),-1-1/sqrt(3),1+1/sqrt(3),1);
  			glVertex4d(-1,-1,1,1);
  			glVertex4f(-1-1/sqrt(3),-1-1/sqrt(3),1+1/sqrt(3),1);
  			glVertex4d(-1,1,1,1);
  			glVertex4f(-1-1/sqrt(3),1+1/sqrt(3),1+1/sqrt(3),1);
  			glVertex4d(-1,1,-1,1);
  			glVertex4f(-1-1/sqrt(3),1+1/sqrt(3),-1-1/sqrt(3),1);
  			glVertex4d(-1,-1,-1,1);
  			glVertex4f(-1-1/sqrt(3),-1-1/sqrt(3),-1-1/sqrt(3),1);
  			glVertex4d(1,1,1,1);
  			glVertex4f(1+1/sqrt(3),1+1/sqrt(3),1+1/sqrt(3),1);
  		}
  		glEnd();

  }
}

/***********************************
  FUNCTION: draw_axes
  ARGS: none
  RETURN: none
  DOES: draws main X, Y, Z axes
************************************/
void draw_axes( void ) {
    glLineWidth( 5.0 );
    glBegin(GL_LINES); 
  {
    glColor3fv(colors[1]);
    glVertex4fv(vertices_axes[0]);
    glVertex4fv(vertices_axes[1]);

    glColor3fv(colors[4]);
    glVertex4fv(vertices_axes[0]);
    glVertex4fv(vertices_axes[2]);

    glColor3fv(colors[6]);
    glVertex4fv(vertices_axes[0]);
    glVertex4fv(vertices_axes[3]);
  }
	glEnd();
	glLineWidth( 1.0 );

}

/***********************************
  FUNCTION: draw_local axes
  ARGS: none
  RETURN: none
  DOES: draws local X, Y, Z axes
************************************/
void draw_local_axes( void ) {
    glLineWidth( 5.0 );
    glBegin(GL_LINES); 
  {
    glColor3fv(colors[1]);
    glVertex4fv(local_axes[0]);
    glVertex4fv(local_axes[1]);

    glColor3fv(colors[4]);
    glVertex4fv(local_axes[0]);
    glVertex4fv(local_axes[2]);

    glColor3fv(colors[6]);
    glVertex4fv(local_axes[0]);
    glVertex4fv(local_axes[3]);
  }
	glEnd();
	glLineWidth( 1.0 );

}





/*************************************
FUNCTION: make_*; reuse your stitcher code here.
*************************************/
void make_cube_smart( double size ){
}

/***********************************
  FUNCTION: make_cylinder 
  ARGS: height (along the Y axis), ray, and tesselation parameter (how many side faces) 
  RETURN: none
  DOES: helper build function; 
        defines a cylinder centered at the origin, 
        of height 'height' along the Y axis,
        and ray 'ray' in the XOZ plane
************************************/


//drawing function for cylinder
void draw(int rs, int vs)
{
	int i;
	int j;
	index_=0;
	for(i=0;i<vs;i++){
		for(j=0;j<rs;j++){
			draw_param_quad((*gen).vertices,i,j,GREEN);
		}
	}
}


//set up the x rotation matrix
void set_Rx(double theta){
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

//set up the y rotation matrix
void set_Ry(double theta){
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
// set up the z rotation matrix
void set_Rz(double theta){
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
// set up the translation matrix
void set_T(double tx,double ty, double tz){
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
void set_S(double sx,double sy, double sz){
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
// matrix multiplication function multiplies a 4x4 by a 4x1
void matrix_mult(GLfloat M[][4],GLfloat V[]){


	double ans=0;
	int i;
	int j;
	
	GLfloat stored[4];
	stored = V;
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



/***********************************
  FUNCTION: draw_object 
  ARGS: shape to create (HOUSE, CUBE, CYLINDER, etc)
  RETURN: none
  DOES: draws an object from triangles
************************************/
/*TODO: expand according to assignment 3 specs*/
void draw_object(int shape) {

  switch(shape){
  case HOUSE: draw_house(); break;
  case CUBE: draw_cube_brute(); break; 
  case CYLINDER: 
  case SPHERE: 
  case CONE: 
  case TORUS: draw(crt_rs,crt_vs) ; break;
  

  default: break;
  }

}

/***********************************
  FUNCTION: my_display
  ARGS: none
  RETURN: none
  DOES: main drawing function
************************************/
/*TODO add on*/
void my_display() {

  // clear all pixels, reset depth 
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
  
   // init to identity 

   glLoadIdentity() ;

   gluLookAt(x_camera, y_camera, z_camera,  // x,y,z coord of the camera 
	    0.0, 0.0, 0.0,  // x,y,z LookAt
	    0.0, 1.0, 0.0); // the direction of Up (default is y-axis)

   draw_axes();
   draw_local_axes();//local axes


   draw_object(crt_shape);

  // this buffer is ready

  glutSwapBuffers();

}



/***********************************
  FUNCTION: my_TimeOut
  ARGS: timer id
  RETURN: none
  DOES: handles "timer" events
************************************/
void my_TimeOut(int id) {
  /* spin counter-clockwise*/
  /* schedule next timer event, 0.2 secs from now */ 
  if(spin_val == SPIN_ENABLED){
   
    real_rot_arbitrary_axis_point(pi/256,local_axes[2][0]-local_axes[0][0],local_axes[2][1]-local_axes[0][1],local_axes[2][2]-local_axes[0][2],local_axes[0][0],local_axes[0][1],local_axes[0][2]);
	glutPostRedisplay();
	
  };
  glutTimerFunc(20, my_TimeOut, 0);

  return ;
}


/***********************************
  FUNCTION: my_idle
  ARGS: none
  RETURN: none
  DOES: handles the "idle" event
************************************/
void my_idle(void) {
}
