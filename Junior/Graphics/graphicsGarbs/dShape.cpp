

class dShape {


public:

	int type,dir;//0 down 1 up
	
	dShape(){}
	~dShape() {}
	//main constuctor
	void set_shape(double &x,double &y, double &h, double &w, double type, double side_v,double v,int dir, double &x_dir){
		this->initial=0;
		if(type==0){
			set_x(x);
			set_y(y);
			set_w(w);
			set_h(h);
			set_type(type);
			set_side_v(side_v);
			set_v(v);
			set_dir(dir);
			set_x_dir(x_dir);
		}
		else{
			set_x(x);
			set_y(y);
			set_r(h);
			set_type(type);
			set_side_v(side_v);
			set_v(v);
			set_dir(dir);
			set_x_dir(x_dir);
		}
		this->initial=1;
	
	}
	
	/* Setters and Getters*/
	double get_x() {
		return x;
	}
	double get_y() {
		return y;
	}
	double get_w() {
		return w;
	}
	double get_h() {
		return h;
	}
	double get_v() {
		return v;
	}
	double get_side_v() {
		return side_v;
	}
	int get_dir() {
		return dir;
	}
	int get_x_dir() {
		return x_dir;
	}
	void set_x(double new_x) {
		this-> x = new_x;
		
		return;
	}
	void set_y(double new_y) {
		this-> y = new_y;
		
		return;
	}
	void set_w(double new_w) {
		if(this->type==0)
			this-> w = new_w;
		
		return;
	}
	
	void set_h(double new_h) {
		if(this->type==0)
			this-> h = new_h;
			
			return;
	}
	void set_v(double new_v) {
		if(new_v<.4&&new_v>-.4&&this->initial==0){
			new_v =1;
		}
		this-> v = new_v;
		
		return;
	}
	void set_side_v(double new_v) {
		this-> side_v = new_v;
		
		return;
	}
	void set_dir(int new_dir) {
		this-> dir = new_dir;
		
		return;
	}
	void set_x_dir(double new_dir) {
		if(new_dir>0)
		{
			this->x_dir =1;
		}
		else{
			this->x_dir=0;
		}
		
		return;
	}
	void set_r(double new_r) {
		if(this->type==1)
			this-> r = new_r;
			
		return;
	}
	void set_type(int new_type) {
		this-> type = new_type;
		
		return;
	}
	/* Main drawing method */
	void draw(int window_w,int window_h,double radius){
		if(this->type ==0){
			glColor3f(0,0,1) ; /* (Red, Green, Blue); so here we ask for Blue */
			glRectf( this->x/window_w, this->y/window_h, this->x/window_w + this->w, this->y/window_h + this->h);
			
		}
		
		if(this->type ==1){
			glColor3f(1,0,0);
			glBegin(GL_TRIANGLE_FAN);     	// draw triangle
			glVertex2d(this->x/window_w,this->y/window_h);
				int i;
				for(i=0;i<360;i+=6){
					glVertex2d(this->x/window_w+radius*cos(i),  this->y/window_h+radius*sin(i));
				}
			glEnd();	
		}	
	}

/*private variables*/
private:
	double x,y,w,h,r,v,side_v;
	int x_dir,initial;
};		
		