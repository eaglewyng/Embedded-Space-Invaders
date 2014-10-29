/*
 * movement.c
 *
 *  Created on: Sep 25, 2014
 *      Author: superman
 */
#include "space_invaders.h"
#include "movement.h"
#include <stdlib.h>
#include "gamestatus.h"
#include "sound.h"

extern Bunker bunkerArray[];
extern Bullet bulletArray[];
extern Bullet tankBullet;
extern int alienInOut;
extern int tankState;
extern int* framePointer0;
extern int lightningBullet_state1[];
extern int lightningBullet_state2[];
extern int lightningBullet_state3[];
extern int crossBullet_state1[];
extern int crossBullet_state2[];
extern int crossBullet_state3[];
extern int tankBulletSymbol[];
extern int alienArray[];
extern int bigSquidIn[];
extern int bigSquidOut[];
extern int jumpingJackIn[];
extern int jumpingJackOut[];
extern int littleSquidIn[];
extern int littleSquidOut[];
extern int noAlien[];
extern int mothership[];
extern int redSpaceshipStatus;
extern int lastAlienKilled;
extern int lastRowAliveAliens;

int alienOriginX;		//we put the origin on the top left corner
int alienOriginY;
int firstColAliveAliens;
int lastColAliveAliens;
int alienFarRightOffset;
int alienDirection;		//see #defines in movement.h for possible directions
int redSpaceshipOriginX;
int redSpaceshipOriginY;
int redSpaceshipDirection;
int tankOriginX;
int tankOriginY;
int clearRedSpaceshipScoreFlag = 0;
int tankDeathOccurred;




//returns a 0 if successful, returns an error if not
void initializeMovement(){
	firstColAliveAliens = 0;
	lastColAliveAliens = ALIENS_PER_ROW - 1;
	alienOriginX = ALIEN_ORIGIN_X_INITIAL;
	alienOriginY = ALIEN_ORIGIN_Y_INITIAL;
	tankOriginX = TANK_X_INITIAL;
	tankOriginY = TANK_Y_INITIAL;
	alienFarRightOffset = FARRIGHT_OFFSET_INITIAL;
	alienDirection = ALIEN_DIRECTION_INITIAL;
	redSpaceshipOriginY = RED_SPACESHIP_ORIGIN_Y_INITIAL;

}


//updates the locations of the aliens and bullets between every interval of time


void clearRedSpaceshipScore(){
	int row, col;
	for(row = 0; row < TEXT_HEIGHT; row++){
		for(col = 0; col < (TEXT_WIDTH+SPACE_BETWEEN_CHARACTERS)*3; col++){
			framePointer0[(row + redSpaceshipOriginY) * 640 + col +redSpaceshipOriginX] = BLACK;
		}
	}
	clearRedSpaceshipScoreFlag = 0;
}

void moveRedSpaceship(){

	if(!redSpaceshipStatus == RED_SPACESHIP_ALIVE)
		return;
	//call red spaceship sound
	moveRedSpaceshipSound();

	if(redSpaceshipDirection == LEFT){
		redSpaceshipOriginX -= RED_SPACESHIP_PIXELS_PER_MOVE;
		undrawRedSpaceship(LEFT);
		if(redSpaceshipOriginX + RED_SPACESHIP_WIDTH < 0){
			redSpaceshipStatus = RED_SPACESHIP_INACTIVE;
			insertDC(rand() % MAX_TICS_BETWEEN_RED_SPACESHIP_APPEAR, EVENT_RED_SPACESHIP_APPEAR);
			deactiveRedSpaceshipOnScreenSound();
			//TODO add spaceshipleavescreen sound sometime
		}
	}
	//redSpaceships are on the right side of the screen
	else if(redSpaceshipDirection == RIGHT){
		redSpaceshipOriginX += RED_SPACESHIP_PIXELS_PER_MOVE;
		undrawRedSpaceship(RIGHT);
		if(redSpaceshipOriginX >= SCREEN_X_PIXELS){
			redSpaceshipStatus = RED_SPACESHIP_INACTIVE;
			insertDC(rand() % MAX_TICS_BETWEEN_RED_SPACESHIP_APPEAR, EVENT_RED_SPACESHIP_APPEAR);
			deactiveRedSpaceshipOnScreenSound();
			//TODO add spaceshipleavescreen sound sometime
		}
	}

	drawRedSpaceship();

}


