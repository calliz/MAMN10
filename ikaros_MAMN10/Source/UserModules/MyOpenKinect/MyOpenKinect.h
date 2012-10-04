//
//	OpenKinect     This file is a part of the IKAROS project
//                 A module to grab images from Kinect
//
//    Copyright (C) 2012  Christian Balkenius
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

#ifndef OPENKINECT
#define OPENKINECT

#include "IKAROS.h"

#ifdef OPENKINECT

class OpenKinect: public Module {
public:
    int				size_x;
    int				size_y;

    float **        intensity;
    float **		depth;
    float **		red;
    float **		green;
    float **		blue;

    static Module * Create(Parameter * p) {return new OpenKinect(p);};
    
    OpenKinect(Parameter * p) : Module(p) {};
    virtual ~OpenKinect() {};

    void    Init();
    void    Tick();
};

#endif
#ifndef OPENKINECT

class InputVideoQT {
public:
    static Module * Create(Parameter * p)
    {
        printf("This version was not compiled with OpenKinect\n.");
        return NULL;
    }
};

#endif

#endif


