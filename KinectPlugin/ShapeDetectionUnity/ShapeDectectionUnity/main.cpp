
#include "ColorBasics.h"


int main()
{
	CColorBasics colorKinect;
	colorKinect.CreateFirstConnected();
	/*while(1){
		Sleep(100);
		 if ( WAIT_OBJECT_0 == WaitForSingleObject(colorKinect.m_hNextColorFrameEvent, 0) )
		{*/
			colorKinect.ProcessColor();
			float* x = new float[1000];
			float* y = new float[1000];
			int num = 0;
			colorKinect.ShapeBoundingbox(x, y, num);
	//		break;
	//	}
	//	//colorKinect.ProcessColor();
	//}
	

	system("pause");
}