using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;


public class LevelSetup : MonoBehaviour {

	float maxRadius = 40;
	int threshold = 80; //range 0-255
	float[] objPosX;
	float[] objPosY;
	float[] objHeight;
	float[] objWidth;
	
//	[DllImport("ShapeDetectionUnity")]
//	public static extern int detectShape(float maxRadius, int threshold, float[] objPosX, float[] objPosY, float[] objHeight, float[] objWidth); 

	// Use this for initialization
	void Start () {
//		objPosX = new float[100];
//		objPosY = new float[100];
//		objHeight = new float[100];
//		objWidth = new float[100];
//		
//		for(int i = 0; i < 100; i++)
//		{
//			objPosX[i] = objPosY[i] = objHeight[i] = objWidth[i] = 0;
//		}
//		int sizeNum = detectShape(maxRadius, threshold, objPosX, objPosY, objHeight, objWidth);
//		
//		print(sizeNum);
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
