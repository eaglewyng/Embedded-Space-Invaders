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
#include "sound.h"


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
extern int clearRedSpaceshipScoreFlag;
extern int redSpaceshipScore;


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

void drawGameOver(){
	int x, y;

	//G
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreG[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + x] = ORANGE;
			}
		}
	}

	//A
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreA[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + SPACE_BETWEEN_CHARACTERS + TEXT_WIDTH + x] = ORANGE;
			}
		}
	}

	//M
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreM[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + 2*SPACE_BETWEEN_CHARACTERS + 2*TEXT_WIDTH + x] = ORANGE;
			}
		}
	}

	//E
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreE[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + 3*SPACE_BETWEEN_CHARACTERS + 3*TEXT_WIDTH + x] = ORANGE;
			}
		}
	}

	//SPACE


	//O
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreO[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + 5*SPACE_BETWEEN_CHARACTERS + 5*TEXT_WIDTH + x] = ORANGE;
			}
		}
	}

	//V
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreV[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + 6*SPACE_BETWEEN_CHARACTERS + 6*TEXT_WIDTH + x] = ORANGE;
			}
		}
	}

	//E
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreE[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + 7*SPACE_BETWEEN_CHARACTERS + 7*TEXT_WIDTH + x] = ORANGE;
			}
		}
	}

	//R
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreR[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(GAME_OVERTEXT_START_Y + y) * 640 + GAME_OVERTEXT_START_X + 8*SPACE_BETWEEN_CHARACTERS + 8*TEXT_WIDTH + x] = ORANGE;
			}
		}
	}
}

int drawTank(){

	int x = tankOriginX;
	int y = tankOriginY;

	int aX;
	int aY;
	for(aX = 0; aX < TANK_WIDTH; aX++){
		for(aY = 0; aY < TANK_HEIGHT; aY++){
			if(tankState == ALIVE_TANK){
				if(1 && (tank[aY] & (1<<(TANK_WIDTH - 1 - aX)))){

					framePointer0[(y+aY) * 640 + x + aX] = GREEN;
					framePointer1[(y+aY) * 640 + x + aX] = GREEN;
				}
				else{
					framePointer0[(y+aY) * 640 + x + aX] = BLACK;
					framePointer1[(y+aY) * 640 + x + aX] = BLACK;
				}
			}
			else if(tankState == DEAD_TANK1){
				if(1 && (tankDeath1[aY] & (1<<(TANK_WIDTH - 1 - aX)))){

					framePointer0[(y+aY) * 640 + x + aX] = GREEN;
					framePointer1[(y+aY) * 640 + x + aX] = GREEN;
				}
				else{
					framePointer0[(y+aY) * 640 + x + aX] = BLACK;
					framePointer1[(y+aY) * 640 + x + aX] = BLACK;
				}
			}
			else if(tankState == DEAD_TANK2){
				if(1 && (tankDeath2[aY] & (1<<(TANK_WIDTH - 1 - aX)))){

					framePointer0[(y+aY) * 640 + x + aX] = GREEN;
					framePointer1[(y+aY) * 640 + x + aX] = GREEN;
				}
				else{
					framePointer0[(y+aY) * 640 + x + aX] = BLACK;
					framePointer1[(y+aY) * 640 + x + aX] = BLACK;
				}
			}
		}
	}

	if(tankState == DEAD_TANK1){
		tankState = DEAD_TANK2;
	}
	else if(tankState == DEAD_TANK2){
		tankState = DEAD_TANK1;
	}

	return 0;
}

