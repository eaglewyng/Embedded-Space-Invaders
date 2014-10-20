/*
 * gameplay.c
 *
 *  Created on: Oct 1, 2014
 *      Author: superman
 */

#include "space_invaders.h"
#include "gameplay.h"
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


extern int firstColAliveAliens;
extern int lastColAliveAliens;
extern int alienArray[];
extern int bottomRowAliens[];
extern Bullet bulletArray[];
extern int alienOriginX;
extern int alienOriginY;
extern Bullet tankBullet;
extern int tankOriginX;
extern int tankOriginY;
extern int redSpaceshipStatus;
extern int redSpaceshipDirection;
extern int redSpaceshipOriginX;
extern int redSpaceshipOriginY;
extern int gameOver;


int tankState;


//-----------------------------------------functions related to killing aliens---------------------------------------//
void killAlien(int alienIndex){
	alienArray[alienIndex] = NO_ALIEN;
	drawAliens();
	firstColAliveAliens = findFirstAliveColumn();
	lastColAliveAliens = findLastAliveColumn();
	if(DB_ON1) xil_printf("First Alive Column: %d, Last Alive Column: %d", firstColAliveAliens, lastColAliveAliens);

}

/**
 * Algorithm to find the first column with alive aliens. This assumes that the variable firstColAliveAliens
 * is initialized
 */
int findFirstAliveColumn(){
	int i, g;

	for(i = firstColAliveAliens + 44; i <= lastColAliveAliens + 44; i++){
		for(g = i; g >= 0 ; g -= 11){
			if(alienArray[g] > DEAD_ALIEN){
				return i - 44;
			}
		}
	 }
	return -1;
}


/**
 * Algorithm to find the last column with alive aliens. This assumes that the variable last ColAliveAliens
 * is initialized
 */
int findLastAliveColumn(){
	int i, g;
	for(i = lastColAliveAliens + 44; i >= firstColAliveAliens + 44; i--){
		for(g = i; g >= 0 ; g -= 11){
			if(alienArray[g] > DEAD_ALIEN){
				return i - 44;
			}
		}
	 }
	return -1;
}

/*Algorithm to find the last row with alive aliens.*/
int findLastAliveRow(){
	int row, col;
	for(row = ALIEN_ROWS-1; row >= 0; row--){
		for(col = ALIENS_PER_ROW-1; col >= 0 ; col --){
			if(alienArray[row*ALIENS_PER_ROW+col] > DEAD_ALIEN){
				return row;
			}
		}
	 }
	return -1;
}


//--------------------------------------------functions related to tank bullets----------------------------------------//
int fireTankBullet(){
	if(tankBullet.type == INACTIVE_BULLET && tankState == 0){
		tankBullet.type = TANK_BULLET;
		tankBullet.x = tankOriginX + TANK_WIDTH / 2 - 3;
		tankBullet.y = tankOriginY - BULLET_HEIGHT;
		tankBullet.state = 0;
		drawTankBullet();
	}
	return 0;
}

//--------------------------------------------functions related to alien bullets---------------------------------------//
//Algorithm to find the aliens in the bottom of their column
   //The max number of bottom row aliens is 11
   //Perhaps the best way to get all the bottom row aliens is to go back through the array starting from the last alien and add until either we have 11 aliens or we went through all the spots
int findBottomRowAliens(){
	int o;
	for(o = 0; o < ALIENS_PER_ROW; o++){
		bottomRowAliens[o] = -1;
	}
	 int i, g;
	 int counter = 0;
	 for(i = 44; i < 55; i++){
		for(g = i; g >= 0 ; g -= 11){
			if(alienArray[g] > DEAD_ALIEN){
				bottomRowAliens[counter] = g;
				counter++;
				break; //because we've found an alien in this column
			}
		}
		if(counter > 10){ //means we've found 11 bottom row aliens
				break;
		 }
	 }

	 return 0;
 }

//check to make sure there's not two alien bullets in the same spot
int checkDuplicateAlienBullet(int index){
	int i;
	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		if(bulletArray[i].type != INACTIVE_BULLET && i != index){
			if((bulletArray[i].x == bulletArray[index].x) && (bulletArray[i].y == bulletArray[index].y)){
				return 1;
			}
		}
	}
	return 0;
}


//Algorithm to fire the bullets

void fireAlienBullet(){
	//find the first inactive bullet (max of 4 bullets can be active)

	int i;
	int bullet = -1; //means all the bullets are active
	for(i = 0; i < NUM_ALIEN_BULLETS; i++)
	{
		if(bulletArray[i].type == INACTIVE_BULLET){ //bullet is not active
			bullet = i;
		}
	}

	if(bullet > -1)
	{
		//find a bottom row alien
		//int bottomRowAliens[11]; --> this is a global array
		findBottomRowAliens();
		int alive = 0;
		int j;
		for(j = 10; j >= 0; j--){
			if(bottomRowAliens[j] > -1){
				alive = 1;
				break;
			}
		}
		if(alive == 1){ //there is an alien who is alive, then select one and fire the bullet from them

			int temp;
			temp = rand() % (j+1);
			if(DB_ON1) xil_printf("\nrandom number was %d", temp);
			int myAlien = bottomRowAliens[temp];
			temp = rand() % 2;
			bulletArray[bullet].type = temp == 0 ? SQUIGGLY_BULLET : CROSS_BULLET;
			bulletArray[bullet].x = alienOriginX + (ALIEN_WIDTH * (myAlien % ALIENS_PER_ROW) + ALIEN_WIDTH/2) - BULLET_WIDTH/2;
			bulletArray[bullet].y = alienOriginY + (myAlien / ALIENS_PER_ROW + 1) * (ALIEN_HEIGHT);
			drawAlienBullet(bullet);
			moveAlienBullets();

			//set the bullet x coordinate to the middle of the alien block and the y to the bottom of the alien block
		}
	}
}

void redSpaceshipAppear(){
	redSpaceshipStatus = RED_SPACESHIP_ALIVE;
	if(redSpaceshipDirection == LEFT){
		redSpaceshipDirection = RIGHT;
		redSpaceshipOriginX = 0 - RED_SPACESHIP_WIDTH;

	}
	else{
		redSpaceshipDirection = LEFT;
		redSpaceshipOriginX = SCREEN_X_PIXELS;
	}
}

void tankDeath(){
	takeLife();
	if(!gameOver){
		tankState = DEAD_TANK1;
		clearGameDC();
		insertDC(MAX_TICS_AFTER_TANK_DEATH ,EVENT_TANK_DEATH);
		deactivateActiveComponents();

	}



}

void tankRevive(){
	clearTank();
	tankOriginX = TANK_X_INITIAL;
	tankState = 0;
	initializeDC();
	drawTank();

}

int allAliensDead(){
	int i;
	for(i = 0; i < ALIENS_PER_ROW*ALIENS_PER_COL; i++){
		if(alienArray[i] > DEAD_ALIEN){
			return 0;
		}
	}
	return 1;
}

//Deactivates the tankbullets, alienbullets, and red spaceship.
//DOES NOT ADD THEM BACK INTO THE DELTA CLOCK, NOR DOES IT CLEAR THE DELTA CLOCK
void deactivateActiveComponents(){
	int i;
	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		bulletArray[i].type = INACTIVE_BULLET;
		clearAlienBullet(i);
	}
	tankBullet.type = INACTIVE_BULLET;
	clearTankBullet();
	redSpaceshipStatus = RED_SPACESHIP_INACTIVE;
	clearRedSpaceship();

}
