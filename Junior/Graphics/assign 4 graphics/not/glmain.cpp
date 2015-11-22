/**************************************************************************
File: glmain.c
Does: basic lighting and modeling for cs1566 hw4 Modeler
Author: Steven Lauck, based on some hwa
Date: 01/08/09
**************************************************************************/

#include "glmain.h"
#include <glut/glut.h> 


#define my_assert(X,Y) ((X)?(void) 0:(printf("error:%s in %s at %d", Y, __FILE__, __LINE__), myabort()))


#define min(a,b) ((a) < (b)? a:b)
#define FALSE 0 
#define TRUE  1
#define MAX_LIGHTS  8
#define NUM_OBJECTS 8




typedef struct _Object {
  int sid;

  // example object storage arrays for vertex and normals
  GLfloat vertices_cube_smart[8][4];
  GLfloat normals_cube_smart[8][3];
 
  GLfloat shine;
  GLfloat emi[4];
  GLfloat amb[4];
  GLfloat diff[4];
  GLfloat spec[4];

  GLfloat translate[4];
  GLfloat scale[4];
  GLfloat rotate[4];

}OBJECT;

typedef struct _CAM{
  GLfloat pos[4];
  GLfloat at[4];
  GLfloat up[4];

  GLfloat dir[4];
}CAM;

typedef struct _LITE{
  GLfloat amb[4];
  GLfloat diff[4];
  GLfloat spec[4];
  GLfloat pos[4];
  GLfloat dir[3];
  GLfloat angle;
}LITE;

GLfloat colors [][3] = {
  {0.0, 0.0, 0.0},  /* black   */
  {1.0, 0.0, 0.0},  /* red     */
  {1.0, 1.0, 0.0},  /* yellow  */
  {1.0, 0.0, 1.0},  /* magenta */
  {0.0, 1.0, 0.0},  /* green   */
  {0.0, 1.0, 1.0},  /* cyan    */
  {0.0, 0.0, 1.0},  /* blue    */
  {0.5, 0.5, 0.5},  /* 50%grey */
  {1.0, 1.0, 1.0}   /* white   */
};

GLfloat vertices_axes[][4] = {
	{0.0, 0.0, 0.0, 1.0},  /* origin */ 
	{5.0, 0.0, 0.0, 1.0},  /* maxx */ 
	{0.0, 5.0, 0.0, 1.0}, /* maxy */ 
	{0.0, 0.0, 5.0, 1.0}  /* maxz */ 

};


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
Shape* po;

void real_scaling(GLfloat sx, GLfloat sy, GLfloat sz);
void real_translation(GLfloat tx, GLfloat ty, GLfloat tz);
void real_rotation(GLfloat deg,GLfloat rx, GLfloat ry, GLfloat rz);

Shape my_objects[NUM_OBJECTS];

LITE my_lights[MAX_LIGHTS];
int num_objects;
int  num_lights;

// camera variables
CAM my_cam;
GLfloat camx, camy, camz;
GLfloat atx, aty, atz;
GLfloat upx, upy, upz;

//TRUE or FALSE
int firstPersonView;

int crt_render_mode;
int crt_shape, crt_rs, crt_vs;
int crt_transform;

void myabort(void) {
  abort();
  exit(1); /* exit so g++ knows we don't return. */
} 

int main(int argc, char** argv)
{ 
  setbuf(stdout, NULL);   /* for writing to stdout asap */
  glutInit(&argc, argv);

  my_setup(argc, argv);  
  glut_setup();
  gl_setup();

  glutMainLoop();
  return(0);
}


void glut_setup (){

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  
  glutInitWindowSize(700,700);
  glutInitWindowPosition(20,20);
  glutCreateWindow("CS1566 Project 4");

  /* set up callback functions */
  glutDisplayFunc(my_display);
  glutReshapeFunc(my_reshape);
  glutMouseFunc(my_mouse);
  glutMotionFunc(my_mouse_drag);
  glutKeyboardFunc(my_keyboard);
  glutIdleFunc( my_idle );	

  return;
}

