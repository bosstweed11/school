#include "ray_trace.h"

#define MAX_LIGHTS  8
#define NUM_OBJECTS 400
#define my_assert(X,Y) ((X)?(void) 0:(printf("error:%s in %s at %d", Y, __FILE__, __LINE__), myabort()))
void initGL(void);
void displayScene(void);
void idle(void);
void resizeWindow(int width, int height);
void my_raytrace(int mousex, int mousey);

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
float held_val[4];
float illu_r;
float illu_g;
float illu_b;
float lineStart[3];
float lineEnd[3];
float k_a=.3;
float k_d=.5;

double illum_light_b;
double illum_light_g;
double illum_light_r;


void create_camera();
void normalize(GLfloat*);
void read_spec(char *fname);

//object
typedef struct _Object {
  int sid;
  double ray;
  float center[4];
  
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


GLuint texId;

color_t screen[WINDOW_WIDTH * WINDOW_HEIGHT];

static int raster_x=0;
static int raster_y=0;

void plotPixel(color_t *screen, int x, int y, float r, float g, float b, float a)
{
	screen[y*WINDOW_WIDTH + x].r = r;
	screen[y*WINDOW_WIDTH + x].g = g;
	screen[y*WINDOW_WIDTH + x].b = b;
	screen[y*WINDOW_WIDTH + x].a = a;
}

void myabort(void) {
  abort();
  exit(1); /* exit so g++ knows we don't return. */
} 

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	// depth buffer not really needed, but whatever
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("Ray Tracer");
	
	
	my_assert(argc >1, "need to supply a spec file");
	read_spec(argv[1]);
	
	initGL();

	glutDisplayFunc(displayScene);
	glutReshapeFunc(resizeWindow);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

float dotprod(float v1[], float v2[]) {
  float tot = 0;
  int i;
  for (i=0; i<3; i++)
    tot += v1[i]*v2[i];
  return tot;
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
	
  po->center[0] = 0;
  po->center[1] = 0;
  po->center[2] = 0;
  po->center[3] = 1;
	
  
  //translation
	held_val[0] = po->center[0];
	held_val[1] = po->center[1];
	held_val[2] =po->center[2];
	held_val[3] =po->center[3];
	set_T(po->translate[0],po->translate[1],po->translate[2]);
	matrix_mult(T,held_val);
	po->center[0] = result[0];
	po->center[1] = result[1];
	po->center[2] = result[2];
	po->center[3] = result[3];
	//rotation point
	
	
	
	
	

		
	
  
  
  
  printf("objects center:\n x: %f\ny: %f\nz: %f\n",po->center[0],po->center[1],po->center[2]);
  // scale, rotate, translate using your real tranformations from assignment 3 depending on input from spec file
 
  
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


void initGL()
{
	int i, j;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);

	resizeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	// erase texture
	for (i=0;i<sizeof(screen)/sizeof(color_t);i++)
	{
		screen[i].r = 0.0f;
		screen[i].g = 0.0f;
		screen[i].b = 0.0f;
		screen[i].a = 1.0f;
	}

	// put a red square on the top left
	for (i=0; i < 32; i++)
	{
		for (j=0; j<32;j++)
		{
			plotPixel(screen, i, j, 1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	// put a green square on the top right
	for (i=WINDOW_WIDTH-32-1; i<WINDOW_WIDTH; i++)
	{
		for (j=0; j<32;j++)
		{
			plotPixel(screen, i, j, 0.0f, 1.0f, 0.0f, 1.0f);
		}
	}

	// put a blue square on the bottom left
	for (i=0; i<32; i++)
	{
		for (j=WINDOW_HEIGHT-32-1; j<WINDOW_HEIGHT;j++)
		{
			plotPixel(screen, i, j, 0.0f, 0.0f, 1.0f, 1.0f);
		}
	}

	// put a white square on the bottom left
	for (i=WINDOW_WIDTH-32-1; i<WINDOW_WIDTH; i++)
	{
		for (j=WINDOW_HEIGHT-32-1; j<WINDOW_HEIGHT;j++)
		{
			plotPixel(screen, i, j, 1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	
	

	// create texture for drawing
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, (void*)screen);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void displayScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);

	// load orthographic projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	//gluLookAt(my_cam.pos[0],my_cam.pos[1], my_cam.pos[2],my_cam.at[0],my_cam.at[1],my_cam.at[2],my_cam.up[0], my_cam.up[1], my_cam.up[2]);


	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	
	
	// restore projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutSwapBuffers();
}

void idle()
{
	float r, g, b, a;
	float x, y;
	// This is where your ray-tracing code will go

	// Please note this is set up to change one pixel at a time. When
	// you do this assignment, updating the framebuffer every pixel
	// will be a bottleneck. It is best to do several at once, then
	// update the screen.
	
	if (raster_x < WINDOW_WIDTH && raster_y < WINDOW_HEIGHT)
	{
		// first, do a ray trace at raster position (x,y). I made the coordinate system the same as in GLUT.
		//x = (float)raster_x - (float)WINDOW_WIDTH/2.0f;
		//y = (float)raster_y - (float)WINDOW_HEIGHT/2.0f;
		my_raytrace(raster_x,raster_y);
		
		b = illu_b;
		g=illu_g;
		r=illu_r;
		// now we must change the color in our buffer
		plotPixel(screen, raster_x, raster_y, r, g, b, a);

		// now lets upload the changes to the video card (slow)
		// Unfortunately, the final parameter of glTexSubImage2D can only be a 1D array
		// containing the exact pixels you want to transfer (it isn't smart enough to 
		// select a square, for instance)
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, raster_x, raster_y, 1, 1, GL_RGBA, GL_FLOAT, (void*)(screen + (raster_y*WINDOW_WIDTH+raster_x)));
		illu_b=0;
		illu_g=0;
		illu_r=0;
	}

	if (raster_x < WINDOW_WIDTH)
		raster_x++;

	if (raster_x == WINDOW_WIDTH) // finished line, increment Y raster
	{
		raster_x = 0;
		raster_y++;
		glutPostRedisplay();
	}

	// redraw
	// Note that this function will not be called again until the display
	// update completes. This limits the number of times this function
	// is called to you monitor refresh rate. To get around this, you
	// could calculate several lines and then update, or you can do the
	// entire thing and update.
	
}

void resizeWindow(int width, int height)
{
	if (height == 0) // prevent div/0
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width/(double)height, 0.1, 4096.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void normalize(GLfloat *p) { 
  double d=0.0;
  int i;
  for(i=0; i<3; i++) d+=p[i]*p[i];
  d=sqrt(d);
  if(d > 0.0) for(i=0; i<3; i++) p[i]/=d;
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
	OBJECT *temp;
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
		
		case 3:
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
				temp=cur;
			}
			else
			{
				closestPoint[0] = intersectionPoint[0];
				closestPoint[1] = intersectionPoint[1];
				closestPoint[2] = intersectionPoint[2];
				temp= cur;
				
			}

			foundIntersection = 1;
		}
	}

	if (foundIntersection)
	{
		LITE *pl;
		printf("Intersected with object %s at (%f, %f, %f)\n", "object_name", closestPoint[0], closestPoint[1], closestPoint[2]);

  		illum_light_b=0;
  		illum_light_g=0;
  		illum_light_r=0;
  		float L_vec[4];
  		float temp_center[4];
  		float N_vec[4];
  		L_vec[0]=0;
  		L_vec[1]=0;
  		L_vec[2]=0;
  		L_vec[3]=0;
  		
  		N_vec[0]=0;
  		N_vec[1]=0;
  		N_vec[2]=0;
  		N_vec[3]=0;
  		
  		
  		
  		illu_r=0;
  		illu_g=0;
  		illu_b=0;
  		
  		//go through lights
  		for(i=1;i<num_lights+1;i++){
  		
  			pl = my_lights+i;
  			
  			
  			
  			//calculate normal
  			N_vec[0] = closestPoint[0]-temp->translate[0];
  			N_vec[1] = closestPoint[1]-temp->translate[1];
  			N_vec[2] = closestPoint[2]-temp->translate[2];
  			N_vec[3] = 0;
  			
  			
  			//normalize normal
  			normalize(N_vec);
  			
  			
  			//calc light dir
  			L_vec[0]=pl->pos[0] - closestPoint[0];
  			L_vec[1]=pl->pos[1] - closestPoint[1];
  			L_vec[2]=pl->pos[2] - closestPoint[2];
  			L_vec[3]=0;
  			
  				
  			
  			
  			
  			//normalize light
  			normalize(L_vec);
  			
  			//calculate light illumination
  			illum_light_r+=pl->diff[0]*(k_d*temp->diff[0]*dotprod(N_vec,L_vec));
  			illum_light_g+=pl->diff[1]*(k_d*temp->diff[1]*dotprod(N_vec,L_vec));
  			illum_light_b+=pl->diff[2]*(k_d*temp->diff[2]*dotprod(N_vec,L_vec));
  		

  		
  		}
  		
  		//calculate entire rgb illumination
  		illu_r=k_a*temp->amb[0]+illum_light_r;
  		illu_g=k_a*temp->amb[1]+illum_light_g;
  		illu_b =k_a*temp->amb[2]+illum_light_b;
  		
  		
  		
  		
  		
	}
}



