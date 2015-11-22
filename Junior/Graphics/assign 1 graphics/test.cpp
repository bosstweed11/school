#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Shape {

public:
	Shape(double x,double y, double w,double h){
		this->x =x;
		this->y = y;
		this->w = w;
		this ->h = h;
	
	}
	Shape(double x,double y, double r){
		this->x =x;
		this->y = y;
		this->r = r;
	}
	Shape(){}
	~Shape() {}
	
	double get_x() {
		return x;
	}

	double get_y() {
		return y;
	}
	
	void set_x(double new_x) {
		this-> x = new_x;
	}
	
	void set_y(double new_y) {
		this-> y = new_y;
	}
	
	void set_type(int the_type){
		this-> type = the_type;
	}
	void draw(){
		if(this->type ==0){
			cout << "This is a square" <<endl;
		}
		else if(this->type ==1){
			cout << "this is a circle" << endl;
		}
	}
	
private:
	double x,y,w,h,r;
	int type;
};




int main() {
	
	double new_x;
	double new_y;
	int i=0,num_shapes=3;
	Shape shapes[num_shapes];
	int boss[5];
	
	cout << "How many shapes: ";
	cin >> num_shapes;
	
	for(i=0;i<num_shapes;++i){
		cout << "Shape";
		cout << (" %d",i+1);
		cout << "(x,y):"<<endl;
	 	cin >> new_x;
	 	cin >> new_y;
	 	shapes[i].set_x(new_x);
	 	shapes[i].set_y(new_y);
	}
	cout << "Results";
	
	for(i=0;i<num_shapes;++i){
		cout << "Shape";
		cout << (" %d",i+1);
		cout << "(x,y):"<<endl;
	 
	 	cout << shapes[i].get_x() << endl;
	 	cout << shapes[i].get_y() << endl;
	 	
	}
	shapes[0].set_type(0);
	shapes[1].set_type(1);
	
	shapes[0].draw();
	shapes[1].draw();
	
	
	
	
	return 0;

}