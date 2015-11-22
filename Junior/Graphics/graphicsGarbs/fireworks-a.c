/*********************************
 * fireworks.c
 * The main program for the firework display.
 * Copied in the ability to make and draw shapes.
 * For now, it displays 10 cylinders and you can press 'q' to quit.
 * As well as press '1' to rotate the 5th cylinder, so that you
 * can easily see they're 3d and not 2d. 
 */

#include "fireworks-a.h"
#include "bass.h"

#define NUM_NOZZLES 7
#define NUM_SOUNDS 9
#define MAX_FIREWORKS 25
#define NUM_SHAPES 2


SOUND test_sound;

// arrays
NOZZLE nozzles[NUM_NOZZLES];
EXPLOSION explosions[MAX_FIREWORKS];
DWORD sounds[NUM_SOUNDS];

int current_shape=0;

// curr variables
int curr_vs = 10;
int curr_rs = 10;

GLfloat cur_radius = 17;
GLfloat cur_density = 3;
int cur_quantity = 460;
GLfloat cur_cube_size = .007;
int cur_after_counter = 100;
 
GLfloat reg_radius = 17;
GLfloat reg_density = 3;
int reg_quantity = 460;
GLfloat reg_cube_size = .007;
int reg_after_counter = 100;

GLfloat bur_radius = 3.5;
GLfloat bur_density = 3;
int bur_quantity = 800;
GLfloat bur_cube_size = .007;
int bur_after_counter = 100;

// for print_cur
int cur_variable = RADIUS;

// for time_out
int timer_speed = 20;             // time in ms till next drop

// counters
int num_fireworks = 0;


// booleans
int light_screen = 0;
short shift_camera = 0;   // change this value to set the camera at an angle
int music_playing = 0;



/*======================================================*/
/* =========== W O R K   O N    T H E S E =============*/
/*======================================================*/


void my_keyboard( unsigned char key, int x, int y ) 
{

  GLfloat used_radius;
  GLfloat used_density;
  int used_quantity;
  GLfloat used_cube_size;
  int used_after_counter;
  int num = rand() % (NUM_SOUNDS - 2);
  
   
  int cur_shape = rand() % (NUM_SHAPES+1);			//right now 2 0-regular 1-burst
  
  if(cur_shape){
  		used_radius = reg_radius;
  		used_density = reg_density;
  		used_quantity = reg_quantity;
  		used_after_counter = reg_after_counter;
  
  }
  else{
  		used_radius = bur_radius;
  		used_density = bur_density;
  		used_quantity = bur_quantity;
  		used_after_counter = bur_after_counter;
  		BASS_ChannelSetAttribute(sounds[num], BASS_ATTRIB_VOL, 90);
  	
  
  }

  GLfloat new_radius = (float)((rand()%41 -20)/100.)*used_radius +used_radius;
  GLfloat new_density = (float)((rand()%41 -20)/100.)*used_density +used_density;
  int new_quantity = (int)((float)(rand()%41 -20)/100.)*used_quantity +used_quantity;
  GLfloat new_after_counter= (float)((rand()%41 -20)/100.)*used_after_counter +used_after_counter;
  //GLfloat cur_cube_size = .007;

    
  

  switch( key ) {

  case 'q': 
  case 'Q': {
    exit(0) ;
    break ; 
  }
  case 't':{
  	current_shape =3;
  }
  case 'b':{
  	current_shape=4;
  }

  case '+':
  case '=': {
    if (cur_variable == RADIUS) {
      cur_radius += 2;
    }
    if (cur_variable == DENSITY) {
      cur_density += 1;
    }
    if (cur_variable == QUANTITY) {
      cur_quantity += 5;
    }
    if (cur_variable == CUBE_SIZE) {
      cur_cube_size += .001;
    }
    if (cur_variable == AFTER_COUNTER) {
      cur_after_counter += 10;
    }
    print_cur();
  } break;

  case 'j': {
    light_screen = 1;
  } break;

  case '-':
  case '_': {
    if (cur_variable == RADIUS) {
      cur_radius -= 2;
    }
    if (cur_variable == DENSITY) {
      cur_density -= 1;
    }
    if (cur_variable == QUANTITY) {
      cur_quantity -= 5;
    }
    if (cur_variable == CUBE_SIZE) {
      cur_cube_size -= .001;
    }
    if (cur_variable == AFTER_COUNTER) {
      cur_after_counter -= 10;
    }
    print_cur();
  } break;


  case 'z': {
    music_playing = (music_playing + 1) % 2;
    if (music_playing) {
      BASS_ChannelPlay(sounds[8], FALSE);
    }
    else {
      BASS_ChannelPause(sounds[8]);
    }
  } break;



  case 'a':
  case 'A': {
    cur_variable = RADIUS;
    print_cur();
  } break;
  case 's':
  case 'S': {
    cur_variable = DENSITY;
    print_cur();
  } break;
  case 'd':
  case 'D': {
    cur_variable = QUANTITY;
    print_cur();
  } break;
  case 'f':
  case 'F': {
    cur_variable = CUBE_SIZE;
    print_cur();
  } break;
  case 'g':
  case 'G': {
    cur_variable = AFTER_COUNTER;
    print_cur();
  } break;

  case '1': {

  	make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[0])->x, (&nozzles[0])->y + 1.2);
    //make_explosion(&explosions[0], 60, 1, 5000, .02, 48, 1, test_sound, (&nozzles[0])->x, (&nozzles[0])->y + 1);
    BASS_ChannelPlay(sounds[cur_shape], TRUE);

    break;
  }
  case '2': {
  	 make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[1])->x, (&nozzles[1])->y + 1.2);
    //make_explosion(&explosions[1], 500, 1, 100, .01, 1100, 1, test_sound, (&nozzles[1])->x, (&nozzles[1])->y + 1);
        BASS_ChannelPlay(sounds[cur_shape], TRUE);

    break;
  }
  case '3': {
  	 make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[2])->x, (&nozzles[2])->y + 1.2);
    //make_explosion(&explosions[2], 6MAX_FIREWORKS, 1, 100, .05, 800, 1, test_sound, (&nozzles[2])->x, (&nozzles[2])->y + 1);
        BASS_ChannelPlay(sounds[cur_shape], TRUE);

    break;
  }
  case '4': {
     if(cur_shape){
     	if(cur_shape ==1){
     		new_radius *=1.5;
     		new_quantity *=1.2;
     	}
     
     }
  	 make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[3])->x, (&nozzles[3])->y + 1.2);
    //make_explosion(&explosions[3], 1000, 1, 100, .05, 900, 1, test_sound, (&nozzles[3])->x, (&nozzles[3])->y + 1);
        BASS_ChannelPlay(sounds[cur_shape], TRUE);

    break;
  }
  case '5': {
  	 make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[4])->x, (&nozzles[4])->y + 1.2);
    //make_explosion(&explosions[4], 1MAX_FIREWORKS0, 1, 100, .1, 1000, 1, test_sound, (&nozzles[4])->x, (&nozzles[4])->y + 1);
        BASS_ChannelPlay(sounds[cur_shape], TRUE);

    break;
  }
  case '6': {
  	 make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[5])->x, (&nozzles[5])->y + 1.2);
    //make_explosion(&explosions[5], 600, 1, 100, .09, 1100, 1, test_sound, (&nozzles[5])->x, (&nozzles[5])->y + 1);
        BASS_ChannelPlay(sounds[cur_shape], TRUE);

    break;
  }
  case '7': {
  	 make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[6])->x, (&nozzles[6])->y + 1.2);
    //make_explosion(&explosions[6], 2MAX_FIREWORKS0, 1, 100, .3, 1200, 1, test_sound, (&nozzles[6])->x, (&nozzles[6])->y + 1);
        BASS_ChannelPlay(sounds[cur_shape], TRUE);

    break;
  }
  case '0': {
  	
    make_explosion(&explosions[num_fireworks = (num_fireworks + 1 )% MAX_FIREWORKS], new_radius, new_density, new_quantity, cur_cube_size, new_after_counter, cur_shape, test_sound, (&nozzles[3])->x, (&nozzles[3])->y + 1);
  } break;

  default: break;
  }


  glutPostRedisplay();
}




