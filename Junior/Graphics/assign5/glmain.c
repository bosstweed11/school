/**************************************************************************
File: glmain.c
Does: basic lighting and modeling for cs1566 hw4 Modeler
Author: Steven Lauck, based on some hwa
Date: 01/08/09
**************************************************************************/

#include "glmain.h"
#define my_assert(X,Y) ((X)?(void) 0:(printf("error:%s in %s at %d", Y, __FILE__, __LINE__), myabort()))

#define min(a,b) ((a) < (b)? a:b)
#define FALSE 0 
#define TRUE  1
#define MAX_LIGHTS  8
#define NUM_OBJECTS 400


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
GLfloat left;
GLfloat right;
GLfloat near;
GLfloat far;
GLfloat top;
GLfloat bottom;
GLdouble width;
GLdouble height;
float lineStart[3];
float lineEnd[3];

GLfloat heightang=40;
GLfloat aspectratio=1;

//object
typedef struct _Object {
  int sid;
  double ray;
  // example object storage arrays for vertex and normals
  GLfloat vertices_cube_smart[8][4];
  GLfloat normals_cube_smart[8][3];
  GLfloat vertices_cylinder[50][50][4];
  GLfloat vertices_sphere[50][50][4];
  GLfloat vertices_cone[50][50][4];
  GLfloat vertices_torus[50][50][4];
  GLfloat normals[50][50][3];
  //GLfloat vertices_house[][4] //cube with top point?  //didnt get to this
  GLfloat v_start[4];
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
  GLfloat u[4];
  GLfloat v[4];
  GLfloat w[4];

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

//functions
void make_cube_smart(OBJECT *po, double size );
void make_cylinder(OBJECT *po);
void draw_cylinder(OBJECT *po);
void make_cone(OBJECT *po,double,double);
void draw_cone(OBJECT *po);
void make_sphere(OBJECT *po,double ray);
void draw_sphere(OBJECT *po);
void make_torus(OBJECT *po);
void create_camera();
void draw_torus(OBJECT *po);
void draw_triangle(GLfloat vertices[][4], int iv1, int iv2, int iv3, int ic);
void draw_param_quad(OBJECT *po,GLfloat vertices[][50][4], int line, int col, int ic);
void real_translation(OBJECT *po, GLfloat x, GLfloat y, GLfloat z);
void real_scaling(OBJECT *po, GLfloat sx, GLfloat sy, GLfloat sz);
void real_rotation(OBJECT *po, GLfloat deg, GLfloat x, GLfloat y, GLfloat z);

OBJECT my_objects[NUM_OBJECTS];
LITE my_lights[MAX_LIGHTS];
int num_objects;
int  num_lights;
OBJECT *hit_point;

// camera variables
CAM my_cam;
GLfloat camx, camy, camz;
GLfloat atx, aty, atz;
GLfloat upx, upy, upz;

//TRUE or FALSE
int firstPersonView;
int ortho=0;
int crt_render_mode;
int crt_shape, crt_rs, crt_vs;
int crt_transform;
GLfloat S[4][4];

void myabort(void) {
  abort();
  exit(1); /* exit so g++ knows we don't return. */
} 

int main(int argc, char** argv)
{ 
  setbuf(stdout, NULL);   /* for writing to stdout asap */
  glutInit(&argc, argv);

  my_setup(argc, argv); 
  
  //set up camera bounds
  left =-10;
  right =10;
  bottom =-10;
  top =10;
  near =-10;
  far =10; 
  
  width = right-left;
 height = top -bottom;
 //set up ortho matrix
  S[0][0] = 2/width;
  S[0][1] =0;
  S[0][2] =0;
  S[0][3] =0;
  S[1][0] =0;
  S[1][1] = 2/height;
  S[1][2] =0;
  S[1][3] =0;
  S[2][0] =0;
  S[2][1] =0;
  S[2][2] =1/far;
  S[2][3] =0;
  S[3][0] =0;
  S[3][1] =0;
  S[3][2] =0;
  S[3][3] =1;
  
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

	near =1;
	far =200;
  // enable depth handling (z-buffer)
  glEnable(GL_DEPTH_TEST);

  // enable auto normalize
  glEnable(GL_NORMALIZE);

  // define the background color 
  glClearColor(0,0,0,1);
  
  glMatrixMode(GL_PROJECTION) ;
  glLoadIdentity() ;
  gluPerspective(heightang, aspectratio, near, far);
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
  my_assert(argc >1, "need to supply a spec file");
  read_spec(argv[1]);
  return;
}

void parse_floats(char *buffer, GLfloat nums[]) {
  int i;
  char *ps;

  ps = strtok(buffer, " ");
  for (i=0; ps; i++) {
    nums[i] = atof(ps);
    ps = strtok(NULL, " ");
    //printf("read %f ",nums[i]);
  }

}

void parse_obj(char *buffer){
  OBJECT *po;
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
	
  // use switch to create your objects, cube given as example
  switch (po->sid){
  case 1: //cube
    make_cube_smart(po, 1);
    break;
  case 3: //sph
  	po->ray =1;
    make_sphere(po,po->ray);
    break;
  case 4: //cyl
  	make_cylinder(po);
  	break;
  case 5: //cone
  	make_cone(po,1,1);
  	break;
  case 6: //tor
  	make_torus(po);
  	break;
  }
  
  // scale, rotate, translate using your real tranformations from assignment 3 depending on input from spec file
  
  real_scaling(po, po->scale[0], po->scale[1], po->scale[2]);  
  real_rotation(po, po->rotate[0], 1, 0, 0);
  real_rotation(po, po->rotate[1], 0, 1, 0);
  real_rotation(po, po->rotate[2], 0, 0, 1);
  real_translation(po, po->translate[0], po->translate[1], po->translate[2]);
  
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
  normalize(pc->dir);
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
 	case '3': //sph
 	  parse_obj(buffer);
 	  break;
 	case '4': //cyl
 	  //read in the cyl
 	  parse_obj(buffer);
 	  break;
 	  
 	case '5': //cone
 	  parse_obj(buffer);
 	  break; 
 	case '6': //tor
 	  parse_obj(buffer);
 	  break;
	//etc

    case 'l':
      parse_light(buffer);
      break;

    case 'c':
      parse_camera(buffer);
      create_camera();//read in camera matrix
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
  glEnable(GL_COLOR_MATERIAL);

  // reflective propoerites -- global ambiant light
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

  // this was for the flashlights

 // glLightfv(GL_LIGHT0, GL_POSITION, my_cam.at);
 // glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, my_cam.dir);

 // glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
 // glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
 // glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
 // glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);

 // glEnable(GL_LIGHT0);

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

// reuse code from assignment 3 here to do transformations when reading spec
void real_translation(OBJECT *po, GLfloat x, GLfloat y, GLfloat z) {

	
	//my real trans switch on object for verts
	int i;
	int j;
	set_T(x,y,z);
	
	switch(po->sid){
		case 1:
		for(i=0;i<8;i++){
			matrix_mult(T,po->vertices_cube_smart[i]);
			po->vertices_cube_smart[i][0]= result[0];
			po->vertices_cube_smart[i][1]= result[1];
			po->vertices_cube_smart[i][2]= result[2];
			po->vertices_cube_smart[i][3]= result[3];
		}
		break;
		case 3:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(T,po->vertices_sphere[i][j]);
				po->vertices_sphere[i][j][0]= result[0];
				po->vertices_sphere[i][j][1]= result[1];
				po->vertices_sphere[i][j][2]= result[2];
				po->vertices_sphere[i][j][3]= result[3];
			}
		}
		break;
		case 4:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(T,po->vertices_cylinder[i][j]);
				po->vertices_cylinder[i][j][0]= result[0];
				po->vertices_cylinder[i][j][1]= result[1];
				po->vertices_cylinder[i][j][2]= result[2];
				po->vertices_cylinder[i][j][3]= result[3];
			}
		}
		break;
		case 5:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(T,po->vertices_cone[i][j]);
				po->vertices_cone[i][j][0]= result[0];
				po->vertices_cone[i][j][1]= result[1];
				po->vertices_cone[i][j][2]= result[2];
				po->vertices_cone[i][j][3]= result[3];
			}
		}
		break;
		case 6:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(T,po->vertices_torus[i][j]);
				po->vertices_torus[i][j][0]= result[0];
				po->vertices_torus[i][j][1]= result[1];
				po->vertices_torus[i][j][2]= result[2];
				po->vertices_torus[i][j][3]= result[3];
			}
		}
		break;
		
	}
	
}

