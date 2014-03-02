using UnityEngine;
using System.Collections;

public class sliding : MonoBehaviour {

	public Vector3 positionA;
	public Vector3 positionB;
	public float period;

	private bool movingAtoB = true;
	private float periodTimer = 0;

	// Use this for initialization
	void Start () {
		positionA = transform.position;
		positionB = positionA + new Vector3(10, 0, 0);
	
	}
	
	// Update is called once per frame
	void Update () {

		//print (movingAtoB);
		Vector3 currentPosition = Vector3.Lerp(positionA, positionB, movingAtoB ? periodTimer / period : (1 - periodTimer / period));
		transform.position = currentPosition;

		periodTimer += Time.deltaTime;
		if(periodTimer > period)
		{
			periodTimer = 0;
			movingAtoB = !movingAtoB;
		}
	}
}
