
#include "FaceModule.h"
#include <sys/time.h>
#include <unistd.h>  

#define buffer_size 20
#define number_of_objects 1
#define velocity_distance 5
#define x_activity_scale_factor 1
#define y_activity_scale_factor 1
#define z_activity_scale_factor 1
// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;
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

FaceModule::FaceModule()
{
	buffer = (buffer_node*) calloc(sizeof(buffer_node), buffer_size);
	object_list = (object*) calloc(sizeof(object), number_of_objects);
	current_node = 0;
}


void
FaceModule::Init()
{

    input_position_array = GetInputArray("FACE_POSITION");
    input_position_array_size = GetInputSize("FACE_POSITION");
    
    input_dist_array = GetInputArray("FACE_DISTANCE");
    input_dist_array_size = GetInputSize("FACE_DISTANCE");

    output_position_array = GetOutputArray("OBJECT_POSITION");
    output_position_array_size = GetOutputSize("OBJECT_POSITION");

    output_status_array = GetOutputArray("OBJECT_STATUS");
    output_status_array_size = GetOutputSize("OBJECT_STATUS");
}



FaceModule::~FaceModule()
{
    // Destroy data structures that you allocated in Init.

    //destroy_array(internal_array);
    //destroy_matrix(internal_matrix);
	

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

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

void setReturnValues()
{
	output_position_array[0]=buffer[current_node].x_cm;
	output_position_array[1]=buffer[current_node].y_cm;
	output_position_array[2]=buffer[current_node].z_cm;

    output_status_array[0] = object_list->x_velo;
	output_status_array[1] = object_list->y_velo;
	output_status_array[2] = object_list->z_velo;
	output_status_array[3] = object_list->activity;
}

void
FaceModule::Tick()
{
	double x, y;
	int x_cm, y_cm, z_cm;
	
	x= input_position_array[1];
	y = input_position_array[0];
	z_cm = input_dist_array[0];

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

	setReturnValues();
	current_node++;

	if(current_node == buffer_size){
		current_node = 0;
	}	   
}