// quantity needs to be a square number
void make_explosion(EXPLOSION *po, GLfloat radius, GLfloat density, 
                          int quantity, GLfloat cube_size, int after_counter,
                          short shape, SOUND sound, GLfloat start_x, GLfloat start_y) {
  int i, j, k;
  int count = 0;
  short solid = rand()%5;
  GLfloat color[3];
  GLfloat other_color[3];
  
  if(shape ==0){
  		solid =2;
  }
  if(solid){
  		for(i=0;i<3;++i){
  			color[i] =(float)((rand() % (100)))/100.;			//randomize color
  			
  		}
  		
  		if(solid<3){
  			other_color[0] =color[0];
  			other_color[1] = color[1];
  			other_color[2] = color[2];
  		}
  		else{
  			for(i=0;i<3;++i){
  				other_color[i] =(float)((rand() % (100)))/100.;			//randomize color
  			
  			}
  		}
  }
  
  else{
  	 	for(i=0;i<3;++i){
  			color[i] =0;
  		}
  
  }
  	
  SHAPE cur;
  GLfloat trans_x, trans_y, trans_z;
  GLfloat ray = cube_size;
  
  start_y +=(float)((rand() % (7)) - 3)/10.;
  start_x +=(float)((rand() % (11)) - 5)/100.;
	radius=40;
	after_counter=100;
  po -> radius = radius;
  po -> density = density;
  po -> quantity = quantity;
  po -> cube_size = cube_size;
  po -> shape = shape;
  po -> sound = sound;
  po -> counter = 0;
  po -> start_x = start_x;
  po -> start_y = start_y;
  po -> after_counter = after_counter + radius;
  po -> fizz=0;
  po -> particles = malloc(quantity*sizeof(PARTICLE));
  shape =current_shape;
  
  trans_z=0;
  double y_fac=1.3;
  double scale_x=.025;
  double scale_y=.025;
  shape =5;
  if(shape==1){
  		
  		//make_heart(po,scale_x,scale_y,0,0);
  		//make_heart(po,scale_x*1.7,scale_y*1.7,1,28);
  		make_b(po,scale_x*2,scale_y*2,1,0);
  		make_y(po,scale_x*2,scale_y*2,1,19);
  	
  }
  else if(shape ==2){
  		make_z(po,scale_x*2,scale_y*2,1,0);
  		make_a(po,scale_x*2,scale_y*2,1,40);
  		make_c(po,scale_x*1.6,scale_y*1.6,1,58);
  		make_h(po,scale_x*2,scale_y*2,1,71);
  
  }
  else if(shape ==3){
  		make_a(po,scale_x*2,scale_y*2,2,0);
  		make_n(po,scale_x*1.2,scale_y*1.2,1,18);
  		make_d(po,scale_x*2,scale_y*2,1,43);
  	
  }
  else if(shape ==4){
  		make_a(po,scale_x*2,scale_y*2,3,0);
  		make_n(po,scale_x*1.2,scale_y*1.2,0,18);
  		make_d(po,scale_x*2,scale_y*2,0,43);
  		make_y(po,scale_x*2.1,scale_y*2.1,0,65);
  }
  else if(shape ==5){
  		make_shroom(po,scale_x,scale_y,0,0);
  }

  else{
		k = sqrt(quantity)/2;
		
		// draw the top half
		for (i = 0; i <= k; i++) {
			for (j = 0; j <= k; j++) {
			
			  make_cube_smart(&cur, ray);
			  real_translation(&cur, start_x, start_y, 0);
			
			  int t = (360.0/(float)(k) * j);    // theta
			  int p = (180.0/(float)(k) * i); // phi
			
			  GLfloat theta = (t * M_PI/180.); // theta angle
			  GLfloat phi = (p * M_PI/180.);  // phi angle
			  
			
			
			  trans_x = ray * cos(phi) * cos(theta) + (float)((rand() % (3)) - 1)/80.;
			  trans_y = ray * sin(phi) + (float)((rand() % (3)) - 1)/80.;
			  trans_z = -ray * cos(phi) * sin(theta) + (float)((rand() % (3)) - 1)/80.;
			
			  make_particle(&(po->particles[i*4 + j]), color, solid, cur, trans_x, trans_y, trans_z);
			  count++;
			}
		}
		// draw the bottom half
		for (i = 0; i <= k; i++) {
			for (j = 0; j <= k; j++) {
		
			  make_cube_smart(&cur, ray);
			  real_translation(&cur, start_x, start_y, 0);
			
			  int t = (360.0/(float)(k) * j);    // theta
			  int p = (180.0/(float)(k) * i - 180); // phi
			
			  GLfloat theta = (t * M_PI/180.); // theta angle
			  GLfloat phi = (p * M_PI/180.);  // phi angle
			  
			  trans_x = ray * cos(phi) * cos(theta) + (float)((rand() % (3)) - 1)/100.;;
			  trans_y = ray * sin(phi) + (float)((rand() % (3)) - 1)/100.;;
			  trans_z = -ray * cos(phi) * sin(theta) + (float)((rand() % (3)) - 1)/100.;;
			
			  make_particle(&(po->particles[count - 1 + i*4 + j]), other_color, solid, cur, trans_x, trans_y, trans_z);
			}
		}
	}
}

