//Andrew Beers sphere class

#include "Sphere.h"

using namespace std;


Sphere::Sphere(double ray, int rs, int vs) : Shape(2)
{
  
  this->ray = ray;
  this->rs = rs;
  this->vs = vs;
  
  
}

void Sphere::Create(){

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
}
