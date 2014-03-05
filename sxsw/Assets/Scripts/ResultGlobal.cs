using UnityEngine;
using System.Collections;

public class ResultGlobal : MonoBehaviour {

	//bool gameOver = false;
	// Use this for initialization
	void Start () {
		Debug.Log (PlayerPrefs.GetString ("result"));
		if (PlayerPrefs.GetString ("result") == "win") {
			GameObject.Find("result").guiText.text = "YOU WIN";
			//gameOver = true;
		}
		else
		{
			GameObject.Find("result").guiText.text = "You LOSE!";
		}
	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetButtonDown("scan"))
			Application.LoadLevel("Main_Kinect");	
	}
}