void real_scaling(OBJECT *po, GLfloat sx, GLfloat sy, GLfloat sz) {
	int i;
	int j;
	
	set_S(sx,sy,sz);
	
	switch(po->sid){
		case 1:
		for(i=0;i<8;i++){
			matrix_mult(S,po->vertices_cube_smart[i]);
			po->vertices_cube_smart[i][0]= result[0];
			po->vertices_cube_smart[i][1]= result[1];
			po->vertices_cube_smart[i][2]= result[2];
			po->vertices_cube_smart[i][3]= result[3];
		
		}
		break;
		case 3:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(S,po->vertices_sphere[i][j]);
				po->vertices_sphere[i][j][0]= result[0];
				po->vertices_sphere[i][j][1]= result[1];
				po->vertices_sphere[i][j][2]= result[2];
				po->vertices_sphere[i][j][3]= result[3];
			}
		}
		break;
		case 4:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(S,po->vertices_cylinder[i][j]);
				po->vertices_cylinder[i][j][0]= result[0];
				po->vertices_cylinder[i][j][1]= result[1];
				po->vertices_cylinder[i][j][2]= result[2];
				po->vertices_cylinder[i][j][3]= result[3];
			}
		}
		break;
		case 5:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(S,po->vertices_cone[i][j]);
				po->vertices_cone[i][j][0]= result[0];
				po->vertices_cone[i][j][1]= result[1];
				po->vertices_cone[i][j][2]= result[2];
				po->vertices_cone[i][j][3]= result[3];
			}
		}
		break;
		case 6:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				
				matrix_mult(S,po->vertices_torus[i][j]);
				po->vertices_torus[i][j][0]= result[0];
				po->vertices_torus[i][j][1]= result[1];
				po->vertices_torus[i][j][2]= result[2];
				po->vertices_torus[i][j][3]= result[3];
			}
		}
		break;
	}

	
}

void real_rotation(OBJECT *po, GLfloat deg, GLfloat x, GLfloat y, GLfloat z) {

	int i;
	int j;
	
	if(x!=0){
		set_Rx(deg);
	}
	else if(y!=0){
		set_Ry(deg);
	}
	else if(z!=0){
		set_Rz(deg);
	}
	
	switch(po->sid){
		case 1:
		for(i=0;i<8;i++){
			if(x!=0){
				matrix_mult(Rx,po->vertices_cube_smart[i]);
			}
			else if(y!=0){
				matrix_mult(Ry,po->vertices_cube_smart[i]);
			}
			else if(z!=0){
				matrix_mult(Rz,po->vertices_cube_smart[i]);
			}
			
			po->vertices_cube_smart[i][0]= result[0];
			po->vertices_cube_smart[i][1]= result[1];
			po->vertices_cube_smart[i][2]= result[2];
			po->vertices_cube_smart[i][3]= result[3];
		
		}
		break;
		case 3:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				if(x!=0){
					matrix_mult(Rx,po->vertices_sphere[i][j]);
				}
				else if(y!=0){
					matrix_mult(Ry,po->vertices_sphere[i][j]);
				}
				else if(z!=0){
					matrix_mult(Rz,po->vertices_sphere[i][j]);
				}
				po->vertices_sphere[i][j][0]= result[0];
				po->vertices_sphere[i][j][1]= result[1];
				po->vertices_sphere[i][j][2]= result[2];
				po->vertices_sphere[i][j][3]= result[3];
			}
		}
		break;
		case 4:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				if(x!=0){
					matrix_mult(Rx,po->vertices_cylinder[i][j]);
				}
				else if(y!=0){
					matrix_mult(Ry,po->vertices_cylinder[i][j]);
				}
				else if(z!=0){
					matrix_mult(Rz,po->vertices_cylinder[i][j]);
				}
				po->vertices_cylinder[i][j][0]= result[0];
				po->vertices_cylinder[i][j][1]= result[1];
				po->vertices_cylinder[i][j][2]= result[2];
				po->vertices_cylinder[i][j][3]= result[3];
			}
		}
		break;
		case 5:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				if(x!=0){
					matrix_mult(Rx,po->vertices_cone[i][j]);
				}
				else if(y!=0){
					matrix_mult(Ry,po->vertices_cone[i][j]);
				}
				else if(z!=0){
					matrix_mult(Rz,po->vertices_cone[i][j]);
				}
				po->vertices_cone[i][j][0]= result[0];
				po->vertices_cone[i][j][1]= result[1];
				po->vertices_cone[i][j][2]= result[2];
				po->vertices_cone[i][j][3]= result[3];
			}
		}
		break;
		case 6:
		for(i=0;i<crt_vs;i++){
			for(j=0;j<crt_rs;j++){
				if(x!=0){
					matrix_mult(Rx,po->vertices_torus[i][j]);
				}
				else if(y!=0){
					matrix_mult(Ry,po->vertices_torus[i][j]);
				}
				else if(z!=0){
					matrix_mult(Rz,po->vertices_torus[i][j]);
				}
				po->vertices_torus[i][j][0]= result[0];
				po->vertices_torus[i][j][1]= result[1];
				po->vertices_torus[i][j][2]= result[2];
				po->vertices_torus[i][j][3]= result[3];
			}
		}
		break;
			
		
	}
	
	
}



