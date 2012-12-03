
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
    double x;
    double y;
	double x_velo;
	double y_velo;
	double z_velo;
	double activity;
}object;

struct timeval tim;

//Global variables
buffer_node** buffer;
object* object_list;
int* current_node;
int* error_counter;
void
ObjectModule::Init()
{
    buffer = new buffer_node*[number_of_objects];

    for(int i = 0; i< number_of_objects;i++){
        buffer[i] = new buffer_node[buffer_size];
    }

 //   (buffer_node**) calloc(sizeof(buffer_node), buffer_size*number_of_objects);
	object_list = (object*) calloc(sizeof(object), number_of_objects);
	current_node = (int*) calloc(sizeof(int), number_of_objects);
    error_counter = (int*) calloc(sizeof(int), number_of_objects);

    input_position_array = GetInputArray("FACE_POSITION");
    input_position_array_size = GetInputSize("FACE_POSITION");
    input_faces_matrix = GetInputMatrix("FACES");
    input_faces_matrix_size_x = GetInputSizeX("FACES");
    input_faces_matrix_size_y = GetInputSizeY("FACES");
    input_dist_matrix = GetInputMatrix("FACE_DISTANCE");

    output_object_matrix = GetOutputMatrix("OBJECTS");

    output_status_array = GetOutputArray("OBJECT_STATUS");
    output_status_array_size = GetOutputSize("OBJECT_STATUS");
    output_status_array[2] = 300;

}



ObjectModule::~ObjectModule()
{
    // Destroy data structures that you allocated in Init.

    //destroy_array(internal_array);
    //destroy_matrix(internal_matrix);


    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

void calculateVelocity(int object_index){
	int buddy_node = (current_node[object_index] - velocity_distance) % buffer_size;
	if(buddy_node<0){
		buddy_node = buddy_node+buffer_size;
	}
	float buddy_micros;
	if((buddy_micros = buffer[object_index][buddy_node].micros)!= 0){
		object_list[object_index].x_velo= ((buffer[object_index][current_node[object_index]].x_cm - buffer[object_index][buddy_node].x_cm)) / (buffer[object_index][current_node[object_index]].micros - buffer[object_index][buddy_node].micros);
		object_list[object_index].y_velo= ((buffer[object_index][current_node[object_index]].y_cm - buffer[object_index][buddy_node].y_cm)) / (buffer[object_index][current_node[object_index]].micros - buffer[object_index][buddy_node].micros);
		object_list[object_index].z_velo= ((buffer[object_index][current_node[object_index]].z_cm - buffer[object_index][buddy_node].z_cm)) / ((buffer[object_index][current_node[object_index]].micros - buffer[object_index][buddy_node].micros));
	}
}


void calculateActivity(int object_index){
	object_list[object_index].activity= x_activity_scale_factor*abs((float)object_list[object_index].x_velo) + y_activity_scale_factor*abs((float)object_list[object_index].y_velo) + z_activity_scale_factor*abs((float)object_list[object_index].z_velo);
}

bool valid_velocity(int object_index){
    if(abs((float)object_list[object_index].x_velo)>max_x_velo){
        return false;
    }
    if(abs((float)object_list[object_index].y_velo)>max_y_velo){
        return false;
    }
    if(abs((float)object_list[object_index].z_velo)>max_z_velo){
        return false;
    }
    return true;
}

void
ObjectModule::Tick()
{
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec;
    double t2 =tim.tv_usec;
    t1 = t1 + t2/1000000;
//    for(int i = 0; i<10;i++){
//        fprintf(stderr, "%lf, %lf, %lf, %lf\n",input_faces_matrix[i][0],input_faces_matrix[i][1],input_faces_matrix[i][2],input_faces_matrix[i][3]);
//    }
    for(int object_index = 0; object_index <10; object_index++){
	double x, y;
	int x_cm, y_cm, z_cm;

    x= input_faces_matrix[object_index][0];
	y = input_faces_matrix[object_index][1];
	if(y<0 || y>1 || x<0 || x>1){
        z_cm=-36;

	}else{
	z_cm = input_dist_matrix[(int)(480 * y)][(int)(640 * x)];


        float camera_y_deg = 43/2 * pi /180;
        float camera_x_deg = 57/2 * pi /180;
        int picture_width = (int)(2*z_cm * tan(camera_x_deg));
        int picture_height = (int)(2*z_cm * tan(camera_y_deg));


    x_cm = (int)(x * picture_width);
    y_cm = (int)(y * picture_height);
	}

    if(z_cm != -36){
        buffer[object_index][current_node[object_index]].x_cm = x_cm;
        buffer[object_index][current_node[object_index]].y_cm = y_cm;
        buffer[object_index][current_node[object_index]].z_cm = z_cm;
        buffer[object_index][current_node[object_index]].micros = t1;

        calculateVelocity(object_index);
        calculateActivity(object_index);

        object_list[object_index].x=x;
        object_list[object_index].y=y;
        error_counter[object_index]=0;
    }else{

//        if(error_counter[object_index]>3){
//            buffer[object_index][current_node[object_index]].x_cm = 0;
//            buffer[object_index][current_node[object_index]].y_cm = 0;
//            buffer[object_index][current_node[object_index]].z_cm = 0;
//            buffer[object_index][current_node[object_index]].micros = 0;
//            calculateVelocity(object_index);
//            calculateActivity(object_index);
//
//            object_list[object_index].x=-1;
//            object_list[object_index].y=-1;
//            error_counter[object_index]=0;
//        }else{
//            current_node[object_index]--;
//            if(current_node[object_index]<0){
//                current_node[object_index]=current_node[object_index]+buffer_size;
//            }
//        }
//        error_counter[object_index]++;
    }
//        if(object_list[object_index].activity == 0){
//            error_counter[object_index]++;
//        }

        if(valid_velocity(object_index)){
            output_object_matrix[object_index][0] = object_list[object_index].x;
            output_object_matrix[object_index][1] = object_list[object_index].y;
            output_object_matrix[object_index][2] = z_cm;
            output_object_matrix[object_index][3] = object_list[object_index].activity;
            output_object_matrix[object_index][4] = object_list[object_index].z_velo;

            current_node[object_index]++;

            if(current_node[object_index] == buffer_size){
                current_node[object_index] = 0;
            }
         }
    }
//        if(t1-buffer[object_index][current_node[object_index]].micros>2000000){
//                buffer[object_index][current_node[object_index]].micros = 0;
//                buffer[object_index][current_node[object_index]].x_cm = 0;
//                buffer[object_index][current_node[object_index]].y_cm = 0;
//                buffer[object_index][current_node[object_index]].z_cm = 0;
//
//        }

//    for(int y = 0; y<10;y++){
//
//    }

}

// Install the module. This code is executed during start-up.

static InitClass init("ObjectModule", &ObjectModule::Create, "Source/UserModules/ObjectModule/");

