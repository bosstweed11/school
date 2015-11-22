//Andrew Beers Cyl class



using namespace std;
class Cyl {
	public:
	
	
	Cyl::Cyl(double height, double ray, int rs, int vs)
	{
	  
	  
	}
	
	//set up the x rotation matrix
	void Cyl::set_Rx(GLfloat theta){
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
	void Cyl::set_Ry(GLfloat theta){
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
	void Cyl::set_Rz(GLfloat theta){
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
	void Cyl::set_T(GLfloat tx,GLfloat ty, GLfloat tz){
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
	void Cyl::set_S(GLfloat sx,GLfloat sy, GLfloat sz){
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
	void Cyl::matrix_mult(GLfloat M[][4],GLfloat V[]){
	
	
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
	int Cyl::get_type(){
	
		return Cyl_type;
	}
	
	private:
	
 		GLfloat vertices[50][50][4];
  		GLfloat v_start[4];
  		GLfloat result[4];
  		GLfloat result2[4];
  		double Rz[4][4];
  		double Ry[4][4];
  		double Rx[4][4];
  		double S[4][4];
  		double T[4][4];
  		int rs;
  		int vs;
  		double height;
  		double ray;
}