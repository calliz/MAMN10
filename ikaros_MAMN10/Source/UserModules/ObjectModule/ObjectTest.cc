#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>  

#define buffer_size 20
#define number_of_objects 1
#define velocity_distance 5
#define x_activity_scale_factor 1
#define y_activity_scale_factor 1
#define z_activity_scale_factor 1

//Structs
typedef struct
{
	int x_cm;
	int y_cm;
	int z_cm;
	float micros;
}buffer_node;

typedef struct
{
	float x_velo;
	float y_velo;
	float z_velo;
	float activity;
}object;

struct timeval tim; 

//Global variables
buffer_node* buffer;
object* object_list;
int current_node;


//Code
void calculateVelocity(){
	int buddy_node = (current_node - velocity_distance) % buffer_size;
	if(buddy_node<0){
		buddy_node = buddy_node+buffer_size;
	}
	float buddy_micros;
	if((buddy_micros = buffer[buddy_node].micros)!= 0){
		object_list->x_velo= ((float) (buffer[current_node].x_cm - buffer[buddy_node].x_cm)) / (buffer[current_node].micros - buffer[buddy_node].micros);
		object_list->y_velo= ((float) (buffer[current_node].y_cm - buffer[buddy_node].y_cm)) / (buffer[current_node].micros - buffer[buddy_node].micros);
		object_list->z_velo= ((float) (buffer[current_node].z_cm - buffer[buddy_node].z_cm)) / (buffer[current_node].micros - buffer[buddy_node].micros);
	}
}

void calculateActivity(){
	object_list->activity= x_activity_scale_factor*fabs(object_list->x_velo*1000) + y_activity_scale_factor*fabs(object_list->y_velo*1000) + z_activity_scale_factor*fabs(object_list->z_velo*1000);
}




void tick(int identifier, double x, double y, int z_cm)
{
	int x_cm, y_cm;
	float camera_y_deg = 43/2 * M_PI /180;
	float camera_x_deg = 57/2 * M_PI /180;
	int picture_width = (int)(2*z_cm * tan(camera_x_deg));
	int picture_height = (int)(2*z_cm * tan(camera_y_deg));
			
	x_cm = (int)(x * picture_width);
	y_cm = (int)(y * picture_height);

    gettimeofday(&tim, NULL);  
    double t1=(tim.tv_usec); 
	buffer[current_node].x_cm = x_cm;
	buffer[current_node].y_cm = y_cm;
	buffer[current_node].z_cm = z_cm;
	buffer[current_node].micros = t1;
	
	calculateVelocity();
	calculateActivity();
	current_node++;

	if(current_node == buffer_size){
		current_node = 0;
	}	
}

void print_buffer()
{
	fprintf(stdout, "current_node=%d\n", current_node);
	int i;
	for(i=0; i<buffer_size; i++){
		fprintf(stdout, "i:%d x=%d y=%d z=%d micros=%lf\n", i,buffer[i].x_cm, buffer[i].y_cm, buffer[i].z_cm, buffer[i].micros);
	}
}

void print_object()
{
	int i;
	for(i=0; i<number_of_objects;i++){
		fprintf(stdout, "i:%d x_velo=%f y_velo=%f z_velo=%f activity=%f\n", i, object_list[i].x_velo, object_list[i].y_velo, object_list[i].z_velo, object_list[i].activity);
	}

}

int main()
{ 
	buffer = (buffer_node*) calloc(sizeof(buffer_node), buffer_size);
	object_list = (object*) calloc(sizeof(object), number_of_objects);
	current_node = 0;
	
	int i;
	for(i=0;i<30;i++){
	tick(0, 1-0.03*i,1-0.02*i,50-10*i);
		print_object();
		usleep(1000);
	}
	print_object();
	print_buffer();
}

