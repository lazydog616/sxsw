using UnityEngine;
using System.Collections;

public class Destination : MonoBehaviour {
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
				//destination score
				gamestate.score += Constants.reaching_destination_score;
				//remaining life bonus
				gamestate.score += gamestate.remaining_life_amount * Constants.life_bonus;
				//time bonus
				gamestate.score += (Constants.level_time_limit - gamestate.timer)/Constants.level_time_limit * 500;
				PlayerPrefs.SetFloat(gamestate.player_name, gamestate.score);
				//TODO: handle this level loading
				Application.LoadLevel("Score_Level");

			}

			
			
		}
		
	}
}
