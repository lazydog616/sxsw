using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

public class Global : MonoBehaviour {
	
	int maxRadius = 40;
	float[] objPosX;
	float[] objPosY;
	
	[DllImport("ShapeDetectionUnity")]
	public static extern void detectShape(int maxRadius, float[] objPosX, float[] objPosY); 
	
	
	// Use this for initialization
	void Start () {
		objPosX = new float[100];
		objPosY = new float[100];
		for(int i = 0; i < 100; i++)
		{
			objPosX[i] = objPosY[i] = 0;
		}
		detectShape(maxRadius, objPosX, objPosY);
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
