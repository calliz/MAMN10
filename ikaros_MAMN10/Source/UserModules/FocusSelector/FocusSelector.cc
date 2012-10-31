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
#include "IKAROS_math.h"



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
    if(input_objects_array[4] > 0){
        output_focus_array[0] = input_objects_array[0];
        output_focus_array[1] = input_objects_array[1];
        output_focus_array[2] = input_objects_array[2];
        output_focus_array[3] = 0;

    }
    
//    else{
//        idle_count = idle_count + 1;
//        if (idle_count >= 10) {
//            output_focus_array[3] = -1;
//            idle_count = 0;
//        }
//    }
    
    output_focus_array[0] = input_objects_array[0];
    output_focus_array[1] = input_objects_array[1];
    output_focus_array[2] = input_objects_array[2];
    
}

