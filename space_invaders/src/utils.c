/*
 * utils.c
 *
 *  Created on: Sep 24, 2014
 *      Author: superman
 */

#include "space_invaders.h"
#include <stdlib.h>

extern DeltaClock* dcFront;
DCResult dcResult;



//functions to convert alien row and column to a location and back again

int getAlienRow(int arrLoc){
	return arrLoc / (ALIENS_PER_ROW);
}

int getAlienCol(int arrLoc){
	return arrLoc % ALIENS_PER_ROW;
}

int getAlienArrLoc(int row, int col){
	return row * ALIENS_PER_ROW + col;
}



//----------------------code for the delta clock---------------------//


int destroyDC(DeltaClock* dc){
	while(dc){
		DeltaClock* dcBefore = dc;
		dc = dc->next;
		free(dcBefore);
	}

	return 1;
}

int incrementDC(int tics){
	
	dcResult.numEntries = 0;
	
	int retcode = -1;
	int rem = tics;
	DeltaClock* currClk = dcFront;
	int finished = 0;
	while(!finished){

		if(currClk == 0){
			break;
		}

		currClk -> tics = currClk -> tics - rem;
		if(currClk->tics <= 0){
			//there is no time left on this clock.
			//free this clock and decrement the next one
			rem = currClk->tics * -1;
			DeltaClock* prevClk = currClk;
			if(dcResult.numEntries < DC_MAX_FIRES){
				dcResult.triggeredEvents[dcResult.numEntries] = currClk->evnum;
				dcResult.numEntries++;
			}
			currClk = currClk->next;
			//advance the front of the clock
			dcFront = currClk;



			free(prevClk);

		}
		else{
			//there is still time on this clock; keep it
			finished = 1;
		}
	}
	return dcResult.numEntries;
}

int insertDC(int tics, int evnum){
	DeltaClock* dc = dcFront;

	DeltaClock* dcNew = (DeltaClock*)malloc(sizeof(DeltaClock));
	dcNew->evnum = evnum;
	dcNew->tics = tics;


	if(!dc){
		dcNew->next = dcFront;
		//Set front
		dcFront = dcNew;
		return 1;
	}

	DeltaClock* dcBefore = 0;

	while(dc){
		if(dcNew->tics < dc->tics){
			dcNew->next = dc;
			if(dcBefore) dcBefore->next = dcNew;
			else dcFront = dcNew;

			//update the count on each dc node after this one
			dc->tics = dc->tics - dcNew->tics;
			dc = dc->next;

			return 1;
		}
		else{
			dcNew->tics = dcNew->tics - dc->tics;
			dcBefore = dc;
			dc = dc->next;
		}
	}

	dcNew->next = 0;
	if(dcBefore != 0) dcBefore->next = dcNew;


	return 1;
}

void clearGameDC(){
	dcFront = 0;
}