void my_keyboard( unsigned char key, int x, int y ) {
  
  switch( key ) {

  
  case 'p':
  case 'P':{
  	 if(ortho){
  		printf("Orthographic camera:\nEye x:%f\nEye y:%f\nEye z:%f\nLook x:%f\nLook y:%f\nLook z:%f\nUp x:%f\nUp y:%f\nUp z:%f\n\nAspect ratio: %f\nCamera Matrix:\nS[0][0] =%f\tS[0][1] =%f\tS[0][2] =%f\tS[0][3] =%f\nS[1][0] =%f\tS[1][1] =%f\tS[1][2] =%f\tS[1][3] =%f\nS[2][0] =%f\tS[2][1] =%f\tS[2][2] =%f\tS[2][3] =%f\nS[3][0] =%f\tS[3][1] =%f\tS[3][2] =%f\tS[3][3] =%f\n",my_cam.pos[0],my_cam.pos[1], my_cam.pos[2],my_cam.at[0],my_cam.at[1],my_cam.at[2],my_cam.up[0], my_cam.up[1], my_cam.up[2],(right-left)/(top-bottom),S[0][0],S[0][1],S[0][2],S[0][3],S[1][0],S[1][1],S[1][2],S[1][3],S[2][0],S[2][1],S[2][2],S[2][3],S[3][0],S[3][1],S[3][2],S[3][3]);
  	}
  	else{
  		printf("Perspective camera:\nEye x:%f\nEye y:%f\nEye z:%f\nLook x:%f\nLook y:%f\nLook z:%f\nUp x:%f\nUp y:%f\nUp z:%f\nHeight Angle: %f\nAspect Ratio: %f\n",my_cam.pos[0],my_cam.pos[1], my_cam.pos[2],my_cam.at[0],my_cam.at[1],my_cam.at[2],my_cam.up[0], my_cam.up[1], my_cam.up[2],heightang,aspectratio);
  	}
  };break;
  
  //change cameras 
  case 'C':
  case 'c':{
  	if(ortho){
  		ortho =0;
  	}
  	else{
  	 ortho =1;
  	}
  	glutPostRedisplay();
  };break;
  //far plane move
  case 'F':
  case 'f':{
  	crt_transform = 2;
  };
  break;
  
  //zoom
  case 'L':
  case 'l':{
  	crt_transform = 5;
  };
  break;
  // near plane
  case 'N':
  case 'n':{
  	crt_transform = 4;
  };
  //height angle move
  break;
   case 'H':
  case 'h':{
  	crt_transform = 5;
  };
  break;
  //aspect ratio move
   case 'A':
  case 'a':{
  	crt_transform = 6;
  };
  break;
  break;
  //switch for all adds
  case '+':{
  	switch(crt_transform){
  		case 2:
  		{
  			far+=2;
  		};
  		break;
  		
  		case 4:
  		{
  		if(near+2<=far){
  				near +=2;
  			}
  		};
  		break;
  		case 5:{
  			heightang+=1;
  		};
  		break;
  		case 6:{
  			aspectratio+=.1;
  			
  		};
  		break;
  };
  glutPostRedisplay();
  break;
  };
  break;
  //switch for all minuses
  case '-':{
  	switch(crt_transform){
  		case 2:
  		{
  		if(far-2<=my_cam.pos[2]){
  			far-=2;
  		}
  		};
  		break;
  		case 4:
  		{
  			if(near-2<=my_cam.pos[2]){
  				near -=2;
  			}
  		};
  		break;
  		case 5:{
  			heightang-=1;
  		};
  		break;
  		case 6:{
  			aspectratio-=.1;
  			
  		};
  		break;
  		
  };
  glutPostRedisplay();
  break;
  };
  break;
  
  //translate
  case 'T':
  case 't': {
    crt_transform = 1;
  
  }; break;
  
  //used for world translate
  case 'x':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] -=.04;
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
  case 'X':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] +=.04;
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
  	
  case 'y':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[1] -=.04;
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
  case 'Y':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[1] +=.04;
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
   
   case 'z':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[2] -=.04;
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
  
  case 'Z':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[2] +=.04;
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
  	//translate about local
  case 'u':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] -=.04*my_cam.u[0];
  			my_cam.pos[1] -=.04*my_cam.u[1];
  			my_cam.pos[2] -=.04*my_cam.u[2];
  			my_cam.at[0] -=.04*my_cam.u[0];
  			my_cam.at[1] -=.04*my_cam.u[1];
  			my_cam.at[2] -=.04*my_cam.u[2];
  			glutPostRedisplay();
  			};break;
  	}
  	
  	};break;
  case 'U':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] +=.04*my_cam.u[0];
  			my_cam.pos[1] +=.04*my_cam.u[1];
  			my_cam.pos[2] +=.04*my_cam.u[2];
  			my_cam.at[0] +=.04*my_cam.u[0];
  			my_cam.at[1] +=.04*my_cam.u[1];
  			my_cam.at[2] +=.04*my_cam.u[2];
  			glutPostRedisplay();
  			};break;
  	}
  	
  	};break;
  	
  case 'v':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] -=.04*my_cam.v[0];
  			my_cam.pos[1] -=.04*my_cam.v[1];
  			my_cam.pos[2] -=.04*my_cam.v[2];
  			my_cam.at[0] -=.04*my_cam.v[0];
  			my_cam.at[1] -=.04*my_cam.v[1];
  			my_cam.at[2] -=.04*my_cam.v[2];
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
  
  case 'V':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] +=.04*my_cam.v[0];
  			my_cam.pos[1] +=.04*my_cam.v[1];
  			my_cam.pos[2] +=.04*my_cam.v[2];
  			my_cam.at[0] +=.04*my_cam.v[0];
  			my_cam.at[1] +=.04*my_cam.v[1];
  			my_cam.at[2] +=.04*my_cam.v[2];
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
   
   case 'w':{
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] -=.04*my_cam.w[0];
  			my_cam.pos[1] -=.04*my_cam.w[1];
  			my_cam.pos[2] -=.04*my_cam.w[2];
  			my_cam.at[0] -=.04*my_cam.w[0];
  			my_cam.at[1] -=.04*my_cam.w[1];
  			my_cam.at[2] -=.04*my_cam.w[2];
  			};break;
  	}
  	glutPostRedisplay();
  	};break;
  
  case 'W': {
  	switch(crt_transform){
  		case 1:{
  			my_cam.pos[0] +=.04*my_cam.w[0];
  			my_cam.pos[1] +=.04*my_cam.w[1];
  			my_cam.pos[2] +=.04*my_cam.w[2];
  			my_cam.at[0] +=.04*my_cam.w[0];
  			my_cam.at[1] +=.04*my_cam.w[1];
  			my_cam.at[2] +=.04*my_cam.w[2];
  			};break;
  	}
  	glutPostRedisplay();
  	}; break;
  	
  
  case 'q': 
  case 'Q':
    exit(0) ;
    break ;	
    
    
	
  default: break;
  }
  glMatrixMode(GL_PROJECTION) ;
  glLoadIdentity() ;
  gluPerspective(heightang, aspectratio, near, far);
  glMatrixMode(GL_MODELVIEW) ;
  
  return ;
}

