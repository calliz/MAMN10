//
//	ARToolKitPlusModule		This file is a part of the IKAROS project
//                          Wrapper module for ARToolKitPlus available at:
//                          https://launchpad.net/artoolkitplus
//
//    Copyright (C) 2011-2012 Christian Balkenius
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

#ifndef ARToolKitPlusModule_
#define ARToolKitPlusModule_


#include "IKAROS.h"

#ifdef USE_ARTOOLKITPLUS

class MFTracker;

class ARToolKitPlusModule: public Module
{
public:

    ARToolKitPlusModule(Parameter * p) : Module(p) {}
    virtual ~ARToolKitPlusModule();

    static Module * Create(Parameter * p) { return new ARToolKitPlusModule(p); }

    void 		Init();
    void 		Tick();

    float **    markers;
    int         markers_count;
    
    int         max_markers;
    float *     calibration;
    
    float **    input;

    float **    r;
    float **    g;
    float **    b;

    float **    output_r;
    float **    output_g;
    float **    output_b;

    float **    edges;
    int         max_positions;
    int         max_edges;
    
    MFTracker     * tracker;
    unsigned char * buffer;
    
    int         size_x;
    int         size_y;

    float **    output;
};

#endif

#ifndef USE_ARTOOLKITPLUS

class ARToolKitPlusModule {
public:
    static Module * Create(Parameter * p)
    {
        printf("This version was not compiled with ARToolKitPlus\n.");
        return NULL;
    }
};

#endif

#endif
