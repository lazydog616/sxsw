using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;


public class LevelSetup : MonoBehaviour {

	float minRadius = 20;
	float maxRadius = 140;
	int threshold = 180; //range 0-255
	float[] objPosX;
	float[] objPosY;
	float[] objHeight;
	float[] objWidth;
	float[] boundingBox;

	public Camera mainCam;
	public Transform player;
	float levelDepth;

	[DllImport("ShapeDetectionUnity")]
	public static extern int detectShape(float minRadius, float maxRadius, int threshold, float[] objPosX, float[] objPosY, float[] objHeight, float[] objWidth, float[] boundingBox); 


	public AudioSource scanAs;
	public AudioClip sacnClip;

	// Use this for initialization
	void Start () {
		scanAs = (AudioSource)gameObject.AddComponent<AudioSource>();
		scanAs.clip = sacnClip;
		objPosX = new float[100];
		objPosY = new float[100];
		objHeight = new float[100];
		objWidth = new float[100];
		boundingBox = new float[4];
		
		for(int i = 0; i < 100; i++)
		{
			objPosX[i] = objPosY[i] = objHeight[i] = objWidth[i] = 0;
		}
		boundingBox[0] = 0;
		boundingBox[1] = 0;
		boundingBox[2] = 640;
		boundingBox[3] = 480;
		

		levelDepth = Mathf.Abs((mainCam.transform.position - player.position).z);

	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetKey("c"))
		{
			scanAs.Play();
			int sizeNum = detectShape(minRadius, maxRadius, threshold, objPosX, objPosY, objHeight, objWidth, boundingBox);

			for(int i = 0; i < 4; i++)
			{
				print("boudingbox " + i + " " + boundingBox[i]);
			}

			for(int i = 0; i < sizeNum; i++)
			{
				print ("objWidth: " + objWidth[i]);
				print ("objHeight: " + objHeight[i]);
				GameObject newPlatform = GameObject.CreatePrimitive(PrimitiveType.Cube) ;
				Vector3 screenPosition = new Vector3((boundingBox[2] - objPosX[i]) / (boundingBox[2] - boundingBox[0]) * Screen.currentResolution.width, 
				                                     Screen.currentResolution.height - (objPosY[i] - boundingBox[1]) / (boundingBox[3] - boundingBox[1]) * Screen.currentResolution.height, 
				                                     levelDepth); 
				print ("previous: " + (Screen.currentResolution.width - (objPosX[i] - boundingBox[0]) / (boundingBox[2] - boundingBox[0]) * Screen.currentResolution.width));
				print ("now: " + ((boundingBox[2] - objPosX[i]) / (boundingBox[2] - boundingBox[0]) * Screen.currentResolution.width));
				newPlatform.transform.position = mainCam.ScreenToWorldPoint(screenPosition);
				newPlatform.transform.localScale = new Vector3(objWidth[i] / (boundingBox[2] - boundingBox[0]) * 160, objHeight[i] / (boundingBox[3] - boundingBox[1]) * 120);
			}
			
			Destroy(GameObject.Find("White Screen"));
			
			print(sizeNum);
		}
		
	}
}
