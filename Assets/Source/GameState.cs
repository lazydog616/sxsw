using UnityEngine;
using System.Collections;

public class GameState : MonoBehaviour {

	public string player_name;
	public float score;
	public int remaining_life_amount;
	public float timer;
	private GUIText gameplay_info;

	// Use this for initialization
	void Start () {
		player_name = "test_player";
		score = 0;
		remaining_life_amount = Constants.player_life_amount;
		timer = Constants.level_time_limit;
		gameplay_info = GameObject.Find ("GamePlayInfo").GetComponent<GUIText> ();
	
	}
	
	// Update is called once per frame
	void Update () {
		//game play info gui
		gameplay_info.text = "Player life : " + remaining_life_amount + "   Score : " + score + "  Timer : " + (int)timer;

		//time code
		timer -= Time.deltaTime;
		//TODO: change to right level name
		if (timer <= 0) {
			if(remaining_life_amount > 0)
			{
				//time limit passed, move player back to the start pos, and decrease player life amount
				//reset timer
				GameObject player = GameObject.FindGameObjectWithTag("Player");
				player.transform.position = Constants.player_start_pos;
				remaining_life_amount -= 1;
				timer = Constants.level_time_limit;
			}
			else{
				PlayerPrefs.SetFloat(player_name, score);
				Application.LoadLevel ("Score_Scene");
			}
		}
	
	}
}
