
#include "FaceModule.h"

// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;


void
FaceModule::Init()
{

    input_array = GetInputArray("FACE_POSITION");
    input_array_size = GetInputSize("FACE_POSITION");
    
    input_dist_array = GetInputArray("FACE_DISTANCE");
    input_dist_array_size = GetInputSize("FACE_DISTANCE");
    

    // Get pointer to a matrix and treat it as a matrix. If an array is
    // connected to this input, the size_y will be 1.

    //input_matrix = GetInputMatrix("INPUT2");
    //input_matrix_size_x = GetInputSizeX("INPUT2");
    //input_matrix_size_y = GetInputSizeY("INPUT2");

    // Do the same for the outputs

    output_array = GetOutputArray("OUTPUT");
    output_array_size = GetOutputSize("OUTPUT");

    //output_matrix = GetOutputMatrix("OUTPUT2");
    //output_matrix_size_x = GetOutputSizeX("OUTPUT2");
    //output_matrix_size_y = GetOutputSizeY("OUTPUT2");

    // Allocate some data structures to use internaly
    // in the module

    // Create an array with ten elements
    // To access the array use internal_array[i].

    //internal_array = create_array(10);

    // Create a matrix with the same size as INPUT2
    // IMPORTANT: For the matrix the sizes are given as X, Y
    // which is the OPPOSITE of ROW, COLUMN.

    //internal_matrix = create_matrix(input_matrix_size_x, input_matrix_size_y);

    // To acces the matrix use internal_matrix[y][x].
    //
    // It is also possible to use the new operator to
    // create arrays, but create_array and create_matix
    // should be used to make sure that memeory is
    // allocated in a way that is suitable for the math
    // library and fast copying operations.
}



FaceModule::~FaceModule()
{
    // Destroy data structures that you allocated in Init.

    //destroy_array(internal_array);
    //destroy_matrix(internal_matrix);

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}



void
FaceModule::Tick()
{
    /*
	float camera_y_deg = 43 * pi /180;
	float camera_x_deg = 57 * pi /180;
	int picture_width, picture_height, face_distance, face_x, face_y, face_x_cm, face_y_cm;
	float x_angle, y_angle;
	face_distance = input_dist_array[0];
	face_x = input_array[1];
	face_y = input_array[0];

	//Bildens verkliga bredd utifrån ansiktets avstånd från kameran.
	picture_width = (int)(2*face_distance * tan(camera_x_deg));
	picture_height = (int)(2*face_distance * tan(camera_y_deg));
	
	//Ansiktets avstånd från bildens vänstra övre hörn.	
	face_x_cm = face_x * picture_width;
	face_y_cm = face_y * picture_height;

	//Korrigera kring center.
    
    if(face_x_cm <= picture_width/2){
        face_x_cm = picture_width/2 -face_x_cm;
    }else{
        face_x_cm = - picture_width/2 + (face_x_cm-picture_width) ;
    }
    
    if(face_y_cm <= picture_height/2){
        face_y_cm = picture_height/2 -face_y_cm;
    }else{
        face_y_cm = - picture_height/2 + (face_y_cm - picture_height) ;
    }

	//Räknar ut vinklen i x och y led mellan kinect kameran och ansiktet.
	x_angle = atan(((float)face_x_cm/(float) face_distance)) * 180 / pi;
	y_angle = atan(((float)face_y_cm/(float) face_distance)) * 180 / pi;


    //random(output_array, 0.0, 1.0, output_array_size);
	
	
	//printf("X=%lf Y=%lf", input_array[0], input_array[1]);

    fprintf(stderr,"X angle=%g Y angle=%g\n ", x_angle, y_angle);
     */

    if(input_dist_array[0] < 60){
       
        output_array[0] = 200 - 100 * input_array[0];
        output_array[1] = 60;
        output_array[2] = 280 - 10 * input_array[1];
    
    
    }else{
        output_array[0] = 200 - 100 * input_array[0];
    	output_array[1] = 150;
        output_array[2] = 210 - 80 * input_array[1];
    }
    
    /*
    px/cm i SIDLED vid den aktuella distansen d är: (640 * 80)/(d * 87)
    px/cm i HÖJDLED vid den aktuella distansen d är: (480 * 80)/(d * 63)
     */
    
    /*
    
    output_array[0] = 200 - 100 * input_array[0];
    output_array[1] = 150;
    output_array[2] = 210 - 80 * input_array[1];
    */    
}


