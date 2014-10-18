/*
 * space_invaders.c
 *
 *  Created on: Sep 24, 2014
 *      Author: superman
 */
#include "space_invaders.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"

int alienArray[55];	//1 denotes the alien is alive, 0 denotes the alien is dead
Bunker bunkerArray[NUM_BUNKERS];
Bullet bulletArray[NUM_ALIEN_BULLETS];
Bullet tankBullet;
int bottomRowAliens[ALIENS_PER_ROW];
DeltaClock* dcFront;

int lastAlienKilled;

extern int tankState;
int redSpaceshipStatus = 0;
int lastRowAliveAliens;
extern int firstColAliveAliens;
extern int lastColAliveAliens;
extern int alienOriginX;
extern int alienOriginY;
int idleCounter = 0;
extern int timerHandlerCounter;
extern int fitcounter;

int main(){
	srand(time(NULL));

	initializeDisplay();
	initButtons();
	initInterrupts();
	initializeGame();
	runDisplay();
	while(1)
	{
		idleCounter++;
		if(idleCounter % 10000 == 0){
			xil_printf("Adam said to print the idle counter, which is %d, and the timerHandlerCounter, which is %d, and los FIT Tima, vhich iz %d\n\r",idleCounter,timerHandlerCounter,fitcounter);
		}
	}
	;
	return 0;

}

void initializeDC(){
	dcFront = 0;
	int i;

	//insert the alien bullet fires into the delta clock
	for(i = 0; i < 4; i++){
		insertDC(rand() % MAX_TICS_BETWEEN_ALIEN_FIRE, EVENT_ALIEN_FIRE);
	}

	insertDC(rand() % MAX_TICS_BETWEEN_RED_SPACESHIP_APPEAR, EVENT_RED_SPACESHIP_APPEAR);

}

void intermediateInitializeDC(){
	int i;

	//insert the alien bullet fires into the delta clock
	for(i = 0; i < 4; i++){
		if(bulletArray[i].type == INACTIVE_BULLET){
			insertDC(rand() % MAX_TICS_BETWEEN_ALIEN_FIRE, EVENT_ALIEN_FIRE);
		}
	}
	if(redSpaceshipStatus == RED_SPACESHIP_INACTIVE)
		insertDC(rand() % MAX_TICS_BETWEEN_RED_SPACESHIP_APPEAR, EVENT_RED_SPACESHIP_APPEAR);
}

int initializeGame(){
	clearScreen();
	initializeLevel();
	initStatus();
	initializeMovement();
	drawAll();
	return 0;
}

void initializeNextLevel(){
	initializeDC();
	initializeMovement();


	alienOriginX = ALIEN_ORIGIN_X_INITIAL;
	alienOriginY = ALIEN_ORIGIN_Y_INITIAL;

	int i;


	for(i = 0; i < ALIEN_ROWS * ALIENS_PER_ROW; i++){
		int tempAlienRow = getAlienRow(i);
		alienArray[i] = tempAlienRow == 0 ? LITTLE_SQUID:
		tempAlienRow < 3 ? JUMPING_JACK:
		BIG_SQUID;
	}

	firstColAliveAliens = 0;
	lastColAliveAliens = ALIENS_PER_ROW - 1;

	tankState = ALIVE_TANK;

	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		bulletArray[i].x = -1;
		bulletArray[i].y = -1;
		bulletArray[i].type = INACTIVE_BULLET;
		bulletArray[i].state = 0;

	}

	lastAlienKilled = -1;

	lastRowAliveAliens = findLastAliveRow();

	tankBullet.x = -1;
	tankBullet.y = -1;
	tankBullet.type = INACTIVE_BULLET;
	tankBullet.state = 0;

}

int initializeLevel(){
	initializeDC();
	int i;


	for(i = 0; i < ALIEN_ROWS * ALIENS_PER_ROW; i++){
		int tempAlienRow = getAlienRow(i);
		alienArray[i] = tempAlienRow == 0 ? LITTLE_SQUID:
		tempAlienRow < 3 ? JUMPING_JACK:
		BIG_SQUID;
	}


	bunkerArray[0].damageStateTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBlockTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBlockTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBlockRightSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBlockBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBlockLeftSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBlockTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBlockTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBlockRightSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBlockBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBlockLeftSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[1].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBlockTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBlockTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBlockRightSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBlockBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBlockLeftSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[2].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBlockTopLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBlockTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateTopRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBlockRightSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBlockBottomLeft = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBlockLeftSide = BUNKER_INITIAL_DAMAGE;
	bunkerArray[3].damageStateBottomRight = BUNKER_INITIAL_DAMAGE;
	bunkerArray[0].x = BUNKER0_INITIAL_X;
	bunkerArray[1].x = BUNKER1_INITIAL_X;
	bunkerArray[2].x = BUNKER2_INITIAL_X;
	bunkerArray[3].x = BUNKER3_INITIAL_X;
	bunkerArray[0].y = BUNKER_INITIAL_Y;
	bunkerArray[1].y = BUNKER_INITIAL_Y;
	bunkerArray[2].y = BUNKER_INITIAL_Y;
	bunkerArray[3].y = BUNKER_INITIAL_Y;
	tankState = ALIVE_TANK;

	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		bulletArray[i].x = -1;
		bulletArray[i].y = -1;
		bulletArray[i].type = INACTIVE_BULLET;
		bulletArray[i].state = 0;

	}

	tankBullet.x = -1;
	tankBullet.y = -1;
	tankBullet.type = INACTIVE_BULLET;
	tankBullet.state = 0;

	lastAlienKilled = -1;

	lastRowAliveAliens = findLastAliveRow();
	return 0;
}

