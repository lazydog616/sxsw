#include<Windows.h>
#include "ColorBasics.h"

#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif


extern "C"
{
	typedef struct
	{
		float tlx;
		float tly;
		float brx;
		float bry;
	} box;

	static CColorBasics colorKinect;
	int EXPORT_API detectShape(int maxRadius, float* objPosX, float* objPosY, float* objHeight, float* objWidth)
	{
		colorKinect.maxRadius = maxRadius;
		colorKinect.CreateFirstConnected();
		int shapeNum = 0;
		
		while(1){
			Sleep(100);
			if ( WAIT_OBJECT_0 == WaitForSingleObject(colorKinect.m_hNextColorFrameEvent, 0) )
			{
				colorKinect.ProcessColor();
				colorKinect.ShapeBoundingbox(objPosX, objPosY, objHeight, objWidth, shapeNum);
				break;
			}
		}

		//shapeNum = 10;

		return shapeNum;
	}
}