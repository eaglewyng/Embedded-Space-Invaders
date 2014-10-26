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
#include "math.h"
#include "xac97_l.h"
#include "xparameters.h"

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
extern int tankState;
extern int redSpaceshipStatus;
extern int score;
extern int lastAlienKilled;



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
extern int score_digit_0[];
extern int score1[];
extern int score2[];
extern int score3[];
extern int score4[];
extern int score5[];
extern int score6[];
extern int score7[];
extern int score8[];
extern int score9[];

extern int scoreI[];
extern int scoreS[];
extern int scoreO[];
extern int scoreR[];
extern int scoreC[];
extern int scoreE[];

extern Bullet tankBullet;

extern int volumeAttenuation;

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

	//Initialize the AC97 Sound Chip
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MID);




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

void drawGreenLine(){
	int x;
	int y = GREEN_LINE_Y;
	for(x = 0; x < SCREEN_X_PIXELS; x++){
		framePointer0[y*640 + x] = GREEN;
		framePointer1[y*640 + x] = GREEN;
	}
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
			framePointer1[row*640 + col] = BLACK;
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
	for(in = 0; in < NUM_ALIEN_BULLETS; in++){
		drawAlienBullet(in);
	}
	drawGreenLine();
	drawScoreText();
	drawScoreNumbers();
	drawLivesText();
	drawLivesTanks();
	return 0;
}

void clearScreen(){
	int x, y;
	for(y = 0; y < SCREEN_Y_PIXELS; y++){
		for(x = 0; x < SCREEN_X_PIXELS; x++){
			framePointer0[y*SCREEN_X_PIXELS + x] = BLACK;
		}
	}
}



void updateScreen(){
	int frameIndex = 0;
	if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
		xil_printf("vdma parking failed\n\r");
	}
}


















