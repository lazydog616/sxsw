using UnityEngine;
using System.Collections;

public class ResultGlobal : MonoBehaviour {

	// Use this for initialization
	void Start () {
		Debug.Log (PlayerPrefs.GetString ("result"));
		if (PlayerPrefs.GetString ("result") == "win") {
			GameObject.Find("result").guiText.text = "YOU WIN";
		}
		else
		{
			GameObject.Find("result").guiText.text = "You LOSE!";
		}
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
