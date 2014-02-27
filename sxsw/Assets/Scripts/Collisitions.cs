using UnityEngine;
using System.Collections;

public class Collisitions : MonoBehaviour {
	public int life = 3;
	public Vector3 positionori;

	// Use this for initialization
	void Start () {
		PlayerPrefs.SetString ("result", "lose");
		positionori = gameObject.transform.position;
	}
	bool get_springed = false;
	Vector3 spring_up_velocity;
	// Update is called once per frame
	void Update () {
		
	}
	void FixedUpdate(){
		if (get_springed) {
			//Debug.Log("hello");
			gameObject.transform.position += spring_up_velocity * Time.deltaTime;
			spring_up_velocity -= Vector3.up * 9.8f * Time.deltaTime;
			if(spring_up_velocity.y < 100.0f) get_springed = false;

			//get_springed = false;
		}

	}
	void OnTriggerEnter(Collider collider)
	{
		Debug.Log (collider.name);
		if (collider.tag == "fire" || collider.tag == "water" || collider.tag == "gear") {
						life --;
						if (life > 0)
								reSet ();
						else {
								Destroy (gameObject);
								Application.LoadLevel (1);
						}
				} else if (collider.tag == "door") {
						PlayerPrefs.SetString ("result", "win");
						Application.LoadLevel (1);			
				} else if (collider.tag == "spring") {
						Debug.Log("spring!!");
						get_springed = true;
						spring_up_velocity = Vector3.up * 200;
				}
	}
	void reSet()
	{
		gameObject.transform.position = positionori;
	}
}
