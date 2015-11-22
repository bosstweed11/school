//Andrew Beers cylinder class



using namespace std;

class Cylinder : public Shape {

public


Cylinder::Cylinder(double height, double ray, int rs, int vs) : Shape(1)
{
  this->height = height;
  this->ray = ray;
  this->rs = rs;
  this->vs = vs;
  
  Create();
}


void Cylinder::Create(){


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

protected:
  int rs;
  int vs;
  double height;
  double ray;
  int type;