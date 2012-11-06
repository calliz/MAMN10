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
//    Created: 2011-12-09

#include "ARToolKitPlusModule.h"

#ifdef USE_ARTOOLKITPLUS

#include <ARToolKitPlus/Tracker.h>

using namespace ikaros;
using namespace ARToolKitPlus;

const bool useBCH = true;
typedef struct { ARFloat para[16];} ARFloatArray16;



// World data structure

const int marker_pos_x = 0;
const int marker_pos_y = 1;
const int marker_id = 2;
const int confidence = 3;
const int cam_coord = 4;



// This class is only necessary because the variables we want to set are protected;
// Maybe we should make the class a friend instead?

class MTCamera: public Camera
{
public:
    bool setCameraCalibration(float * calibration);
};



bool
MTCamera::setCameraCalibration(float * calibration)
{
    xsize = int(calibration[0]);
    ysize = int(calibration[1]);
    cc[0] = calibration[2];
    cc[1] = calibration[3];
    fc[0] = calibration[4];
    fc[1] = calibration[5];
    kc[0] = calibration[6];
    kc[1] = calibration[7];
    kc[2] = calibration[8];
    kc[3] = calibration[9];
    kc[4] = calibration[10];
    kc[5] = calibration[11];
    undist_iterations = int(calibration[12]);

    undist_iterations = min(undist_iterations, CAMERA_ADV_MAX_UNDIST_ITERATIONS);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] = 0.;

    mat[0][0] = fc[0]; // fc_x
    mat[1][1] = fc[1]; // fc_y
    mat[0][2] = cc[0]; // cc_x
    mat[1][2] = cc[1]; // cc_y
    mat[2][2] = 1.0;

    return true;
}




class MFTracker: public Tracker
{
public:
    MFTracker(int imWidth, int imHeight, int maxImagePatterns = 8, int pattWidth = 6, int pattHeight = 6, int pattSamples = 24, int maxLoadPatterns = 0);
    virtual bool init(float * calibration, ARFloat nNearClip, ARFloat nFarClip);
    virtual void calc(const uint8_t* nImage);
    virtual void setPatternWidth(ARFloat nWidth) { patt_width = nWidth; }

    void GetEdge(int marker, int edge, float &x0, float & y0, float & x1, float & y1);
   
    ARFloat confidence;
    ARFloat patt_width;
    ARFloat patt_center[2];
    ARFloat patt_trans[3][4];

    ARMarkerInfo *marker_info;
    ARFloatArray16 * marker_gl;
    float ***   marker_matrix;
    int marker_num;
};



MFTracker::MFTracker(int imWidth, int imHeight, int maxImagePatterns, int pattWidth, int pattHeight,
        int pattSamples, int maxLoadPatterns) :
    Tracker(imWidth, imHeight, maxImagePatterns, pattWidth, pattHeight, pattSamples, maxLoadPatterns)
{
    thresh = 100;
    patt_width = 2.0;
    patt_center[0] = patt_center[1] = 0.0;
}



bool
MFTracker::init(float * calibration, ARFloat nNearClip, ARFloat nFarClip)
{
    if (!this->checkPixelFormat())
        return false;

    if (this->marker_infoTWO == NULL)
        this->marker_infoTWO = new ARMarkerInfo2[MAX_IMAGE_PATTERNS];

    this->marker_gl = new ARFloatArray16[MAX_IMAGE_PATTERNS];
    
    marker_matrix = create_matrix(4, 4, MAX_IMAGE_PATTERNS);
    
    MTCamera * cam = new MTCamera();

    cam->setCameraCalibration(calibration);

    if (arCamera)
        delete arCamera;

    arCamera = NULL;

    setCamera(cam, nNearClip, nFarClip);

    return true;
}



void
MFTracker::calc(const uint8_t* nImage)
{
    if (nImage == NULL)
        return;

    // detect the markers in the video frame
    
    if (arDetectMarker(const_cast<unsigned char*> (nImage), this->thresh, &marker_info, &marker_num) < 0) // Uses tracking history
         return;

    // calculate all matrices
    
    for (int i = 0; i < marker_num; i++)
        if (marker_info[i].id != -1)
        {
            // Lookup size
            
            float m_size = 45; // FIXME: allow different sizes

            // Estimate position
            
            // printf("converting %d with id = %d and cf = %f\n", i, marker_info[i].id, marker_info[i].cf);
            float err = executeSingleMarkerPoseEstimator(&marker_info[i], patt_center, m_size, patt_trans); 
            if(err > 3) // > 3 == err, not visible
                marker_info[i].cf = 0;

            convertTransformationMatrixToOpenGLStyle(patt_trans, marker_gl[i].para);
            
            float s = 0;
            for(int x=0; x<4; x++)
                for(int y=0; y<4; y++)
                    s += marker_matrix[i][y][x] = marker_gl[i].para[4*x+y];
        }
}



