using UnityEngine;
using System.Collections;

public class Destructor : MonoBehaviour {
	private GameState gamestate;
	// Use this for initialization
	void Start () {
		gamestate = GameObject.FindGameObjectWithTag("GameState").gameObject.GetComponent<GameState>();
		
	}
	
	// Update is called once per frame
	void Update () {
		
		
	}
	
	void OnCollisionEnter(Collision collision){
		if (collision.gameObject.tag == "Player") {
			GameObject player = GameObject.FindGameObjectWithTag("Player");
			if(gamestate.remaining_life_amount > 0){
				//life amount -1, and move player back to the start pos
				player.transform.position = Constants.player_start_pos;
				gamestate.remaining_life_amount -= 1;
			}
			else{
				Destroy(player);
				PlayerPrefs.SetFloat(gamestate.player_name, gamestate.score);
				//TODO: change the level name to right score scene name
				Application.LoadLevel("Score_Level");
			}


		}
		
	}
}
