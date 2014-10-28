/*
 * space_invaders.h
 *
 *  Created on: Sep 24, 2014
 *      Author: superman
 */
#pragma once
#ifndef SPACE_INVADERS_H_
#define SPACE_INVADERS_H_

#define DB_ON1 0			//debug mode
#define INPUT_MODE 1 		//0 = keyboard, 1 = buttons

#define BULLET_HEIGHT 10
#define ALIEN_HEIGHT 20
#define TANK_HEIGHT 16
#define TANK_WIDTH 26
#define ALIEN_WIDTH 32

#define ALIENS_PER_COL 5
#define ALIENS_PER_ROW 11
#define ALIEN_ROWS 5
#define ALIEN_COLS 11
#define SPACE_BETWEEN_ALIENS_X 5
#define SPACE_BETWEEN_ALIENS_Y 5

#define BUNKER_WIDTH 12
#define BUNKER_HEIGHT 12
#define BUNKER_GAP 10
#define NUM_BUNKERS 4
#define BUNKER0_INITIAL_X 74
#define BUNKER1_INITIAL_X 222
#define BUNKER2_INITIAL_X 370
#define BUNKER3_INITIAL_X 518
#define BUNKER_INITIAL_Y 352
#define BUNKER_INITIAL_DAMAGE 0


#define SCREEN_X_PIXELS 640
#define SCREEN_Y_PIXELS 480

#define NO_ALIEN 0
#define DEAD_ALIEN 1
#define LITTLE_SQUID 2
#define BIG_SQUID 3
#define JUMPING_JACK 4
#define NOTHING -1

#define DOWN 0
#define RIGHT 1
#define LEFT 2
#define PIXELS_PER_MOVE 12
#define TANK_PIXELS_PER_MOVE 5

#define INACTIVE_BULLET 0
#define SQUIGGLY_BULLET 1
#define CROSS_BULLET 2
#define TANK_BULLET 3
#define NUM_ALIEN_BULLETS 4
#define BULLET_HEIGHT 10
#define BULLET_WIDTH 6
#define ALIEN_BULLETS_PIXELS_PER_MOVE 10
#define TANK_BULLET_PIXELS_PER_MOVE 12

#define BOTTOM_OF_PLAYABLE_AREA SCREEN_Y_PIXELS - 30
#define TOP_OF_PLAYABLE_AREA 60

#define RED_SPACESHIP_PIXELS_PER_MOVE 3
#define RED_SPACESHIP_ORIGIN_Y_INITIAL TOP_OF_PLAYABLE_AREA + 2
#define RED_SPACESHIP_WIDTH 32
#define RED_SPACESHIP_HEIGHT 14

#define NUMBER_HEIGHT 14
#define NUMBER_WIDTH 14

#define GREEN 0x0000FF00
#define RED 0x00FF0000
#define WHITE 0xFFFFFFFF
#define BLACK 0x00000000
#define BLUE  0x000000FF
#define STATUSTEXT_COLOR WHITE
#define ORANGE 0x00FF6600
#define YELLOW 0x00FFFF00
#define GRAY   0x00808080
#define PURPLE 0x00800080
#define BROWN  0x00310C0C
#define PINK   0x00FFC0CB



#define EVENT_ALIEN_FIRE 1
#define EVENT_RED_SPACESHIP_APPEAR 2
#define EVENT_TANK_DEATH 3
#define MAX_TICS_AFTER_TANK_DEATH 200
#define MAX_TICS_BETWEEN_ALIEN_FIRE 1500 //1500
#define MAX_TICS_BETWEEN_RED_SPACESHIP_APPEAR 3000
#define MAX_TICS_BETWEEN_LEVELS 200

#define TEXT_WIDTH 14
#define TEXT_HEIGHT 14
#define SPACE_BETWEEN_CHARACTERS 1
#define SCORETEXT_START_X 20
#define SCORETEXT_START_Y 10
#define SCORENUMBERS_START_Y SCORETEXT_START_Y
#define SCORENUMBERS_START_X SCORETEXT_START_X + TEXT_WIDTH * 5 + SPACE_BETWEEN_CHARACTERS * 6
#define SCORENUMBERS_COLOR YELLOW//GREEN
#define SCORETEXT_COLOR WHITE;

#define SPACE_BETWEEN_LIVESTEXT_AND_TANKS 8
#define SPACE_BETWEEN_TANKLIVES 4
#define LIVES_END_X SCREEN_X_PIXELS - SCORETEXT_START_X - SPACE_BETWEEN_LIVESTEXT_AND_TANKS
#define LIVES_START_Y SCORETEXT_START_Y
#define TANKLIVES_PER_ROW 3
#define TANKLIVES_START_X LIVES_END_X - (TANK_WIDTH * TANKLIVES_PER_ROW) - (SPACE_BETWEEN_TANKLIVES * TANKLIVES_PER_ROW)
#define LIVESTEXT_START_X TANKLIVES_START_X - (TEXT_WIDTH * 5  + SPACE_BETWEEN_CHARACTERS * 5) - SPACE_BETWEEN_LIVESTEXT_AND_TANKS
#define SPACE_BETWEEN_TANKLIVES_VERTICAL 4


