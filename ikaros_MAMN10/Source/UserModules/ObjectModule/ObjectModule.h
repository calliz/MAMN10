//
//	MyModule.h		This file is a part of the IKAROS project
//                  <Short description of the module>
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
//	<Additional description of the module>

#ifndef ObjectModule_
#define ObjectModule_

#include "IKAROS.h"

class ObjectModule: public Module
{
public:
    static Module * Create(Parameter * p) { return new ObjectModule(p); }

    ObjectModule(Parameter * p) : Module(p) {}
    virtual ~ObjectModule();

    void 		Init();
    void 		Tick();

    // pointers to inputs and outputs
    // and integers to represent their sizes

    float *     input_position_array;
    int         input_position_array_size;
    
    float **    input_dist_matrix;
    int         input_dist_matrix_size_x;
    int         input_dist_matrix_size_y;

    float *     output_status_array;
    int         output_status_array_size;


    // internal data storage


    // parameter values

};

#endif
