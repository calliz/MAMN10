//
//	ModeSelector.cc		This file is a part of the IKAROS project
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
//	Created: <2010-10-09>
//

#include "ModeSelector.h"
#include "IKAROS_math.h"

#define IDLE_MODE 1
#define INTEREST_MODE 2
#define GIVE_EGG_MODE 3
#define DEFENSE_MODE 4

// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;


void
ModeSelector::Init()
{
    input_array = GetInputArray("INPUT");
    input_array_size = GetInputSize("INPUT");

    output_array = GetOutputArray("STRESS");
    output_array_size = GetOutputSize("STRESS");
    
    new_mode_array = GetOutputArray("STRESS");
    new_mode_array_size = GetOutputSize("STRESS");
    
    idle_count = 0;
    interest_count = 0;
    give_egg_count = 0;
    limit = 10;
    
    
}


ModeSelector::~ModeSelector()
{
    // Destroy data structures that you allocated in Init.

    //destroy_array(internal_array);
    //destroy_matrix(internal_matrix);

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}


void
ModeSelector::Tick()
{
    
//    if (input_array[2] < 100 && input_array[2] > 0) {
//        output_array[0] = GIVE_EGG_MODE;
//    }
//    else if (input_array[2] < 250){
//        output_array[0] = INTEREST_MODE;
//    }else{
//        output_array[0] = IDLE_MODE;
//    }
    
    copy_array( output_array, new_mode_array, output_array_size);


    if (input_array[2] < 100 ) { //&& input_array[2] > 0
        new_mode_array[0] = GIVE_EGG_MODE;
    }
    else if (input_array[2] < 250){
        new_mode_array[0] = INTEREST_MODE;
    }else{
        new_mode_array[0] = IDLE_MODE;
    }
    
    if (new_mode_array[0] == IDLE_MODE) {
        idle_count = idle_count + 1;
        if (idle_count >= limit) {
            output_array[0] = IDLE_MODE;
            idle_count = 0;
            interest_count = 0;
            give_egg_count = 0;
            limit = 10;
        }
    }else if(new_mode_array[0] == GIVE_EGG_MODE) {
        give_egg_count = give_egg_count + 1;
        if (give_egg_count >= limit) {
            output_array[0] = GIVE_EGG_MODE;
            idle_count = 0;
            interest_count = 0;
            give_egg_count = 0;
            limit = 100;
        }
    }else if(new_mode_array[0] == INTEREST_MODE) {
        interest_count = interest_count + 1;
        if (interest_count >= limit) {
            output_array[0] = INTEREST_MODE;
            idle_count = 0;
            interest_count = 0;
            give_egg_count = 0;
            limit = 10;
        }
    }else if(new_mode_array[0] == DEFENSE_MODE) {
        output_array[0] = DEFENSE_MODE;
        idle_count = 0;
        interest_count = 0;
        give_egg_count = 0;
        limit = 100;
        
    }else{
        output_array[0] = new_mode_array[0];
    }

    
    copy_array(new_mode_array, output_array, output_array_size);

    
    
    
//     output_array[0] = IDLE_MODE;
    
    
}
