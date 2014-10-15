/*
 * interrupts.c
 *
 *  Created on: Oct 7, 2014
 *      Author: superman
 */
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "space_invaders.h"
#include "interrupts.h"

u32 const TICS_UNTIL_DEBOUNCED = 5;		//the number of ticks needed from the timer for the switches to be debounced
u32 const MS_PER_TIC = 10;
u32 const TICS_PER_SECOND = 100;
u32 const SECONDS_PER_MINUTE = 60;
u32 const MINUTES_PER_HOUR = 60;
u32 const HOURS_PER_DAY = 24;
u32 const FITCOUNTER_MAX = 999999;
u32 const TICS_PER_ALIEN_LOC_UPDATE = 10;			//1/2 a second before every location update
u32 const TICS_PER_TANK_LOC_UPDATE = 2;
u32 const TICS_PER_BULLET_LOC_UPDATE = 2;
u32 const TICS_PER_SCREEN_UPDATE = 2;
u32 const TICS_PER_RED_SPACESHIP_LOC_UPDATE = 10;

extern int alienInOut;
extern int redSpaceshipStatus;


u32 fitcounter;


XGpio gpPB;   // This is a handle for the push-button GPIO block.

void initInterrupts(){
	//register interrupts
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}


//main interrupt handler--calls the appropriate ISR
void interrupt_handler_dispatcher(){
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
}

void timer_interrupt_handler(){
	incrementDC(1);

	int haveDrawnAliens = 0;
	int haveDrawnBullets = 0;

	//is it time to update the location?
	if(fitcounter % TICS_PER_ALIEN_LOC_UPDATE == 0){
		moveAliens();
		alienInOut = !alienInOut;
	}
	if(fitcounter % TICS_PER_TANK_LOC_UPDATE == 0){
		getButtonInput();
	}
	if(fitcounter % TICS_PER_BULLET_LOC_UPDATE == 0){
		moveAlienBullets();
		moveTankBullet();
	}
	if(redSpaceshipStatus == RED_SPACESHIP_ALIVE && fitcounter % TICS_PER_RED_SPACESHIP_LOC_UPDATE == 0){
		moveRedSpaceship();
	}




	if(fitcounter == FITCOUNTER_MAX){
		fitcounter = 0;
	}
	else{
		fitcounter++;
	}

}
