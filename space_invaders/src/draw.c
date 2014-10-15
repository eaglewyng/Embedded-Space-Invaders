/*
 * draw.c
 *
 *  Created on: Oct 15, 2014
 *      Author: superman
 */

#include "space_invaders.h"
#include <stdlib.h>


extern int scoreI[];
extern int scoreS[];
extern int scoreO[];
extern int scoreR[];
extern int scoreC[];
extern int scoreE[];
extern int score1[];
extern int score2[];
extern int score3[];
extern int score4[];
extern int score5[];
extern int score6[];
extern int score7[];
extern int score8[];
extern int score9[];
extern int score0[];
extern int framePointer0[];

extern int score;


//manages the drawing of all of the numbers
void drawScoreNumbers(){
	int tempScore = score;
	int count = 0;
	if(score == 0){
			drawDigit(SCORENUMBERS_START_X,  SCORENUMBERS_START_Y, 0, SCORENUMBERS_COLOR);
		return;
	}

	while(tempScore){
		int digit = tempScore % 10;
		drawDigit(SCORENUMBERS_START_X + NUMBER_WIDTH * count + SPACE_BETWEEN_CHARACTERS * count,  SCORENUMBERS_START_Y, digit, SCORENUMBERS_COLOR);
		tempScore /= 10;
		count++;
	}

	drawDigit(SCORENUMBERS_START_X + NUMBER_WIDTH * count + SPACE_BETWEEN_CHARACTERS * count,  SCORENUMBERS_START_Y, 0, SCORENUMBERS_COLOR);
}


//draws a single digit in the specified location with the specified color
void drawDigit(int x, int y, int digit, int color){
	int* digitBMP;
	switch(digit){
	case 0:
		digitBMP = score0;
		break;
	case 1:
		digitBMP = score1;
		break;
	case 2:
		digitBMP = score2;
		break;
	case 3:
		digitBMP = score3;
		break;
	case 4:
		digitBMP = score4;
		break;
	case 5:
		digitBMP = score5;
		break;
	case 6:
		digitBMP = score6;
		break;
	case 7:
		digitBMP = score7;
		break;
	case 8:
		digitBMP = score8;
		break;
	default:
		digitBMP = score9;
		break;
	}

	int aX;
	int aY;
	for(aX = 0; aX < NUMBER_WIDTH; aX++){
		for(aY = 0; aY < NUMBER_HEIGHT; aY++){
			if(1 && (digitBMP[aY] & (1<<(NUMBER_WIDTH - 1 - aX)))){

				framePointer0[(y+aY) * 640 + x + aX] = color;
			}
			else{
				framePointer0[y+aY * 640 + x + aX] = BLACK;
			}
		}
	}

}

