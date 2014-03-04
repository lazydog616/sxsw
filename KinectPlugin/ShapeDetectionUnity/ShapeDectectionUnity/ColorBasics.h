//------------------------------------------------------------------------------
// <copyright file="ColorBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"
//#include "resource.h"
#include "NuiApi.h"

#define EXE

//#define DEPTHDISPLAY
/// Function header

class CColorBasics
{
    static const int        cColorWidth  = 640;
    static const int        cColorHeight = 480;
	static const int        cBytesPerPixel = 4;

    static const int        cStatusMessageMaxLen = MAX_PATH*2;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    CColorBasics();

    /// <summary>
    /// Destructor
    /// </summary>
    ~CColorBasics();

//private:
    HWND                    m_hWnd;

    bool                    m_bSaveScreenshot;

	int						maxRadius;

	int						minRadius;

	int						threshhold;

    // Current Kinect
    INuiSensor*             m_pNuiSensor;

    // Direct2D
    //ImageRenderer*          m_pDrawColor;
    //ID2D1Factory*           m_pD2DFactory;
    
    HANDLE                  m_pColorStreamHandle;
    HANDLE                  m_hNextColorFrameEvent;

	// Depth
	HANDLE                  m_pDepthStreamHandle;
	HANDLE                  m_hNextDepthFrameEvent;

	//depth
	BYTE*                   m_depthRGBX;


	IplImage*				imgThresh;
	vector<int>				depthValue;

    /// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();

    /// <summary>
    /// Create the first connected Kinect found 
    /// </summary>
    /// <returns>S_OK on success, otherwise failure code</returns>
    HRESULT                 CreateFirstConnected();

    /// <summary>
    /// Handle new color data
    /// </summary>
    void                    ProcessColor();

	void					ProcessDepth();

	void					ProcessColorDepth();

	void					ShapeBoundingbox(float*, float*, float*, float*, int&, float*);
};