void gl_setup(void) {

  // enable depth handling (z-buffer)
  glEnable(GL_DEPTH_TEST);

  // enable auto normalize
  glEnable(GL_NORMALIZE);

  // define the background color 
  glClearColor(0,0,0,1);

  glMatrixMode(GL_PROJECTION) ;
  glLoadIdentity() ;
  gluPerspective( 40, 1.0, 1, 200.0);
  glMatrixMode(GL_MODELVIEW) ;
  glLoadIdentity() ;  // init modelview to identity

  // toggle to smooth shading (instead of flat)
  glShadeModel(GL_SMOOTH); 
  lighting_setup();


  return ;
}

void my_setup(int argc, char **argv){

  firstPersonView = 1;
  num_objects = num_lights = 0;

  // initialize global shape defaults and mode for drawing
  crt_render_mode = GL_POLYGON;
  crt_shape = 0;

  crt_rs = 40;
  crt_vs = 40; 
 
  //If you want to allow the user to type in the spec file
  //then modify the following code.
  //Otherwise, the program will attempt to load the file as specified
  //on the command line:
  //EX: ./glmain spec3
  my_assert(argc >1, "spec");
  read_spec(argv[1]);
  printf("read\n");
  return;
}

void parse_floats(char *buffer, GLfloat nums[]) {
  int i;
  char *ps;

  ps = strtok(buffer, " ");
  for (i=0; ps; i++) {
    nums[i] = atof(ps);
    ps = strtok(NULL, " ");
    printf("read %f ",nums[i]);
  }

}

void parse_obj(char *buffer){
  
  char *pshape, *pshine, *pemi, *pamb, *pdiff, *pspec, *ptranslate, *pscale, *protate;



  my_assert ((num_objects < NUM_OBJECTS), "too many objects");
  po = &my_objects[num_objects++];

  pshape  = strtok(buffer, " ");
  //printf("pshape is %s\n",pshape);

  ptranslate    = strtok(NULL, "()");  strtok(NULL, "()");
  pscale        = strtok(NULL, "()");  strtok(NULL, "()"); 
  protate       = strtok(NULL, "()");  strtok(NULL, "()");  

  pshine  = strtok(NULL, "()");strtok(NULL, "()");
  //printf("pshine is %s\n",pshine);
 
  pemi    = strtok(NULL, "()");  strtok(NULL, "()"); 
  pamb    = strtok(NULL, "()");  strtok(NULL, "()"); 
  pdiff   = strtok(NULL, "()");  strtok(NULL, "()"); 
  pspec   = strtok(NULL, "()");  strtok(NULL, "()"); 


  po->sid  = atoi(pshape);
  po->shine = atof(pshine);

  parse_floats(ptranslate, po->translate);
  parse_floats(pscale, po->scale);
  parse_floats(protate, po->rotate);

  parse_floats(pemi, po->emi);
  parse_floats(pamb, po->amb);
  parse_floats(pdiff, po->diff);
  parse_floats(pspec, po->spec);
  
  printf("scale: %f",po->scale[0]);
  // use switch to create your objects, cube given as example
  switch (po->sid){
  case 1: //cube
    make_cube_smart(po, 1);
    break;
  }
  
  // scale, rotate, translate using your real tranformations from assignment 3 depending on input from spec file
   printf("read object\n");
  real_scaling(po->scale[0], po->scale[1], po->scale[2]);  
  real_rotation(po->rotate[0], 1, 0, 0);
  real_rotation(po->rotate[1], 0, 1, 0);
  real_rotation(po->rotate[2], 0, 0, 1);
  real_translation(po->translate[0], po->translate[1], po->translate[2]);
  
  printf("read object\n");
}

void parse_camera(char *buffer){
  CAM *pc;
  char *ppos, *plook, *pup;

  pc = &my_cam;

  strtok(buffer, "()");
  ppos  = strtok(NULL, "()");  strtok(NULL, "()"); 
  plook  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pup  = strtok(NULL, "()");  strtok(NULL, "()"); 

  parse_floats(ppos, pc->pos);
  parse_floats(plook, pc->at);
  parse_floats(pup, pc->up);

  pc->at[0] += pc->pos[0];
  pc->at[1] += pc->pos[1];
  pc->at[2] += pc->pos[2];

  pc->dir[0] = pc->at[0] - pc->pos[0];
  pc->dir[1] = pc->at[1] - pc->pos[1];
  pc->dir[2] = pc->at[2] - pc->pos[2];
  normalized(pc->dir);
  printf("read camera\n");
}

