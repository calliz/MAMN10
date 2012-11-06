#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _USE_MATH_DEFINES
#define camera_height_offset 15
#define camera_dept_offset 15

void calc_degrees(int distance, double x, double y){
	float camera_y_deg = 43/2 * M_PI /180;
	float camera_x_deg = 57/2 * M_PI /180;
	int face_distance = distance;
	int picture_width, picture_height, face_x_cm, face_y_cm;
	double face_x, face_y;
	float x_angle, y_angle;
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
	x_angle = atan(((float)face_x_cm/(float) (face_distance-camera_dept_offset))) * 180 / M_PI;
	y_angle = atan(((float)(face_y_cm+camera_height_offset)/(float) (face_distance-camera_dept_offset))) * 180 / M_PI;
	
	fprintf(stdout, "d=%d x=%lf y=%lf\n", face_distance, face_x, face_y);
	fprintf(stdout, "angle X=%g\n angle Y=%g\n", x_angle, y_angle);
}

int main(void)
{
	calc_degrees(100, 0.5, 0.5);
	calc_degrees(100, 0,0);
	calc_degrees(100,1,1);
	calc_degrees(100,0,1);
	calc_degrees(100,1,0);
	calc_degrees(100, 0.75,0);
}