int drawAliens(){
	int row;
	int col;
	int* alienBMP;

	for(row = 0; row < ALIEN_ROWS; row++){
		int y = row * ALIEN_HEIGHT + alienOriginY;

		for(col = firstColAliveAliens; col <= lastColAliveAliens; col++){
			int x = col * ALIEN_WIDTH + alienOriginX;

			int alienType = alienArray[row * ALIENS_PER_ROW + col];
			//populate the alienBMP
			switch(alienType){
			case NO_ALIEN:
				alienBMP = noAlien;
				break;
			case DEAD_ALIEN:
				alienBMP = deadAlien;
				break;
			case BIG_SQUID:
				alienBMP = (alienInOut == 0 ? bigSquidIn :
						bigSquidOut);
				break;
			case LITTLE_SQUID:
				alienBMP = (alienInOut == 0 ? littleSquidIn :
						littleSquidOut);
				break;
			case JUMPING_JACK:
				alienBMP = (alienInOut == 0 ? jumpingJackIn :
						jumpingJackOut);
				break;
			default:
				alienBMP = noAlien;
				break;
			}


			//draw the actual alien
			int aX;
			int aY;
			for(aX = 0; aX < ALIEN_WIDTH; aX++){
				for(aY = 0; aY < ALIEN_HEIGHT; aY++){
					if(1 && (alienBMP[aY] & (1<<(ALIEN_WIDTH - 1 - aX)))){

						framePointer0[(y+aY) * 640 + x + aX] = WHITE;
					}
					else{
						if(((y+aY) >= BUNKER_INITIAL_Y) && (y <= (BUNKER_INITIAL_Y+BUNKER_HEIGHT*3))){
							if((((x+aX) >= BUNKER0_INITIAL_X) && (x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y+aY) * 640 + x + aX] == GREEN)){
								framePointer0[(y+aY) * 640 + x + aX] = GREEN;
							}
							else if((((x+aX) >= BUNKER1_INITIAL_X) && (x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y+aY) * 640 + x + aX] == GREEN)){
								framePointer0[(y+aY) * 640 + x + aX] = GREEN;
							}
							else if((((x+aX) >= BUNKER2_INITIAL_X) && (x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y+aY) * 640 + x + aX] == GREEN)){
								framePointer0[(y+aY) * 640 + x + aX] = GREEN;
							}
							else if((((x+aX) >= BUNKER3_INITIAL_X) && (x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(y+aY) * 640 + x + aX] == GREEN)){
								framePointer0[(y+aY) * 640 + x + aX] = GREEN;
							}
							else{
								if((x+aX) >= tankBullet.x && x <= (tankBullet.x + BULLET_WIDTH) &&  ((y+aY) >= tankBullet.y) && (y <= (tankBullet.y+BULLET_WIDTH)) && framePointer1[(y+aY) * 640 + x + aX] == WHITE)
								{

								}
								else{
									framePointer0[(y+aY) * 640 + x + aX] = BLACK; //BLACK

								}
							}
						}
						else if((x+aX) >= tankOriginX && x <= (tankOriginX + TANK_WIDTH) &&  ((y+aY) >= tankOriginY) && (y <= (tankOriginY+TANK_WIDTH))){
							if((framePointer1[(y+aY) * 640 + x + aX] == GREEN)){
								framePointer0[(y+aY) * 640 + x + aX] = GREEN;//ORANGE
							}
							else{
								framePointer0[(y+aY) * 640 + x + aX] = BLACK;//BLUE;
							}
						}
						else{
							//don't draw over tank bullet
							if((x+aX) >= tankBullet.x && x <= (tankBullet.x + BULLET_WIDTH) &&  ((y+aY) >= tankBullet.y) && (y <= (tankBullet.y+BULLET_HEIGHT)) && framePointer1[(y+aY) * 640 + x + aX] == WHITE){
							}
							else if(((y) >= tankOriginY) && (framePointer1[(y+aY) * 640 + x + aX] == GREEN)){

							}
							else{

								framePointer0[(y+aY) * 640 + x + aX] = BLACK; //GRAY

							}

						}
					}
				}
			}
			if(alienBMP == deadAlien){
				alienArray[row * ALIENS_PER_ROW + col] = NO_ALIEN;
				//deactiveInvaderDieSound();
			}
		}
	}

	return 0;
}