void parse_light(char *buffer){
  LITE *pl;
  char *pamb, *pdiff, *pspec, *ppos, *pdir, *pang;
  my_assert ((num_lights < MAX_LIGHTS), "too many lights");
  pl = &my_lights[++num_lights];

  strtok(buffer, "()");
  pamb  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pdiff = strtok(NULL, "()");  strtok(NULL, "()"); 
  pspec = strtok(NULL, "()");  strtok(NULL, "()"); 
  ppos  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pdir  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pang  = strtok(NULL, "()");

  parse_floats(pamb, pl->amb);
  parse_floats(pdiff, pl->diff);
  parse_floats(pspec, pl->spec);
  parse_floats(ppos, pl->pos);
  if (pdir) {
    parse_floats(pdir, pl->dir);
    pl->angle = atof(pang);
    //printf("angle %f\n", pl->angle);
  }
  else
    pl->dir[0]= pl->dir[1]= pl->dir[2] =0;
    
  printf("read light\n");

}

/* assuming the spec is going to be properly written
   not error-checking here */
void read_spec(char *fname) {
  char buffer[300];
  FILE *fp;
  printf("in read spec\n");
  fp = fopen(fname, "r");
  my_assert(fp, "can't open spec");
  while(!feof(fp)){
    fgets(buffer, 300, fp);
    //printf("read line: %s\n", buffer);
    switch (buffer[0]) {
    case '#':
      break;
    case '1': //cube
   	  //read in the cube
	  parse_obj(buffer);
 	  break;
	//etc

    case 'l':
      parse_light(buffer);
      break;

    case 'c':
      parse_camera(buffer);
      break;

    default:
      break;
    }
  }
}

void lighting_setup () {
  int i;
  GLfloat globalAmb[]     = {.1, .1, .1, .1};

  // create flashlight
  GLfloat amb[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat dif[] = {0.8, 0.8, 0.8, 1.0};
  GLfloat spec[] = {5.0, 5.0, 5.0, 1.0};

  //enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  // reflective propoerites -- global ambiant light
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

  glLightfv(GL_LIGHT0, GL_POSITION, my_cam.at);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, my_cam.dir);

  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);

  glEnable(GL_LIGHT0);

  // setup properties of lighting
  for (i=1; i<=num_lights; i++) {
    glEnable(GL_LIGHT0+i);
    glLightfv(GL_LIGHT0+i, GL_AMBIENT, my_lights[i].amb);
    glLightfv(GL_LIGHT0+i, GL_DIFFUSE, my_lights[i].diff);
    glLightfv(GL_LIGHT0+i, GL_SPECULAR, my_lights[i].spec);
    glLightfv(GL_LIGHT0+i, GL_POSITION, my_lights[i].pos);
    if ((my_lights[i].dir[0] > 0) ||  (my_lights[i].dir[1] > 0) ||  (my_lights[i].dir[2] > 0)) {
      glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, my_lights[i].angle);
      glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, my_lights[i].dir);
    }
  }

}

void my_reshape(int w, int h) {
  // ensure a square view port
  glViewport(0,0,min(w,h),min(w,h)) ;
  return ;
}

//For the following 3 functions, do with them as you like.
//You will have to somehow rotate / translate / scale\
//the object as specified in the spec file.

void my_rev(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) {
 
}

void my_trans(GLfloat x, GLfloat y, GLfloat z) {

}

