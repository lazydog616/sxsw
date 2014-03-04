//------------------------------------------------------------------------------
// <copyright file="ColorBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "ColorBasics.h"
//#include "resource.h"

CColorBasics application;

/// <summary>
/// Constructor
/// </summary>
CColorBasics::CColorBasics() :
//    m_pD2DFactory(NULL),
//	m_pDrawColor(NULL),
    m_hNextColorFrameEvent(INVALID_HANDLE_VALUE),
    m_pColorStreamHandle(INVALID_HANDLE_VALUE),
	m_hNextDepthFrameEvent(INVALID_HANDLE_VALUE),
    m_pDepthStreamHandle(INVALID_HANDLE_VALUE),
    m_bSaveScreenshot(false),
    m_pNuiSensor(NULL)
{
	// create heap storage for depth pixel data in RGBX format
	m_depthRGBX = new BYTE[cColorWidth*cColorHeight*cBytesPerPixel];

	depthValue = vector<int>(cColorWidth*cColorHeight);
}

/// <summary>
/// Destructor
/// </summary>
CColorBasics::~CColorBasics()
{
    if (m_pNuiSensor)
    {
        m_pNuiSensor->NuiShutdown();
    }

    if (m_hNextColorFrameEvent != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hNextColorFrameEvent);
    }

    // clean up Direct2D renderer
  //  delete m_pDrawColor;
    //m_pDrawColor = NULL;

	delete[] m_depthRGBX;

    // clean up Direct2D
    //SafeRelease(m_pD2DFactory);

    SafeRelease(m_pNuiSensor);
}

/// <summary>
/// Main processing function
/// </summary>
void CColorBasics::Update()
{
   /* if (NULL == m_pNuiSensor)
    {
        return;
    }

    if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextColorFrameEvent, 0) )
    {
        ProcessColor();
    }

	 if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextDepthFrameEvent, 0) )
    {
        ProcessDepth();
		ProcessColorDepth();		
		ShapeBoundingbox();
    }
*/

}



/// <summary>
/// Create the first connected Kinect found 
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CColorBasics::CreateFirstConnected()
{
    INuiSensor * pNuiSensor;
    HRESULT hr;

    int iSensorCount = 0;
    hr = NuiGetSensorCount(&iSensorCount);
    if (FAILED(hr))
    {
        return hr;
    }

    // Look at each Kinect sensor
    for (int i = 0; i < iSensorCount; ++i)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);
        if (FAILED(hr))
        {
            continue;
        }

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();
        if (S_OK == hr)
        {
            m_pNuiSensor = pNuiSensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }

    if (NULL != m_pNuiSensor)
    {
        // Initialize the Kinect and specify that we'll be using color
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH); 
        if (SUCCEEDED(hr))
        {
            // Create an event that will be signaled when color data is available
            m_hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

            // Open a color image stream to receive color frames
            hr = m_pNuiSensor->NuiImageStreamOpen(
                NUI_IMAGE_TYPE_COLOR,
                NUI_IMAGE_RESOLUTION_640x480,
                0,
                2,
                m_hNextColorFrameEvent,
                &m_pColorStreamHandle);

			 // Create an event that will be signaled when depth data is available
             m_hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			 hr = m_pNuiSensor->NuiImageStreamOpen(
                NUI_IMAGE_TYPE_DEPTH,
                NUI_IMAGE_RESOLUTION_640x480,
                0,
                2,
                m_hNextDepthFrameEvent,
                &m_pDepthStreamHandle);

			 if (FAILED(hr) ) { return hr; }
        }
    }

    return hr;
}



//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){        
       IplImage* imgThresh = cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS(imgHSV, cvScalar(170,160,60), cvScalar(180,256,256), imgThresh); 
       return imgThresh;
} 

/// <summary> 
/// Handle new color data
/// </summary>
/// <returns>indicates success or failure</returns>