#define ALIVE_TANK 0
#define DEAD_TANK1 1
#define DEAD_TANK2 2



#define ALIEN_ORIGIN_X_INITIAL 0
#define ALIEN_ORIGIN_Y_INITIAL TOP_OF_PLAYABLE_AREA + 30
#define FARRIGHT_OFFSET_INITIAL 352
#define ALIEN_DIRECTION_INITIAL 1

#define TANK_X_INITIAL 0
#define TANK_Y_INITIAL BOTTOM_OF_PLAYABLE_AREA - 30

#define GREEN_LINE_Y BOTTOM_OF_PLAYABLE_AREA +1
#define RED_SPACESHIP_INACTIVE 0
#define RED_SPACESHIP_DEAD 1
#define RED_SPACESHIP_ALIVE 2

#define GAME_OVERTEXT_START_X 240
#define GAME_OVERTEXT_START_Y 180

#define DC_MAX_FIRES 10

#define AUDIO_BUFFER_BASE_ADDRESS_IN_DDR 0x02000000
#define VOLUME_UP 0x0
#define VOLUME_DOWN 0x1

#define INACTIVE 0x0
#define ACTIVE 0x1
#define TANK_BULLET_SOUND 0xA
#define TANK_EXPLODE_SOUND 0x9
#define INVADER_MOVE_1_SOUND 0x4
#define INVADER_MOVE_2_SOUND 0x3
#define INVADER_MOVE_3_SOUND 0x2
#define INVADER_MOVE_4_SOUND 0x1
#define INVADER_DIE_SOUND 0x8
#define INVADER_KILLED_SOUND 0x7
#define REDSPACESHIP_LEAVE_SOUND 0x6
#define REDSPACESHIP_ONSCREEN_SOUND 0x5
#define NO_SOUND 0x0

typedef struct{
	int x;
	int y;
	int type;
	int state;
} Bullet;

typedef struct{
	int numEntries;
	int triggeredEvents[DC_MAX_FIRES];
} DCResult;

typedef struct {
	int xcoord;
	int ycoord;
} ScreenPoint;

typedef struct {
	int x;
	int y;
	int damageStateTopLeft;
	int damageStateBlockTopLeft;
	int damageStateBlockTopRight;
	int damageStateTopRight;
	int damageStateBlockLeftSide;
	int damageStateBottomLeft;
	int damageStateBottomRight;
	int damageStateBlockRightSide;
	int damageStateBlockBottomLeft;
	int damageStateBlockBottomRight;

} Bunker;

typedef struct deltaclock
{
	struct deltaclock* next;
	int evnum;
	int tics;
} DeltaClock;

typedef struct
{
	int state;
	int currentIndex;
} GameSound;

//--------space_invaders.c--------//
//the main game routines
void initializeGame();
void initializeLevel();
void initializeDC();
void intermediateInitializeDC();
void initializeNextLevel();


//--------utils.c--------//
int getAlienRow(int);
int getAlienCol(int);
int getAlienArrLoc(int,int);


//-------movement.c---------//
void moveAliens();
void initializeMovement();
void moveTank();
void damageBunker(int,int);
void moveAlienBullets();
void moveTankBullet();
int chooseAlienToKill(int, int);
int chooseBunkerBlockToDamage(int,int);
void moveRedSpaceship();
void moveTankBullet();

//-------display.c----------//
int initializeDisplay();
int runDisplay();
void clearScreen();


//-----gameplay.c---------//
void killAlien(int);
void findBottomRowAliens();
int findLastAliveRow();
int fireTankBullet();
void redSpaceshipAppear();
void tankRevive();
void tankDeath();
int allAliensDead();
void deactivateActiveComponents();
void fireAlienBullet();


//-----input.c----------//
void getKeyboardInput();
void getButtonInput();
void initButtons();
void getButtonInputGameOver();


//------interrupts.c-----------//
void initInterrupts();


//------utils.c--------------//
int insertDC(int, int);
int destroyDC();
int incrementDC(int tics);
void clearGameDC();

//-----------gamestatus.c--------------//
void initStatus();
void addToScore(int);
void takeLife();
void runGameOver(int);

//----------draw.c---------------------//
void drawScoreNumbers();
void drawDigit(int, int, int, int);
void drawScoreText();
void drawLivesTanks();
void drawGameOver();
void drawLivesText();
void drawBunker(int);
void drawTankBullet();
int drawTank();
int drawAliens();
void drawGreenLine();
void drawRedSpaceship();
void drawAlienBullet();
void drawScoreText();
void drawAll();

//-------------undraw.c--------------//
int undrawTank(int);
int undrawAliens(int);
void clearTank();
void undrawTankBullet();
void clearTankBullet();
void undrawRedSpaceship(int);
void undrawAlienBullet();
void clearAlienBullet(int);
void clearRedSpaceshipScore();
void clearRedSpaceship();


#endif /* SPACE_INVADERS_H_ */