void my_mouse_drag(int x, int y) {
}

int my_raytrace_cube(OBJECT *s, float *rayStart, float *rayDirection, float result[3])
{
	// If there is an intersection, this should set result to the closest one to the ray start.

	// should return non-zero there is an intersection
	float t1;
	float t2;
	float t3;
	float t4;
	float t5;
	float t6;
	float t[6];
	float min;
	int ind=0;
	int hit=0;
	float z=0;
	float rt2 = sqrt(2)/2;
	
	t1 =(-rayStart[0]-.5)/rayDirection[0];
	t2 =(-rayStart[0]+.5)/rayDirection[0];
	t3 =(-rayStart[1]-.5)/rayDirection[1];
	t4 =(-rayStart[1]+.5)/rayDirection[1];
	t5 =(-rayStart[2]-.5)/rayDirection[2];
	t6 =(-rayStart[2]+.5)/rayDirection[2];
	
	
	
	t[0]=t1;
	t[1]=t2;
	t[2]=t3;
	t[3]=t4;
	t[4]=t5;
	t[5]=t6;
	int k;
	
	
	
	for(k=0;k<6;k++){
		
		result[0] = rayStart[0] + rayDirection[0]*t[k];
		result[1] = rayStart[1] + rayDirection[1]*t[k];
		result[2] = rayStart[2] + rayDirection[2]*t[k];
		
		
		
		if((result[2]>=-rt2&&result[2]<=rt2)&&(result[1]<=rt2&&result[1]>=-rt2)&&(result[0]<=rt2&&result[0]>=-rt2)){
			if(z!=0){
				if(result[2]>z){
					ind=k;
					
				}
			}
			else{
				ind =k;
				z = result[2];
			}
			
			hit=1;
		}
		
	}
	
	
	if(hit){
		printf("You have an intersection!\n");
		result[0] = rayStart[0] + rayDirection[0]*t[ind];
		result[1] = rayStart[1] + rayDirection[1]*t[ind];
		result[2] = rayStart[2] + rayDirection[2]*t[ind];
		
	}
	

	return hit; 
}

int my_raytrace_cone(OBJECT *s, float *rayStart, float *rayDirection, float result[3])
{
	// If there is an intersection, this should set result to the closest one to the ray start.

	// should return non-zero there is an intersection
	double a;
	double b;
	double c;
	double dis;
	double t1;
	double t2;
	double t3;
	int hit;
	
	a=rayDirection[0]*rayDirection[0] - rayDirection[1]*rayDirection[1] +rayDirection[2]*rayDirection[2];
	b = 2*(rayStart[0]*rayDirection[0]-rayStart[1]*rayStart[1]+rayDirection[2]*rayStart[2]+rayDirection[1]);
	c = rayStart[0]*rayStart[0]-rayStart[1]*rayStart[1]+rayStart[2]*rayStart[2]+2*rayStart[1]-1;
	
	dis = b*b - 4*a*c;
	
	
	t3 = -(rayStart[1]/rayDirection[1]);
	
	if(dis<0 && t3<=0){
		hit=0;
		
	}
	else if(dis<0 && t3>0){
		result[0]=rayStart[0]+rayDirection[0]*t3;
				result[1]=rayStart[1]+rayDirection[1]*t3;
				result[2]=rayStart[2]+rayDirection[2]*t3;
		hit=1;
		
	
	}
	else{
		
		t1= (-b + sqrt(dis))/(2*a);
		t2= (-b - sqrt(dis))/(2*a);
		
		if(t1>0 &&t2>0){
			if(t1<t2){
				if(t1<t3||t3==0){
					result[0]=rayStart[0]+rayDirection[0]*t1;
					result[1]=rayStart[1]+rayDirection[1]*t1;
					result[2]=rayStart[2]+rayDirection[2]*t1;
					hit=1;
					
					
				}
				else if(t3>0){
					result[0]=rayStart[0]+rayDirection[0]*t3;
					result[1]=rayStart[1]+rayDirection[1]*t3;
					result[2]=rayStart[2]+rayDirection[2]*t3;
					hit=1;
					
				
				}
			
			}
			else{
				if(t2<t3||t3==0){
					result[0]=rayStart[0]+rayDirection[0]*t2;
					result[1]=rayStart[1]+rayDirection[1]*t2;
					result[2]=rayStart[2]+rayDirection[2]*t2;
					hit=1;
					
				}
				else if(t3>0){
					result[0]=rayStart[0]+rayDirection[0]*t3;
					result[1]=rayStart[1]+rayDirection[1]*t3;
					result[2]=rayStart[2]+rayDirection[2]*t3;
					hit=1;
				
				}
			
			}
		
		}
		else if(t1>0&&t2<0){
			if(t1<t3||t3==0){
				result[0]=rayStart[0]+rayDirection[0]*t1;
				result[1]=rayStart[1]+rayDirection[1]*t1;
				result[2]=rayStart[2]+rayDirection[2]*t1;
				hit=1;
				
			}
			else if(t3>0){
				result[0]=rayStart[0]+rayDirection[0]*t3;
				result[1]=rayStart[1]+rayDirection[1]*t3;
				result[2]=rayStart[2]+rayDirection[2]*t3;
				hit=1;
			
			}
		
		}
		else if(t1<0&&t2>0){
			if(t2<t3||t3==0){
				result[0]=rayStart[0]+rayDirection[0]*t2;
				result[1]=rayStart[1]+rayDirection[1]*t2;
				result[2]=rayStart[2]+rayDirection[2]*t2;
				hit=1;
				
			
			}
			else if(t3>0){
				result[0]=rayStart[0]+rayDirection[0]*t3;
				result[1]=rayStart[1]+rayDirection[1]*t3;
				result[2]=rayStart[2]+rayDirection[2]*t3;
				hit=1;
				
			
			}
		
		}
		else if(t3>0){
			result[0]=rayStart[0]+rayDirection[0]*t3;
				result[1]=rayStart[1]+rayDirection[1]*t3;
				result[2]=rayStart[2]+rayDirection[2]*t3;
			hit=1;
			
		}
	
	}
	
	
	if(result[1]<=0||result[1]>=1){
		
		hit=0;
	}
	
	//IMPLICIT CONE EQUATION

	return hit; 
}