void my_scale(GLfloat x, GLfloat y, GLfloat z) {

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

	//real_rot_arbitrary_axis_point(deg,x,y,z,0,0,0);

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
	
	
	
	for(i=0;i<crt_vs;i++){
		for(j=0;j<crt_rs;j++){
			(*po).v_start[0] = (*po).vertices[i][j][0];
			(*po).v_start[1] = (*po).vertices[i][j][1];
			(*po).v_start[2] = (*po).vertices[i][j][2];
			(*po).v_start[3] = (*po).vertices[i][j][3];
			matrix_mult(T,(*po).v_start);
			(*po).vertices[i][j][0] =result[0];
			(*po).vertices[i][j][1] =result[1];
			(*po).vertices[i][j][2] =result[2];
			(*po).vertices[i][j][3] =result[3];
		
		result[0]=0;
		result[1]=0;
		result[2]=0;
		result[3]=0;
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
		(*po).v_start[0] = local_axes[0][0];
		(*po).v_start[1] = local_axes[0][1];
		(*po).v_start[2] = local_axes[0][2];
		(*po).v_start[3] = local_axes[0][3];
		printf("flagflag\n");
		matrix_mult(S,(*po).v_start);
		
		local_axes[0][0]= result[0];
		local_axes[0][1]= result[1];
		local_axes[0][2]= result[2];
		local_axes[0][3]= result[3];
		
		//"X" axis
		(*po).v_start[0] = local_axes[1][0];
		(*po).v_start[1] = local_axes[1][1];
		(*po).v_start[2] = local_axes[1][2];
		(*po).v_start[3] = local_axes[1][3];
		
		matrix_mult(S,(*po).v_start);
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
		(*po).v_start[0] = local_axes[2][0];
		(*po).v_start[1] = local_axes[2][1];
		(*po).v_start[2] = local_axes[2][2];
		(*po).v_start[3] = local_axes[2][3];
		
		matrix_mult(S,(*po).v_start);
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
		(*po).v_start[0] = local_axes[3][0];
		(*po).v_start[1] = local_axes[3][1];
		(*po).v_start[2] = local_axes[3][2];
		(*po).v_start[3] = local_axes[3][3];
		
		matrix_mult(S,(*po).v_start);
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
			(*po).v_start[0] = (*po).vertices[i][j][0];
			(*po).v_start[1] = (*po).vertices[i][j][1];
			(*po).v_start[2] = (*po).vertices[i][j][2];
			(*po).v_start[3] = (*po).vertices[i][j][3];
			matrix_mult(S,(*po).v_start);
			(*po).vertices[i][j][0] =result[0];
			(*po).vertices[i][j][1] =result[1];
			(*po).vertices[i][j][2] =result[2];
			(*po).vertices[i][j][3] =result[3];
		
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
		(*po).v_start[0] = local_axes[0][0];
		(*po).v_start[1] = local_axes[0][1];
		(*po).v_start[2] = local_axes[0][2];
		(*po).v_start[3] = local_axes[0][3];
		
		matrix_mult(Rx,(*po).v_start);
		local_axes[0][0]= result[0];
		local_axes[0][1]= result[1];
		local_axes[0][2]= result[2];
		local_axes[0][3]= result[3];
		
		//"X" axis
		(*po).v_start[0] = local_axes[1][0];
		(*po).v_start[1] = local_axes[1][1];
		(*po).v_start[2] = local_axes[1][2];
		(*po).v_start[3] = local_axes[1][3];
		
		matrix_mult(Rx,(*po).v_start);
		local_axes[1][0]= result[0];
		local_axes[1][1]= result[1];
		local_axes[1][2]= result[2];
		local_axes[1][3]= result[3];
		
		//"Y" axis
		(*po).v_start[0] = local_axes[2][0];
		(*po).v_start[1] = local_axes[2][1];
		(*po).v_start[2] = local_axes[2][2];
		(*po).v_start[3] = local_axes[2][3];
		
		matrix_mult(Rx,(*po).v_start);
		local_axes[2][0]= result[0];
		local_axes[2][1]= result[1];
		local_axes[2][2]= result[2];
		local_axes[2][3]= result[3];
		
		//"Z" axis
		(*po).v_start[0] = local_axes[3][0];
		(*po).v_start[1] = local_axes[3][1];
		(*po).v_start[2] = local_axes[3][2];
		(*po).v_start[3] = local_axes[3][3];
		
		matrix_mult(Rx,(*po).v_start);
		local_axes[3][0]= result[0];
		local_axes[3][1]= result[1];
		local_axes[3][2]= result[2];
		local_axes[3][3]= result[3];
	}
	else if(y==1){
		set_Ry(deg);
		
		//origin
		(*po).v_start[0] = local_axes[0][0];
		(*po).v_start[1] = local_axes[0][1];
		(*po).v_start[2] = local_axes[0][2];
		(*po).v_start[3] = local_axes[0][3];
		
		matrix_mult(Ry,(*po).v_start);
		local_axes[0][0]= result[0];
		local_axes[0][1]= result[1];
		local_axes[0][2]= result[2];
		local_axes[0][3]= result[3];
		
		//"X" axis
		(*po).v_start[0] = local_axes[1][0];
		(*po).v_start[1] = local_axes[1][1];
		(*po).v_start[2] = local_axes[1][2];
		(*po).v_start[3] = local_axes[1][3];
		
		matrix_mult(Ry,(*po).v_start);
		local_axes[1][0]= result[0];
		local_axes[1][1]= result[1];
		local_axes[1][2]= result[2];
		local_axes[1][3]= result[3];
		
		//"Y" axis
		(*po).v_start[0] = local_axes[2][0];
		(*po).v_start[1] = local_axes[2][1];
		(*po).v_start[2] = local_axes[2][2];
		(*po).v_start[3] = local_axes[2][3];
		
		matrix_mult(Ry,(*po).v_start);
		local_axes[2][0]= result[0];
		local_axes[2][1]= result[1];
		local_axes[2][2]= result[2];
		local_axes[2][3]= result[3];
		
		//"Z" axis
		(*po).v_start[0] = local_axes[3][0];
		(*po).v_start[1] = local_axes[3][1];
		(*po).v_start[2] = local_axes[3][2];
		(*po).v_start[3] = local_axes[3][3];
		
		matrix_mult(Ry,(*po).v_start);
		local_axes[3][0]= result[0];
		local_axes[3][1]= result[1];
		local_axes[3][2]= result[2];
		local_axes[3][3]= result[3];
		
		
	}
	else if(z==1){
		set_Rz(deg);
		
		//origin
		(*po).v_start[0] = local_axes[0][0];
		(*po).v_start[1] = local_axes[0][1];
		(*po).v_start[2] = local_axes[0][2];
		(*po).v_start[3] = local_axes[0][3];
		
		matrix_mult(Rz,(*po).v_start);
		local_axes[0][0]= result[0];
		local_axes[0][1]= result[1];
		local_axes[0][2]= result[2];
		local_axes[0][3]= result[3];
		
		//"X" axis
		(*po).v_start[0] = local_axes[1][0];
		(*po).v_start[1] = local_axes[1][1];
		(*po).v_start[2] = local_axes[1][2];
		(*po).v_start[3] = local_axes[1][3];
		
		matrix_mult(Rz,(*po).v_start);
		local_axes[1][0]= result[0];
		local_axes[1][1]= result[1];
		local_axes[1][2]= result[2];
		local_axes[1][3]= result[3];
		
		//"Y" axis
		(*po).v_start[0] = local_axes[2][0];
		(*po).v_start[1] = local_axes[2][1];
		(*po).v_start[2] = local_axes[2][2];
		(*po).v_start[3] = local_axes[2][3];
		
		matrix_mult(Rz,(*po).v_start);
		local_axes[2][0]= result[0];
		local_axes[2][1]= result[1];
		local_axes[2][2]= result[2];
		local_axes[2][3]= result[3];
		
		//"Z" axis
		(*po).v_start[0] = local_axes[3][0];
		(*po).v_start[1] = local_axes[3][1];
		(*po).v_start[2] = local_axes[3][2];
		(*po).v_start[3] = local_axes[3][3];
		
		matrix_mult(Rz,(*po).v_start);
		local_axes[3][0]= result[0];
		local_axes[3][1]= result[1];
		local_axes[3][2]= result[2];
		local_axes[3][3]= result[3];
		
	}
	
	
	
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
			(*po).v_start[0] = (*po).vertices[i][j][0];
			(*po).v_start[1] = (*po).vertices[i][j][1];
			(*po).v_start[2] = (*po).vertices[i][j][2];
			(*po).v_start[3] = (*po).vertices[i][j][3];
			if(x==1){
				matrix_mult(Rx,(*po).v_start);
			}
			else if(y==1){
				matrix_mult(Ry,(*po).v_start);
				
			}
			else if(z==1){
				matrix_mult(Rz,(*po).v_start);
			}
			(*po).vertices[i][j][0] =result[0];
			(*po).vertices[i][j][1] =result[1];
			(*po).vertices[i][j][2] =result[2];
			(*po).vertices[i][j][3] =result[3];
		
			result[0]=0;
			result[1]=0;
			result[2]=0;
			result[3]=0;
			}
		}

}



