/*******************************************************
FILE: stitcher.c
AUTHOR: gem, loosely based on random hwa skel
DATE: 01/24/08
DOES: skeleton for hmkw2 redux
PLATFORM: tested on osx, linux, and vs
********************************************************/
//Andrew Beers 9/25 CS 1566 Assignment 2
#include "stitcher.h"
#include <glut/glut.h>
#include <stdio.h>

//global variables
int theta_x;
int theta_y;
int crt_render_mode;
int crt_shape, crt_rs, crt_vs;
int index_ =0;
int draw_norms=0;
double cyl_height=3;
double con_height =1;
double con_ray =1;
double cyl_ray=1;
double sph_ray=1;
double v_start[4];
double Rz[4][4];
double Ry[4][4];
double Rx[4][4];
double S[4][4];
double T[4][4];
double r1=1.2;
double r2=.8;
double pi = atan(1.0)*4;
double normalize;
double result[4];
double result2[4];

 
/*******************************************************
FUNCTION: main
ARGS: argc, argv
RETURN: 0
DOES: main function (duh!); starts GL, GLU, GLUT, then loops 
********************************************************/
int main(int argc, char **argv) {

  /* General initialization for GLUT and OpenGL
     Must be called first */
  glutInit( &argc, argv ) ;
  
  /* we define these setup procedures */
  glut_setup() ;  
  gl_setup() ;
  my_setup();

  /* go into the main event loop */
  glutMainLoop() ;

  return(0) ;
}


/*******************************************************
FUNCTION: glut_setup
ARGS: none
RETURN: none
DOES: sets up GLUT; done here because we like our 'main's tidy
********************************************************/
/* This function sets up the windowing related glut calls */
void glut_setup(void) {

  /* specify display mode -- here we ask for a double buffer and RGB coloring */
  /* NEW: tell display we care about depth now */
  glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB |GLUT_DEPTH);

  /* make a 400x400 window with the title of "Stitcher" placed at the top left corner */
  glutInitWindowSize(400,400);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Stitcher");

  /*initialize callback functions */
  glutDisplayFunc( my_display );
  glutReshapeFunc( my_reshape ); 
  glutMouseFunc( my_mouse);
  glutKeyboardFunc(my_keyboard);


  //  glutIdleFunc( my_idle );
	
  glutTimerFunc( 20000, my_TimeOut, 0);/* schedule a my_TimeOut call with the ID 0 in 20 seconds from now */
  return ;
}


/*******************************************************
FUNCTION: gl_setup
ARGS: none
RETURN: none
DOES: sets up OpenGL (name starts with gl but not followed by capital letter, so it's ours)
********************************************************/
void gl_setup(void) {

  /* specifies a background color: black in this case */
  glClearColor(0,0,0,0) ;

  /* NEW: now we have to enable depth handling (z-buffer) */
  glEnable(GL_DEPTH_TEST);

  /* NEW: setup for 3d projection */
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  // perspective view
  gluPerspective( 20.0, 1.0, 1.0, 100.0);
  return;
}