void
MFTracker::GetEdge(int marker, int edge, float & x0, float & y0, float & x1, float & y1)
{
    int dir = marker_info[marker].dir;
    x0 = marker_info[marker].vertex[(4+edge-dir)%4][0];
    y0 = marker_info[marker].vertex[(4+edge-dir)%4][1];
    x1 = marker_info[marker].vertex[(4+edge-dir+1)%4][0];
    y1 = marker_info[marker].vertex[(4+edge-dir+1)%4][1];
}



static float **
create_matrix_view(float * data, int sizex, int sizey)
{
    float ** b = (float **)malloc(sizey*sizeof(float *));
    for (int j=0; j<sizey; j++)
        b[j] = &data[j*sizex];
    return b;
}



static void
destroy_matrix_view(float ** m)
{
    free(m);
}



void
ARToolKitPlusModule::Init()
{
    calibration = GetArray("calibration", 13);
    max_markers = GetIntValue("max_markers");

    markers = GetOutputMatrix("MARKERS");

    input = GetInputMatrix("INPUT");
    
    r = GetInputMatrix("RED");
    g = GetInputMatrix("GREEN");
    b = GetInputMatrix("BLUE");

    output_r = GetOutputMatrix("OUTPUT_RED");
    output_g = GetOutputMatrix("OUTPUT_GREEN");
    output_b = GetOutputMatrix("OUTPUT_BLUE");

//    pos     = GetOutputMatrix("POS");
    edges   = GetOutputMatrix("EDGES");

    max_positions = GetOutputSizeY("POS");
    max_edges = GetOutputSizeY("EDGES");
    
    size_x = GetInputSizeX("INPUT");
    size_y = GetInputSizeY("INPUT");

    tracker = new MFTracker(size_x, size_y, max_markers, 6, 6, 6, 0);
    tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);
    if (!tracker->init(calibration, 1.0f, 1000.0f))
    {
        Notify(msg_fatal_error, "Could not find camera calibration data\n");
    }

//  tracker->getCamera()->printSettings();
    tracker->setBorderWidth(useBCH ? 0.125 : 0.25);
    tracker->activateAutoThreshold(true);
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
    tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);
    
    buffer = new unsigned char [size_x*size_y];
}



ARToolKitPlusModule::~ARToolKitPlusModule()
{
    delete [] buffer;
}



void
ARToolKitPlusModule::Tick()
{		 
    reset_matrix(markers, max_markers, 20);
    for(int i=0; i<max_markers; i++)
    {
        markers[i][0] = -1;
        markers[i][1] = -1;
    }

    copy_matrix(output_r, r, size_x, size_y);
    copy_matrix(output_g, g, size_x, size_y);
    copy_matrix(output_b, b, size_x, size_y);
    
    multiply(output_r, 0.5, size_x, size_y);
    multiply(output_g, 0.5, size_x, size_y);
    multiply(output_b, 0.5, size_x, size_y);
    
    float_to_byte(buffer, *input, 0, 1, size_x*size_y);
    
    if(norm(input, size_x, size_y) == 0) // No image, return
        return;
    
    tracker->calc(buffer);

    int ix = 0;
    for (int j = 0; j<tracker->marker_num; j++)
        if(ix < max_markers && tracker->marker_info[j].cf > 0.7)
        {
            // Set position in camera coordinates
            float ** m = create_matrix_view(&markers[ix][cam_coord], 4, 4);
            copy_matrix(m, tracker->marker_matrix[j], 4, 4);
            destroy_matrix_view(m);
            markers[ix][marker_pos_x] = tracker->marker_info[j].pos[0]/size_x;
            markers[ix][marker_pos_y] = tracker->marker_info[j].pos[1]/size_y;
            markers[ix][marker_id] = float(tracker->marker_info[j].id);
            markers[ix][confidence] = tracker->marker_info[j].cf;
            ix++;
        }
    
    // Create output for drawing image overlay

    set_matrix(edges, -1, 4, max_edges);

    int i = 0;
//    int p =0;
    for (int j=0; j<tracker->marker_num; j++)
    {
        if (tracker->marker_info[j].cf > 0.7 && tracker->marker_info[j].id != -1)
        {
            if(i<max_edges)
            {
                for(int l=0; l<4; l++)
                {
                    float x0, y0, x1, y1;
                    tracker->GetEdge(j, l, x0, y0, x1, y1);
                    edges[i][0] = x0/size_x;
                    edges[i][1] = y0/size_y;
                    edges[i][2] = x1/size_x;
                    edges[i][3] = y1/size_y;
                    
                    i++;
                }
            }
        }
    }
}

#endif

