/*
 * gamestatus.c
 *	This file will contain all of the game status information and modifiers, including the score and
 *	the number of lives the player has
 *  Created on: Oct 15, 2014
 *      Author: superman
 */

#include "space_invaders.h"
#include "gamestatus.h"
#include <stdlib.h>


int score;
int lives;

void initStatus(){
	score = INITIAL_SCORE;
	lives = INITIAL_LIVES;
}


//adds to the score, and updates the displayed score if necessary
void addToScore(int scoringcode){
	if(scoringcode == HIT_BIG_SQUID){
		score += POINTS_FOR_BIG_SQUID;
	}
	else if(scoringcode == HIT_SMALL_SQUID){
		score += POINTS_FOR_SMALL_SQUID;
	}
	else if(scoringcode == HIT_JUMPING_JACK){
		score += POINTS_FOR_JUMPING_JACK;
	}
	else if(scoringcode == HIT_RED_SPACESHIP){
		score += getRedSpaceshipScore();
	}
	drawScoreNumbers();
}

int getRedSpaceshipScore(){
	//choose a random number between the high score and low score for the red alien
	//and add that to the low score
	int choice = rand() % (((HIGH_SCORE_FOR_RED_ALIEN - LOW_SCORE_FOR_RED_ALIEN) / SCORE_INCREMENTS_FOR_RED_ALIEN));
	int ssScore  = choice * SCORE_INCREMENTS_FOR_RED_ALIEN + LOW_SCORE_FOR_RED_ALIEN;
	return ssScore;
}


void takeLife(){
	lives--;
	if(lives == 0){
		runGameOver(0);
	}
	drawLivesTanks();
}

void addLife(){
	lives++;
	drawLivesTanks();
}


//runs the game over routine--this should be invoked under only two conditions:
//1. The user has won, and a simple level reset is needed, or
//2. The loser has lost (either run out of lives or the aliens have reached the bottom)
//and the whole game needs to be reset.
void runGameOver(int userHasWon){
	if(userHasWon){
		lives++;
		drawLivesTanks();
	}
	else{

	}
}