void make_particle(PARTICLE *po, GLfloat *init_color, short single_color, SHAPE shape,
                            GLfloat trans_x, GLfloat trans_y, GLfloat trans_z) {
  // just put it all in
  
  int i;
  for (i = 0; i < 3; i++) {
  	if(!single_color)
  		init_color[i] =(float)((rand() % (100)))/100.;			//randomize color
  		
  		
    po -> init_color[i] = init_color[i];
  
  }
  po->single_color = single_color;

  po -> shape = shape;

  po -> translation[0] = trans_x;
  po -> translation[1] = trans_y;
  po -> translation[2] = trans_z;


}


void my_time_out(int id) { 
  int i, j,k;
  
  PARTICLE *cur;
  EXPLOSION *cur_explosion;
  float fall_time;

  for (j = 0; j < MAX_FIREWORKS; j++) {
    
    cur_explosion = &explosions[j];
    for (i = 0; i < (&explosions[j])->quantity; i++) {
    
      cur = &(cur_explosion->particles[i]);
    
      if (cur_explosion->counter < cur_explosion->radius) {
        real_translation(&(cur->shape), cur->translation[0], cur->translation[1], cur->translation[2]);
      }
    
      else {
    
        if (cur_explosion->counter > cur_explosion->radius && cur_explosion->counter < cur_explosion->radius + cur_explosion->after_counter && cur_explosion -> radius > 0) {
          fall_time =(float)((rand() % (10)))/100000.;
          real_translation(&(cur->shape), 0, -fall_time*5, 0);
          cur->init_color[0]=cur->init_color[0]-250*fall_time;
          cur->init_color[1]=cur->init_color[1]-250*fall_time;
          cur->init_color[2]=cur->init_color[2]-250*fall_time;
          if(!cur_explosion->fizz){
         // 	BASS_ChannelPlay(sounds[7], TRUE);
          	cur_explosion->fizz=1;
          	
          }
          // TODO THIS IS WHERE WE WANT TO START FADING AS WELL
        }

        if (cur_explosion->counter > cur_explosion->after_counter && cur_explosion -> radius > 0) {
          cur_explosion->fizz=0;
          free(&(cur_explosion->particles[0]));


          cur_explosion -> radius = 0;
          cur_explosion -> density = 0;
          cur_explosion -> quantity = 0;
          cur_explosion -> cube_size = 0;
          cur_explosion -> shape = 0;
          cur_explosion -> counter = 0;
          cur_explosion -> start_x = 0;
          cur_explosion -> start_y = 0;
          cur_explosion -> after_counter = 0;
        }
      }  
    }
    if (cur_explosion->radius > 0) {
      cur_explosion->counter += 1;
    }
  }


  glutPostRedisplay();                       // re-draw the scene
  glutTimerFunc(timer_speed, my_time_out, 0); // keeps the timer moving, at timer_speed ms

}

void make_heart(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	
	SHAPE cur;
	int solid = 1;
	
	double trans_z=0;
	double y_fac =1.3;
	double ray =.007;
	
	GLfloat color[3];
	if(color_type){
		color[0]=1;
  		color[1]=1;
  		color[2]=1;
  		scale_y+=.003;
  	}
  	else{
  		color[0]=1;
  		color[1]=0;
  		color[2]=0;
  		scale_y-=.003;
  	}
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .107*scale_x,0.01*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .092*scale_x,-0.014*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .12*scale_x,.03*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .13*scale_x,.06*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .12*scale_x,.09*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .09*scale_x,.12*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .05*scale_x,.12*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,.1*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0,.07*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.107*scale_x,0.01*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.092*scale_x,-0.014*y_fac*scale_y, trans_z);

	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.12*scale_x,.03*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.13*scale_x,.06*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.12*scale_x,.09*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.09*scale_x,.12*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.05*scale_x,.12*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.02*scale_x,.1*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.08*scale_x,-.03*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.065*scale_x,-.045*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.05*scale_x,-.06*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.035*scale_x,-.075*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.02*scale_x,-.0925*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);	
	make_particle(&(po->particles[i++]), color, solid, cur, .08*scale_x,-.03*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .065*scale_x,-.045*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .05*scale_x,-.06*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .035*scale_x,-.075*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,-.0925*y_fac*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0,-.11*y_fac*scale_y, trans_z);

}

