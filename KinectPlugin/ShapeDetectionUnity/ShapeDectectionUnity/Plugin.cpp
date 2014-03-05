#include<Windows.h>
#include "ColorBasics.h"

#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif


extern "C"
{
	static CColorBasics colorKinect;
	int EXPORT_API detectShape(float minRadius, float maxRadius, int threshold, 
		float* objPosX, float* objPosY, float* objHeight, float* objWidth, float* boundingBox, float* objHue)
	{
		colorKinect.maxRadius = maxRadius;
		colorKinect.minRadius = minRadius;
		colorKinect.threshhold = threshold;
		colorKinect.CreateFirstConnected();
		int shapeNum = 0;
		
		while(1){
			Sleep(100);
			if ( WAIT_OBJECT_0 == WaitForSingleObject(colorKinect.m_hNextColorFrameEvent, 0) )
			{
				colorKinect.ProcessColor();
				colorKinect.ShapeBoundingbox(objPosX, objPosY, objHeight, objWidth, shapeNum, boundingBox, objHue);
				break;
			}
		}

		//shapeNum = 10;

		return shapeNum;
	}
}