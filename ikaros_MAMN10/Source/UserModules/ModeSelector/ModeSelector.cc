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

// use the ikaros namespace to access the math library
// this is preferred to using math.h


using namespace ikaros;


void
ModeSelector::Init()
{
//    input_array = GetInputArray("INPUT");
//    input_array_size = GetInputSize("INPUT");
//    
    output_array = GetOutputArray("STRESS");
    output_array_size = GetOutputSize("STRESS");
    
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
    output_array[0] = 1;
}

