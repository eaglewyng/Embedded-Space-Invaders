/*
 * undraw.c
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
extern Bullet tankBullet;
extern int bottomRowAliens[];
extern int alienInOut;

//external integers
extern int alienOriginX;
extern int alienOriginY;
extern int tankOriginX;
extern int tankOriginY;
extern int alienFarRightOffset;
extern int alienArray[];
extern int noAlien[];
extern int tankState;
extern int redSpaceshipStatus;

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
extern int scoreG[];
extern int scoreA[];
extern int scoreM[];
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

void clearRedSpaceship(){
	int x;
	int y;
	for(x = 0; x < RED_SPACESHIP_WIDTH; x++){
		for(y = 0; y < RED_SPACESHIP_HEIGHT; y++){
			//make sure the spaceship is in a drawable area and that there should be a red pixel drawn here
			framePointer0[(y+redSpaceshipOriginY) * 640 + redSpaceshipOriginX + x] = BLACK;
		}
	}
	redSpaceshipStatus = RED_SPACESHIP_INACTIVE;
	insertDC(rand() % MAX_TICS_BETWEEN_RED_SPACESHIP_APPEAR, EVENT_RED_SPACESHIP_APPEAR);
}

void undrawRedSpaceship(int direction){
	int x;
	int y;
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;

	switch(direction){
	case LEFT:
		if(redSpaceshipOriginX > 0){
			leftBound = redSpaceshipOriginX + RED_SPACESHIP_WIDTH;

		}
		else{
			leftBound = 0;
		}
		rightBound = redSpaceshipOriginX + RED_SPACESHIP_WIDTH + RED_SPACESHIP_PIXELS_PER_MOVE - 1;
		upperBound = redSpaceshipOriginY;
		lowerBound = redSpaceshipOriginY + RED_SPACESHIP_HEIGHT - 1;
		break;
	default:
		if(redSpaceshipOriginX < SCREEN_X_PIXELS){
			rightBound = redSpaceshipOriginX - 1;
		}
		else{
			rightBound = SCREEN_X_PIXELS - 1;
		}
		leftBound = redSpaceshipOriginX - RED_SPACESHIP_PIXELS_PER_MOVE;

		upperBound = redSpaceshipOriginY;
		lowerBound = redSpaceshipOriginY + RED_SPACESHIP_HEIGHT - 1;
		break;

	}
	for(x = leftBound; x <= (rightBound); x++){
		for(y = upperBound; y <= (lowerBound); y++){
			framePointer0[y*640 + x] = BLACK;
		}
	}
}


void clearTank(){
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;
	leftBound = tankOriginX;
	rightBound = tankOriginX + TANK_WIDTH - 1;
	upperBound = tankOriginY;
	lowerBound = tankOriginY + TANK_HEIGHT - 1;

	int x, y;
	for(x = leftBound; x <= (rightBound); x++){
		for(y = upperBound; y <= (lowerBound); y++){
			framePointer0[y*640 + x] = BLACK;
			framePointer1[y*640 + x] = BLACK;
		}
	}
}



int undrawTank(int direction){
	int x;
	int y;
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;

	switch(direction){
	case LEFT:
		leftBound = tankOriginX + TANK_WIDTH;
		rightBound = tankOriginX + TANK_WIDTH + PIXELS_PER_MOVE - 1;
		upperBound = tankOriginY;
		lowerBound = tankOriginY + TANK_HEIGHT - 1;
		break;
	case RIGHT:
		leftBound = tankOriginX - PIXELS_PER_MOVE;
		rightBound = tankOriginX - 1;
		upperBound = tankOriginY;
		lowerBound = tankOriginY + TANK_HEIGHT - 1;
		break;
	default:
		leftBound = tankOriginX;
		rightBound = tankOriginX + TANK_WIDTH - 1;
		upperBound = tankOriginY - PIXELS_PER_MOVE;
		lowerBound = tankOriginY + 1;
		break;

	}
	for(x = leftBound; x <= (rightBound); x++){
		for(y = upperBound; y <= (lowerBound); y++){
			framePointer0[y*640 + x] = BLACK;
			framePointer1[y*640 + x] = BLACK;
		}
	}
	return 0;
}



int undrawAliens(int direction){
	int x;
	int y;
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;

	switch(direction){
	case LEFT:
		leftBound = alienOriginX  + (lastColAliveAliens + 1) * ALIEN_WIDTH;
		rightBound = alienOriginX + (lastColAliveAliens + 1) * ALIEN_WIDTH + PIXELS_PER_MOVE - 1;
		upperBound = alienOriginY;
		lowerBound = alienOriginY + ALIEN_HEIGHT * ALIEN_ROWS - 1;
		break;
	case RIGHT:
		leftBound = alienOriginX + (firstColAliveAliens) * ALIEN_WIDTH - PIXELS_PER_MOVE;
		rightBound = alienOriginX + (firstColAliveAliens) * ALIEN_WIDTH - 1;
		upperBound = alienOriginY;
		lowerBound = alienOriginY + ALIEN_HEIGHT * ALIEN_ROWS - 1;
		break;
	default:
		leftBound = alienOriginX;
		rightBound = alienOriginX + (lastColAliveAliens + 1) * ALIEN_WIDTH - 1;
		upperBound = alienOriginY - PIXELS_PER_MOVE;
		lowerBound = alienOriginY + 1;
		break;

	}
	for(x = leftBound; x <= (rightBound); x++){
		for(y = upperBound; y <= (lowerBound); y++){
			if(((y) >= BUNKER_INITIAL_Y) && (y <= (BUNKER_INITIAL_Y+BUNKER_HEIGHT*3))){
				if((((x) >= BUNKER0_INITIAL_X) && (x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x ] == GREEN)){
					framePointer0[(y) * 640 + x ] = GREEN;
				}
				else if((((x) >= BUNKER1_INITIAL_X) && (x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[(y) * 640 + x ] = GREEN;
				}
				else if((((x) >= BUNKER2_INITIAL_X) && (x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[(y) * 640 + x ] = GREEN;
				}
				else if((((x) >= BUNKER3_INITIAL_X) && (x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[(y) * 640 + x] = GREEN;
				}
				else{
					framePointer0[(y) * 640 + x] = BLACK; //BLACK
				}
			}
			else if(((y) >= tankOriginY)){
				if((framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[(y) * 640 + x] = GREEN;//ORANGE

				}
				else{
					framePointer0[(y) * 640 + x] = BLACK;//PURPLE;
				}
			}
			else{
				framePointer0[(y) * 640 + x] = BLACK; //BLACK
			}


			//framePointer0[y*640 + x] = BLACK;
		}
	}

	return 0;
}

void undrawTankBullet(){
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;

	//get the bounds of the coordinates to undraw
	leftBound = tankBullet.x;
	rightBound = tankBullet.x + BULLET_WIDTH - 1;
	upperBound = tankBullet.y + BULLET_HEIGHT;
	lowerBound = tankBullet.y + + BULLET_HEIGHT + TANK_BULLET_PIXELS_PER_MOVE - 1;
	//undraw them (turn them into black background)
	int x, y;
	for(x = leftBound; x <= (rightBound); x++){
		for(y = upperBound; y <= (lowerBound); y++){
			if((((x) >= BUNKER0_INITIAL_X) && (x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
				framePointer1[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER1_INITIAL_X) && (x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
				framePointer1[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER2_INITIAL_X) && (x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
				framePointer1[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER3_INITIAL_X) && (x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
				framePointer1[y*640 + x] = GREEN;
			}
			else{
				framePointer0[y*640 + x] = BLACK;
				framePointer1[y*640 + x] = BLACK;
			}
			//framePointer0[y*640 + x] = BLACK;
		}
	}

}

void clearTankBullet(){
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;
	if(tankBullet.y < 0){
		leftBound = tankBullet.x;
		rightBound = tankBullet.x + BULLET_WIDTH - 1;
		upperBound = 0;
		lowerBound = tankBullet.y + BULLET_HEIGHT;
	}
	else{
		leftBound = tankBullet.x;
		rightBound = tankBullet.x + BULLET_WIDTH - 1;
		upperBound = tankBullet.y;
		lowerBound = tankBullet.y + BULLET_HEIGHT - 1;
	}

	int x, y;
	for(x = leftBound; x <= (rightBound); x++){
		for(y = upperBound; y <= (lowerBound); y++){
			if((((x) >= BUNKER0_INITIAL_X) && (x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER1_INITIAL_X) && (x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER2_INITIAL_X) && (x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER3_INITIAL_X) && (x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else{
				framePointer0[y*640 + x] = BLACK;
				framePointer1[y*640 + x] = BLACK;
			}

			//framePointer0[y*640 + x] = BLACK;
		}
	}
}


void undrawAlienBullet(){
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;

	int i;
	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		//get the bounds of the coordinates to undraw
		leftBound = bulletArray[i].x;
		rightBound = bulletArray[i].x + BULLET_WIDTH - 1;
		upperBound = bulletArray[i].y - ALIEN_BULLETS_PIXELS_PER_MOVE;
		lowerBound = bulletArray[i].y - 1;
		//undraw them (turn them into black background)
		int x, y;
		for(x = leftBound; x <= (rightBound); x++){
			for(y = upperBound; y <= (lowerBound); y++){
				if((((x) >= BUNKER0_INITIAL_X) && (x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[y*640 + x] = GREEN;
				}
				else if((((x) >= BUNKER1_INITIAL_X) && (x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[y*640 + x] = GREEN;
				}
				else if((((x) >= BUNKER2_INITIAL_X) && (x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[y*640 + x] = GREEN;
				}
				else if((((x) >= BUNKER3_INITIAL_X) && (x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
					framePointer0[y*640 + x] = GREEN;
				}
				else{
					framePointer0[y*640 + x] = BLACK;
				}
			}
		}
	}
}

void clearAlienBullet(int bulletIndex){
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;
	if(bulletArray[bulletIndex].y >= BOTTOM_OF_PLAYABLE_AREA - BULLET_HEIGHT){
		leftBound = bulletArray[bulletIndex].x;
		rightBound = bulletArray[bulletIndex].x + BULLET_WIDTH - 1;
		upperBound = bulletArray[bulletIndex].y;
		lowerBound = BOTTOM_OF_PLAYABLE_AREA - 1;
	}
	else{
		leftBound = bulletArray[bulletIndex].x;
		rightBound = bulletArray[bulletIndex].x + BULLET_WIDTH - 1;
		upperBound = bulletArray[bulletIndex].y;
		lowerBound = bulletArray[bulletIndex].y + BULLET_HEIGHT - 1;
	}

	int x, y;
	for(x = leftBound; x <= (rightBound); x++){
		for(y = upperBound; y <= (lowerBound); y++){
			if((((x) >= BUNKER0_INITIAL_X) && (x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER1_INITIAL_X) && (x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER2_INITIAL_X) && (x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else if((((x) >= BUNKER3_INITIAL_X) && (x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y) * 640 + x] == GREEN)){
				framePointer0[y*640 + x] = GREEN;
			}
			else{
				framePointer0[y*640 + x] = BLACK;
			}
		}
	}
}