int my_raytrace_cyl(OBJECT *s, float *rayStart, float *rayDirection, float result[3])
{
	// If there is an intersection, this should set result to the closest one to the ray start.

	// should return non-zero there is an intersection
	
	float a;
	float b;
	float c;
	float dis;
	float t1;
	float t2;
	int hit=0;
	
	a = rayDirection[0]*rayDirection[0] +rayDirection[2]*rayDirection[2];
	b = 2*(rayStart[0]*rayDirection[0]+rayStart[2]*rayDirection[2]);
	c = rayStart[0]*rayStart[0]+rayStart[2]*rayStart[2]-1;
	
	dis = b*b -4*a*c;
	if(dis>0){
		t1 = (-b + sqrt(dis))/(2*a);
		t2 = (-b - sqrt(dis))/(2*a);
		
		if(t1 >0 && t2>0){
			if(t1<t2){
				result[0] = rayStart[0]+rayDirection[0]*t1;
				result[1] = rayStart[1]+rayDirection[1]*t1;
				result[2] = rayStart[2]+rayDirection[2]*t1;
				
				if(result[1]>1||result[1]<-1){
					hit=0;
				
				}
				else{
					hit=1;
				}
				
			}
			else{
				result[0] = rayStart[0]+rayDirection[0]*t2;
				result[1] = rayStart[1]+rayDirection[1]*t2;
				result[2] = rayStart[2]+rayDirection[2]*t2;
				
				if(result[1]>1||result[1]<-1){
					hit=0;
				
				}
				else{
					hit=1;
				}
			}
		}
		else if(t1>0 && t2<0){
			result[0] = rayStart[0]+rayDirection[0]*t1;
			result[1] = rayStart[1]+rayDirection[1]*t1;
			result[2] = rayStart[2]+rayDirection[2]*t1;
			
			if(result[1]>1||result[1]<-1){
					hit=0;
				
			}
			else{
				hit=1;
			}
		}
		else if(t1<0 && t2>0){
			result[0] = rayStart[0]+rayDirection[0]*t2;
			result[1] = rayStart[1]+rayDirection[1]*t2;
			result[2] = rayStart[2]+rayDirection[2]*t2;
		
			if(result[1]>1||result[1]<-1){
					hit=0;
				
			}
			else{
				hit=1;
			}
		}
		else{
			hit =0;
		
		}
	}
	else{
		hit =0;
	}
	//IMPLICIT CYLINDER EQUATION

	return hit; 
}

int my_raytrace_sph(OBJECT *s, float *rayStart, float *rayDirection, float result[3])
{
	// If there is an intersection, this should set result to the closest one to the ray start.

	// should return non-zero there is an intersection
	
	float a;
	float b;
	float c;
	float dis;
	float t1;
	float t2;
	int hit=0;
	
	a = rayDirection[0]*rayDirection[0]+rayDirection[1]*rayDirection[1]+rayDirection[2]*rayDirection[2];
	b = 2*(rayStart[0]*rayDirection[0]+rayStart[1]*rayDirection[1]+rayStart[2]*rayDirection[2]);
	c = rayStart[0]*rayStart[0]+rayStart[1]*rayStart[1]+rayStart[2]*rayStart[2]-1;
	
	dis = b*b -4*a*c;
	if(dis>0){
		t1 = (-b + sqrt(dis))/(2*a);
		t2 = (-b - sqrt(dis))/(2*a);
		
		if(t1 >0 && t2>0){
			if(t1<t2){
				result[0] = rayStart[0]+rayDirection[0]*t1;
				result[1] = rayStart[1]+rayDirection[1]*t1;
				result[2] = rayStart[2]+rayDirection[2]*t1;
				hit =1;
				
			}
			else{
				result[0] = rayStart[0]+rayDirection[0]*t2;
				result[1] = rayStart[1]+rayDirection[1]*t2;
				result[2] = rayStart[2]+rayDirection[2]*t2;
				hit =1;
			}
		}
		else if(t1>0 && t2<0){
			result[0] = rayStart[0]+rayDirection[0]*t1;
			result[1] = rayStart[1]+rayDirection[1]*t1;
			result[2] = rayStart[2]+rayDirection[2]*t1;
			hit =1;
		}
		else if(t1<0 && t2>0){
			result[0] = rayStart[0]+rayDirection[0]*t2;
			result[1] = rayStart[1]+rayDirection[1]*t2;
			result[2] = rayStart[2]+rayDirection[2]*t2;
			hit =1;
		}
		else{
			hit =0;
		
		}
	}
	else{
		hit =0;
	}
	
	//IMPLICIT SPHERE EQUATION
	


	return hit; 
}

