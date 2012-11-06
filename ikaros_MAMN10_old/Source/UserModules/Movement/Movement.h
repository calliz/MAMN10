//
//	Movement.h		This file is a part of the IKAROS project
//                  <Short description of the module>
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
//
//	<Additional description of the module>

#ifndef Movement_
#define Movement_

#include "IKAROS.h"

class Movement: public Module
{
public:
    static Module * Create(Parameter * p) { return new Movement(p); }

    Movement(Parameter * p) : Module(p) {}
    virtual ~Movement();

    void 		Init();
    void 		Tick();

    // pointers to inputs and outputs
    // and integers to represent their sizes

    float *     stress_array;
    int         stress_array_size;
    
    float *     focus_array;
    int         focus_array_size;
    
    float *     output_array;
    int         output_array_size;
    
    float *     output_speed_array;
    int         output_speed_array_size;

    // internal data storage

    float **    internal_matrix;

    // parameter values
    
    bool        up;
    bool        rotated;
    bool        egg_back;
    bool        initiated_move_a;
    bool        initiated_move_b;
    bool        initiated_move_c;
    bool        defence_move_a;
    bool        defence_move_b;
    bool        defence_move_c;
    float       float_parameter;
    double      idle_timestamp;
    double      give_egg_timestamp;
    double      requested_time;
};

#endif