void drawTankBullet(){
	int col;
	int row;
	int type = tankBullet.type;
	for(row = 0; row < BULLET_HEIGHT; row++){
		for(col = 0; col < BULLET_WIDTH; col++){
			switch(type){
			case TANK_BULLET:
				if((tankBulletSymbol[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
					framePointer0[(tankBullet.y + row)*640 + (tankBullet.x+col)] = WHITE;
					if(!(((tankBullet.y+row) >= BUNKER_INITIAL_Y) && (tankBullet.y <= (BUNKER_INITIAL_Y+BUNKER_HEIGHT*3)) && ((((tankBullet.x+col) >= BUNKER0_INITIAL_X) && (tankBullet.x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(tankBullet.y+row) * 640 + tankBullet.x+col] == GREEN)) || (((tankBullet.x+col) >= BUNKER1_INITIAL_X) && (tankBullet.x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(tankBullet.y+row) * 640 + tankBullet.x+col] == GREEN) || (((tankBullet.x+col) >= BUNKER2_INITIAL_X) && (tankBullet.x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(tankBullet.y+row) * 640 + tankBullet.x+col] == GREEN) || (((tankBullet.x+col) >= BUNKER3_INITIAL_X) && (tankBullet.x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH))) && (framePointer1[(tankBullet.y+row) * 640 + tankBullet.x+col] == GREEN))){
						framePointer1[(tankBullet.y + row)*640 + (tankBullet.x+col)] = WHITE;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

/* #define INACTIVE_BULLET 0
#define SQUIGGLY_BULLET 1
#define CROSS_BULLET 2
#define TANK_BULLET 3
#define NUM_ALIEN_BULLETS 4*/
void drawAlienBullet(int myIndex){
	int col;
	int row;
	int i;
	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		int type = bulletArray[i].type;
		for(row = 0; row < BULLET_HEIGHT; row++){
			for(col = 0; col < BULLET_WIDTH; col++){
				switch (type){
				case INACTIVE_BULLET:
					break;
				case SQUIGGLY_BULLET:
					if(bulletArray[i].state == 0){
						if((lightningBullet_state1[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = WHITE;
						}
						else{
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = BLACK;
						}
					}
					else if(bulletArray[i].state == 1){
						if((lightningBullet_state2[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = WHITE;
						}
						else{
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = BLACK;
						}
					}
					else{
						if((lightningBullet_state3[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = WHITE;
						}
						else{
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = BLACK;
						}
					}
					break;
				case CROSS_BULLET:
					if(bulletArray[i].state == 0){
						if((crossBullet_state1[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = WHITE;
						}
						else{
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = BLACK;
						}
					}
					else if(bulletArray[i].state == 1){
						if((crossBullet_state2[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = WHITE;
						}
						else{
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = BLACK;
						}
					}
					else{
						if((crossBullet_state3[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = WHITE;
						}
						else{
							framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] = BLACK;
						}
					}
					break;
				default:
					break;

				}
			}
		}
	}
}

/*int moveTankBullet(){
	y_tankBullet -= BULLET_MOVEMENT;
	if(y_tankBullet <= 0){
		x_tankBullet = 480; //bullet has gone offscreen so deactivate the bullet
	}
}*/


void drawBunker(int bunkerNum){
	int col;
	int row;
	for(row = 0; row < BUNKER_HEIGHT; row++){ //the algorithm draws each block of the bunker, starting with the top row, furthest left
		for(col = 0; col < BUNKER_WIDTH; col++){ //the algorithm determining whether to draw the pixel or not is repeated every block
			if(bunkerArray[bunkerNum].damageStateTopLeft <= 4){ //make sure the bunker block hasn't been completely destroyed
				if ((bunkerTopLeftSymbol[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))) { //make sure we should draw a pixel here based on the bitmap
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						//check if the aliens are over the bunkers --> if they are, we need to make sure we don't draw over an alien pixel
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){ //if there's not an alien pixel already in the buffer, check the damage state and draw the appropriate pixel
							if(bunkerArray[bunkerNum].damageStateTopLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{ //if there is an alien pixel, update what the state of the bunker is in the second frame buffer, but don't update the first frame buffer
							if(bunkerArray[bunkerNum].damageStateTopLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //since the aliens aren't over the bunker, perform our normal checks on what pixel to draw
						if(bunkerArray[bunkerNum].damageStateTopLeft == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}
				}
				else
				{ //no pixel should be drawn, so don't draw any
					//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;

				}
			}
		}
		for(col = BUNKER_WIDTH; col < BUNKER_WIDTH*2; col++){
			if(bunkerArray[bunkerNum].damageStateBlockTopLeft <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBlockTopLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBlockTopLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{//normal
						if(bunkerArray[bunkerNum].damageStateBlockTopLeft == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}
				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}
		}
		for(col = BUNKER_WIDTH*2; col < BUNKER_WIDTH*3; col++){
			if(bunkerArray[bunkerNum].damageStateBlockTopRight <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBlockTopRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBlockTopRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateBlockTopRight == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}
		}

		for(col = BUNKER_WIDTH*3; col < BUNKER_WIDTH*4; col++){
			if(bunkerArray[bunkerNum].damageStateTopRight <= 4){
				if ((bunkerTopRightSymbol[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateTopRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateTopRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateTopRight == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}

		}

	}
	for(row = BUNKER_HEIGHT; row < 2*BUNKER_HEIGHT; row++){
		for(col = 0; col < BUNKER_WIDTH; col++){
			if(bunkerArray[bunkerNum].damageStateBlockLeftSide <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBlockLeftSide == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBlockLeftSide == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateBlockLeftSide == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}

		}
		for(col = BUNKER_WIDTH; col < BUNKER_WIDTH*2; col++){
			if(bunkerArray[bunkerNum].damageStateBottomLeft <= 4){
				if ((bunkerBottomLeftSymbol[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBottomLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBottomLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateBottomLeft == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}

		}
		for(col = BUNKER_WIDTH*2; col < BUNKER_WIDTH*3; col++){
			if(bunkerArray[bunkerNum].damageStateBottomRight <= 4){
				if ((bunkerBottomRightSymbol[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBottomRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBottomRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateBottomRight == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}
		}


		for(col = BUNKER_WIDTH*3; col < BUNKER_WIDTH*4; col++){
			if(bunkerArray[bunkerNum].damageStateBlockRightSide <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBlockRightSide == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBlockRightSide == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateBlockRightSide == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}
		}

	}

	for(row = 2*BUNKER_HEIGHT; row < 3*BUNKER_HEIGHT; row++){
		for(col = 0; col < BUNKER_WIDTH; col++){
			if(bunkerArray[bunkerNum].damageStateBlockBottomLeft <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBlockBottomLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBlockBottomLeft == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateBlockBottomLeft == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}

		}
		for(col = BUNKER_WIDTH; col < BUNKER_WIDTH*2; col++){
			//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
		}
		for(col = BUNKER_WIDTH*2; col < BUNKER_WIDTH*3; col++){
			//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
		}
		for(col = BUNKER_WIDTH*3; col < BUNKER_WIDTH*4; col++){
			if(bunkerArray[bunkerNum].damageStateBlockBottomRight <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))) {
					if((bunkerArray[bunkerNum].y + row) >= alienOriginY && (bunkerArray[bunkerNum].y + row) <= (alienOriginY + ALIEN_ROWS*ALIEN_HEIGHT) && (bunkerArray[bunkerNum].x+col) >= alienOriginX && (bunkerArray[bunkerNum].x+col) <= (alienOriginX + (lastColAliveAliens+1)*ALIEN_WIDTH)){
						if(framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] != WHITE){
							if(bunkerArray[bunkerNum].damageStateBlockBottomRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
						else{
							if(bunkerArray[bunkerNum].damageStateBlockBottomRight == 0){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							}
							else {//bunkerArray[bunkerNum].damageState == 4
								framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
								//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							}
						}
					}
					else{ //normal
						if(bunkerArray[bunkerNum].damageStateBlockBottomRight == 0){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}

				}

			}
			else
			{
				//framePointer1[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				//framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
			}
		}
	}


	if(bunkerArray[bunkerNum].damageStateTopLeft == 4){ //update the damage states after the bunker is complete destroyed so we don't redraw black over bullets, etc.
		bunkerArray[bunkerNum].damageStateTopLeft++;
	}
	if(bunkerArray[bunkerNum].damageStateBlockTopLeft == 4){
		bunkerArray[bunkerNum].damageStateBlockTopLeft++;
	}
	if(bunkerArray[bunkerNum].damageStateBlockTopRight == 4){
		bunkerArray[bunkerNum].damageStateBlockTopRight++;
	}
	if(bunkerArray[bunkerNum].damageStateTopRight == 4){
		bunkerArray[bunkerNum].damageStateTopRight++;
	}
	if(bunkerArray[bunkerNum].damageStateBlockLeftSide == 4){
		bunkerArray[bunkerNum].damageStateBlockLeftSide++;
	}
	if(bunkerArray[bunkerNum].damageStateBottomLeft == 4){
		bunkerArray[bunkerNum].damageStateBottomLeft++;
	}
	if(bunkerArray[bunkerNum].damageStateBottomRight == 4){
		bunkerArray[bunkerNum].damageStateBottomRight++;
	}
	if(bunkerArray[bunkerNum].damageStateBlockRightSide == 4){
		bunkerArray[bunkerNum].damageStateBlockRightSide++;
	}
	if(bunkerArray[bunkerNum].damageStateBlockBottomLeft == 4){
		bunkerArray[bunkerNum].damageStateBlockBottomLeft++;
	}
	if(bunkerArray[bunkerNum].damageStateBlockBottomRight == 4){
		bunkerArray[bunkerNum].damageStateBlockBottomRight++;
	}

}

//draw EVERYTHING
void drawAll(){
	drawAliens();
	drawTank();
	int in;
	for(in = 0; in < NUM_BUNKERS; in++){
		drawBunker(in);
	}
	drawTankBullet();
	for(in = 0; in < NUM_ALIEN_BULLETS; in++){
		drawAlienBullet(in);
	}
	drawGreenLine();
	drawScoreText();
	drawScoreNumbers();
	drawLivesText();
	drawLivesTanks();
}

void drawScoreText(){
	int x;
	int y;
	//S
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreS[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(SCORETEXT_START_Y + y) * 640 + SCORETEXT_START_X + x] = 0xFFFFFFFF;
			}
		}
	}

	//C
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreC[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(SCORETEXT_START_Y + y )* 640 +
				              (SCORETEXT_START_X + TEXT_WIDTH + SPACE_BETWEEN_CHARACTERS + x )] = 0xFFFFFFFF;
			}
		}
	}

	//O
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreO[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(SCORETEXT_START_Y + y )* 640 +
				              (SCORETEXT_START_X + TEXT_WIDTH*2 + SPACE_BETWEEN_CHARACTERS *2 + x )] = 0xFFFFFFFF;
			}
		}
	}
	//R
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreR[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(SCORETEXT_START_Y + y )* 640 +
				              (SCORETEXT_START_X + TEXT_WIDTH*3 + SPACE_BETWEEN_CHARACTERS *3 + x )] = 0xFFFFFFFF;
			}
		}
	}

	//E
	for(y = 0; y < TEXT_HEIGHT; y++){
		for(x = 0; x < TEXT_WIDTH; x++){
			if(scoreE[y] & (1<<(TEXT_WIDTH-1-x))){
				framePointer0[(SCORETEXT_START_Y + y )* 640 +
				              (SCORETEXT_START_X + TEXT_WIDTH*4 + SPACE_BETWEEN_CHARACTERS *4 + x )] = 0xFFFFFFFF;
			}
		}
	}

}

//draw the red spaceship, or the mothership
void drawRedSpaceship(){
	int x;
	int y;
	if(redSpaceshipStatus == RED_SPACESHIP_ALIVE){
		for(x = 0; x < RED_SPACESHIP_WIDTH; x++){
			for(y = 0; y < RED_SPACESHIP_HEIGHT; y++){
				//make sure the spaceship is in a drawable area and that there should be a red pixel drawn here
				if(x + redSpaceshipOriginX >= 0 && x + redSpaceshipOriginX < SCREEN_X_PIXELS){
					if(1 && (mothership[y] & (1<<(RED_SPACESHIP_WIDTH - 1 - x))))
						framePointer0[(y+redSpaceshipOriginY) * 640 + redSpaceshipOriginX + x] = RED;
					else
						framePointer0[(y+redSpaceshipOriginY) * 640 + redSpaceshipOriginX + x] = BLACK;
				}
			}
		}

	}

}

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

	//drawDigit(SCORENUMBERS_START_X + NUMBER_WIDTH * count + SPACE_BETWEEN_CHARACTERS * count,  SCORENUMBERS_START_Y, 0, SCORENUMBERS_COLOR);
}


void drawRedSpaceshipScore(int color){
	int count = 0;
	int tempScore = redSpaceshipScore;
	int numDigits = (int)(log10((double)redSpaceshipScore) + 1.0);
	int digArr[numDigits];
	int countDown = numDigits - 1;
	while(countDown >= 0){
		int digit = tempScore % 10;
		digArr[countDown] = digit;
		countDown--;
		tempScore /= 10;
	}

	for(count = 0; count < numDigits; count++){
		drawDigit(redSpaceshipOriginX + NUMBER_WIDTH * count + SPACE_BETWEEN_CHARACTERS * count,  redSpaceshipOriginY, digArr[count], color);
	}

	clearRedSpaceshipScoreFlag = 1;
	//drawDigit(SCORENUMBERS_START_X + NUMBER_WIDTH * count + SPACE_BETWEEN_CHARACTERS * count,  SCORENUMBERS_START_Y, 0, SCORENUMBERS_COLOR);
}


//draws a single digit in the specified location with the specified color
void drawDigit(int x, int y, int digit, int color){

	int* digitDrawBMP;
	switch(digit){
	case 0:
		digitDrawBMP = score_digit_0;
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


	int currX = 0;
	int currY = 0;
	for(currX = 0; currX < NUMBER_WIDTH; currX++){
		for(currY = 0; currY < NUMBER_HEIGHT; currY++){
			if(digitDrawBMP[currY] & (1 << (NUMBER_WIDTH - 1 - currX)))
				framePointer0[(y + currY) * SCREEN_X_PIXELS + currX + x] = RED;
			else{
				framePointer1[(y + currY) * SCREEN_X_PIXELS + currX + x] = BLACK;
			}
		}
	}

	int aX;
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
	}

}



