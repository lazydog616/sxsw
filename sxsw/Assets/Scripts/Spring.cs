using UnityEngine;
using System.Collections;

public class Spring : MonoBehaviour {

	public AudioSource spas;
	public AudioClip spclip;
	bool collide_with_player = false;
	Vector3 start_pos;
	// Use this for initialization
	void Start () {
		spas = (AudioSource)gameObject.AddComponent<AudioSource> ();
		spas.clip = spclip;
		start_pos = gameObject.transform.position;
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void FixedUpdate() {
		Vector3 displacement = gameObject.transform.position - start_pos;
		float k = 9.80f;
		Vector3 force = -k * displacement - gameObject.rigidbody.velocity;
		//Debug.Log ("force  : " + force);
		gameObject.rigidbody.AddForce (force);
	}

	void OnTriggerEnter(Collider collider){

		if (collider.tag == "Player")
		{
			spas.Play ();
			gameObject.transform.position -= Vector3.up * 2;
		}
	}
//	void OnCollisionEnter(Collision collision){
//		Debug.Log (collision.collider.name);
//		if (collision.collider.tag == "Player")
//						Debug.Log ("abc");
//			gameObject.transform.position -= Vector3.up * 2;
//	}
}
