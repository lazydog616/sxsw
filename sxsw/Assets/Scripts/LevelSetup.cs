using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;


public class LevelSetup : MonoBehaviour {

	float minRadius = 20;
	float maxRadius = 140;
	int threshold = 150; //range 0-255
	float[] objPosX;
	float[] objPosY;
	float[] objHeight;
	float[] objWidth;
	float[] boundingBox;
	float[] objBGR;

	public Camera mainCam;
	public Transform player;

	public GameObject basic_platform;
	public GameObject sliding_platform;
	public GameObject spring_platform;
	float levelDepth;

	[DllImport("ShapeDetectionUnity")]
	public static extern int detectShape(float minRadius, float maxRadius, int threshold, float[] objPosX, float[] objPosY, float[] objHeight, float[] objWidth, float[] boundingBox, float[] objBGR, bool debugging); 


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
		objBGR = new float[100];
		boundingBox = new float[4];

		
		for(int i = 0; i < 100; i++)
		{
			objPosX[i] = objPosY[i] = objHeight[i] = objWidth[i] = objBGR[i] = 0;
		}
		boundingBox[0] = 0;
		boundingBox[1] = 0;
		boundingBox[2] = 640;
		boundingBox[3] = 480;
		

		levelDepth = Mathf.Abs((mainCam.transform.position - player.position).z);

	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetButtonDown("scan"))
		{
			scanAs.Play();
			int sizeNum = detectShape(minRadius, maxRadius, threshold, objPosX, objPosY, objHeight, objWidth, boundingBox, objBGR, false);

			for(int i = 0; i < 4; i++)
			{
				print("boudingbox " + i + " " + boundingBox[i]);
			}

			for(int i = 0; i < sizeNum; i++)
			{
				GameObject newPlatform;

				print ("objWidth: " + objWidth[i]);
				print ("objHeight: " + objHeight[i]);
				print ("objHue: " + objBGR[3*i+2] + ", " + objBGR[3*i+1] + ", " + objBGR[3*i]);

				float primeValue = Mathf.Max (objBGR[3*i+2], Mathf.Max(objBGR[3*i+1], objBGR[3*i]));

				if(objBGR[3*i+2] == primeValue)
					newPlatform = Instantiate(basic_platform) as GameObject;
				else if(objBGR[3*i+1] == primeValue)
					newPlatform = Instantiate(sliding_platform) as GameObject;
				else 
					newPlatform = Instantiate(spring_platform) as GameObject;

				//GameObject newPlatform = GameObject.CreatePrimitive(PrimitiveType.Cube);
				//newPlatform.renderer.material.color = new Color(objHue[3*i+2]/255, objHue[3*i+1]/255, objHue[3*i]/255);
				Vector3 screenPosition = new Vector3((boundingBox[2] - objPosX[i]) / (boundingBox[2] - boundingBox[0]) * Screen.currentResolution.width, 
				                                     Screen.currentResolution.height - (objPosY[i] - boundingBox[1]) / (boundingBox[3] - boundingBox[1]) * Screen.currentResolution.height, 
				                                     levelDepth); 
				print ("previous: " + (Screen.currentResolution.width - (objPosX[i] - boundingBox[0]) / (boundingBox[2] - boundingBox[0]) * Screen.currentResolution.width));
				print ("now: " + ((boundingBox[2] - objPosX[i]) / (boundingBox[2] - boundingBox[0]) * Screen.currentResolution.width));
				newPlatform.transform.position = mainCam.ScreenToWorldPoint(screenPosition);
				newPlatform.transform.localScale = new Vector3(objWidth[i] / (boundingBox[2] - boundingBox[0]) * 160, objHeight[i] / (boundingBox[3] - boundingBox[1]) * 120, 20);
			}
			
			Destroy(GameObject.Find("White Screen"));
			
			print(sizeNum);
		}
		
	}
}