IplImage* img;
void CColorBasics::ProcessColor()
{
	img = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 4);
    HRESULT hr;
    NUI_IMAGE_FRAME imageFrame;

    // Attempt to get the color frame
    hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pColorStreamHandle, 0, &imageFrame);
    if(FAILED(hr))
    {		
        return;
    }

	INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
    NUI_LOCKED_RECT LockedRect;

	// Lock the frame data so the Kinect knows not to modify it while we're reading it
    pTexture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {

		//change color to opencs format
		BYTE * pBuffer = static_cast<BYTE *>(LockedRect.pBits);		
		
		cvSetData(img, (BYTE*) pBuffer, img->widthStep);
		Mat m(img);
		IplImage iplimg(m);

		cvSmooth(img, img, CV_GAUSSIAN,3,3); 
		IplImage* imgHSV = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3); 
		cvCvtColor(img, imgHSV, CV_BGR2HSV);
		/*IplImage* */
		imgThresh = GetThresholdedImage(imgHSV);
		cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3);

		//CV_IMAGE_ELEM(imgThresh, uchar, 0,0);

		namedWindow( "Color", CV_WINDOW_AUTOSIZE );
		imshow( "Color", m);
#ifdef EXE
		if( waitKey (30) >= 0) return;
#endif
        // Draw the data with Direct2D
		//#ifndef DEPTHDISPLAY
		//        m_pDrawColor->Draw(static_cast<BYTE *>(LockedRect.pBits), LockedRect.size);
		//#endif
    }

    // We're done with the texture so unlock it
    pTexture->UnlockRect(0);

    // Release the frame
    m_pNuiSensor->NuiImageStreamReleaseFrame(m_pColorStreamHandle, &imageFrame);	
}


