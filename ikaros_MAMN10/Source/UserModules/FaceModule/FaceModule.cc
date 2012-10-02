//
//	MyModule.cc		This file is a part of the IKAROS project
//                      <Short description of the module>
//
//    Copyright (C) 2011 <Author Name>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    See http://www.ikaros-project.org/ for more information.
//
//	Created: <date>
//
//  This example is intended as a starting point for writing new Ikaros modules
//  The example includes most of the calls that you may want to use in a module.
//  If you prefer to start with a clean example, use he module MinimalModule instead.
//
//  MyModule is a new version of MyModule that uses the IKC file rather than 
//  function calls during initialization.

#include "FaceModule.h"

// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;


void
FaceModule::Init()
{
    // You can get parameters from the IKC file here or in any other
    // member functions. Since the method for reading the values
    // from the XML structure is not very efficient, it is best to store
    // the parameters in member variables as below.
    // GetFloatValue gets a parameter as a float with a particular
    // name. If the parameter is not found, the default value 7.0
    // is returned instead.
    
    //float_parameter = GetFloatValue("parameter1");
    
    // GetIntValue gets parameter2 from the XML structure or
    // sets the parameter to the default value 2 if it is not found
    
  
    // This is were we get pointers to the inputs and outputs

    // Get a pointer to the input INPUT1 and its size which we set
    // to 10 above
    // It does not matter whether a matrix of array is connected
    // to the inputs. We will teat it an array in this module
    // anyway.

    input_array = GetInputArray("FACE_POSITION");
    input_array_size = GetInputSize("FACE_POSITION");

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
	float camera_y_deg = 43;
	float camera_x_deg = 57;
	int picture_width, picture_height, face_distance, face_x, face_y, face_x_cm, face_y_cm;
	float x_angle, y_angle;
	face_distance = input_array[2];
	face_x = input_array[0];
	face_y = input_array[1];

	//Bildens verkliga bredd utifrån ansiktets avstånd från kameran.
	picture_width = (int) 2*face_distance * tan(camera_x_deg);
	picture_height = (int) 2*face_distance * tan(camera_y_deg);
	
	//Ansiktets avstånd från bildens vänstra övre hörn.	
	face_x_cm = face_x * picture_width;
	face_y_cm = face_y * picture_height;

	//Korrigera kring center.
	face_x_cm = face_x_cm -picture_width/2;
	face_y_cm = face_y_cm - picture_height/2;
	
	//Räknar ut vinklen i x och y led mellan kinect kameran och ansiktet.
	x_angle = atan(((float)face_x_cm/(float) face_distance));
	y_angle = atan(((float)face_y_cm/(float) face_distance));


	printf("X angle=%g Y angle=%g", x_angle, y_angle);
	
	output_array[0] = 150 + x_angle;
    	output_array[1] = 150;
	output_array[2] = 150 + y_angle;
    
        
	
}


