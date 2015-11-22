//Andrew Beers cone class

#include "Cone.h"

using namespace std;


Cone::Cone(double height, double ray, int rs, int vs) :Shape(1)
{
  this->height = height;
  this->ray = ray;
  this->rs = rs;
  this->vs = vs;
  
  
}

void Cone::Create(){



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
  		
  		
  	}
  	//radius change of the cone
  	new_ray*=(height/new_ray)*(height-h);
  }
}

}
