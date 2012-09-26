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

#include "MyModule.h"

// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;


void
MyModule::Init()
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
    
    //int_parameter = GetIntValue("parameter2");
    
    // This is were we get pointers to the inputs and outputs

    // Get a pointer to the input INPUT1 and its size which we set
    // to 10 above
    // It does not matter whether a matrix of array is connected
    // to the inputs. We will teat it an array in this module
    // anyway.

    //input_array = GetInputArray("INPUT1");
    //input_array_size = GetInputSize("INPUT1");

    // Get pointer to a matrix and treat it as a matrix. If an array is
    // connected to this input, the size_y will be 1.

    /*input_matrix = GetInputMatrix("INPUT");
    input_matrix_size_x = GetInputSizeX("INPUT");
    input_matrix_size_y = GetInputSizeY("INPUT");
*/
   // input_matrix_pos = GetInputMatrix("INPUT_POS");
    input_matrix_depth = GetInputMatrix("INPUT_DEPTH");
    input_matrix_depth_size_x = GetInputSizeX("INPUT_DEPTH");
    input_matrix_depth_size_y = GetInputSizeY("INPUT_DEPTH");
    
    printf("x: %d \n", input_matrix_depth_size_x);
    printf("Y: %d \n", input_matrix_depth_size_y);


    //input_matrix_pos_size_x = GetInputSizeX("INPUT_POS");
    //input_matrix_pos_size_y = GetInputSizeY("INPUT_POS");
    // Do the same for the outputs

    //output_array = GetOutputArray("OUTPUT");
    //output_array_size = GetOutputSize("OUTPUT");

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



MyModule::~MyModule()
{
    // Destroy data structures that you allocated in Init.

    //destroy_array(internal_array);
    //destroy_matrix(internal_matrix);

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}



void
MyModule::Tick()
{
    // This is where you implement your algorithm
    // to calculate the outputs from the inputs

    // Make a copy of the data on INPUT2 which is now
    // in input_matrix to internal_matrix
    // Arrays can be copied with copy_array
    // To clear an array or matrix use reset_array and reset_matrix

    //copy_matrix(internal_matrix, input_matrix, input_matrix_size_x, input_matrix_size_y);

    // Calculate the output by iterating over the elements of the
    // output matrix. Note the order of the indices into the matrix.
    //
    // In most cases it is much faster to put the for-loops with
    // the row index (j) in the outer loop, because it will lead
    // to more efficient cache use.
    /*
    for (int j=0; j<input_matrix_size_y; j++){
        if (input_matrix[j][2] == 1781 ) {
                output_array[0] = 100;
                output_array[1] = 150;
                output_array[2] = 150;
           }
        else if (input_matrix[j][2] == 1782 ) {
            output_array[0] = 200;
            output_array[1] = 150;
            output_array[2] = 150;
        }
        else if (input_matrix[j][2] == 1785 ) {
            output_array[0] = 150;
            output_array[1] = 150;
            output_array[2] = 200;
        }
        else if (input_matrix[j][2] == 1783 ) {
            output_array[0] = 150;
            output_array[1] = 150;
            output_array[2] = 100;
        }
        else if (input_matrix[j][2] == 1780 ) {
            output_array[0] = 150;
            output_array[1] = 240;
            output_array[2] = 240;
        }
     */
    
    
    
    
    //for (int j=0; j<input_matrix_pos_size_y; j++){

        //printf("/n");
            
    float max = INT_MIN;
    float min = INT_MAX;
    int yMin = 0;
    int xMin =0;
    int yMax = 0;
    int xMax = 0;
    int i = 0;
    int j = 0;
    
    for (i= 0; i < input_matrix_depth_size_y; i ++) {
        for (j = 0; j < input_matrix_depth_size_x; j ++) {
          
            if (input_matrix_depth[i][j] > max) {
                max = input_matrix_depth[i][j];
                yMax = i;
                xMax = j;

            }
            if (input_matrix_depth[i][j] < min){
                min = input_matrix_depth[i][j];
                yMin = i;
                xMin = j;
            }
        }
    }
    //fprintf(stderr,"Max = %f, yMax = %d, xMax = %d \n", max, yMax, xMax);

    fprintf(stderr,"Min = %f, yMin = %d, xMin = %d\n", min, yMin, xMin);

    

    
            //fprintf(stderr,"%f ", input_matrix_pos [0][4]);
    /*if (input_matrix_pos[0][0] != -1) {
        
    output_array[0] = 170 - 40 * input_matrix_pos[0][0];
    output_array[2] = 170 - 30 * input_matrix_pos[0][1];
    output_array[1] = 150;
        
        
    }
     */
    
    /*
    if (input_matrix[0][2] == 1782) {
        output_array[0] = 150;
        output_array[1] = 150;
        output_array[2] = 120;
    }else if (input_matrix[0][2] == 1781) {
        output_array[0] = 150;
        output_array[1] = 150;
        output_array[2] = 170;
    }else if (input_matrix[0][2] == 1783) {
        output_array[0] = 150;
        output_array[1] = 150;
        output_array[2] = 150;
    }else if (input_matrix[0][2] == 1785) {
        output_array[0] = 150;
        output_array[1] = 150;
        output_array[2] = 150;
    }else if (input_matrix[0][2] == 1784) {
        output_array[0] = 150;
        output_array[1] = 150;
        output_array[2] = 150;
    }
    */
        
    //for (int j=0; j<output_matrix_size_y; j++)
     //for (int i=0; i<output_matrix_size_x; i++)
     //       output_matrix[j][i] = 0.5;

    // Fill the output array with random values

    //random(output_array, 0.0, 1.0, output_array_size);

}

