//
//	Movement.cc		This file is a part of the IKAROS project
//                      <This module handels the movements of the chicken>
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
//	Created: <2010-10-06>

#include "Movement.h"

// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;


void
Movement::Init()
{
    //float_parameter = GetFloatValue("parameter1");
    
    // GetIntValue gets parameter2 from the XML structure or
    // sets the parameter to the default value 2 if it is not found
    
    up = true;
    rotated = false;
    egg_back = true;
    initiated_move_a = false;
    initiated_move_b = false;
    initiated_move_c = false;

  
    // This is were we get pointers to the inputs and outputs
    focus_array = GetInputArray("FOCUS");
    focus_array_size = GetInputSize("FOCUS");
    
    stress_array = GetInputArray("STRESS");

    // Do the same for the outputs
    output_array = GetOutputArray("OUTPUT");
    output_array_size = GetOutputSize("OUTPUT");
    last_position_array = create_array(focus_array_size);
    //copy_array(last_position_array, output_array, focus_array_size);
    last_position_array[0]= 150;
    last_position_array[1]= 130;
    last_position_array[2]= 140;

}



Movement::~Movement()
{
    // Destroy data structures that you allocated in Init.

    destroy_array(last_position_array);
    //destroy_matrix(internal_matrix);

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}



void
Movement::Tick()
{
    /*
     px/cm i SIDLED vid den aktuella distansen d är: (640 * 80)/(d * 87)
     px/cm i HÖJDLED vid den aktuella distansen d är: (480 * 80)/(d * 63)
     */
    
    copy_array(output_array, last_position_array, focus_array_size);

    
//    switch ((int)(stress_array[0])) {
    switch (2) {

        case 1:
            // The chicken's moves to peck the ground
            if (up) {
                if (rotated) {
                    output_array[1] = 70;
                    sleep(1);
                    output_array[2] = 120;
                    up = false;
                }else{
                    sleep((int)(random(1, 2)));
                    int angle = random(0, 100) + 100;
                    
                    if (angle >= 136 && angle < 150) {
                        angle = 135;
                    }else if (angle >= 150 && angle < 165){
                        angle = 165;
                    }
                    output_array[0] = angle;
                    rotated = true;
                }
                
            }else{
                output_array[1] = 130;
                output_array[2] = 140;
                up = true;
                rotated = false;
            }
            sleep((int)(random(1, 2)));
            break;
        
        case 2:
                        
            if ((focus_array[2] < 75 || initiated_move_a) && egg_back) {
                
                //The chickens's moves to push her egg towards the visitor
                if (initiated_move_a) {
                    if (initiated_move_b && !initiated_move_c) {
                        sleep(1);
                        output_array[1] = 86;
                        initiated_move_c = true;
                        
                    }else if(initiated_move_c){
                        sleep(1);
                        output_array[2] = 140;
                        initiated_move_a = false;
                        initiated_move_b = false;
                        egg_back = false;
                    
                    }else{
                        sleep(1);
                        output_array[2] = 60;
                        initiated_move_b = true;
                    }
                    
                }else{
                    output_array[0] = 150;
                    output_array[1] = 130;
                    output_array[2] = 160;
                    initiated_move_a = true;
                }
                
            }else{
                if (initiated_move_c) {
                    sleep(1);
                    initiated_move_c = false;
                }
                
                // The chicken's moves to follow interesting objects
                output_array[0] = 200 - 100 * random(1, 640)/640; // ändra detta till: focus_array[0]/640
                output_array[1] = 130;
                output_array[2] = 230 - 80 * focus_array[1]/480;
                up = true;
            }
            break;
        
        case 3:
            
                        
            if (!egg_back){
                // The chicken's moves to protect her egg
                
                if (last_position_array[0] == 150) {
                    output_array[1] = 130;
                    output_array[2] = 140;
                    egg_back = true;
                    
                }else{
                    output_array[0] = 150;
                    output_array[1] = 110;
                    output_array[2] = 60;
                }
                
            }else{
                
                // The chicken's moves to follow interesting objects
                output_array[0] = 200 - 100 * random(1, 640)/640; // ändra detta till: focus_array[0]/640
                output_array[1] = 60;
                output_array[2] = 280 - 10 * focus_array[1]/480;
                up = true;
            }
            break;
            
        default:
            break;
    }
    
    copy_array(last_position_array, output_array, focus_array_size);
    
}


