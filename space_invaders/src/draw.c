/*
 * draw.c
 *
 *  Created on: Oct 15, 2014
 *      Author: superman
 */

#include "space_invaders.h"
#include <stdlib.h>
#include <math.h>
#include "space_invaders.h"
#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "display.h"
#include <xuartlite_l.h>
#include <xparameters.h>
#include <stdlib.h>


#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.
#define MAX_SILLY_TIMER 500000;
#define ASCII_OFFSET 48

void print(char *str);


extern Bunker bunkerArray[];
extern Bullet bulletArray[];
extern int bottomRowAliens[];

//external integers
extern int alienOriginX;
extern int alienOriginY;
extern int tankOriginX;
extern int tankOriginY;
extern int alienFarRightOffset;
extern int alienArray[];
extern int noAlien[];
extern int tankState;

//bitmaps
extern int bigSquidIn[];
extern int bigSquidOut[];
extern int littleSquidIn[];
extern int littleSquidOut[];
extern int jumpingJackIn[];
extern int jumpingJackOut[];
extern int tank[];
extern int bunkerTopRightSymbol[];
extern int bunkerBottomLeftSymbol[];
extern int bunkerBottomRightSymbol[];
extern int bunkerTopLeftSymbol[];
extern int bunkerBlockSymbolD1[];
extern int bunkerBlockSymbolD2[];
extern int bunkerBlockSymbolD3[];
extern int bunkerBlockSymbol[];
extern int lightningBullet_state1[];
extern int lightningBullet_state2[];
extern int lightningBullet_state3[];
extern int crossBullet_state1[];
extern int crossBullet_state2[];
extern int crossBullet_state3[];
extern int tankBulletSymbol[];
extern int firstColAliveAliens;
extern int lastColAliveAliens;
extern int deadAlien[];
extern int tankDeath1[];
extern int tankDeath2[];
extern int mothership[];
extern int redSpaceshipOriginX;
extern int redSpaceshipOriginY;


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
	int numDigits = (int)(log10((double)score) + 1.0);
	int digArr[numDigits];
	int countDown = numDigits - 1;
	while(countDown >= 0){
		int digit = tempScore % 10;
		digArr[countDown] = digit;
		countDown--;
		tempScore /= 10;
	}

	for(count = 0; count < numDigits; count++){
		drawDigit(SCORENUMBERS_START_X + NUMBER_WIDTH * count + SPACE_BETWEEN_CHARACTERS * count,  SCORENUMBERS_START_Y, digArr[count], SCORENUMBERS_COLOR);
	}

	drawDigit(SCORENUMBERS_START_X + NUMBER_WIDTH * count + SPACE_BETWEEN_CHARACTERS * count,  SCORENUMBERS_START_Y, 0, SCORENUMBERS_COLOR);
}


//draws a single digit in the specified location with the specified color
void drawDigit(int x, int y, int digit, int color){

	int* digitDrawBMP;
	switch(digit){
	case 0:
		digitDrawBMP = score0;
		break;
	case 1:
		digitDrawBMP = score1;
		break;
	case 2:
		digitDrawBMP = score2;
		break;
	case 3:
		digitDrawBMP = score3;
		break;
	case 4:
		digitDrawBMP = score4;
		break;
	case 5:
		digitDrawBMP = score5;
		break;
	case 6:
		digitDrawBMP = score6;
		break;
	case 7:
		digitDrawBMP = score7;
		break;
	case 8:
		digitDrawBMP = score8;
		break;
	default:
		digitDrawBMP = score9;
		break;
	}

	/*int currX = 0;
	int currY = 0;
	for(currX = 0; currX < NUMBER_WIDTH; currX++){
		for(currY = 0; currY < NUMBER_HEIGHT; currY++){
			if(1 && digitDrawBMP[currY] & (1 << (NUMBER_WIDTH - 1 - currX)))
				framePointer0[(y + currY) * SCREEN_X_PIXELS + currX + x] = color;
			else{
				framePointer0[(y + currY) * SCREEN_X_PIXELS + currX + x] = BLACK;
			}
		}
	}*/

	/*int aX;
	int aY;
	for(aX = 0; aX < NUMBER_WIDTH; aX++){
		for(aY = 0; aY < NUMBER_HEIGHT; aY++){
			if(1 && (digitDrawBMP[aY] & (1<<(NUMBER_WIDTH - 1 - aX)))){

				framePointer0[(y+aY) * 640 + x + aX] = color;
			}
			else{
				framePointer0[(y+aY) * 640 + x + aX] = BLACK;
			}
		}
	}*/

}