//updates the origin location by the predefined amount
void moveAliens(){
	//call move aliens sound
	moveInvadersSound();

	//what to do if aliens are on the left side of the screen
	if(alienDirection == LEFT && (alienOriginX - PIXELS_PER_MOVE < -1*(ALIEN_WIDTH * firstColAliveAliens))){
		alienOriginY += PIXELS_PER_MOVE;
		undrawAliens(DOWN);
		alienDirection = RIGHT;

	}
	//aliens are on the right side of the screen
	else if(alienDirection == RIGHT && (alienOriginX + alienFarRightOffset + PIXELS_PER_MOVE > (SCREEN_X_PIXELS + (ALIENS_PER_ROW - lastColAliveAliens - 1)*ALIEN_WIDTH))){
		alienOriginY += PIXELS_PER_MOVE;
		undrawAliens(DOWN);
		alienDirection = LEFT;
	}
	//aliens are free to move as normal
	else{
		alienOriginX += alienDirection == RIGHT ? PIXELS_PER_MOVE : PIXELS_PER_MOVE * -1;
		undrawAliens(alienDirection);
	}
	int in;
	for(in = 0; in < NUM_BUNKERS; in++){
		drawBunker(in);
	}
	drawAliens();
	firstColAliveAliens = findFirstAliveColumn();
	lastColAliveAliens = findLastAliveColumn();
	lastRowAliveAliens = findLastAliveRow();
	int j;
	for(j = 0; j < NUM_ALIEN_BULLETS; j++){
		drawAlienBullet(j);
	}
	//drawAliens();

	if((alienOriginY + (lastRowAliveAliens+1) * ALIEN_HEIGHT)  >= (BUNKER_INITIAL_Y + 3*BUNKER_HEIGHT)){
		runGameOver(0);
	}
}

//updates the position of the tank
void moveTank(int direction){
	if(tankState == ALIVE_TANK){
		if(direction == LEFT){
			if(!(tankOriginX - TANK_PIXELS_PER_MOVE < 0)){
				tankOriginX -= TANK_PIXELS_PER_MOVE;
			}
			else{
				tankOriginX = 0;
			}

		}
		else{
			if(!(tankOriginX + TANK_PIXELS_PER_MOVE + TANK_WIDTH >= SCREEN_X_PIXELS )){
				tankOriginX += TANK_PIXELS_PER_MOVE;
			}
			else{
				tankOriginX = SCREEN_X_PIXELS - 1 - TANK_WIDTH;
			}
		}

		undrawTank(direction);
	}
	drawTank();


}



