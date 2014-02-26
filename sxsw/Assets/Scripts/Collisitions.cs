using UnityEngine;
using System.Collections;

public class Collisitions : MonoBehaviour {
	public int life = 3;
	public Vector3 positionori;

	// Use this for initialization
	void Start () {
		PlayerPrefs.SetString ("result", "lose");
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	void OnTriggerEnter(Collider collider)
	{
		Debug.Log (collider.name);
		if (collider.tag == "fire" || collider.tag == "water" ||collider.tag == "gear") 
		{
			life --;
			if (life > 0)
					reSet ();
			else
			{
				Destroy (gameObject);
				Application.LoadLevel(1);
			}
		} 
		else if (collider.tag == "door") {
			PlayerPrefs.SetString("result","win");
			Application.LoadLevel(1);			
		}
	}
	void reSet()
	{
		gameObject.transform.position = positionori;
	}
}