void make_b(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	
	SHAPE cur;
	int solid = 1;
	
	double trans_z=0;
	double ray =.007;
	
	
	double start_x=-.6;
	double start_y=.5;
	GLfloat color[3];
	
	color[0]=1;
  	color[1]=1;
  	color[2]=1;
  	
  	po->start_x = start_x;
  	po->start_y = start_y;


	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,0, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.01*scale_x,0, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .035*scale_x,0.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.035*scale_x,-.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .06*scale_x,.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .06*scale_x,-.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.055*scale_x,.09*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.055*scale_x,-.09*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .03*scale_x,.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .03*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0,0.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0,-0.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,0, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,.033*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,-.033*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,.066*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,-.066*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,-.1*scale_y, trans_z);
	
	

}



void make_y(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){

	SHAPE cur;
	int solid = 1;
	
	double trans_z=0;
	double ray =.007;
	double start_x;
	double start_y;
	
	
	
	
	GLfloat color[3];
	if(color_type){
		color[0]=1;
  		color[1]=1;
  		color[2]=1;
  		start_x=-.4;
  		start_y=.4;
  	}
  	else{
  		color[0]=.1;
  		color[1]=.3;
  		color[2]=.6;
  		start_x=.8;
  		start_y=-.25;
  	
  	}
  	
  	po->start_x = start_x;
  	po->start_y = start_y;
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -0.021*scale_x,-.055*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -0.025*scale_x,-.075*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.012*scale_x,-.089*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .006*scale_x,-.093*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .024*scale_x,-0.09*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .032*scale_x,-0.075*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .034*scale_x,-.055*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .035*scale_x,-.035*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .032*scale_x,-.013*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .03*scale_x,.005*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .029*scale_x,.029*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .032*scale_x,.059*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .035*scale_x,.068*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .022*scale_x,.019*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .013*scale_x,.005*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.018*scale_x,.009*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.025*scale_x,.023*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.030*scale_x,.041*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.037*scale_x,.057*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.045*scale_x,.065*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .032*scale_x,.044*scale_y, trans_z);
	

}


void make_z(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){

	SHAPE cur;
	int solid = 1;

	double trans_z=0;
	double ray =.007;
	
	
	double start_x=0;
	double start_y=.45;
	
	GLfloat color[3];
	color[0]=.9;
  	color[1]=.3;
  	color[2]=.1;
  	
  	po->start_x = start_x;
  	po->start_y = start_y;


	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0,0, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .041*scale_x,.037*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .058*scale_x,0.053*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .069*scale_x,0.069*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur,0.077*scale_x,.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.07*scale_x,.099*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.055*scale_x,.11*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .023*scale_x,.112*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .005*scale_x,.114*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.021*scale_x,.112*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.058*scale_x,0.108*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.08*scale_x,.106*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.103*scale_x,.104*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.11*scale_x,.083*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .025*scale_x,.007*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.045*scale_x,.009*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.067*scale_x,.01*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0.085*scale_x,-.001*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.013*scale_x,-.003*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.029*scale_x,-.007*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.051*scale_x,-.01*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.022*scale_x,-.018*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.049*scale_x,-.038*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.069*scale_x,-.047*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.09*scale_x,-.065*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.11*scale_x,-.079*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.12*scale_x,-.099*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.107*scale_x,-.116*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.08*scale_x,-.118*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.057*scale_x,-.119*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.021*scale_x,-.12*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .009*scale_x,-.121*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .037*scale_x,-.120*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .06*scale_x,-.118*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .093*scale_x,-.118*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .120*scale_x,-.115*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .15*scale_x,-.111*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .172*scale_x,-.103*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .186*scale_x,-.094*scale_y, trans_z);
	



}

void make_a(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	SHAPE cur;
	int solid = 1;
	double trans_z=0;
	double ray =.007;
	double start_x;
	double start_y;
	
	
	
	GLfloat color[3];
	if(color_type==1){
		color[0]=.9;
  		color[1]=.3;
  		color[2]=.1;
  		start_x=.2;
  		start_y=.45;
  	}
  	else if(color_type ==2){
  		color[0]=1;
  		color[1]=1;
  		color[2]=1;
  		start_x=-.55;
		start_y=.05;
  	
  	}
  	else if(color_type ==3){
  		color[0]=.1;
  		color[1]=.3;
  		color[2]=.6;
  		start_x=.18;
		start_y=-.18;
  	
  	}
  	
  	po->start_x = start_x;
  	po->start_y = start_y;
  	
  	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .045*scale_x,0, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .043*scale_x,.014*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .041*scale_x,.031*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .039*scale_x,.047*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,.043*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .002*scale_x,.035*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.015*scale_x,.022*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.029*scale_x,.013*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.039*scale_x,-.006*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.036*scale_x,-.029*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.021*scale_x,-.045*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.001*scale_x,-.048*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,-.041*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .038*scale_x,-.03*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .052*scale_x,-.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .062*scale_x,-.041*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .078*scale_x,-.051*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .099*scale_x,-.056*scale_y, trans_z);
  	


}