void CColorBasics::ProcessDepth()
{
	HRESULT hr;
	NUI_IMAGE_FRAME imageFrame;
	 // Attempt to get the depth frame
    hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 0, &imageFrame);
    if (FAILED(hr))
    {
        return;
    }

	BOOL nearMode;
    INuiFrameTexture* depthTexture;
	hr = m_pNuiSensor->NuiImageFrameGetDepthImagePixelFrameTexture(m_pDepthStreamHandle, &imageFrame, &nearMode, &depthTexture);
	NUI_LOCKED_RECT LockedDepthRect;

	if (FAILED(hr))
    {
        //goto ReleaseFrame;
		return;
    }
	depthTexture->LockRect(0, &LockedDepthRect, NULL, 0);


	 if (LockedDepthRect.Pitch != 0)
    {

		int minDepth = (nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
		int maxDepth = (nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;

		minDepth = 10;
		BYTE * rgbrun = m_depthRGBX;
		const NUI_DEPTH_IMAGE_PIXEL * pBufferRun = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL *>(LockedDepthRect.pBits);

			// end pixel is start + width*height - 1
		const NUI_DEPTH_IMAGE_PIXEL * pBufferEnd = pBufferRun + (cColorWidth * cColorHeight);
		int countrun = 0;
		while ( pBufferRun < pBufferEnd )
		{
			// discard the portion of the depth that contains only the player index
			USHORT depth = pBufferRun->depth;

			// To convert to a byte, we're discarding the most-significant
			// rather than least-significant bits.
			// We're preserving detail, although the intensity will "wrap."
			// Values outside the reliable depth range are mapped to 0 (black).

			// Note: Using conditionals in this loop could degrade performance.
			// Consider using a lookup table instead when writing production code.
			

			BYTE intensity = static_cast<BYTE>(depth >= minDepth && depth <= maxDepth ? float(depth-minDepth) / float(maxDepth-minDepth) * 255.0 : 0);


			depthValue[countrun] = intensity;
			//BYTE intensity = static_cast<BYTE>(depth >= minDepth && depth <= maxDepth ? depth % 256 : 0);

			// Write out blue byte
			*(rgbrun++) = intensity;

			// Write out green byte
			*(rgbrun++) = intensity;

			// Write out red byte
			*(rgbrun++) = intensity;

			// We're outputting BGR, the last byte in the 32 bits is unused so skip it
			// If we were outputting BGRA, we would write alpha here.
			++rgbrun;

			// Increment our index into the Kinect's depth buffer
			++pBufferRun;
			++countrun;
		}
#ifdef DEPTHDISPLAY
		m_pDrawColor->Draw(m_depthRGBX, cColorWidth * cColorHeight * cBytesPerPixel);
#endif
	 }

	   // We're done with the texture so unlock it
    depthTexture->UnlockRect(0);

    depthTexture->Release();

ReleaseFrame:
	 // Release the frame
	 m_pNuiSensor->NuiImageStreamReleaseFrame(m_pDepthStreamHandle, &imageFrame);

}


void CColorBasics::ProcessColorDepth()
{
	int size = imgThresh->imageSize;
	
	for(int i = 0; i < size; i++)
	{
		//if(depthValue[i]<20)
		//	imgThresh->imageData[i] = 30;
		imgThresh->imageData[i] = depthValue[i];

		if(depthValue[i] > 50)
			imgThresh->imageData[i] = 0;

		if(imgThresh->imageData[i] >20)
		{
			imgThresh->imageData[i] = 220;
		}
	}
	
	cvNamedWindow("rgb",1);
	cvShowImage("rgb",imgThresh);
	cv::waitKey(1);	
}

RNG rng(12345);
void CColorBasics::ShapeBoundingbox(float* objPosX, float* objPosY, float* objHeight, float* objWidth, int& shapeNum, float* boundingBox)
{
	//int thresh = 80;
	int max_thresh = 255;

	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	//Mat src_bw(img);
	Mat src_bw;
	Mat src(img); //= imread( "./1.png", 1 );
	
	/// Convert image to gray and blur it
	cvtColor( src, src_bw, COLOR_BGR2GRAY );
	blur( src_bw, src_bw, Size(2,2) );

	///// Detect edges using Threshold
	threshold( src_bw, threshold_output, threshhold, 255, THRESH_BINARY );
	
	///// Find contours
	findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );


	 for( int i = 0; i < contours.size(); i++ )
     {
		 if(contours[i].size() != 0){
			approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
			boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
		 }
     }

	
	 /// Draw polygonal contour + bonding rects + circles
	Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	bool findBoundingbox = false;
	for( int i = 0; i< contours.size(); i++ )
	{
		if(radius[i] > 190)
		{
			findBoundingbox = true;
			boundingBox[0] = boundRect[i].tl().x;
			boundingBox[1] = boundRect[i].tl().y;

			boundingBox[2] = boundRect[i].br().x;
			boundingBox[3] = boundRect[i].br().y;
		}


		if(radius[i] > minRadius && radius[i] < maxRadius){			
			objPosX[shapeNum] = center[i].x;
			objPosY[shapeNum] = center[i].y;
			objHeight[shapeNum] = boundRect[i].height;
			objWidth[shapeNum] = boundRect[i].width;

			//int colorIndex = center[i].x * img->width + center[i].y;
			//img[colorIndex] = 20;
			shapeNum++;

			//Find the color of the polygon(currently only bounding box) created by the contour
			Mat img_roi = Mat(src, boundRect[i]);
			Scalar avgPixelIntensity = mean(img_roi);
			Scalar color = avgPixelIntensity;
			//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

			//Potentially a more accurate way - find the average hue
			Mat img_hsv;
			//convert from rgb to hsv
			cvtColor(img_roi, img_hsv, CV_BGR2HSV);
			Mat channels[3];
			//split into channels of h, s, v
			split(img_hsv, channels);
			Scalar avghue = mean(channels[0]);

			drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
			circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
		}
	}

	//Show in a window
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );
#ifdef EXE
	if( waitKey (30) >= 0) return;
#endif
}


