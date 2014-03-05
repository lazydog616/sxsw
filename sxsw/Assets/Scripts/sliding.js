#pragma strict

public var positionA : Vector3 ;
public var positionB : Vector3 ;
public var period : float  = 3;

private var movingAtoB : boolean  = true;
private var periodTimer : float  = 0;
	
function Start () {
	positionA = transform.position;
	positionB = positionA + Vector3(10, 0, 0);

}

function Update () {
	//print (movingAtoB);
	/*var currentPosition = Vector3.Lerp(positionA, positionB, movingAtoB ? periodTimer / period : (1 - periodTimer / period));
	transform.position = currentPosition;

	periodTimer += Time.deltaTime;
	if(periodTimer > period)
	{
		periodTimer = 0;
		movingAtoB = !movingAtoB;
	}*/
}
	/*
function getVelocity() {

	return movingAtoB? (positionB - positionA) / period : (positionA - positionB) / period;

}*/