void make_c(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	SHAPE cur;
	int solid = 1;
	double trans_z=0;
	double ray =.007;
	
	
	double start_x=.4;
	double start_y=.45;
	
	GLfloat color[3];
	color[0]=.9;
  	color[1]=.3;
  	color[2]=.1;
  	
  	po->start_x = start_x;
  	po->start_y = start_y;
  	
  	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .054*scale_x,.038*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .059*scale_x,.063*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .041*scale_x,.072*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.005*scale_x,.065*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.038*scale_x,.045*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.052*scale_x,.010*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.049*scale_x,-.024*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.020*scale_x,-.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .001*scale_x,-.044*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .04*scale_x,-.035*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .07*scale_x,-.015*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .081*scale_x,-.005*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .115*scale_x,.022*scale_y, trans_z);
	
	


}

void make_h(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	SHAPE cur;
	int solid = 1;
	double trans_z=0;
	double ray =.007;
	
	
	double start_x=.6;
	double start_y=.45;
	
	GLfloat color[3];
	color[0]=.9;
  	color[1]=.3;
  	color[2]=.1;
  	
  	po->start_x = start_x;
  	po->start_y = start_y;
  	
  	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.014*scale_x,.099*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.022*scale_x,.077*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,.057*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.04*scale_x,.03*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.038*scale_x,0, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.035*scale_x,-.026*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.022*scale_x,-.039*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.005*scale_x,-.022*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .01*scale_x,-.01*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .022*scale_x,.01*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .030*scale_x,.029*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .032*scale_x,-.01*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .042*scale_x,-.022*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .063*scale_x,-.035*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .081*scale_x,-.038*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .1*scale_x,-.04*scale_y, trans_z);


}

void make_n(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	SHAPE cur;
	int solid = 1;
	double trans_z=0;
	double ray =.007;
	double start_x;
	double start_y;
	
	GLfloat color[3];
	if(color_type){
		color[0]=1;
  		color[1]=1;
  		color[2]=1;
  		start_x=-.3;
  		start_y=.05;
  	}
  	else{
  		color[0]=.1;
  		color[1]=.3;
  		color[2]=.6;
  		start_x=.42;
  		start_y=-.18;
  	
  	}
  	
  	po->start_x = start_x;
  	po->start_y = start_y;
  	
  	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .07*scale_x,-.075*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .056*scale_x,-.079*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .052*scale_x,-.059*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .049*scale_x,-.036*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .045*scale_x,-.018*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .044*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .042*scale_x,.026*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .041*scale_x,.044*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .028*scale_x,.063*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .014*scale_x,.071*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,.076*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.018*scale_x,.075*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.032*scale_x,.068*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.046*scale_x,.058*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.05*scale_x,.038*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.06*scale_x,.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.06*scale_x,-.008*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.063*scale_x,-.03*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.066*scale_x,-.046*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.075*scale_x,-.065*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.081*scale_x,-.081*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.067*scale_x,.039*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.074*scale_x,.058*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.078*scale_x,.078*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.092*scale_x,.081*scale_y, trans_z);

	//25
}

void make_d(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	SHAPE cur;
	int solid = 1;
	double trans_z=0;
	double ray =.007;

	
	double start_x=.15;
	double start_y=.45;
	
	GLfloat color[3];
	if(color_type){
		color[0]=1;
  		color[1]=1;
  		color[2]=1;
  		start_x=-.1;
		start_y=.05;
  	}
  	else{
  		color[0]=.1;
  		color[1]=.3;
  		color[2]=.6;
  		
  		start_x=.6;
		start_y=-.18;
  	
  	}
  	
  	po->start_x = start_x;
  	po->start_y = start_y;
  	
  	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .083*scale_x,-.057*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .065*scale_x,-.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .056*scale_x,-.048*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .046*scale_x,-.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .043*scale_x,.005*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .041*scale_x,.035*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .043*scale_x,.058*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .048*scale_x,.084*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .051*scale_x,.103*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .06*scale_x,.124*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .038*scale_x,.031*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .019*scale_x,.039*scale_y, trans_z);
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.007*scale_x,.036*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,.024*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.043*scale_x,.008*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.052*scale_x,-.009*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.042*scale_x,-.045*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.021*scale_x,-.059*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,-.064*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .022*scale_x,-.056*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .04*scale_x,-.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.052*scale_x,-.031*scale_y, trans_z);


}


void make_tri(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	SHAPE cur;
	int solid = 1;
	double trans_z=0;
	double ray =.007;
	
	GLfloat color[3];
	color[0]=.8;
	color[1]=.8;
	color[2]=.5;
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .0*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.02*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .04*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.04*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .06*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.06*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .012*scale_x,.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.012*scale_x,.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .024*scale_x,.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.024*scale_x,.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .036*scale_x,.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.036*scale_x,.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .048*scale_x,.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.048*scale_x,.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .012*scale_x,-.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.012*scale_x,-.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .024*scale_x,-.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.024*scale_x,-.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .036*scale_x,-.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.036*scale_x,-.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .048*scale_x,-.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.048*scale_x,-.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.12*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .12*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.108*scale_x,-.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.096*scale_x,-.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.084*scale_x,-.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.072*scale_x,-.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .108*scale_x,-.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .096*scale_x,-.06*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .084*scale_x,-.04*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .072*scale_x,-.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.1*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.08*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.06*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.04*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.02*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .1*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .08*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .06*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .04*scale_x,-.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .02*scale_x,-.1*scale_y, trans_z);
	
	

}