void damageBunker(int bunkerNum, int bunkerBlock){
	switch(bunkerBlock){
	case 0:
		if(bunkerArray[bunkerNum].damageStateTopLeft < 5){
			bunkerArray[bunkerNum].damageStateTopLeft=bunkerArray[bunkerNum].damageStateTopLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateTopLeft = 5;
		}
		break;
	case 1:
		if(bunkerArray[bunkerNum].damageStateBlockTopLeft < 5){
			bunkerArray[bunkerNum].damageStateBlockTopLeft=bunkerArray[bunkerNum].damageStateBlockTopLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockTopLeft = 5;
		}
		break;
	case 2:
		if(bunkerArray[bunkerNum].damageStateBlockTopRight < 5){
			bunkerArray[bunkerNum].damageStateBlockTopRight=bunkerArray[bunkerNum].damageStateBlockTopRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockTopRight = 5;
		}
		break;
	case 3:
		if(bunkerArray[bunkerNum].damageStateTopRight < 5){
			bunkerArray[bunkerNum].damageStateTopRight=bunkerArray[bunkerNum].damageStateTopRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateTopRight = 5;
		}
		break;
	case 4:
		if(bunkerArray[bunkerNum].damageStateBlockLeftSide < 5){
			bunkerArray[bunkerNum].damageStateBlockLeftSide=bunkerArray[bunkerNum].damageStateBlockLeftSide + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockLeftSide = 5;
		}
		break;
	case 5:
		if(bunkerArray[bunkerNum].damageStateBottomLeft < 5){
			bunkerArray[bunkerNum].damageStateBottomLeft=bunkerArray[bunkerNum].damageStateBottomLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBottomLeft = 5;
		}
		break;
	case 6:
		if(bunkerArray[bunkerNum].damageStateBottomRight < 5){
			bunkerArray[bunkerNum].damageStateBottomRight=bunkerArray[bunkerNum].damageStateBottomRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBottomRight = 5;
		}
		break;
	case 7:
		if(bunkerArray[bunkerNum].damageStateBlockRightSide < 5){
			bunkerArray[bunkerNum].damageStateBlockRightSide=bunkerArray[bunkerNum].damageStateBlockRightSide + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockRightSide = 5;
		}
		break;
	case 8:
		if(bunkerArray[bunkerNum].damageStateBlockBottomLeft < 5){
			bunkerArray[bunkerNum].damageStateBlockBottomLeft=bunkerArray[bunkerNum].damageStateBlockBottomLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockBottomLeft = 5;
		}
		break;
	case 9:
		if(bunkerArray[bunkerNum].damageStateBlockBottomRight < 5){
			bunkerArray[bunkerNum].damageStateBlockBottomRight=bunkerArray[bunkerNum].damageStateBlockBottomRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockBottomRight = 5;
		}
		break;
	default:
		break;
	}

	drawBunker(bunkerNum);

}

void moveAlienBullets(){
	int i;
	tankDeathOccurred = 0;
	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		if(bulletArray[i].type != INACTIVE_BULLET){ //bullet is active
			bulletArray[i].y += ALIEN_BULLETS_PIXELS_PER_MOVE;
			bulletArray[i].state = (bulletArray[i].state + 1) % 3;
			undrawAlienBullet();
			ScreenPoint hitCoord = alienBulletCollision(i);
			if(bulletArray[i].y >= BOTTOM_OF_PLAYABLE_AREA - BULLET_HEIGHT){
				bulletArray[i].type = INACTIVE_BULLET; //bullet has gone offscreen so deactivate the bullet
				clearAlienBullet(i);
				bulletArray[i].y = -BULLET_HEIGHT; //make sure the tank isn't overwritten by a rogue bullet


				//schedule the next bullet fire
				if(tankDeathOccurred)
					tankDeath();

				else
					insertDC(rand() % MAX_TICS_BETWEEN_ALIEN_FIRE, EVENT_ALIEN_FIRE);
			}
			else if(hitCoord.xcoord >= 0 && hitCoord.ycoord >= 0){
				if(DB_ON1) xil_printf("These are the alien bullet hit coordiantes: %d, %d\n\r",hitCoord.xcoord, hitCoord.ycoord);
				if(DB_ON1) xil_printf("And these are the bullet's x,y coordinates: %d, %d\n\r",bulletArray[i].x, bulletArray[i].y);
				bulletArray[i].type = INACTIVE_BULLET; //deactive the bullet since it's hit either the bunker or the tank
				clearAlienBullet(i);
				if(tankDeathOccurred)
					tankDeath();
				else
					insertDC(rand() % MAX_TICS_BETWEEN_ALIEN_FIRE, EVENT_ALIEN_FIRE);
			}

		}
	}


	drawAlienBullet(-1);
}


int destroyRedSpaceship(int col, int row){
	if(mothership[row % RED_SPACESHIP_HEIGHT] & (1<<(RED_SPACESHIP_WIDTH - 1 - col))){
		if(DB_ON1){xil_printf("You've hit the mothership! DIE! \n\r");}
		redSpaceshipStatus = RED_SPACESHIP_DEAD;
		//call the sound for the red spaceship death
		deactiveRedSpaceshipOnScreenSound();
		destroyRedMothershipSound();
		clearRedSpaceship();
		return 1;
	}
	return 0;
}

