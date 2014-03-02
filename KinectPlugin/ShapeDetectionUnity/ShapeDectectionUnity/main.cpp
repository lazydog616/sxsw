
#include "ColorBasics.h"


int main()
{
	CColorBasics colorKinect;
	colorKinect.maxRadius = 120;
	colorKinect.minRadius = 40;
	colorKinect.threshhold = 180;
	colorKinect.CreateFirstConnected();
	int shapeNum = 0;

	float* objPosX = new float[100];
	float* objPosY = new float[100];
	float* objHeight = new float[100];
	float* objWidth = new float[100];
	float* boudingBox = new float[4];

	while(1){
		Sleep(100);
		if ( WAIT_OBJECT_0 == WaitForSingleObject(colorKinect.m_hNextColorFrameEvent, 0) )
		{
			colorKinect.ProcessColor();
			colorKinect.ShapeBoundingbox(objPosX, objPosY, objHeight, objWidth, shapeNum, boudingBox);
			break;
		}
	}
	

	//system("pause");
}