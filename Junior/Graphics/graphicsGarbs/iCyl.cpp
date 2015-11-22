//Andrew Beers iCyl class

#include <stdlib.h>

#define _USE_MATH_DEFINES //cause vs is dumb :-(
#include <math.h>
#include <glut/glut.h>

#ifndef min //In VC++ 2008, Ryan got a warning about min redefinition, so let's not redefine it if something else already did; sigh
#define min(a,b) ((a) < (b)? a:b)
#endif

using namespace std;


double  pi = 3.14159265359;
class Shape {

	private:
	
 		GLfloat vertices[50][50][4];
  		GLfloat v_start[4];
  		GLfloat result[4];
  		GLfloat result2[4];
  		GLfloat Rz[4][4];
  		GLfloat Ry[4][4];
  		GLfloat Rx[4][4];
  		GLfloat S[4][4];
  		GLfloat T[4][4];
  		int rs;
  		int vs;
  		double height;
  		double ray;
  		int type;
	public:
	~Shape();
	
	
	
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
	// matrix multiplication function multiplies a 4x4 by a 4x1
	void matrix_mult(GLfloat M[][4],GLfloat V[]){
	
	
		GLfloat ans=0;
		int i;
		int j;
		
		GLfloat stored[4];
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
	
	void set_rs(int rs){
		this->rs=rs;
	}
	
	int get_rs(){
		return rs;
	}
	
	
void create_cyl(){


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

  		vertices[i][j][0] = result[0];
  		vertices[i][j][1] = result[1];
  		vertices[i][j][2] = result[2];
  		vertices[i][j][3] = result[3];

  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		printf("omg\n");
  	}
  }
}

Shape(int type,double param_one, double param_two, int rs, int vs)
	{
		if(type==1){
			printf("type 1: cylinder\n");
			this->height = param_one;
			this->ray = param_two;
			this->rs=rs;
			this->vs=vs;
			create_cyl();
		}
		else if(type ==2){
			printf("type 2: sphere\n");
			this->ray = param_one;
			this->rs=rs;
			this->vs=vs;
			create_sph();
		}
		else if(type ==3){
			printf("type 3: cone\n");
			this->height = param_one;
			this->ray = param_two;
			this->rs=rs;
			this->vs=vs;
			create_con();
		
		}
		else if(type ==4){
			printf("type 4: torus\n");
			this->r1 = param_one;
			this->r2 = param_two;
			this->rs=rs;
			this->vs=vs;
			create_tor();
			
			
		}
		
	    
	}
	
	
};