void make_shroom(EXPLOSION *po,double scale_x,double scale_y,int color_type,int i){
	SHAPE cur;
	int solid = 1;
	double trans_z=0;
	double ray =.007;
	
	GLfloat color[3];
	
	if(color_type){
		color[0]=0;
		color[1]=.8;
		color[2]=0;
	}
	else{
		color[0]=.8;
		color[1]=0;
		color[2]=0;
	
	}
	
	
	//
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,.22*scale_y, trans_z);
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .019*scale_x,.218*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .04*scale_x,.215*scale_y, trans_z);
	//6
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .063*scale_x,.212*scale_y, trans_z);//*****
	//5
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .082*scale_x,.201*scale_y, trans_z);//****
	//4
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .1*scale_x,.19*scale_y, trans_z);//***
	//3
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .117*scale_x,.18*scale_y, trans_z);//**
	//2
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .133*scale_x,.162*scale_y, trans_z);//*
	//1
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .145*scale_x,.15*scale_y, trans_z);
	//1
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .158*scale_x,.128*scale_y, trans_z);//*
	//2
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .17*scale_x,.112*scale_y, trans_z);//**
	//3
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .18*scale_x,.096*scale_y, trans_z);//***
	//4
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .189*scale_x,.08*scale_y, trans_z);//****
	//5
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .198*scale_x,.055*scale_y, trans_z);//*****
	//6
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .204*scale_x,.039*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .206*scale_x,.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .208*scale_x,0*scale_y, trans_z);
	
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.019*scale_x,.218*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.04*scale_x,.215*scale_y, trans_z);
	//6
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.063*scale_x,.212*scale_y, trans_z);//*****
	//5
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.082*scale_x,.201*scale_y, trans_z);//****
	//4
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.1*scale_x,.19*scale_y, trans_z);//***
	//3
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.117*scale_x,.18*scale_y, trans_z);//**
	//2
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.133*scale_x,.162*scale_y, trans_z);//*
	//1
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.145*scale_x,.15*scale_y, trans_z);
	//1
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.158*scale_x,.128*scale_y, trans_z);//*
	//2
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.17*scale_x,.112*scale_y, trans_z);//**
	//3
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.18*scale_x,.096*scale_y, trans_z);//***
	//4
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.189*scale_x,.08*scale_y, trans_z);//****
	//5
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.198*scale_x,.055*scale_y, trans_z);//*****
	//6
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.204*scale_x,.039*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.206*scale_x,.02*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.208*scale_x,0*scale_y, trans_z);
	
	//
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .209*scale_x,-.019*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .207*scale_x,-.036*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .204*scale_x,-.045*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .199*scale_x,-.061*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .190*scale_x,-.08*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .179*scale_x,-.097*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .159*scale_x,-.116*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .139*scale_x,-.12*scale_y, trans_z);//**
	
	
	//
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.209*scale_x,-.019*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.207*scale_x,-.036*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.204*scale_x,-.045*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.199*scale_x,-.061*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.190*scale_x,-.08*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.179*scale_x,-.097*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.159*scale_x,-.116*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.139*scale_x,-.12*scale_y, trans_z);//**
	
	color[0]=.8;
	color[1]=.7;
	color[2]=.5;
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,-.056*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .018*scale_x,-.056*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .032*scale_x,-.057*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .050*scale_x,-.059*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .065*scale_x,-.062*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .083*scale_x,-.073*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .1*scale_x,-.083*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .118*scale_x,-.102*scale_y, trans_z);//**
	
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .12*scale_x,-.12*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .118*scale_x,-.141*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .105*scale_x,-.162*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .096*scale_x,-.179*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .083*scale_x,-.189*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .07*scale_x,-.194*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .052*scale_x,-.196*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .035*scale_x,-.198*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .017*scale_x,-.199*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,-.2*scale_y, trans_z);//**
	
	//
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.018*scale_x,-.056*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.032*scale_x,-.057*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.050*scale_x,-.059*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.065*scale_x,-.062*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.083*scale_x,-.073*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.1*scale_x,-.083*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.118*scale_x,-.102*scale_y, trans_z);//**
	
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.12*scale_x,-.12*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.118*scale_x,-.141*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.105*scale_x,-.162*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.096*scale_x,-.179*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.083*scale_x,-.189*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.07*scale_x,-.194*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.052*scale_x,-.196*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.035*scale_x,-.198*scale_y, trans_z);//**
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.017*scale_x,-.199*scale_y, trans_z);//**
	

	
	
	
	
	
	
	
	
	color[0]=1;
	color[1]=1;
	color[2]=1;
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .158*scale_x,.108*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .148*scale_x,.082*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .142*scale_x,.063*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .14*scale_x,.045*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .143*scale_x,.03*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .147*scale_x,.009*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .158*scale_x,-.008*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .168*scale_x,-.023*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .179*scale_x,-.03*scale_y, trans_z);
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .193*scale_x,-.04*scale_y, trans_z);
	//
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.158*scale_x,.108*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.148*scale_x,.082*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.142*scale_x,.063*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.14*scale_x,.045*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.143*scale_x,.03*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.147*scale_x,.009*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.158*scale_x,-.008*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.168*scale_x,-.023*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.179*scale_x,-.03*scale_y, trans_z);
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.193*scale_x,-.04*scale_y, trans_z);
	
	//&
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,0*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .03*scale_x,.008*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .057*scale_x,.019*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .072*scale_x,.039*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .09*scale_x,.059*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .1*scale_x,.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .103*scale_x,.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .1*scale_x,.12*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .09*scale_x,.141*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .072*scale_x,.161*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .057*scale_x,.181*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .03*scale_x,.192*scale_y, trans_z);
	//
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, .0*scale_x,.2*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,.008*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.057*scale_x,.019*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.072*scale_x,.039*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.09*scale_x,.059*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.1*scale_x,.08*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.103*scale_x,.1*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.1*scale_x,.12*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.09*scale_x,.141*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.072*scale_x,.161*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.057*scale_x,.181*scale_y, trans_z);
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, -.03*scale_x,.192*scale_y, trans_z);
	
	
	make_cube_smart(&cur,ray);
	real_translation(&cur,po->start_x,po->start_y,0);
	make_particle(&(po->particles[i++]), color, solid, cur, 0*scale_x,.2*scale_y, trans_z);

	
	
	
	
}



/*======================================================*/
/* =========== G O O D     F U N C T I O N S ===========*/
/*======================================================*/


