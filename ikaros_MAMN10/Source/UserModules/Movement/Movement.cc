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
#include <sys/time.h>
//#include "line.h"

#define _USE_MATH_DEFINES
#define x_angle_index 0
#define y_angle_index 1
#define IDLE_MODE 1
#define INTEREST_MODE 2
#define GIVE_EGG_MODE 3
#define DEFENSE_MODE 4


struct timeval timestamp;


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
    defence_move_a = false;
    defence_move_b = false;
    defence_move_c = false;
    give_egg_timestamp = 0;
    idle_timestamp = 0;
    requested_time = 0;

    // This is were we get pointers to the inputs and outputs
    focus_array = GetInputArray("FOCUS");
    focus_array_size = GetInputSize("FOCUS");

    stress_array = GetInputArray("STRESS");

    // Do the same for the outputs
    output_array = GetOutputArray("OUTPUT");
    output_array_size = GetOutputSize("OUTPUT");

    output_speed_array = GetOutputArray("SPEED");
    output_speed_array_size = GetOutputSize("SPEED");

    stress_array[0]= IDLE_MODE;
    output_array[0] = 180;
    output_array[1] = 160;
    output_array[2] = 190;

    output_speed_array[0]= 0.3;
    output_speed_array[1]= 0.1;
    output_speed_array[2]= 0.1;

}



Movement::~Movement()
{
    // Destroy data structures that you allocated in Init.

    //destroy_matrix(internal_matrix);

    // Do NOT destroy data structures that you got from the
    // kernel with GetInputArray, GetInputMatrix etc.
}

float* calc_degrees(int distance, float x, float y, int height_offset, int dept_offset){
	float camera_y_deg = 43/2 * pi /180;
	float camera_x_deg = 57/2 * pi /180;
	int face_distance = distance;
	int picture_width, picture_height, face_x_cm, face_y_cm;
	float face_x, face_y;
	float angles[2];
	face_x = x;
	face_y = y;

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
        	face_x_cm = - picture_width/2 + (picture_width-face_x_cm) ;
    	}

   	if(face_y_cm <= picture_height/2){
        	face_y_cm = picture_height/2 -face_y_cm;
    	}else{
    		face_y_cm = - picture_height/2 + (picture_height-face_y_cm) ;
    	}

	//Räknar ut vinklen i x och y led mellan kinect kameran och ansiktet.
	angles[x_angle_index] = atan(((float)face_x_cm/(float) (face_distance-dept_offset))) * 180 / pi;
	angles[y_angle_index] = atan(((float)(face_y_cm+height_offset)/(float) (face_distance-dept_offset))) * 180 / pi;
	fprintf(stderr, "d=%d x=%lf y=%lf\n", face_distance, face_x, face_y);
	fprintf(stderr, "angle X=%g\n angle Y=%g\n", angles[x_angle_index], angles[y_angle_index]);

    return angles;
}