ScreenPoint tankBulletCollision(){//returns the number of the bunker you hit
	ScreenPoint myPoint;
	myPoint.xcoord = -1;
	myPoint.ycoord = -1;

	//check if in range of the aliens
	if(((tankBullet.y+BULLET_HEIGHT) >= alienOriginY) && (tankBullet.y <= (alienOriginY+ALIEN_HEIGHT*ALIEN_ROWS))){

		int row, col;
		if(((tankBullet.x+BULLET_WIDTH) >= (alienOriginX + ALIEN_WIDTH * firstColAliveAliens)) && (tankBullet.x <= (alienOriginX + ALIEN_WIDTH * (lastColAliveAliens+1)))){
			for(row = BULLET_HEIGHT-1; row >= 0; row--){
				for(col = 0; col < BULLET_WIDTH; col++){
					if((tankBulletSymbol[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
						if(chooseAlienToKill(tankBullet.x + col, tankBullet.y + row)){
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}
					}
				}
			}
		}
	}

	//check if in range of mothership
	if(((tankBullet.y+BULLET_HEIGHT) >= redSpaceshipOriginY) && (tankBullet.y <= (redSpaceshipOriginY+RED_SPACESHIP_HEIGHT))){
		int row, col;
		if((((tankBullet.x+BULLET_WIDTH) >= redSpaceshipOriginX) && (tankBullet.x <= (redSpaceshipOriginX+RED_SPACESHIP_WIDTH)))){
			for(row = BULLET_HEIGHT-1; row >= 0; row--){
				for(col = 0; col < BULLET_WIDTH; col++){
					if((tankBulletSymbol[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
						if(destroyRedSpaceship(tankBullet.x + col, tankBullet.y + row)){
							addToScore(HIT_RED_SPACESHIP);
							drawRedSpaceshipScore(YELLOW);
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}
					}
				}
			}
		}
	}

	//check if in the range of the bunkers
	if(((tankBullet.y+BULLET_HEIGHT) >= BUNKER_INITIAL_Y) && (tankBullet.y <= (BUNKER_INITIAL_Y+BUNKER_HEIGHT*3))){
		int row, col;
		if((((tankBullet.x+BULLET_WIDTH) >= BUNKER0_INITIAL_X) && (tankBullet.x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 0
			for(row = BULLET_HEIGHT-1; row >= 0; row--){
				for(col = 0; col < BULLET_WIDTH; col++){
					if((tankBulletSymbol[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){

						if(chooseBunkerBlockToDamage(tankBullet.x + col, tankBullet.y + row)){
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}
						/*if(framePointer0[(tankBullet.y + row)*640 + (tankBullet.x+col)] == GREEN){
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}*/
					}
				}
			}
		}
		else if((((tankBullet.x+BULLET_WIDTH) >= BUNKER1_INITIAL_X) && (tankBullet.x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 1
			for(row = BULLET_HEIGHT-1; row >= 0; row--){
				for(col = 0; col < BULLET_WIDTH; col++){
					if((tankBulletSymbol[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
						if(chooseBunkerBlockToDamage(tankBullet.x + col, tankBullet.y + row)){
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}
						/*if(framePointer0[(tankBullet.y + row)*640 + (tankBullet.x+col)] == GREEN){
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}*/
					}
				}
			}
		}
		else if((((tankBullet.x+BULLET_WIDTH) >= BUNKER2_INITIAL_X) && (tankBullet.x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 2
			for(row = BULLET_HEIGHT-1; row >= 0; row--){
				for(col = 0; col < BULLET_WIDTH; col++){
					if((tankBulletSymbol[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
						if(chooseBunkerBlockToDamage(tankBullet.x + col, tankBullet.y + row)){
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}
						/*if(framePointer0[(tankBullet.y + row)*640 + (tankBullet.x+col)] == GREEN){
							myPoint.xcoord = tankBullet.x + col;
							myPoint.ycoord = tankBullet.y + row;
							return myPoint;
						}*/
					}
				}
			}
		}else if((((tankBullet.x+BULLET_WIDTH) >= BUNKER3_INITIAL_X) && (tankBullet.x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 3
			for(row = BULLET_HEIGHT-1; row >= 0; row--){
				for(col = 0; col < BULLET_WIDTH; col++){
					if(chooseBunkerBlockToDamage(tankBullet.x + col, tankBullet.y + row)){
						myPoint.xcoord = tankBullet.x + col;
						myPoint.ycoord = tankBullet.y + row;
						return myPoint;
					}
					/*if(framePointer0[(tankBullet.y + row)*640 + (tankBullet.x+col)] == GREEN){
						myPoint.xcoord = tankBullet.x + col;
						myPoint.ycoord = tankBullet.y + row;
						return myPoint;
					}*/
				}
			}
		}
		else{
			return myPoint; //no collision
		}

	}
	//check if in range of the aliens
	else{
		return myPoint; //no collision
	}
	return myPoint;
}

ScreenPoint alienBulletCollision(int i){
	ScreenPoint myPoint;
	myPoint.xcoord = -1;
	myPoint.ycoord = -1;
	int* bulletBMP;


	if(bulletArray[i].type == SQUIGGLY_BULLET){
		switch(bulletArray[i].state){
		case 0:
			bulletBMP = lightningBullet_state1;
			break;
		case 1:
			bulletBMP = lightningBullet_state2;
			break;
		default:
			bulletBMP = lightningBullet_state3;
			break;
		}
	}
	else{ //(bulletArray[i].type == CROSS_BULLET){
		switch(bulletArray[i].state){
		case 0:
			bulletBMP = crossBullet_state1;
			break;
		case 1:
			bulletBMP = crossBullet_state2;
			break;
		default:
			bulletBMP = crossBullet_state3;
			break;
		}
	}


	//check if in the range of the bunkers
	if((bulletArray[i].y >= BUNKER_INITIAL_Y) && (bulletArray[i].y <= (BUNKER_INITIAL_Y+BUNKER_HEIGHT*3))){
		int row, col;
		if((((bulletArray[i].x+BULLET_WIDTH) >= BUNKER0_INITIAL_X) && (bulletArray[i].x <= (BUNKER0_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 0
			for(row = 0; row <= BULLET_HEIGHT; row++){//row = BULLET_HEIGHT-1; row >= 0; row--
				for(col = 0; col < BULLET_WIDTH; col++){

					if((bulletBMP[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){

						if(chooseBunkerBlockToDamage(bulletArray[i].x + col, bulletArray[i].y + row)){
							myPoint.xcoord = bulletArray[i].x + col;
							myPoint.ycoord = bulletArray[i].y + row;
							return myPoint;
						}
						/*if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] == GREEN){
							myPoint.xcoord = bulletArray[i].x + col;
							myPoint.ycoord = bulletArray[i].y + row;
							return myPoint;
						}*/
					}
				}
			}
		}
		else if((((bulletArray[i].x+BULLET_WIDTH) >= BUNKER1_INITIAL_X) && (bulletArray[i].x <= (BUNKER1_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 1
			for(row = 0; row <= BULLET_HEIGHT; row++){
				for(col = 0; col < BULLET_WIDTH; col++){
					//if((bulletBMP[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
					if(chooseBunkerBlockToDamage(bulletArray[i].x + col, bulletArray[i].y + row)){
						myPoint.xcoord = bulletArray[i].x + col;
						myPoint.ycoord = bulletArray[i].y + row;
						return myPoint;
						//}
						/*if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] == GREEN){
							myPoint.xcoord = bulletArray[i].x + col;
							myPoint.ycoord = bulletArray[i].y + row;
							return myPoint;
						}*/
					}
				}
			}
		}
		else if((((bulletArray[i].x+BULLET_WIDTH) >= BUNKER2_INITIAL_X) && (bulletArray[i].x <= (BUNKER2_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 2
			for(row = 0; row <= BULLET_HEIGHT; row++){
				for(col = 0; col < BULLET_WIDTH; col++){
					if((bulletBMP[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
						if(chooseBunkerBlockToDamage(bulletArray[i].x + col, bulletArray[i].y + row)){
							myPoint.xcoord = bulletArray[i].x + col;
							myPoint.ycoord = bulletArray[i].y + row;
							return myPoint;
						}
						/*if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] == GREEN){
							myPoint.xcoord = bulletArray[i].x + col;
							myPoint.ycoord = bulletArray[i].y + row;
							return myPoint;
						}*/
					}
				}
			}
		}else if((((bulletArray[i].x+BULLET_WIDTH) >= BUNKER3_INITIAL_X) && (bulletArray[i].x <= (BUNKER3_INITIAL_X+4*BUNKER_WIDTH)))){//bunker 3
			for(row = 0; row <= BULLET_HEIGHT; row++){
				for(col = 0; col < BULLET_WIDTH; col++){
					if(chooseBunkerBlockToDamage(bulletArray[i].x + col, bulletArray[i].y + row)){
						myPoint.xcoord = bulletArray[i].x + col;
						myPoint.ycoord = bulletArray[i].y + row;
						return myPoint;
					}
					/*if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] == GREEN){
						myPoint.xcoord = bulletArray[i].x + col;
						myPoint.ycoord = bulletArray[i].y + row;
						return myPoint;
					}*/
				}
			}
		}
		else{
			return myPoint; //no collision
		}

	}
	//check if in range of the tank
	else if((bulletArray[i].y >= tankOriginY) && (bulletArray[i].y <= tankOriginY+TANK_HEIGHT) && (bulletArray[i].x >= tankOriginX) && (bulletArray[i].x <= tankOriginX+TANK_WIDTH)){
		int row, col;
		int type = bulletArray[i].type;
		for(row = 0; row < BULLET_HEIGHT; row++){
			for(col = 0; col < BULLET_WIDTH; col++){
				if((bulletBMP[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
					if(framePointer0[(bulletArray[i].y + row) * 640 + bulletArray[i].x+col] == GREEN){
						tankDeathOccurred = 1;
						myPoint.xcoord = bulletArray[i].x + col;
						myPoint.ycoord = bulletArray[i].y + row;
						return myPoint;
					}
				}
			}
		}
	}
	else{
		return myPoint; //no collision
	}
	return myPoint;

}


int chooseAlienToKill(int x, int y){
	int i;
	int* alienBMP;
	for(i = 0; i < ALIEN_ROWS*ALIENS_PER_ROW; i++){
		if(alienArray[i] > DEAD_ALIEN){
			if((y >= (alienOriginY + i/ALIENS_PER_ROW*ALIEN_HEIGHT)) && (y < (alienOriginY + i/ALIENS_PER_ROW*ALIEN_HEIGHT+ALIEN_HEIGHT))){
				//xil_printf("Made it into checking the alien Y bounds for alien %d.\n\r",i);
				int myLeftBound = alienOriginX + i%ALIENS_PER_ROW*ALIEN_WIDTH;
				int myRightBound = alienOriginX + i%ALIENS_PER_ROW*ALIEN_WIDTH+ALIEN_WIDTH;
				if((x >= (myLeftBound)) && (x < (myRightBound))){
					if(DB_ON1){ xil_printf("You've hit alien %d! Blarg!\n\r",i);}
					/*alienArray[i] = DEAD_ALIEN;
					return 1;*/
					switch(alienArray[i]){
					case BIG_SQUID:
						alienBMP = (alienInOut == 0 ? bigSquidIn :
						bigSquidOut);
						addToScore(HIT_BIG_SQUID);
						break;
					case LITTLE_SQUID:
						alienBMP = (alienInOut == 0 ? littleSquidIn :
						littleSquidOut);
						addToScore(HIT_SMALL_SQUID);
						if(DB_ON1) xil_printf("We're in the little squid space.\n\r");
						break;
					case JUMPING_JACK:
						alienBMP = (alienInOut == 0 ? jumpingJackIn :
						jumpingJackOut);
						addToScore(HIT_JUMPING_JACK);
						break;
					default:
						alienBMP = noAlien;
						break;
					}
					alienArray[i] = DEAD_ALIEN;   //i like boys
					activeInvaderDieSound();
					drawAliens();
					lastAlienKilled = i;

					return 1;
				}
			}

		}
	}

	return 0;
}




int chooseBunkerBlockToDamage(int x, int y){
	if((x >= BUNKER0_INITIAL_X && x < BUNKER0_INITIAL_X+4*BUNKER_WIDTH) && (y >= BUNKER_INITIAL_Y && y < BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)){//bunker 0
		if((x >= BUNKER0_INITIAL_X) && (x < (BUNKER0_INITIAL_X+BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[0].damageStateTopLeft < 4)){
			damageBunker(0,0); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER0_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER0_INITIAL_X+2*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[0].damageStateBlockTopLeft < 4)){
			damageBunker(0,1); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER0_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER0_INITIAL_X+3*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[0].damageStateBlockTopRight < 4)){
			damageBunker(0,2); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER0_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER0_INITIAL_X+4*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[0].damageStateTopRight < 4)){
			damageBunker(0,3); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER0_INITIAL_X) && (x < (BUNKER0_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[0].damageStateBlockLeftSide < 4)){
			damageBunker(0,4); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER0_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER0_INITIAL_X+2*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[0].damageStateBottomLeft < 4)){
			damageBunker(0,5); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER0_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER0_INITIAL_X+3*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[0].damageStateBottomRight < 4)){
			damageBunker(0,6); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER0_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER0_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[0].damageStateBlockRightSide < 4)){
			damageBunker(0,7); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER0_INITIAL_X) && (x < (BUNKER0_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) && (bunkerArray[0].damageStateBlockBottomLeft < 4)){
			damageBunker(0,8); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER0_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER0_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) &&(bunkerArray[0].damageStateBlockBottomRight < 4)){
			damageBunker(0,9); //top left of bunker 0
			return 1;
		}
	}
	else if((x >= BUNKER1_INITIAL_X && x < BUNKER1_INITIAL_X+4*BUNKER_WIDTH) && (y >= BUNKER_INITIAL_Y && y < BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)){//bunker 1
		if((x >= BUNKER1_INITIAL_X) && (x < (BUNKER1_INITIAL_X+BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[1].damageStateTopLeft < 4)){
			damageBunker(1,0); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER1_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER1_INITIAL_X+2*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[1].damageStateBlockTopLeft < 4)){
			damageBunker(1,1); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER1_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER1_INITIAL_X+3*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[1].damageStateBlockTopRight < 4)){
			damageBunker(1,2); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER1_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER1_INITIAL_X+4*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[1].damageStateTopRight < 4)){
			damageBunker(1,3); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER1_INITIAL_X) && (x < (BUNKER1_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[1].damageStateBlockLeftSide < 4)){
			damageBunker(1,4); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER1_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER1_INITIAL_X+2*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[1].damageStateBottomLeft < 4)){
			damageBunker(1,5); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER1_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER1_INITIAL_X+3*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[1].damageStateBottomRight < 4)){
			damageBunker(1,6); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER1_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER1_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[1].damageStateBlockRightSide < 4)){
			damageBunker(1,7); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER1_INITIAL_X) && (x < (BUNKER1_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) && (bunkerArray[1].damageStateBlockBottomLeft < 4)){
			damageBunker(1,8); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER1_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER1_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) &&(bunkerArray[1].damageStateBlockBottomRight < 4)){
			damageBunker(1,9); //top left of bunker 0
			return 1;
		}
	}
	else if((x >= BUNKER2_INITIAL_X && x < BUNKER2_INITIAL_X+4*BUNKER_WIDTH) && (y >= BUNKER_INITIAL_Y && y < BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)){//bunker 1
		if((x >= BUNKER2_INITIAL_X) && (x < (BUNKER2_INITIAL_X+BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[2].damageStateTopLeft < 4)){
			damageBunker(2,0); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER2_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER2_INITIAL_X+2*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[2].damageStateBlockTopLeft < 4)){
			damageBunker(2,1); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER2_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER2_INITIAL_X+3*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[2].damageStateBlockTopRight < 4)){
			damageBunker(2,2); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER2_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER2_INITIAL_X+4*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[2].damageStateTopRight < 4)){
			damageBunker(2,3); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER2_INITIAL_X) && (x < (BUNKER2_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[2].damageStateBlockLeftSide < 4)){
			damageBunker(2,4); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER2_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER2_INITIAL_X+2*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[2].damageStateBottomLeft < 4)){
			damageBunker(2,5); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER2_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER2_INITIAL_X+3*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[2].damageStateBottomRight < 4)){
			damageBunker(2,6); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER2_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER2_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[2].damageStateBlockRightSide < 4)){
			damageBunker(2,7); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER2_INITIAL_X) && (x < (BUNKER2_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) && (bunkerArray[2].damageStateBlockBottomLeft < 4)){
			damageBunker(2,8); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER2_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER2_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) &&(bunkerArray[2].damageStateBlockBottomRight < 4)){
			damageBunker(2,9); //top left of bunker 0
			return 1;
		}
	}
	else if((x >= BUNKER3_INITIAL_X && x < BUNKER3_INITIAL_X+4*BUNKER_WIDTH) && (y >= BUNKER_INITIAL_Y && y < BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)){//bunker 1
		if((x >= BUNKER3_INITIAL_X) && (x < (BUNKER3_INITIAL_X+BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[3].damageStateTopLeft < 4)){
			damageBunker(3,0); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER3_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER3_INITIAL_X+2*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[3].damageStateBlockTopLeft < 4)){
			damageBunker(3,1); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER3_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER3_INITIAL_X+3*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[3].damageStateBlockTopRight < 4)){
			damageBunker(3,2); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER3_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER3_INITIAL_X+4*BUNKER_WIDTH)) && (y >= BUNKER_INITIAL_Y) && (y < (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (bunkerArray[3].damageStateTopRight < 4)){
			damageBunker(3,3); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER3_INITIAL_X) && (x < (BUNKER3_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[3].damageStateBlockLeftSide < 4)){
			damageBunker(3,4); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER3_INITIAL_X+BUNKER_WIDTH)) && (x < (BUNKER3_INITIAL_X+2*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[3].damageStateBottomLeft < 4)){
			damageBunker(3,5); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER3_INITIAL_X+2*BUNKER_WIDTH)) && (x < (BUNKER3_INITIAL_X+3*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[3].damageStateBottomRight < 4)){
			damageBunker(3,6); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER3_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER3_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (bunkerArray[3].damageStateBlockRightSide < 4)){
			damageBunker(3,7); //top left of bunker 0
			return 1;
		}
		else if((x >= BUNKER3_INITIAL_X) && (x < (BUNKER3_INITIAL_X+BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) && (bunkerArray[3].damageStateBlockBottomLeft < 4)){
			damageBunker(3,8); //top left of bunker 0
			return 1;
		}
		else if((x >= (BUNKER3_INITIAL_X+3*BUNKER_WIDTH)) && (x < (BUNKER3_INITIAL_X+4*BUNKER_WIDTH)) && (y >= (BUNKER_INITIAL_Y+2*BUNKER_HEIGHT)) && (y < (BUNKER_INITIAL_Y+3*BUNKER_HEIGHT)) &&(bunkerArray[3].damageStateBlockBottomRight < 4)){
			damageBunker(3,9); //top left of bunker 0
			return 1;
		}
	}
	return 0;
}



void moveTankBullet(){

	if(tankBullet.type != INACTIVE_BULLET){ //bullet is active
		tankBullet.y -= TANK_BULLET_PIXELS_PER_MOVE;
		undrawTankBullet();
		ScreenPoint hitCoord = tankBulletCollision();

		if(tankBullet.y <= TOP_OF_PLAYABLE_AREA ){
			tankBullet.type = INACTIVE_BULLET; //bullet has gone offscreen so deactivate the bullet
			clearTankBullet();
		}
		else if(hitCoord.xcoord >= 0 && hitCoord.ycoord >= 0){
			if(DB_ON1) xil_printf("This is oops: %d, %d\n\r",hitCoord.xcoord, hitCoord.ycoord);
			clearTankBullet();
			tankBullet.type = INACTIVE_BULLET; //bullet has hit something
			if(allAliensDead()){
				drawAliens();
				alienArray[lastAlienKilled] = NO_ALIEN;
				drawAliens();
				runGameOver(1);
			}
			//chooseBunkerBlockToDamage(hitCoord);
		}
		drawTankBullet();
	}

}