int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  glutCreateWindow("Going out with a Bang");
  initGL();
  setup_BASS();
  setup_nozzles();

  glutDisplayFunc(my_display);
  glutKeyboardFunc(my_keyboard);
  glutTimerFunc( timer_speed, my_time_out, 0);/* schedule a my_TimeOut call with the ID 0 in 20 seconds from now */

  glutMainLoop();

  return 0;
}


void print_cur() {
  printf("\nHere's a dump of the cur_variables:\n");
  printf("cur_radius: %f, cur_density: %f\n", cur_radius, cur_density);
  printf("cur_quantity: %d, cur_cube_size: %f\n", cur_quantity, cur_cube_size);
  printf("cur_after_counter: %d\n", cur_after_counter);
  printf("You're adjusting %d\n", cur_variable);
  printf("Where RADIUS 0, DENSITY 1, QUANTITY 2,\nCUBE_SIZE 3, SHAPE 4, AFTER_COUNTER 5\n");
  printf("firework with quantity 200 is %ld\n", sizeof(&explosions[3]) + 200*sizeof(PARTICLE));
}


void initGL()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);

  if (shift_camera) {
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
    gluPerspective( 40, 1.0, 1, 200.0);
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity() ;  // init modelview to identity
  }

  srand((unsigned int)time(NULL));        // seed the rng

}

void setup_BASS()
{
  int i;
  if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
    printf("An incorrect version of BASS was loaded");
    return;
  }
  // setup output - default device
  if (!BASS_Init(-1,44100,0,0,NULL))
    exit(0);

    // this could probably be a simple for loop, but can't figure it out yet
    // TODO sprintf can be used to make the filenames in a for loop (if wanted)
    sounds[0] = BASS_StreamCreateFile(FALSE, "./sound/bang6.mp3", 0, 0, 0);
    sounds[1] = BASS_StreamCreateFile(FALSE, "./sound/bang2.mp3", 0, 0, 0);
    sounds[2] = BASS_StreamCreateFile(FALSE, "./sound/bang3.mp3", 0, 0, 0);
    sounds[3] = BASS_StreamCreateFile(FALSE, "./sound/bang0.mp3", 0, 0, 0);
    sounds[4] = BASS_StreamCreateFile(FALSE, "./sound/bang3.mp3", 0, 0, 0);
    sounds[5] = BASS_StreamCreateFile(FALSE, "./sound/bang4.mp3", 0, 0, 0);
    sounds[6] = BASS_StreamCreateFile(FALSE, "./sound/bang6.mp3", 0, 0, 0);
    sounds[7] = BASS_StreamCreateFile(FALSE, "./sound/fizz_out.wav", 0, 0, 0);
    sounds[8] = BASS_StreamCreateFile(FALSE, "./sound/1812.wav", 0, 0, 0);
    BASS_ChannelSetAttribute(sounds[7], BASS_ATTRIB_VOL, .2);
}

void setup_nozzles()
{
  int i;
  GLfloat trans_x;
  GLfloat trans_y;
  GLfloat trans_z;

  for (i = 0; i < NUM_NOZZLES; i++) {
    trans_x = -1 + (float)2/(float)NUM_NOZZLES + (2 - (float)2/(float)NUM_NOZZLES)*(float)i/(float)NUM_NOZZLES;
    trans_y = -.95;
    trans_z = .7;
    make_nozzle(&nozzles[i], .004, .12, i + 1);
    real_scaling(&(nozzles[i].shape), 1, .3, 1);
    real_translation(&(nozzles[i].shape), trans_x, trans_y, trans_z);
    
    nozzles[i].x = trans_x;
    nozzles[i].y = trans_y;
    nozzles[i].z = trans_z;
    
  }
}

void my_display()
{
 glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  draw_nozzles();


  if (light_screen) {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   light_screen++;

   if (light_screen >= 4) {
    light_screen = 0;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   }
  }

  draw_explosion();

  // restore projection
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glutSwapBuffers();
}

void draw_nozzles() {
  int i;
  GLfloat v1[4];
  GLfloat v2[4];

  for (i = 0; i < NUM_NOZZLES; i++) {
    NOZZLE *cur;
    cur = &nozzles[i];
 
    curr_rs = CYLINDER_RS;
    curr_vs = CYLINDER_VS;

    draw_shape(&(cur->shape), colors[BLUE]);
  }

}

void draw_explosion()
{
  int i, j;
  for (j = 0; j < MAX_FIREWORKS; j++) {
    for (i = 0; i < (&explosions[j])->quantity; i++) {
      PARTICLE *cur;
      cur = &((&explosions[j])->particles[i]);

      curr_rs = CUBE_RS;
      curr_vs = CUBE_VS;

      draw_shape(&(cur->shape), cur->init_color);
    }
  }
}

// this works for all shapes
void draw_shape(SHAPE *verts, GLfloat* color)
{
  int i, j; // loop variables 
  
  for(i = 0; i < curr_vs; i++) {  // handles main body of array
    for(j = 0; j < curr_rs; j++) {
      draw_quads(verts->shape_verts, i, j, i, j+1, i+1, j+1, i+1, j, color); // draws shape
    }
  }
  for (i = 0; i < curr_vs; i ++) {  // connects the sides
    draw_quads(verts->shape_verts, i, curr_rs, i+1, curr_rs, i+1, 0, i, 0, color);
  }

  // this handles the cube top
  if (curr_rs == CUBE_RS && curr_vs == CUBE_VS) {
    for(i = 0; i < 2; i++) {  
    draw_quads(verts->shape_verts, curr_vs - i, 0, curr_vs - i, 1, curr_vs - i, 2, curr_vs - i, 3, color); 
    }
  }

}

