
#include "ObjectModule.h"
#include <sys/time.h>

#define buffer_size 20
#define number_of_objects 10
#define velocity_distance 2 //antal steg bakåt i bufferten
#define x_activity_scale_factor 1
#define y_activity_scale_factor 1
#define z_activity_scale_factor 1
#define error_threshold 5
#define max_x_velo 80
#define max_y_velo 80
#define max_z_velo 80
// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;
//Structs
typedef struct
{
	int x_cm;
	int y_cm;
	int z_cm;
	double micros;
}buffer_node;

typedef struct
{
	double x_velo;
	double y_velo;
	double z_velo;
	double activity;
}object;

struct timeval tim;

//Global variables
buffer_node* buffer;
object* object_list;
int current_node;

void
ObjectModule::Init()
{
    buffer = (buffer_node*) calloc(sizeof(buffer_node), buffer_size);
	object_list = (object*) calloc(sizeof(object), number_of_objects);
	current_node = 0;

    input_position_array = GetInputArray("FACE_POSITION");
    input_position_array_size = GetInputSize("FACE_POSITION");
    input_faces_matrix = GetInputMatrix("FACES");
    input_faces_matrix_size_x = GetInputSizeX("FACES");
    input_faces_matrix_size_y = GetInputSizeY("FACES");
    input_dist_matrix = GetInputMatrix("FACE_DISTANCE");
//    input_dist_matrix_size = GetInputSize("FACE_DISTANCE");

    output_status_array = GetOutputArray("OBJECT_STATUS");
    output_status_array_size = GetOutputSize("OBJECT_STATUS");
    output_status_array[2] = 300;

    error_counter = 0;

}



ObjectModule::~ObjectModule()
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
		object_list->x_velo= ((buffer[current_node].x_cm - buffer[buddy_node].x_cm)) / (buffer[current_node].micros - buffer[buddy_node].micros);
		object_list->y_velo= ((buffer[current_node].y_cm - buffer[buddy_node].y_cm)) / (buffer[current_node].micros - buffer[buddy_node].micros);
		object_list->z_velo= ((buffer[current_node].z_cm - buffer[buddy_node].z_cm)) / ((buffer[current_node].micros - buffer[buddy_node].micros));
	}
}

void calculateActivity(){
	object_list->activity= x_activity_scale_factor*abs((float)object_list->x_velo) + y_activity_scale_factor*abs((float)object_list->y_velo) + z_activity_scale_factor*abs((float)object_list->z_velo);
}

bool valid_velocity(){
    if(abs((float)object_list->x_velo)>max_x_velo){
       // fprintf(stderr,"x_velo=%lf\n", object_list->x_velo);
        return false;
    }
    if(abs((float)object_list->y_velo)>max_y_velo){
       //         fprintf(stderr,"y_velo=%lf\n", object_list->y_velo);
        return false;
    }
    if(abs((float)object_list->z_velo)>max_z_velo){
               // fprintf(stderr,"z_velo=%lf\n", object_list->z_velo);
        return false;
    }
    //fprintf(stderr,"true\n");
    return true;
}

void
ObjectModule::Tick()
{
    for(int j = 0; j <input_faces_matrix_size_y; j++){
        for(int i = 0; i<input_faces_matrix_size_x; i++){
            fprintf(stderr, "%lf ", input_faces_matrix[j][i]);
        }
        fprintf(stderr, "\n");
    }

	double x, y;
	int x_cm, y_cm, z_cm;

    x= input_position_array[0];
	y = input_position_array[1];
	z_cm = input_dist_matrix[(int)(480 * y)][(int)(640 * x)];

            //    fprintf(stderr, "x = %lf, y=%lf \n", x, y);

        float camera_y_deg = 43/2 * pi /180;
        float camera_x_deg = 57/2 * pi /180;
        int picture_width = (int)(2*z_cm * tan(camera_x_deg));
        int picture_height = (int)(2*z_cm * tan(camera_y_deg));


    x_cm = (int)(x * picture_width);
    y_cm = (int)(y * picture_height);

    if(z_cm != -36){
        gettimeofday(&tim, NULL);
        double t1=tim.tv_sec;
        double t2 =tim.tv_usec;
        t1 = t1 + t2/1000000;

        //  if(abs(buffer[current_node - 1].x_cm - x_cm)> ){
        buffer[current_node].x_cm = x_cm;
        buffer[current_node].y_cm = y_cm;
        buffer[current_node].z_cm = z_cm;
        buffer[current_node].micros = t1;

        calculateVelocity();
        calculateActivity();

        if(valid_velocity()){
            output_status_array[0] = x;
            output_status_array[1] = y;
            output_status_array[2] = z_cm;
            output_status_array[3] = object_list->activity;
            output_status_array[4] = object_list->z_velo;

            current_node++;

            if(current_node == buffer_size){
                current_node = 0;
            }
        }
    }
}

// Install the module. This code is executed during start-up.

static InitClass init("ObjectModule", &ObjectModule::Create, "Source/UserModules/ObjectModule/");

