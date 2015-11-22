//Andrew Beers cone class

#include "Torus.h"

using namespace std;


Torus::Torus(double r1, double r2, int rs, int vs)
{
  this->r1 = r1;
  this->r2 = r2;
  this->rs = rs;
  this->vs = vs;
  
  
}

void Torus::Create(){


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