void my_keyboard( unsigned char key, int x, int y ) {
  
  switch( key ) {
  case ' ':
    //code to switch between first person view and back as appropriate
    glutPostRedisplay();
    break;
  case 'd':
    my_rev(-3,0,1,0);
    glutPostRedisplay() ;
    break;
  case 'a':
    my_rev(3, 0, 1, 0);
    glutPostRedisplay() ;
    break;
  case 'w':
    my_trans(0,0,-1);
    glutPostRedisplay();
    break;
  case 's':
    my_trans(0,0,1);
    glutPostRedisplay();
    break;
  case 'q': 
  case 'Q':
    exit(0) ;
    break ;	
  default: break;
  }
  
  return ;
}

void my_mouse_drag(int x, int y) {
}

void my_mouse(int button, int state, int mousex, int mousey) {

  switch( button ) {
	
  case GLUT_LEFT_BUTTON:
    if( state == GLUT_DOWN ) {
    }
    
    if( state == GLUT_UP ) {
    }
    break ;

  case GLUT_RIGHT_BUTTON:
    if ( state == GLUT_DOWN ) {
    }
    
    if( state == GLUT_UP ) {
    }
    break ;
  }
  

  
  return ;
}

float dotprod(float v1[], float v2[]) {
  float tot = 0;
  int i;
  for (i=0; i<4; i++)
    tot += v1[i]*v2[i];
  return tot;
}


