/*
 * display.c
 *
 *  Created on: Sep 26, 2014
 *      Author: superman
 */

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

//0 is in, 1 is out
int alienInOut;

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

extern Bullet tankBullet;


int Status;
XAxiVdma videoDMAController;
XAxiVdma_Config * VideoDMAConfig;
XAxiVdma_FrameCounter myFrameConfig;
unsigned int * framePointer0;
unsigned int * framePointer1;
XAxiVdma_DmaSetup myFrameBuffer;


int spot = 0;

//returns 0 if successful, else returns -1.
int initializeDisplay(){
	alienInOut = 0;


	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.

	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
	VideoDMAConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	// Step 2: Initialize the memory structure and the hardware.
	if(XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController, VideoDMAConfig,    XPAR_AXI_VDMA_0_BASEADDR)) {
		xil_printf("VideoDMA Did not initialize.\r\n");
		return -1;
	}
	// Step 3: (optional) set the frame store number.
	if(XST_FAILURE ==  XAxiVdma_SetFrmStore(&videoDMAController, 2, XAXIVDMA_READ)) {
		xil_printf("Set Frame Store Failed.");
		return -1;
	}
	// Initialization is complete at this point.

	// Setup the frame counter. We want two read frames. We don't need any write frames but the
	// function generates an error if you set the write frame count to 0. We set it to 2
	// but ignore it because we don't need a write channel at all.

	myFrameConfig.ReadFrameCount = 2;
	myFrameConfig.ReadDelayTimerCount = 10;
	myFrameConfig.WriteFrameCount =2;
	myFrameConfig.WriteDelayTimerCount = 10;
	Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", Status);
		if(Status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
		return -1;
	}
	// Now we tell the driver about the geometry of our frame buffer and a few other things.
	// Our image is 480 x 640.

	myFrameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
	myFrameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
	myFrameBuffer.Stride = 640*4;         // Dont' worry about the rest of the values.
	myFrameBuffer.FrameDelay = 0;
	myFrameBuffer.EnableCircularBuf=1;
	myFrameBuffer.EnableSync = 0;
	myFrameBuffer.PointNum = 0;
	myFrameBuffer.EnableFrameCounter = 0;
	myFrameBuffer.FixedFrameStoreAddr = 0;
	if(XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ, &myFrameBuffer)) {
		xil_printf("DMA Config Failed\r\n");
		return -1;
	}
	// We need to give the frame buffer pointers to the memory that it will use. This memory
	// is where you will write your video data. The vdma IP/driver then streams it to the HDMI
	// IP.
	myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
	myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;

	if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
			myFrameBuffer.FrameStoreStartAddr)) {
		xil_printf("DMA Set Address Failed Failed\r\n");
		return -1;
	}
	// Print a sanity message if you get this far.
	xil_printf("Woohoo! I made it through initialization.\n\r");
	// Now, let's get ready to start displaying some stuff on the screen.
	// The variables framePointer and framePointer1 are just pointers to the base address
	// of frame 0 and frame 1.
	framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
	framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + 640*480;

	return 0;
}

int runDisplay()
{
	//blank the screen
	// Just paint some large red, green, blue, and white squares in different
	// positions of the image for each frame in the buffer (framePointer0 and framePointer1).
	int row=0, col=0;
	for( row=0; row<480; row++) {
		for(col=0; col<640; col++) {
			framePointer0[row*640 + col] = BLACK;
		}
	}



	// This tells the HDMI controller the resolution of your display (there must be a better way to do this).
	XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

	// Start the DMA for the read channel only.
	if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
		xil_printf("DMA START FAILED\r\n");
	}
	int frameIndex = 0;
	// We have two frames, let's park on frame 0. Use frameIndex to index them.
	// Note that you have to start the DMA process before parking on a frame.
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
	//draw inital states
	drawAliens();
	drawTank();
	int in;
	for(in = 0; in < NUM_BUNKERS; in++){
		drawBunker(in);
	}
	drawTankBullet();

	return 0;
}

void updateScreen(){
	int frameIndex = 0;
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
}







//returns 1 if a tank pixel should be drawn here, and 0 if not
//the row and column should be the current row & column on the screen
int isTankPixelHere(int row, int col){

	//if the the current row and column is not in the alien "grid", return 0
	if(col < tankOriginX || col >= (tankOriginX + TANK_WIDTH)){
		return 0;
	}

	if(row < tankOriginY || row >= (tankOriginY + TANK_HEIGHT) ){
		return 0;
	}



	int rawIdxX = col - tankOriginX;
	int rawIdxY = row - tankOriginY;

	int idxIntoTankY = rawIdxY % TANK_HEIGHT;
	int idxIntoTankX = rawIdxX % TANK_WIDTH;

	return (1 && (tank[idxIntoTankY] & (1<<(TANK_WIDTH - 1 - idxIntoTankX))));

}

int isBunkerPixelHere(int row, int col){
	return 0;
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
		}
	}
	return 0;
}