/*******************************************************
FUNCTION: my_setup
ARGS: 
RETURN:
DOES: pre-computes staff and presets some values
********************************************************/
/*TODO EC: add make_torus etc.   */
void my_setup(void) {
  theta_x = 0;
  theta_y = 0;
  crt_render_mode = GL_LINE_LOOP;
  crt_shape = HOUSE;
  crt_rs = 10;
  crt_vs = 10;

  make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
  make_sphere(sph_ray,crt_rs,crt_vs);
  make_cone(con_height,con_ray,crt_rs,crt_vs);
  make_torus(r1,r2,crt_rs,crt_vs);
 
  return;
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
void matrix_mult(double M[][4],double V[]){


	double ans=0;
	int i;
	int j;
	
	double stored[4];
	stored[0] = V[0];
	stored[1] =V[1];
	stored[2] =V[2];
	stored[3]=V[3];
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

/*******************************************************
FUNCTION: my_reshape
ARGS: new window width and height 
RETURN:
DOES: remaps viewport to the Window Manager's window
********************************************************/
void my_reshape(int w, int h) {

  /* define viewport -- x, y, (origin is at lower left corner) width, height */
  glViewport (0, 0, min(w,h), min(w,h));
  return;
}


/*******************************************************
FUNCTION: my_keyboard
ARGS: key id, x, y
RETURN:
DOES: handles keyboard events
********************************************************/
// added ability to draw a sphere, torus, cylinder, cone, show normals, and change tessellation features
void my_keyboard( unsigned char key, int x, int y ) {
  
  switch( key ) {
  case 'y':
  case 'Y': {
    theta_y = (theta_y+2) %360;
    glutPostRedisplay(); 
  }; break;
  case 'x':
  case 'X': {
    theta_x = (theta_x+2) %360;
    glutPostRedisplay(); 
  }; break;
  case '1': {
    crt_shape = CUBE;
    glutPostRedisplay();
  }; break;
  case '0': {
    crt_shape = HOUSE;
    glutPostRedisplay();
  }; break;
  case '2': {
    crt_shape = SPHERE;
    glutPostRedisplay();
  }; break;
  case 'V':
  case 'v': {
    if(draw_norms==0){
    	draw_norms=1;
    }
    else{
    	draw_norms=0;
    }	
  }; break;
  case '-':{
  	if(crt_vs>=1){
    	crt_vs--;
    }
    make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
  	make_sphere(sph_ray,crt_rs,crt_vs);
  	make_cone(con_height,con_ray,crt_rs,crt_vs);
  	make_torus(r1,r2,crt_rs,crt_vs);

    glutPostRedisplay();
  }; break;
  case '+': {
  if(crt_vs<50){
    	crt_vs++;
    }
    
    make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
  	make_sphere(sph_ray,crt_rs,crt_vs);
  	make_cone(con_height,con_ray,crt_rs,crt_vs);
  	make_torus(r1,r2,crt_rs,crt_vs);
  
    glutPostRedisplay();
  }; break;
  case '<':{
  	if(crt_rs<50){
    	crt_rs++;
    }
    
    make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
  	make_sphere(sph_ray,crt_rs,crt_vs);
  	make_cone(con_height,con_ray,crt_rs,crt_vs);
  	make_torus(r1,r2,crt_rs,crt_vs);
  
    glutPostRedisplay();
  }; break;
  case '>': {
  	if(crt_rs>=1){
    	crt_rs--;
    }
    make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
  	make_sphere(sph_ray,crt_rs,crt_vs);
  	make_cone(con_height,con_ray,crt_rs,crt_vs);
  	make_torus(r1,r2,crt_rs,crt_vs);
  	
    glutPostRedisplay();
  }; break;
  case '3': {
    crt_shape = CYLINDER;
    glutPostRedisplay();
  }; break;
  case '4': {
    crt_shape = CONE;
    glutPostRedisplay();
  }; break;
  case '5': {
    crt_shape = TORUS;
    glutPostRedisplay();
  }; break;
 case 'q': 
  case 'Q':
    exit(0) ;
  default: break;
  }
  
  return ;
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
      crt_render_mode = GL_LINE_LOOP;
      /* if you're not sure what glutPostRedisplay does for us,
	 try commenting out the line below; observe the effect.*/
      glutPostRedisplay();
    }
    if( state == GLUT_UP ) {
    }
    break ;

  case GLUT_RIGHT_BUTTON:
    if ( state == GLUT_DOWN ) {
      crt_render_mode = GL_POLYGON;
      glutPostRedisplay();
    }
    if( state == GLUT_UP ) {
    }
    break ;
  }
  
  return ;
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

	double v1[4];
	double v2[4];
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
  FUNCTION: make_cylinder 
  ARGS: height (along the Y axis), ray, and tesselation parameter (how many side faces) 
  RETURN: none
  DOES: helper build function; 
        defines a cylinder centered at the origin, 
        of height 'height' along the Y axis,
        and ray 'ray' in the XOZ plane
************************************/
// creates an array of cylindrical vertices with matrix transformations
void make_cylinder(double height, double ray, int rs, int vs)
{
  /*hint: top-face vertices lie on a circle (same for bottom); 
    if need be, generate top-face vertices by cutting the circle in pie slices*/
	v_start[0]=ray;
  v_start[1] =-height/2;
  v_start[2] =0;
  v_start[3] =1;
  
  int i;
  int j;
  double theta;
  double phi;
  double h;
  double half = height/2;
  for(i=0,theta =0;i<vs;i++,theta+=2*pi/vs){
  	for(j=0,h=0;j<rs;h+=height/rs,j++){
  		set_Ry(theta);
  		set_T(0,h,0);
  		
  		matrix_mult(T,v_start);
  		matrix_mult(Ry,result);

  		vertices_cyl[i][j][0] = result[0];
  		vertices_cyl[i][j][1] = result[1];
  		vertices_cyl[i][j][2] = result[2];
  		vertices_cyl[i][j][3] = result[3];

  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		
  	}
  }

}




/***********************************
  FUNCTION: make_sphere 
  ARGS: ray, and tessellation parameters (how many meridians, respectively parallels) 
  RETURN: none
  DOES: helper build function; 
        defines a sphere centered at the origin and of ray 'ray'
************************************/
// creates an array of spherical vertices with matrix transformations
void make_sphere(double ray, int rs, int vs)
{
  /*hint: work out the math first*/
  v_start[0]=0;
  v_start[1] =-ray;
  v_start[2] =0;
  v_start[3] =1;
  
  int i;
  int j;
  double theta;
  double phi;
  
  for(i=0,theta =0;theta<pi;i++,theta+=pi/vs){
  	for(j=0,phi=0;phi<(2*pi);j++,phi+=(2*pi)/rs){
  		set_Rz(theta);
  		set_Ry(phi);
  		
  		matrix_mult(Rz,v_start);
  		matrix_mult(Ry,result);
  	
  		
  		vertices_sph[i][j][0] = result[0];
  		vertices_sph[i][j][1] = result[1];
  		vertices_sph[i][j][2] = result[2];
  		vertices_sph[i][j][3] = result[3];
  		
  	
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		
  	}
  }
//tying the ends together, and the poles if need be.
for(i=0;i<vs+1;i++){
	for(j=0;j<rs;j++){
		if(i==vs){
			vertices_sph[i][j][0]=0;
			vertices_sph[i][j][1]=ray;
			vertices_sph[i][j][2]=0;
			vertices_sph[i][j][3]=1;
		}
	}
}

vertices_sph[rs][vs][0] =0;
vertices_sph[rs][vs][1] =ray;
vertices_sph[rs][vs][2] =0;
vertices_sph[rs][vs][3] =1;


}

/***********************************
  FUNCTION: make_cone 
  ARGS: height (along the Y axis), ray, and tesselation parameter (how many side faces) 
  RETURN: none
  DOES: helper build function; 
        defines a cone centered at the origin, 
        of height 'height' along the Y axis,
        and ray 'ray' in the XOZ plane
************************************/
//creates an array of conical vertices from matrix transformations
void make_cone(double height, double ray, int rs, int vs)
{

  v_start[0]=0;
  v_start[1] =-height/2;
  v_start[2] =0;
  v_start[3] =1;
  
  int i;
  int j;
  double theta;
  double phi;
  double h;
  double new_ray=ray;
  for(i=0,h=0;i<vs;i++,h+=height/vs){
  	for(j=0,theta=0;j<rs;theta+=2*pi/rs,j++){
  		set_Ry(theta);
  		set_T(new_ray,h,0);
  		
  		
  		
  		matrix_mult(T,v_start);
  		matrix_mult(Ry,result);
  		
 
  		vertices_con[i][j][0] = result[0];
  		vertices_con[i][j][1] = result[1];
  		vertices_con[i][j][2] = result[2];
  		vertices_con[i][j][3] = result[3];
  		
  		 
  		
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		
  		
  	}
  	//radius change of the cone
  	new_ray*=(height/new_ray)*(height-h);
  }
}
/***********************************
  FUNCTION: make_torus
  ARGS: ray, and tessellation parameters (how many meridians, respectively parallels) 
  RETURN: none
  DOES: helper build function; 
        
************************************/

//creates an array of vertices for a torus, using matrix transformations
void make_torus(double r1,double r2, int rs, int vs)
{
  
  v_start[0]=r2;
  v_start[1] =0;
  v_start[2] =0;
  v_start[3] =1;
  
  int i;
  int j;
  double theta;
  double phi;
  
  for(i=0,theta =0;theta<2*pi;i++,theta+=2*pi/vs){
  	for(j=0,phi=0;phi<(2*pi);j++,phi+=(2*pi)/rs){
  		set_Rz(theta);
  		set_Ry(phi);
  		set_T(r1,0,0);
  		matrix_mult(Rz,v_start);
  		matrix_mult(T,result);
  		matrix_mult(Ry,result);
  	
  		vertices_tor[i][j][0] = result[0];
  		vertices_tor[i][j][1] = result[1];
  		vertices_tor[i][j][2] = result[2];
  		vertices_tor[i][j][3] = result[3];
  	
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		
  	}
  }

}



/***********************************
  FUNCTION: draw_cylinder() 
  ARGS: none
  RETURN: none
  DOES: draws a cylinder from triangles
************************************/
/*TODO: stitch cylinder vertices together to make faces
don't call gl directly, use my_draw_triangle and my_draw_quad instead*/

//drawing function for cylinder
void draw_cylinder(int rs, int vs)
{
	int i;
	int j;
	index_=0;
	for(i=0;i<vs;i++){
		for(j=0;j<rs;j++){
			draw_param_quad(vertices_cyl,i,j,GREEN);
		}
	}
}

/***********************************
  FUNCTION: draw_sphere() 
  ARGS: none
  RETURN: none
  DOES: draws a sphere from triangles
************************************/
/*TODO: stitch sphere vertices together to make faces
don't call gl directly, use make_triangle and make_quad instead*/
//drawing function for sphere
void draw_sphere(int rs, int vs)
{
	int i;
	int j;
	index_=0;
	//vs+1 for the pole of the sphere
	for(i=0;i<vs+1;i++){
		for(j=0;j<rs;j++){
			draw_param_quad(vertices_sph,i,j,BLUE);
		}
	}
}

/***********************************
  FUNCTION: draw_cone() 
  ARGS: none
  RETURN: none
  DOES: draws a cone from triangles
************************************/
/*TODO: stitch cone vertices together to make faces
don't call gl directly, use make_triangle and make_quad instead*/
//drawing function for cone
void draw_cone(int rs, int vs)
{
	int i;
	int j;
	index_=0;
	for(i=0;i<vs;i++){
		for(j=0;j<rs;j++){
			draw_param_quad(vertices_con,i,j,RED);
		}
	}
}
/***********************************
  FUNCTION: draw_torus() 
  ARGS: none
  RETURN: none
  DOES: draws a torus from triangles
************************************/
/*TODO: stitch torus vertices together to make faces
don't call gl directly, use make_triangle and make_quad instead*/
//drawing function for cone
void draw_torus(int rs, int vs)
{
	int i;
	int j;
	index_=0;
	for(i=0;i<vs;i++){
		for(j=0;j<rs;j++){
			draw_param_quad(vertices_tor,i,j,GREEN);
		}
	}
}
/***********************************
  FUNCTION: draw_object 
  ARGS: shape to create (HOUSE, CUBE, CYLINDER, etc)
  RETURN: none
  DOES: draws an object from triangles
************************************/
/*TODO: expand according to assignment 2 specs*/
void draw_object(int shape) {

  switch(shape){
  case HOUSE: draw_house(); break;
  case CUBE: draw_cube_brute(); break; 
  case CYLINDER: draw_cylinder(crt_rs, crt_vs);  break;
  case SPHERE: draw_sphere(crt_rs, crt_vs);  break;
  case CONE: draw_cone(crt_rs,crt_vs);break;
  case TORUS: draw_torus(crt_rs,crt_vs) ; break;
  case MESH: /*TODO EC: call your function here*/ ; break;

  default: break;
  }

}



/***********************************
  FUNCTION: my_display
  ARGS: none
  RETURN: none
  DOES: main drawing function
************************************/
void my_display(void) {
  /* clear the buffer */
  /* NEW: now we have to clear depth as well */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) ;

  glMatrixMode(GL_MODELVIEW) ;
  
  
  glLoadIdentity();
  gluLookAt(0.0, 5.0, 25.0,  // x,y,z coord of the camera 
	    0.0, 0.0, 0.0,  // x,y,z coord of the origin
	    0.0, 1.0, 0.0); // the direction of up (default is y-axis)
      
	
	

  glRotatef(theta_y,0,1,0);
  glRotatef(theta_x,1,0,0);
  
    
  draw_object(crt_shape);
  
 
  
  /* buffer is ready */
  glutSwapBuffers();
  
     
	
  return ;
}
/***********************************
  FUNCTION: my_idle
  ARGS: none
  RETURN: none
  DOES: handles the "idle" event
************************************/
void my_idle(void) {
  theta_y = (theta_y+2) %360;
  glutPostRedisplay();
  return ;
}
/***********************************
  FUNCTION: my_TimeOut
  ARGS: timer id
  RETURN: none
  DOES: handles "timer" events
************************************/
void my_TimeOut(int id) {
  /* right now, does nothing*/
  /* schedule next timer event, 20 secs from now */ 
  glutTimerFunc(20000, my_TimeOut, 0);

  return ;
}