void
Movement::Tick()
{
    /*
     px/cm i SIDLED vid den aktuella distansen d är: (640 * 80)/(d * 87)
     px/cm i HÖJDLED vid den aktuella distansen d är: (480 * 80)/(d * 63)
     */

    gettimeofday(&timestamp, NULL);
    double t1=timestamp.tv_sec;
    double t2 =timestamp.tv_usec;
    t1 = t1 + t2/1000000;

    int mode =  (int)stress_array[0];

//    if(focus_array[3] == -1 && mode != GIVE_EGG_MODE){
//        mode = IDLE_MODE;
//    }
    if(mode == GIVE_EGG_MODE && !egg_back){
        fprintf(stderr, "No egg, back to intrest mode\n");
        mode = INTEREST_MODE;
    }

    switch(mode) {

        case IDLE_MODE:
            if(idle_timestamp == 0){
                idle_timestamp = t1;
            }

            fprintf(stderr, "IDLE_MODE:\n");

            // The chicken's moves to peck the ground
            if (t1 - idle_timestamp >= requested_time){
                if (up) {
                    if (rotated) {
                        requested_time = 1;
                        output_array[1] = 100;
                        output_array[2] = 150;
                        up = false;
                        idle_timestamp = 0;

                    }else{
                        requested_time = random(1, 2);
                        int angle = random(0, 100) + 100;
                        if (angle >= 136 && angle < 150) {
                            angle = 135;
                        }else if (angle >= 150 && angle < 165){
                            angle = 165;
                        }
                        output_array[0] = angle;
                        rotated = true;
                        idle_timestamp = 0;
                    }
                }else{
                    requested_time = 1;
                    output_array[1] = 160;
                    output_array[2] = 170;
                    up = true;
                    rotated = false;
                    idle_timestamp = 0;
                }
            }
            break;

        case GIVE_EGG_MODE:
            fprintf(stderr, "GIVE_EGG_MODE:\n");

            if(idle_timestamp == 0){
                idle_timestamp = t1;
            }
            if (t1 - idle_timestamp >= requested_time){

                if (initiated_move_a && egg_back) {

                    //The chickens's moves to push her egg towards the visitor
                    if (initiated_move_b && !initiated_move_c) {
                        requested_time = 1.5;
                        output_array[1] = 117;
                        initiated_move_c = true;
                        idle_timestamp = 0;

                    }else if(initiated_move_c){
                        requested_time = 2;
                        output_array[2] = 170;
                        initiated_move_a = false;
                        initiated_move_b = false;
                        initiated_move_c = false;
                        egg_back = false;
                        idle_timestamp = 0;

                    }else{
                        requested_time = 1.5;
                        output_array[2] = 90;
                        initiated_move_b = true;
                        idle_timestamp = 0;
                    }

                }else{
                    output_array[0] = 180;
                    output_array[1] = 160;
                    output_array[2] = 190;
                    initiated_move_a = true;
                }
            }
            break;

        case INTEREST_MODE:
            fprintf(stderr, "INTEREST_MODE: \n");

            if(idle_timestamp == 0){
                idle_timestamp = t1;
            }

            if (t1 - idle_timestamp >= requested_time){

                if(!up){
                    requested_time = 1;
                    output_array[1] = 160;
                    up = true;
                }

                // The chicken's moves to follow interesting objects
                calc_degrees((int)focus_array[2], focus_array[0],focus_array[1],15,15);
                output_array[0] = 230 - 100 * focus_array[0];
                output_array[1] = 160;
                output_array[2] = 260 - 80 * focus_array[1];
                up = true;
            }
            break;

        case DEFENSE_MODE:

            fprintf(stderr, "DEFENSE_MODE: \n");

            if(idle_timestamp == 0){
                idle_timestamp = t1;
            }
            if (t1 - idle_timestamp >= requested_time){

                if (!egg_back){
                    // The chicken's moves to protect her egg
                    if (defence_move_a && !defence_move_b) {
                        requested_time = 1.5;
                        output_array[1] = 95;
                        defence_move_b = true;
                        idle_timestamp = 0;

                    }else if(defence_move_b){
                        requested_time = 1.5;
                        output_array[1] = 100;
                        output_array[2] = 148;
                        defence_move_a = false;
                        defence_move_b = false;
                        defence_move_c = true;
                        egg_back = true;
                        idle_timestamp = 0;

                    }else{
                        requested_time = 1.5;
                        output_array[0] = 180;
                        output_array[1] = 120; //ändrat från 140  //ev ändra till 65 & skippa nästa steg (kolla ur detta funkar från idle_mode)
                        output_array[2] = 200;
                        defence_move_a = true;
                        idle_timestamp = 0;
                    }

                }else if(defence_move_c){
                    requested_time = 0.8;
                    output_array[2] = 180;
                    idle_timestamp = 0;
                    defence_move_c = false;

                }else{
                    // The chicken's moves to follow interesting objects in a crunched position
                    output_array[0] = 230 - 100 * focus_array[0];
                    output_array[1] = 90;
                    output_array[2] = 300 - 10 * focus_array[1];
                    up = true;
                }
            }
            break;
        default:
            break;
    }
}


// Install the module. This code is executed during start-up.

static InitClass init("Movement", &Movement::Create, "Source/UserModules/Movement/");

