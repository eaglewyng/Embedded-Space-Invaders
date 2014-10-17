/*
 * draw.c
 *
 *  Created on: Oct 15, 2014
 *      Author: superman
 */
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
#import "gamestatus.h"


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
extern int scoreV[];
extern int scoreL[];
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
extern int score_digit_0[];
extern int* framePointer0;
extern int* framePointer1;

extern int score;
extern int lives;

void drawLivesText(){
	int x;
	int y;
	//L
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreL[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(LIVES_START_Y + y) * 640 + LIVESTEXT_START_X + x] = STATUSTEXT_COLOR;
			}
		}
	}

	//I
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreI[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(LIVES_START_Y + y )* 640 +
				              (LIVESTEXT_START_X + TEXT_WIDTH + SPACE_BETWEEN_CHARACTERS + x )] = STATUSTEXT_COLOR;
			}
		}
	}

	//V
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreV[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(LIVES_START_Y + y )* 640 +
				              (LIVESTEXT_START_X + TEXT_WIDTH*2 + SPACE_BETWEEN_CHARACTERS *2 + x )] = STATUSTEXT_COLOR;
			}
		}
	}
	//E
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreE[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(LIVES_START_Y + y )* 640 +
				              (LIVESTEXT_START_X + TEXT_WIDTH*3 + SPACE_BETWEEN_CHARACTERS *3 + x )] = STATUSTEXT_COLOR;
			}
		}
	}

	//S
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreS[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(LIVES_START_Y + y )* 640 +
				              (LIVESTEXT_START_X + TEXT_WIDTH*4 + SPACE_BETWEEN_CHARACTERS *4 + x )] = STATUSTEXT_COLOR;
			}
		}
	}
}

void drawLivesTanks(){
	int i;
	for(i = 0; i < MAX_LIVES; i++){
		int strtY = LIVES_START_Y + (i/TANKLIVES_PER_ROW) * (TANK_HEIGHT) + (i/TANKLIVES_PER_ROW) * SPACE_BETWEEN_TANKLIVES_VERTICAL;
		int x, y;
		for(y = 0; y < TANK_HEIGHT; y++){
			for(x = 0; x < TANK_WIDTH; x++){
				if(i < lives && tank[y] & (1<<(TANK_WIDTH-1-x))){
					framePointer0[(strtY + y )* 640 +
					              (TANKLIVES_START_X + TANK_WIDTH*(i%TANKLIVES_PER_ROW) +
					            		  SPACE_BETWEEN_TANKLIVES * (i % TANKLIVES_PER_ROW) + x )] = GREEN;
				}
				else{
					framePointer0[(strtY + y )* 640 +
					              (TANKLIVES_START_X + TANK_WIDTH*(i%TANKLIVES_PER_ROW) +
					            		  SPACE_BETWEEN_TANKLIVES * (i % TANKLIVES_PER_ROW) + x )] = BLACK;
				}
			}
		}
	}
}