void real_translation(SHAPE *po, GLfloat x, GLfloat y, GLfloat z) {
  int i, j;

  for(i = 0; i <= curr_vs; i++) {
    for(j = 0; j <= curr_rs; j++) {
      po->shape_verts[i][j][0] += x; // translate across x
      po->shape_verts[i][j][1] += y; // translates across y
      po->shape_verts[i][j][2] += z; // translates across z
    }
  }
  
}

void real_scaling(SHAPE *po, GLfloat sx, GLfloat sy, GLfloat sz) {
  int i, j;
  
  for(i = 0; i <= curr_vs; i++) {
    for(j = 0; j <= curr_rs; j++) {
      po->shape_verts[i][j][0] = po->shape_verts[i][j][0] * sx; // scales x
      po->shape_verts[i][j][1] = po->shape_verts[i][j][1] * sy; // scales y
      po->shape_verts[i][j][2] = po->shape_verts[i][j][2] * sz; // scales z
    }
  }
  
}

void real_rotation(SHAPE *po, GLfloat deg, GLfloat x, GLfloat y, GLfloat z) { 
  int i, j;

  GLfloat theta = (deg * M_PI/180); // deg to rotate by
  
  for(i = 0; i <= curr_vs; i++) {
    for(j = 0; j <= curr_rs; j++) {
      GLfloat X = po->shape_verts[i][j][0];
      GLfloat Y = po->shape_verts[i][j][1];
      GLfloat Z = po->shape_verts[i][j][2];
     
      if(x == 1) {    // rotate X
        po->shape_verts[i][j][1] = Y * cos(theta) - Z * sin(theta); 
        po->shape_verts[i][j][2] = Z * cos(theta) + Y * sin(theta);
      }
      else if(y ==1) {  // rotate Y             
        po->shape_verts[i][j][0] = X * cos(theta) + Z * sin(theta);
        po->shape_verts[i][j][2] = Z * cos(theta) - X * sin(theta);
      }
      else if (z ==1) { // rotate Z
        po->shape_verts[i][j][0] = X * cos(theta) - Y * sin(theta); 
        po->shape_verts[i][j][1] = X * sin(theta) + Y * cos(theta);
      }
    }
  }

}


void make_nozzle(NOZZLE* noz, double ray, double height, int id){
  SHAPE cyl;
  make_cylinder(&cyl, ray, height);
  noz->shape = cyl;
  noz->cur_firework = 0;    // at position 0 in the fireworks array
  noz->nozzle_id = id;      // set the nozzle id

  // TODO -- set the array of fireworks that are in the nozzle  

}

void make_cube_smart(SHAPE *po, double size){
  size = size/2;

  // top
  po->shape_verts[0][0][0] = -size;
  po->shape_verts[0][0][1] = size;
  po->shape_verts[0][0][2] = size;
  po->shape_verts[0][0][3] = 1;

  po->shape_verts[0][1][0] = size;
  po->shape_verts[0][1][1] = size;
  po->shape_verts[0][1][2] = size;
  po->shape_verts[0][1][3] = 1;
  
  po->shape_verts[0][2][0] = size;
  po->shape_verts[0][2][1] = size;
  po->shape_verts[0][2][2] = -size;
  po->shape_verts[0][2][3] = 1;
  
  po->shape_verts[0][3][0] = -size;
  po->shape_verts[0][3][1] = size;
  po->shape_verts[0][3][2] = -size;
  po->shape_verts[0][3][3] = 1;
  
  // bottom
  po->shape_verts[1][0][0] = -size;
  po->shape_verts[1][0][1] = -size;
  po->shape_verts[1][0][2] = size;
  po->shape_verts[1][0][3] = 1;
  
  po->shape_verts[1][1][0] = size;
  po->shape_verts[1][1][1] = -size;
  po->shape_verts[1][1][2] = size;
  po->shape_verts[1][1][3] = 1;
  
  po->shape_verts[1][2][0] = size;
  po->shape_verts[1][2][1] = -size;
  po->shape_verts[1][2][2] = -size;
  po->shape_verts[1][2][3] = 1;
  
  po->shape_verts[1][3][0] = -size;
  po->shape_verts[1][3][1] = -size;
  po->shape_verts[1][3][2] = -size;
  po->shape_verts[1][3][3] = 1;

  po->shape_id = CUBE;
  
}

void make_cylinder(SHAPE *vert, double ray, double height){
  int i,j,k;

  for(i = 0; i <= curr_vs; i++) {   // iterates through the angles
    for(j = 0; j <= curr_rs; j++) { // iterates through the layers
    
      int t = (360/curr_rs * j);    // theta        
      GLfloat theta = (t * M_PI/180); // theta angle
      
      for(k = 0; k < 4; k++) {      // iterates through x,y,z coordinates
        switch(k){
          case 0:
            vert->shape_verts[i][j][k] = ray * cos(theta); break; // x coordinate
          case 1:
            vert->shape_verts[i][j][k] = (height/curr_vs * i) - height/2; break; // y coordinate
          case 2:
            vert->shape_verts[i][j][k] = ray * sin(theta); break; // z coordinate
          case 3:
            vert->shape_verts[i][j][k] = 1; // homogenious coordinate
        }
      }
    }
  }

  vert->shape_id = CYLINDER;

}


void draw_quads(GLfloat vertices[12][12][4], int iv1a, int iv1b, int iv2a, int iv2b, int iv3a, int iv3b, int iv4a, int iv4b, GLfloat* color)
{
  glMatrixMode(GL_MODELVIEW);
  
  glBegin(GL_POLYGON); 
  {
    glColor3fv(color);    

    glVertex4fv(vertices[iv1a][iv1b]);
    glVertex4fv(vertices[iv2a][iv2b]);
    glVertex4fv(vertices[iv3a][iv3b]);
    glVertex4fv(vertices[iv4a][iv4b]);
  }
  
  glEnd();
}
