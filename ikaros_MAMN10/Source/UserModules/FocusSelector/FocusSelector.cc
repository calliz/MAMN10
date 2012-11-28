//
//	FocusSelector.cc		This file is a part of the IKAROS project
//                      <Short description of the module>
//
//    Copyright (C) 2011 <Olle Klang>
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
//	Created: <2010-10-05>
//

#include "FocusSelector.h"
#include "../Kernel/IKAROS_Math.h"



// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;


void
FocusSelector::Init()
{
    input_objects_array = GetInputArray("OBJECT_STATUS");
    input_objects_array_size = GetInputSize("OBJECT_STATUS");

//    input_stress_array = GetInputArray("STRESS");
//    input_stress_array_size = GetInputSize("STRESS");

    output_focus_array = GetOutputArray("FOCUS");
    output_focus_array_size = GetOutputSize("FOCUS");

    input_objects_matrix = GetInputMatrix("OBJECTS");
    output_focus_array[0] = -1;
    output_focus_array[1] = -1;
    output_focus_array[2] = -1;

    idle_count = 0;
}


FocusSelector::~FocusSelector()
{
    // Destroy data structures that you allocated in Init.

    //destroy_array(internal_array);
    //destroy_matrix(internal_matrix);

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}


void
FocusSelector::Tick()
{
int index_of_max_activity = 0;
double max_activity = 0;
for(int y= 0;y<10;y++){
    if(input_objects_matrix[y][3]>max_activity){
        index_of_max_activity = y;
        max_activity = input_objects_matrix[y][3];
    }
}

    if(input_objects_array[4] > 0){
        output_focus_array[0] = input_objects_matrix[index_of_max_activity][0];
        output_focus_array[1] = input_objects_matrix[index_of_max_activity][1];
        output_focus_array[2] = input_objects_matrix[index_of_max_activity][2];
        output_focus_array[3] = 0;

    }

//    for(int y = 0; y<10;y++){
//            fprintf(stderr, "%lf, %lf, %lf, %lf, %lf \n", input_objects_matrix[y][0], input_objects_matrix[y][1], input_objects_matrix[y][2], input_objects_matrix[y][3], input_objects_matrix[y][4]);
//    }

    output_focus_array[0] = input_objects_matrix[index_of_max_activity][0];
    output_focus_array[1] = input_objects_matrix[index_of_max_activity][1];
    output_focus_array[2] = input_objects_matrix[index_of_max_activity][2];

    fprintf(stderr, "%lf, %lf, %lf, %lf\n", output_focus_array[0], output_focus_array[1], output_focus_array[2], output_focus_array[3]);


}

// Install the module. This code is executed during start-up.

static InitClass init("FocusSelector", &FocusSelector::Create, "Source/UserModules/FocusSelector/");