int drawTank(){

	int x = tankOriginX;
	int y = tankOriginY;

	int aX;
	int aY;
	for(aX = 0; aX < TANK_WIDTH; aX++){
		for(aY = 0; aY < TANK_HEIGHT; aY++){
			if(1 && (tank[aY] & (1<<(TANK_WIDTH - 1 - aX)))){

				framePointer0[(y+aY) * 640 + x + aX] = GREEN;
			}
			else{
				framePointer0[(y+aY) * 640 + x + aX] = BLACK;
			}
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
			framePointer0[y*640 + x] = BLACK;
		}
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
				alienBMP = noAlien;
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

						framePointer0[(y+aY) * 640 + x + aX] = 0xFFFFFFFF;
					}
					else{
						framePointer0[(y+aY) * 640 + x + aX] = BLACK;
					}
				}
			}

		}
	}

	return 0;
}


//draw the bunker with its bunkerArray[bunkerNum].damageState states
void drawBunker(int bunkerNum){
	int col;
	int row;
	for(row = 0; row < BUNKER_HEIGHT; row++){
		for(col = 0; col < BUNKER_WIDTH; col++){
			if(bunkerArray[bunkerNum].damageStateTopLeft <= 4){
				if ((bunkerTopLeftSymbol[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateTopLeft == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
		for(col = BUNKER_WIDTH; col < BUNKER_WIDTH*2; col++){
			if(bunkerArray[bunkerNum].damageStateBlockTopLeft <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateBlockTopLeft == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockTopLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
		for(col = BUNKER_WIDTH*2; col < BUNKER_WIDTH*3; col++){
			if(bunkerArray[bunkerNum].damageStateBlockTopRight <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateBlockTopRight == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
		for(col = BUNKER_WIDTH*3; col < BUNKER_WIDTH*4; col++){
			if(bunkerArray[bunkerNum].damageStateTopRight <= 4){
				if ((bunkerTopRightSymbol[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateTopRight == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateTopRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateTopRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateTopRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
	}
	for(row = BUNKER_HEIGHT; row < 2*BUNKER_HEIGHT; row++){
		for(col = 0; col < BUNKER_WIDTH; col++){
			if(bunkerArray[bunkerNum].damageStateBlockLeftSide <= 4){
				if(bunkerArray[bunkerNum].damageStateBlockLeftSide <= 4){
					if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))) {

						if(bunkerArray[bunkerNum].damageStateBlockLeftSide == 0){
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else if((bunkerArray[bunkerNum].damageStateBlockLeftSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
						}
						else {//bunkerArray[bunkerNum].damageState == 4
							framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
						}
					}
					else
					{
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
			}
		}
		for(col = BUNKER_WIDTH; col < BUNKER_WIDTH*2; col++){
			if(bunkerArray[bunkerNum].damageStateBottomLeft <= 4){
				if ((bunkerBottomLeftSymbol[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateBottomLeft == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(2*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
		for(col = BUNKER_WIDTH*2; col < BUNKER_WIDTH*3; col++){
			if(bunkerArray[bunkerNum].damageStateBottomRight <= 4){
				if ((bunkerBottomRightSymbol[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateBottomRight == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(3*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
		for(col = BUNKER_WIDTH*3; col < BUNKER_WIDTH*4; col++){
			if(bunkerArray[bunkerNum].damageStateBlockRightSide <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateBlockRightSide == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockRightSide == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
	}
	for(row = 2*BUNKER_HEIGHT; row < 3*BUNKER_HEIGHT; row++){
		for(col = 0; col < BUNKER_WIDTH; col++){
			if(bunkerArray[bunkerNum].damageStateBlockBottomLeft <= 4){
				if ((bunkerBlockSymbol[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))) {
					if(bunkerArray[bunkerNum].damageStateBlockBottomLeft == 0){//algorithm for bunkerArray[bunkerNum].damageState sequence
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockBottomLeft == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
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
					if(bunkerArray[bunkerNum].damageStateBlockBottomRight == 0){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 1) && (bunkerBlockSymbolD1[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 2) && (bunkerBlockSymbolD2[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else if((bunkerArray[bunkerNum].damageStateBlockBottomRight == 3) && (bunkerBlockSymbolD3[row % BUNKER_HEIGHT] & (1<<(4*BUNKER_WIDTH-1-col)))){
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = GREEN;
					}
					else {//bunkerArray[bunkerNum].damageState == 4
						framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
					}
				}
				else
				{
					framePointer0[(bunkerArray[bunkerNum].y + row)*640 + (bunkerArray[bunkerNum].x+col)] = BLACK;
				}
			}
		}
	}

	if(bunkerArray[bunkerNum].damageStateTopLeft == 4){
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
				}
				break;
			default:
				break;
			}
		}
	}
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
			framePointer0[y*640 + x] = BLACK;
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
			framePointer0[y*640 + x] = BLACK;
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
				framePointer0[y*640 + x] = BLACK;
			}
		}
	}
}

void clearAlienBullet(int bulletIndex){
	int leftBound;
	int rightBound;
	int upperBound;
	int lowerBound;
	if(bulletArray[bulletIndex].y >= 480 - BULLET_HEIGHT){
		leftBound = bulletArray[bulletIndex].x;
		rightBound = bulletArray[bulletIndex].x + BULLET_WIDTH - 1;
		upperBound = bulletArray[bulletIndex].y;
		lowerBound = SCREEN_X_PIXELS - 1;
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
			framePointer0[y*640 + x] = BLACK;
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




