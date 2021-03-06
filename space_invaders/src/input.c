/*
 * input.c
 *
 *  Created on: Oct 7, 2014
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
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "sound.h"
#include "nes_c_controller.h"
#define ASCII_OFFSET 48

extern int alienInOut;

extern XGpio gpPB;   // This is a handle for the push-button GPIO block.

u32 const LEFT_BUTTON_MASK = 0x08;
u32 const UP_BUTTON_MASK = 0x10;
u32 const DOWN_BUTTON_MASK = 0x4;
u32 const MIDDLE_BUTTON_MASK = 0x1;
u32 const RIGHT_BUTTON_MASK = 0x2;
extern int gameOver;



void getKeyboardInput(){
	Xuint8 c;
	if(DB_ON1) xil_printf("Please enter a number: ");
	c = XUartLite_RecvByte(XPAR_RS232_UART_1_BASEADDR);
	xil_printf("%c\n\r",c);

	//logic for what actions to take

	switch(c){
	case '4':
		//move the tank to the left
		//xil_printf("Move the tank left.\n\r");
		moveTank(LEFT);
		break;
	case '6':
		//move the tank to the right
		//xil_printf("Move the tank right.\n\r");
		moveTank(RIGHT);
		break;
	case '8':
		//update the alien position -- shift it like it does in the game
		//xil_printf("Update the aliens.\n\r");
		alienInOut = !alienInOut;
		moveAliens();
		break;
	case '2':
		//kill an alien
		if(DB_ON1) xil_printf("Please enter a number between 0 (upper left) and 54 (lower right), followed by enter: ");
		Xuint8 f1 = XUartLite_RecvByte(XPAR_RS232_UART_1_BASEADDR);
		xil_printf("%c",f1);
		Xuint8 f2 = XUartLite_RecvByte(XPAR_RS232_UART_1_BASEADDR);
		if(f2 == 13){ //the second entry was enter
			xil_printf("\n\r");
			if((f1 < (int)'0' || f1 > (int) '9')){
				if(DB_ON1) xil_printf("Please enter a number between 0 and 54.\n\r");
			}
			else{
				int myIndex = f1-ASCII_OFFSET;
				killAlien(myIndex);
				if(DB_ON1) xil_printf("The index is: %d\n\r", myIndex);

				//int myint = 2;
			}
		}
		else{
			if(DB_ON1) xil_printf("%c",f2);
			Xuint8 f3 = XUartLite_RecvByte(XPAR_RS232_UART_1_BASEADDR);
			xil_printf("\n\r");
			if((f3 != 13)){
				if(DB_ON1) xil_printf("Please complete your line using enter.\n\r");
			}
			else if((f1 > (int) '5')||(f1 < (int)'0' || f1 > (int) '9') || (f2 < (int)'0' || f2 > (int) '9')){
				if(DB_ON1) xil_printf("Please enter a number between 0 and 54.\n\r");
			}
			else{
				int myIndex = (f1-ASCII_OFFSET) * 10 + (f2-ASCII_OFFSET);
				if(DB_ON1) xil_printf("The index is: %d\n\r", myIndex);
				killAlien(myIndex);
			}
		}
		break;
	case '5':
		//fire tank bullet
		if(DB_ON1) xil_printf("Fire the tank bullet.\n\r");
		fireTankBullet();
		break;
	case '3':
		//fire random alien missile
		fireAlienBullet();
		if(DB_ON1) xil_printf("Fire random alien missile.\n\r");
		break;
	case '9':
		//update all bullets
		moveAlienBullets();
		moveTankBullet();
		if(DB_ON1) xil_printf("Update all bullets.\n\r");
		break;
	case '7':
		//erode bunker
		if(DB_ON1) xil_printf("Please select a tank by entering a number between 0 and 3: ");
		Xuint8 b = XUartLite_RecvByte(XPAR_RS232_UART_1_BASEADDR);
		xil_printf("%c\n\r",b);
		//*
		if((b < (int)'0' || b > (int) '3')){
			if(DB_ON1) xil_printf("Please enter a number between 0 and 3.\n\r");
		}
		else{
			int myIndex = b-ASCII_OFFSET;
			damageBunker(myIndex, 0);
		}


		break;
	default:
		if(DB_ON1) xil_printf("Please enter a number between 2 and 9. Thank you.\n\r", c);
		break;
	}

	int ioa;
	for(ioa = 0; ioa < NUM_BUNKERS; ioa ++){
		drawBunker(ioa);
	}
	drawAliens();
	drawTank();
	drawAlienBullet(-1);
}

void getButtonInput(){

	//int currentButtonState = XGpio_DiscreteRead(&gpPB, 1);
	int currentButtonState = NES_C_CONTROLLER_mReadSlaveReg1(XPAR_NES_C_CONTROLLER_0_BASEADDR, 0);

	if (!(currentButtonState & A_MASK)){ //MIDDLE_BUTTON_MASK
		fireTankBullet();
	}
	if(!(currentButtonState & LEFT_MASK)){//LEFT_BUTTON_MASK
		moveTank(LEFT);
	}
	if(!(currentButtonState & RIGHT_MASK)){//RIGHT_BUTTON_MASK
		moveTank(RIGHT);
	}
	if(!(currentButtonState & START_MASK)){//UP_BUTTON_MASK
		adjustVolume(VOLUME_UP);
	}
	if(!(currentButtonState & SELECT_MASK)){//DOWN_BUTTON_MASK
		adjustVolume(VOLUME_DOWN);
	}
	drawTank();
}

void getButtonInputGameOver(){
	int currentButtonState = XGpio_DiscreteRead(&gpPB, 1);
	if (currentButtonState & UP_BUTTON_MASK){
		gameOver = 0;
		initializeGame();
	}
}

void initButtons(){
	XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
	// Set the push button peripheral to be inputs.
	XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
}
