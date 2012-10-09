//
//	FocusSelector.h		This file is a part of the IKAROS project
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
//	Created: <2010-10-09>
//
//	<Additional description of the module>

#ifndef FocusSelector_
#define FocusSelector_

#include "IKAROS.h"

class FocusSelector: public Module
{
public:
    static Module * Create(Parameter * p) { return new FocusSelector(p); }

    FocusSelector(Parameter * p) : Module(p) {}
    virtual ~FocusSelector();

    void 		Init();
    void 		Tick();

    // pointers to inputs and outputs
    // and integers to represent their sizes

    float *     input_objects_array;
    int         input_objects_array_size;
    
    float *     input_stress_array;
    int         input_stress_array_size;

    float *     output_focus_array;
    int         output_focus_array_size;

    // internal data storage

    float *     internal_array;
    float **    internal_matrix;

    // parameter values

};

#endif