void my_raytrace(int mousex, int mousey)
{
	double modelViewMatrix[16];
	double projMatrix[16];
	float heldSta[3];
	float heldDir[3];
	int viewport[4];
	int foundIntersection = 0;
	int hit = 0;
	int i;
	double clickPoint[3];
	GLfloat clickedPoint[4];
	GLfloat intersectionPoint[3];
	float closestPoint[3];
	float rayStart[3];
	float rayDirection[3];
	OBJECT *cur;
	double norm;

	// first we need to get the modelview matrix, the projection matrix, and the viewport
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	mousey = viewport[3]-mousey;

	// gluUnProject with a Z value of 1 will find the point on the far clipping plane
	// corresponding the the mouse click. This is not the same as the vector
	// representing the click.
	gluUnProject(mousex, mousey, 1.0, modelViewMatrix, projMatrix, viewport, &clickPoint[0], &clickPoint[1], &clickPoint[2]);

	// Now we need a vector representing the click. It should start at the camera
	// position. We can subtract the click point, we will get the vector
	

	/* code for finding direction vector, set rayStart and rayDirection */
	rayStart[0] = my_cam.pos[0];
	rayStart[1] = my_cam.pos[1];
	rayStart[2] = my_cam.pos[2];
	
	
	rayDirection[0] = clickPoint[0]-rayStart[0];
	rayDirection[1] = clickPoint[1]-rayStart[1];
	rayDirection[2] = clickPoint[2]-rayStart[2];
			
	norm = sqrt(rayDirection[0]*rayDirection[0]+rayDirection[1]*rayDirection[1]+rayDirection[2]*rayDirection[2]);
		
	rayDirection[0] = rayDirection[0]/norm;
	rayDirection[1] = rayDirection[1]/norm;
	rayDirection[2] = rayDirection[2]/norm;
	
	heldDir[0] = rayDirection[0];
	heldDir[1] = rayDirection[1];
	heldDir[2] = rayDirection[2];

	heldSta[0] = rayStart[0];
	heldSta[1] = rayStart[1];
	heldSta[2] = rayStart[2];
	
	
	lineStart[0]=rayStart[0];
	lineStart[1]=rayStart[1];
	lineStart[2]=rayStart[2];
	
	lineEnd[0]=clickPoint[0];
	lineEnd[1]=clickPoint[1];
	lineEnd[2]=clickPoint[2];
	
	
	
	
	
	//printf("CLICKED POINT:\nclick x: %f\nclick y: %f\nclick z: %f\n",clickPoint[0],clickPoint[1],clickPoint[2]);
	

	// now go through the shapes and see if there is a hit
	for (i=0; i<num_objects; i++)
	{
		cur = my_objects + i;
		hit = 0;
		rayDirection[0] = heldDir[0];
		rayDirection[1] = heldDir[1];
		rayDirection[2] = heldDir[2];

		rayStart[0] = heldSta[0];
		rayStart[1] = heldSta[1];
		rayStart[2] = heldSta[2];
		

		switch (cur->sid)
		{
		case 1:
			//translation
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_T(-cur->translate[0],-cur->translate[1],-cur->translate[2]);
				matrix_mult(T,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				//rotation point
				
				//RzT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				
				
				
				
				
				
				//scaling point
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				//rotation vector
				
				//RzT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				
				//scaling vector
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
			
			hit = my_raytrace_cube(cur, rayStart, rayDirection, intersectionPoint);
			
			//scale intersect
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_S(cur->scale[0],cur->scale[1],cur->scale[2]);
				matrix_mult(S,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
				
				
				//rotate intersect
				
				//Rx
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rx(cur->rotate[0]);
				matrix_mult(Rx,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Ry
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Ry(cur->rotate[1]);
				matrix_mult(Ry,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Rz
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rz(cur->rotate[2]);
				matrix_mult(Rz,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				
				//translate intersect
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_T(cur->translate[0],cur->translate[1],cur->translate[2]);
				matrix_mult(T,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
			
			//scale vector
			break;
		case 3:
			if(cur->ray ==1){
				//translation
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_T(-cur->translate[0],-cur->translate[1],-cur->translate[2]);
				matrix_mult(T,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				//rotation point
				
				//RzT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				
				
				
				
				
				
				//scaling point
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				//rotation vector
				
				//RzT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				
				//scaling vector
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				
				
				
				
					
				hit = my_raytrace_sph(cur, rayStart, rayDirection, intersectionPoint);
				
				//scale intersect
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_S(cur->scale[0],cur->scale[1],cur->scale[2]);
				matrix_mult(S,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
				
				
				//rotate intersect
				
				//Rx
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rx(cur->rotate[0]);
				matrix_mult(Rx,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Ry
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Ry(cur->rotate[1]);
				matrix_mult(Ry,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Rz
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rz(cur->rotate[2]);
				matrix_mult(Rz,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				
				//translate intersect
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_T(cur->translate[0],cur->translate[1],cur->translate[2]);
				matrix_mult(T,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
				
				
				
			
			}	
			break;
		case 4:
			//scale point
			//translation
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_T(-cur->translate[0],-cur->translate[1],-cur->translate[2]);
				matrix_mult(T,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				//rotation point
				
				//RzT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				
				
				
				
				
				
				//scaling point
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				//rotation vector
				
				//RzT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				
				//scaling vector
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
			hit = my_raytrace_cyl(cur, rayStart, rayDirection, intersectionPoint);
			//scale intersect
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_S(cur->scale[0],cur->scale[1],cur->scale[2]);
				matrix_mult(S,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
				
				
				//rotate intersect
				
				//Rx
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rx(cur->rotate[0]);
				matrix_mult(Rx,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Ry
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Ry(cur->rotate[1]);
				matrix_mult(Ry,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Rz
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rz(cur->rotate[2]);
				matrix_mult(Rz,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				
				//translate intersect
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_T(cur->translate[0],cur->translate[1],cur->translate[2]);
				matrix_mult(T,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
			break;
		case 5:
			//translation
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_T(-cur->translate[0],-cur->translate[1],-cur->translate[2]);
				matrix_mult(T,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				//rotation point
				
				//RzT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				
				
				
				
				
				
				//scaling point
				clickedPoint[0] = rayStart[0];
				clickedPoint[1] = rayStart[1];
				clickedPoint[2] =rayStart[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayStart[0] = result[0];
				rayStart[1] = result[1];
				rayStart[2] = result[2];
				
				
				//rotation vector
				
				//RzT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RzT(cur->rotate[2]);
				matrix_mult(RzT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RyT
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RyT(cur->rotate[1]);
				matrix_mult(RyT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				//RxT
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				set_RxT(cur->rotate[0]);
				matrix_mult(RxT,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
				
				
				//scaling vector
				
				clickedPoint[0] = rayDirection[0];
				clickedPoint[1] = rayDirection[1];
				clickedPoint[2] =rayDirection[2];
				clickedPoint[3] =1;
				
				set_S(1/cur->scale[0],1/cur->scale[1],1/cur->scale[2]);
				matrix_mult(S,clickedPoint);
				rayDirection[0] = result[0];
				rayDirection[1] = result[1];
				rayDirection[2] = result[2];
			hit = my_raytrace_cone(cur, rayStart, rayDirection, intersectionPoint);
			//scale intersect
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_S(cur->scale[0],cur->scale[1],cur->scale[2]);
				matrix_mult(S,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
				
				
				//rotate intersect
				
				//Rx
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rx(cur->rotate[0]);
				matrix_mult(Rx,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Ry
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Ry(cur->rotate[1]);
				matrix_mult(Ry,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				//Rz
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] =intersectionPoint[2];
				clickedPoint[3] =1;
				set_Rz(cur->rotate[2]);
				matrix_mult(Rz,clickedPoint);
				intersectionPoint[0] = result[0];
				intersectionPoint[1] = result[1];
				intersectionPoint[2] = result[2];
				
				
				//translate intersect
				
				clickedPoint[0] = intersectionPoint[0];
				clickedPoint[1] = intersectionPoint[1];
				clickedPoint[2] = intersectionPoint[2];
				clickedPoint[3] = 1;
			
				set_T(cur->translate[0],cur->translate[1],cur->translate[2]);
				matrix_mult(T,clickedPoint);
				intersectionPoint[0]= result[0];
				intersectionPoint[1]=result[1];
				intersectionPoint[2]=result[2];
			break;
		default:
			break;
		}

		// found intersection
		if (hit)
		{
			if (foundIntersection)
			{
				if(intersectionPoint[2]>closestPoint[2]){
					closestPoint[0] = intersectionPoint[0];
					closestPoint[1] = intersectionPoint[1];
					closestPoint[2] = intersectionPoint[2];
					printf("found closer collision\n");
				}
				else{
					printf("found another collision, but it was not closer\n");
				}
			}
			else
			{
				closestPoint[0] = intersectionPoint[0];
				closestPoint[1] = intersectionPoint[1];
				closestPoint[2] = intersectionPoint[2];
			}

			foundIntersection = 1;
		}
	}

	if (foundIntersection)
	{
		printf("Intersected with object %s at (%f, %f, %f)\n", "object_name", closestPoint[0], closestPoint[1], closestPoint[2]);
		my_assert ((num_objects < NUM_OBJECTS), "too many objects");
  		hit_point = &my_objects[num_objects++];
  		hit_point->sid = 3;
  		hit_point->ray =.1;
  		hit_point->translate[0] = closestPoint[0];
  		hit_point->translate[1] = closestPoint[1];
  		hit_point->translate[2] = closestPoint[2];
  		make_sphere(hit_point,hit_point->ray);
  		real_translation(hit_point,hit_point->translate[0],hit_point->translate[1],hit_point->translate[2]);
  		
  		
	}
	else
	{
		printf("No intersection\n");
	}
}

void my_mouse(int button, int state, int mousex, int mousey) {

  switch( button ) {
	
  case GLUT_LEFT_BUTTON:
    if( state == GLUT_DOWN ) {
		
		my_raytrace(mousex, mousey);
		glutPostRedisplay();
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


void normalize(GLfloat *p) { 
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
  normalize(d);
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
void make_cube_smart(OBJECT *po, double size){
  int i;  
  // compute verts on PI/4 angels for x y z, then -x y z  
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
}

void make_cylinder(OBJECT *po){
  double ray =1;
  double height =2;
	
  po->v_start[0]=ray;
  po->v_start[1] =-height/2;
  po->v_start[2] =0;
  po->v_start[3] =1;
  
  int i;
  int j;
  double theta;
  double phi;
  double h;
  double half = height/2;
  for(i=0,theta =0;i<crt_vs;i++,theta+=2*M_PI/crt_vs){
  	for(j=0,h=0;j<crt_rs;h+=height/crt_rs,j++){
  		set_Ry(theta);
  		set_T(0,h,0);
  		
  		matrix_mult(T,po->v_start);
  		matrix_mult(Ry,result);

  		po->vertices_cylinder[i][j][0] = result[0];
  		po->vertices_cylinder[i][j][1] = result[1];
  		po->vertices_cylinder[i][j][2] = result[2];
  		po->vertices_cylinder[i][j][3] = result[3];

  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		
  	}
  }


}
void make_sphere(OBJECT *po,double ray){
  
  
	
  po->v_start[0]=0;
  po->v_start[1] =-ray;
  po->v_start[2] =0;
  po->v_start[3] =1;
  
  int i;
  int j;
  double theta;
  double phi;
  
  for(i=0,theta =0;theta<M_PI;i++,theta+=M_PI/crt_vs){
  	for(j=0,phi=0;phi<(2*M_PI);j++,phi+=(2*M_PI)/crt_rs){
  		set_Rz(theta);
  		set_Ry(phi);
  		
  		matrix_mult(Rz,po->v_start);
  		matrix_mult(Ry,result);
  	
  		
  		po->vertices_sphere[i][j][0] = result[0];
  		po->vertices_sphere[i][j][1] = result[1];
  		po->vertices_sphere[i][j][2] = result[2];
  	    po->vertices_sphere[i][j][3] = result[3];
  		
  	
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		
  	}
  }


}

void make_cone(OBJECT *vert, double height, double ray)
{
	int i,j,k; // initial variables to use outside of loops
	
	for(i = 0; i <= crt_vs; i++) // iterates through the angles
	{
		
		for(j = 0; j <= crt_rs; j++) // iterates through the layers
		{
	
			int t = 360/crt_rs * j; // next angle
			
			GLfloat theta = (t * M_PI/180); // angle
			
			for(k = 0; k < 4; k++) // iterates through x,y,z coordinates
			{
				switch(k){
					case 0:
						vert->vertices_cone[i][j][k] = (height - (height/crt_vs * i))/height * ray * cos(theta);break;   // x coordinate
					case 1:
						vert->vertices_cone[i][j][k] = (height/crt_vs * i);break;  // y coordinate
					case 2:
						vert->vertices_cone[i][j][k] = (height - (height/crt_vs * i))/height * ray * sin(theta); break; // z coordinate
					case 3:
						vert->vertices_cone[i][j][k] = 1; // homogenious coordinate
				}
			}
		
			// map texture		
		
			
			
		
		}	
	}	

	// fix wrap around

	for(i = 0; i < crt_rs; i++){
		vert->vertices_cone[i][crt_vs+1][0] = vert->vertices_cone[i][0][0];
		vert->vertices_cone[i][crt_vs+1][1] = vert->vertices_cone[i][0][1];
		vert->vertices_cone[i][crt_vs+1][2] = vert->vertices_cone[i][0][2];
	}	
	
	for(i = 0; i < crt_vs; i++){
		vert->vertices_cone[crt_rs + 1][i][0] = vert->vertices_cone[0][i][0];
		vert->vertices_cone[crt_rs + 1][i][1] = vert->vertices_cone[0][i][1];
		vert->vertices_cone[crt_rs + 1][i][2] = vert->vertices_cone[0][i][2];
	}
	
	
	GLfloat v1[4][3];
	
	
	
	for(i = 0; i <= crt_vs; i++)
		for(j = 0; j <= crt_rs; j++)
		{
			
			cross(vert->vertices_cone[i][j], vert->vertices_cone[i][(j+1)%crt_rs], vert->vertices_cone[(i-1)%crt_vs][j], v1[0]);
			cross(vert->vertices_cone[i][j], vert->vertices_cone[(i-1)%crt_vs][j], vert->vertices_cone[i][(j-1)%crt_rs], v1[1]);
			cross(vert->vertices_cone[i][j], vert->vertices_cone[i][(j-1)%crt_rs], vert->vertices_cone[(i+1)%crt_vs][j], v1[2]);
			cross(vert->vertices_cone[i][j], vert->vertices_cone[(i+1)%crt_vs][j], vert->vertices_cone[i][(j+1)%crt_rs], v1[3]);
			
			vert->normals[i][j][0] = ((v1[0][0]+v1[1][0]+v1[2][0]+v1[3][0])/4);
			vert->normals[i][j][1] = ((v1[0][1]+v1[1][1]+v1[2][1]+v1[3][1])/4);
			vert->normals[i][j][2] = ((v1[0][2]+v1[1][2]+v1[2][2]+v1[3][2])/4);
			
		}

	
}
void make_torus(OBJECT *po){
  double r1 =.7;
  double r2 =.1;



  po->v_start[0]=r2;
  po->v_start[1] =0;
  po->v_start[2] =0;
  po->v_start[3] =1;
  
  int i;
  int j;
  double theta;
  double phi;
  
  for(i=0,theta =0;theta<2*M_PI;i++,theta+=2*M_PI/crt_vs){
  	for(j=0,phi=0;phi<(2*M_PI);j++,phi+=(2*M_PI)/crt_rs){
  		set_Rz(theta);
  		set_Ry(phi);
  		set_T(r1,0,0);
  		matrix_mult(Rz,po->v_start);
  		matrix_mult(T,result);
  		matrix_mult(Ry,result);
  	
  		po->vertices_torus[i][j][0] = result[0];
  		po->vertices_torus[i][j][1] = result[1];
  		po->vertices_torus[i][j][2] = result[2];
  		po->vertices_torus[i][j][3] = result[3];
  	
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		
  		
  	}
  }

}

void draw_cylinder(OBJECT *po){
	int i;
	int j;
	
	for(i=0;i<crt_vs;i++){
		for(j=0;j<crt_rs;j++){
			draw_param_quad(po,po->vertices_cylinder,i,j,BLUE);
		}
	}


}
void draw_sphere(OBJECT *po){
	int i;
	int j;
	
	for(i=0;i<crt_vs;i++){
		for(j=0;j<crt_rs;j++){
			if(po->ray==1){
				draw_param_quad(po,po->vertices_sphere,i,j,GREEN);
			}
			else{
				draw_param_quad(po,po->vertices_sphere,i,j,RED);
				
			}
		}
	}


}
void draw_torus(OBJECT *po){
	int i;
	int j;
	
	for(i=0;i<crt_vs;i++){
		for(j=0;j<crt_rs;j++){
			draw_param_quad(po,po->vertices_torus,i,j,BLUE);
		}
	}


}
void draw_cone(OBJECT *po){
	int i;
	int j;
	
	for(i=0;i<crt_vs;i++){
		for(j=0;j<crt_rs;j++){
			draw_param_quad(po,po->vertices_cone,i,j,BLUE);
		}
	}


}
// create camera local coordinate system
void create_camera(){
	GLfloat look[4];
	double dot;
	
	look[0] = my_cam.at[0]-my_cam.pos[0];
	look[1] = my_cam.at[1]-my_cam.pos[1];
	look[2] = my_cam.at[2]-my_cam.pos[2];
	look[3] = my_cam.at[3]-my_cam.pos[0];
	my_cam.w[0] = -look[0];
	my_cam.w[1] = -look[1];
	my_cam.w[2] = -look[2];
	my_cam.w[3] = -look[3];
	normalize(my_cam.w);
	
	dot = dotprod(my_cam.up,my_cam.w);
	
	my_cam.v[0]= my_cam.up[0]-dot*my_cam.w[0];
	my_cam.v[1]= my_cam.up[1]-dot*my_cam.w[1];
	my_cam.v[2]= my_cam.up[2]-dot*my_cam.w[2];
	my_cam.v[3]= my_cam.up[3]-dot*my_cam.w[3];
	
	normalize(my_cam.v);
	
	
	my_cam.u[0] = (my_cam.v[1]*my_cam.w[2]) - (my_cam.v[2]*my_cam.w[1]);
  	my_cam.u[1] = (my_cam.v[2]*my_cam.w[0]) - (my_cam.v[0]*my_cam.w[2]);
  	my_cam.u[2] = (my_cam.v[0]*my_cam.w[1]) - (my_cam.v[1]*my_cam.w[0]);
  	my_cam.u[3] = 0;
	

	


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
void draw_param_quad(OBJECT *po, GLfloat vertices[][50][4], int line, int col, int ic) {
	
	
	cross(vertices[line][col], vertices[line+1][col], vertices[line+1][col+1], po->normals[line][col]);
	glBegin(crt_render_mode); 
  	{
   		glColor3fv(colors[ic]);
   		glNormal3fv(po->normals[line][col]);
   		/*note the explicit use of homogeneous coords below: glVertex4f*/
    	glVertex4fv(vertices[line][col]);
    	glVertex4fv(vertices[(line+1)%crt_vs][col]);
    	glVertex4fv(vertices[(line+1)%crt_vs][(col+1)%crt_rs]);
    	glVertex4fv(vertices[line][(col+1)%crt_rs]);
    		
  }
  glEnd();
  
 
  

  
  	
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
  }
  glEnd();
}


/***********************************
 FUNCTION: draw_cube_smart() 
 ARGS: none
 RETURN: none
 DOES: draws a cube from quads
************************************/
void draw_cube_smart(OBJECT *po)
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

  glDisable(GL_LIGHTING);

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

  glEnable(GL_LIGHTING);
	
}

void draw_objects() {
  int i;
  for(i=0; i<num_objects; i++){
    OBJECT *cur;
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
    case 3: //sphere
    	draw_sphere(cur);
    	break;
    case 4: //
    	draw_cylinder(cur);
    	break;
    case 5: //
    	draw_cone(cur);
    	break;
    case 6: //tor
    	draw_torus(cur);
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

  //update the flashlight to follow the person
  
  
  //draw the objects
  draw_axes();

  draw_objects();
  glEnable(GL_BLEND);
  
  glLineWidth(4);
  glBegin(GL_LINES);
    	 glColor3f(1.0,1.0,1.0);
    	 glVertex4f(lineStart[0],lineStart[1],lineStart[2],1);
    	 glVertex4f(lineEnd[0],lineEnd[1],lineEnd[2],1);
  	glEnd();
  	//printf("RAY:\nstart x: %f\nstart y: %f\nstart z: %f\nEND\nend x: %f\nend y: %f\nend z: %f\n",lineStart[0],lineStart[1],lineStart[2],lineEnd[0],lineEnd[1],lineEnd[2]);
 glDisable(GL_BLEND);
  // this buffer is ready
  glutSwapBuffers();
}

void my_idle(void) {
  //EC idea: Make the flashlight flicker a bit (random flicker strength) when the user is idle.
  return ;
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