void normalized(GLfloat *p) { 
  double d=0.0;
  int i;
  for(i=0; i<3; i++) d+=p[i]*p[i];
  d=sqrt(d);
  if(d > 0.0) for(i=0; i<3; i++) p[i]/=d;
}

void cross(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d) { 
  d[0]=(b[1]-a[1])*(c[2]-a[2])-(b[2]-a[2])*(c[1]-a[1]);
  d[1]=(b[2]-a[2])*(c[0]-a[0])-(b[0]-a[0])*(c[2]-a[2]);
  d[2]=(b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
  normalized(d);
}

void print_matrix(float my_matrix[])
{ 
  int i, j;

  for (i=0; i<4; i++) {
    for (j=0; j<4; j++) {
      printf ("%f ", my_matrix[i+j*4]);
    }
    printf ("\n");
  }
  printf ("\n");
}

void make_quad(GLfloat vertices[][3]) {
  glBegin(GL_POLYGON); 
  {
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
  }
  glEnd();
}

/*************************************
 FUNCTION: make_*; reuse your stitcher code here.
*************************************/
void make_cube_smart(Shape *po, double size){
  int i;  
  // compute verts on PI/4 angels for x y z, then -x y z  
  
  printf("we in derr: %f\n",size*cos((M_PI/4)));
  for(i = 0; i < 4; i++){
    po->vertices_cube_smart[i][0] = size*cos((M_PI/4));
   
    po->vertices_cube_smart[i][1] = -size*sin(i*(M_PI/2)+(M_PI/4));
    po->vertices_cube_smart[i][2] = size*cos(i*(M_PI/2)+(M_PI/4));
    po->vertices_cube_smart[i][3] = 1;
    // mirror on x axis
    po->vertices_cube_smart[i+4][0] = -size*cos((M_PI/4));
    po->vertices_cube_smart[i+4][1] = -size*sin(i*(M_PI/2)+(M_PI/4));
    po->vertices_cube_smart[i+4][2] = size*cos(i*(M_PI/2)+(M_PI/4));
    po->vertices_cube_smart[i+4][3] = 1;
  }

  //compute normals
    cross(po->vertices_cube_smart[0], po->vertices_cube_smart[1], po->vertices_cube_smart[2], po->normals_cube_smart[0]);
    cross(po->vertices_cube_smart[1], po->vertices_cube_smart[5], po->vertices_cube_smart[6], po->normals_cube_smart[1]);
    cross(po->vertices_cube_smart[5], po->vertices_cube_smart[4], po->vertices_cube_smart[7], po->normals_cube_smart[2]);
    cross(po->vertices_cube_smart[4], po->vertices_cube_smart[0], po->vertices_cube_smart[3], po->normals_cube_smart[3]);
    
    printf("heyyyy\n");
    
    for(i=0;i<8;i++){
    	printf("vertx: %f\n",po->vertices_cube_smart[i][0]);
    	printf("verty: %f\n",po->vertices_cube_smart[i][1]);
    	printf("vertz: %f\n",po->vertices_cube_smart[i][2]);
    	printf("vertw: %f\n",po->vertices_cube_smart[i][3]);
    }
    
    
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
void draw_quad(GLfloat vertices[][4], GLfloat *normals, int iv1, int iv2, int iv3, int iv4, int ic) {
  glBegin(crt_render_mode); 
  {
    glColor3fv(colors[ic]);
    glNormal3fv(normals);
    /*note the explicit use of homogeneous coords below: glVertex4f*/
    glVertex4fv(vertices[iv1]);
    glVertex4fv(vertices[iv2]);
    glVertex4fv(vertices[iv3]);
    glVertex4fv(vertices[iv4]);
    printf("vertices1: %f\n",vertices[iv1]);
    printf("vertices2: %f\n",vertices[iv2]);
    printf("vertices3: %f\n",vertices[iv3]);
    printf("vertices4: %f\n",vertices[iv4]);
    
  }
  glEnd();
}

/***********************************
 FUNCTION: draw_cube_smart() 
 ARGS: none
 RETURN: none
 DOES: draws a cube from quads
************************************/
void draw_cube_smart(Shape *po)
{
  /* sides */
  draw_quad(po->vertices_cube_smart, po->normals_cube_smart[0],0,1,2,3, BLUE);
  draw_quad(po->vertices_cube_smart, po->normals_cube_smart[1],1,5,6,2, BLUE);
  draw_quad(po->vertices_cube_smart, po->normals_cube_smart[2],5,4,7,6, BLUE);
  draw_quad(po->vertices_cube_smart, po->normals_cube_smart[3],4,0,3,7, BLUE);
  
  /* top and bottom */
  //draw_quad(po->vertices_cube_smart,3,2,6,7, BLUE);
  //draw_quad(po->vertices_cube_smart,0,1,5,4, BLUE);
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

void draw_objects() {
  int i;
  for(i=0; i<num_objects; i++){
    Shape *cur;
    cur = &my_objects[i];

    glMaterialfv(GL_FRONT, GL_AMBIENT, cur->amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cur->diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cur->spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, &cur->shine);
    //glMaterialfv(GL_FRONT, GL_EMISSION, cur->emi);

  switch(cur->sid){
  	case 1: //cube
    	draw_cube_smart(cur);
    	break;
  	default: break;
  }

  }
}

void my_display() {

  // clear all pixels, reset depth 
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
  
  glLoadIdentity();
  //setup the camera (1st person? 3rd person?)

  gluLookAt(my_cam.pos[0],my_cam.pos[1], my_cam.pos[2],
	    my_cam.at[0],my_cam.at[1],my_cam.at[2],
	    my_cam.up[0], my_cam.up[1], my_cam.up[2]);
	    
  printf("posx: %f \tposy: %f \tposz: %f \natx: %f \taty: %f \tatz: %f \nupx: %f \tupy: %f \tupz: %f \n",my_cam.pos[0],my_cam.pos[1], my_cam.pos[2],
	    my_cam.at[0],my_cam.at[1],my_cam.at[2], my_cam.up[0], my_cam.up[1], my_cam.up[2]);

  //update the flashlight to follow the person
  
  //draw the objects
  draw_axes();

  draw_objects();

  // this buffer is ready
  glutSwapBuffers();
}

// matrix multiplication function multiplies a 4x4 by a 4x1
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

void my_idle(void) {
  //EC idea: Make the flashlight flicker a bit (random flicker strength) when the user is idle.
  return ;
}