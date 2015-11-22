//Andrew Beers iCyl class

#include <stdlib.h>

#define _USE_MATH_DEFINES //cause vs is dumb :-(
#include <math.h>
#include <glut/glut.h>

#ifndef min //In VC++ 2008, Ryan got a warning about min redefinition, so let's not redefine it if something else already did; sigh
#define min(a,b) ((a) < (b)? a:b)
#endif

using namespace std;



class Shape {

	private:
	
 		
  		
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
  		double r1;
  		double r2;
  		int type;
  		const static double  pi = 3.14159265359;
	public:
	GLfloat vertices[50][50][4];
	GLfloat v_start[4];
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
	
	
//make cylinder	
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
  		printf("omg_cyl\n");
  	}
  }
}

//create sphere
void create_sph(){


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
  	
  		
  		vertices[i][j][0] = result[0];
  		vertices[i][j][1] = result[1];
  		vertices[i][j][2] = result[2];
  		vertices[i][j][3] = result[3];
  		
  	
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		printf("omg_sph\n");
  	}
  }
//tying the ends together, and the poles if need be.
for(i=0;i<vs+1;i++){
	for(j=0;j<rs;j++){
		if(i==vs){
			vertices[i][j][0]=0;
			vertices[i][j][1]=ray;
			vertices[i][j][2]=0;
			vertices[i][j][3]=1;
		}
	}
}

vertices[rs][vs][0] =0;
vertices[rs][vs][1] =ray;
vertices[rs][vs][2] =0;
vertices[rs][vs][3] =1;
}

//make cone
void create_con(){


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
  		
 
  		vertices[i][j][0] = result[0];
  		vertices[i][j][1] = result[1];
  		vertices[i][j][2] = result[2];
  		vertices[i][j][3] = result[3];
  		
  		 
  		
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		printf("omg_con\n");
  		
  	}
  	//radius change of the cone
  	new_ray*=(height/new_ray)*(height-h);
  }
}

//make torus
void create_tor(){

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
  	
  		vertices[i][j][0] = result[0];
  		vertices[i][j][1] = result[1];
  		vertices[i][j][2] = result[2];
  		vertices[i][j][3] = result[3];
  	
  		result[0]=0;
  		result[1]=0;
  		result[2]=0;
  		result[3]=0;
  		printf("omg_tor\n");
  		
  	}
  }
}

void show_verts(){

	int i;
	int j;
	
	for(i=0;i<vs;i++){
		for(j=0;j<rs;j++){
			printf("x: %f \ny: %f \nz: %f \nw: %f\n",vertices[i][j][0],vertices[i][j][1],vertices[i][j][2],vertices[i][j][3]);
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