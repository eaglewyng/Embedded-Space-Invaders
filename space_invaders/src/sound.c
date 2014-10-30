/*
 * sound.c
 *
 *  Created on: Oct 25, 2014
 *      Author: superman
 */
#include "sound.h"
#include "space_invaders.h"
#include "xparameters.h"
#include "xac97_l.h"



int alienSound1_soundData[] = {1, 2, 3, 4};  // Sound data go here.
int alienSound1_numberOfSamples = 50;        // This tells you how many samples you have.
int alienSound1_sampleRate = 1000;           // This is the sample rate.
int volumeAttenuation = AC97_VOL_ATTN_46_0_DB;
extern int redSpaceship_soundData[];
extern int redSpaceship_numberOfSamples;
extern int redSpaceship_sampleRate;
extern int tankFire_sampleRate;
extern int tankFire_numberOfSamples;
extern int tankFire[];
GameSound invaderDieSound;
extern int invaderDie_soundData[];
extern int invaderDie_numberOfSamples;
extern int invaderDie_sampleRate;
GameSound invaderKilledSound;
extern int invaderKilled_soundData[];
extern int invaderKilled_numberOfSamples;
extern int invaderKilled_sampleRate;
GameSound invaderMove1Sound;
extern int invaderMove1_soundData[];
extern int invaderMove1_numberOfSamples;
extern int invaderMove1_sampleRate;
GameSound invaderMove2Sound;
extern int invaderMove2_soundData[];
extern int invaderMove2_numberOfSamples;
extern int invaderMove2_sampleRate;
GameSound invaderMove3Sound;
extern int invaderMove3_soundData[];
extern int invaderMove3_numberOfSamples;
extern int invaderMove3_sampleRate;
GameSound invaderMove4Sound;
extern int invaderMove4_soundData[];
extern int invaderMove4_numberOfSamples;
extern int invaderMove4_sampleRate;
GameSound redSpaceshipLeaveSound;
extern int redSpaceshipLeave_soundData[];
extern int redSpaceshipLeave_numberOfSamples;
extern int redSpaceshipLeave_sampleRate;
GameSound redSpaceshipOnScreenSound;
extern int redSpaceshipOnScreen_soundData[];
extern int redSpaceshipOnScreen_numberOfSamples;
extern int redSpaceshipOnScreen_sampleRate;
GameSound tankFireBulletSound;
extern int tankFire_soundData[];
extern int tankFire_numberOfSamples;
extern int tankFire_sampleRate;
GameSound tankExplodeSound;
extern int tankExplodeFaster_soundData[];
extern int tankExplodeFaster_numberOfSamples;
extern int tankExplodeFaster_sampleRate;
GameSound redSpaceshipHitSound;
extern int redSpaceshipHit_soundData[];
extern int redSpaceshipHit_numberOfSamples;
extern int redSpaceshipHit_sampleRate;
//int activeSound = 0;

int aliensSoundState;

void initSound(){
	aliensSoundState = ALIENS_BEGINNING_SOUNDSTATE;
}

void activeRedSpaceshipHitSound(){
	redSpaceshipHitSound.state = ACTIVE;
	xil_printf("We've activated redSpaceshipHit.\n\r");
}

void deactiveRedSpaceshipHitSound(){
	redSpaceshipHitSound.state = INACTIVE;
	xil_printf("We've deactivated redSpaceshipHit.\n\r");
}




void moveInvadersSound(){
	invaderMove1Sound.state = ACTIVE;

}

void moveRedSpaceshipSound(){
	redSpaceshipOnScreenSound.state = ACTIVE;
}

void deactiveRedSpaceshipOnScreenSound(){ //some of these we shouldn't need this because it should deactive itself after it writes all its samples
	redSpaceshipOnScreenSound.state = INACTIVE;
}

void activeInvaderDieSound(){
	invaderDieSound.state = ACTIVE;
	//xil_printf("Dangnabbers, we Actived invaderDieSound!\n\r");
}

void deactiveInvaderDieSound(){ //some of these we shouldn't need this because it should deactive itself after it writes all its samples
	invaderDieSound.state = INACTIVE;
	//xil_printf("We deactived invaderDieSound.\n\r");
}

void activeInvaderKilledSound(){
	invaderKilledSound.state = ACTIVE;
}

void deactiveInvaderKilledSound(){ //some of these we shouldn't need this because it should deactive itself after it writes all its samples
	invaderKilledSound.state = INACTIVE;
}

void fireTankBulletSound(){
/*	int i = 0;
	int j = 0;
	int numberOfSamples = tankFire_numberOfSamples;
	while(1){
		while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){//!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankFire_sampleRate);
			int sample = tankFire[i];

			if(i >= numberOfSamples){
				sample = 0;
			}
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample | (sample<<16));
			j++;
			//			if(j == 4){
			j=0;
			i++;

			if(i > numberOfSamples*4){
				i = 0;
			}
			//			}
		}

	}
	XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);*/
	//xil_printf("The tankFireBulletSound.state is now ACTIVE.\n\r");
	tankFireBulletSound.state = ACTIVE;
}

void deactiveFireTankBulletSound(){
	tankFireBulletSound.state = INACTIVE;
	tankFireBulletSound.currentIndex = 0;
}


void tankDeathSound(){
	xil_printf("Huh, we do active tankExplodeSound.\n\r");
	tankExplodeSound.state = ACTIVE;
	deactiveFireTankBulletSound();
}

void deactiveTankDeathSound(){ //some of these we shouldn't need this because it should deactive itself after it writes all its samples
	tankExplodeSound.state = INACTIVE;
}

void destroyRedMothershipSound(){
	//TODO replace this with the actual mothership sound
	redSpaceshipHitSound.state = ACTIVE;
	xil_printf("We've activated redSpaceshipHit.\n\r");
}

void deactiveDestroyRedMothershipSound(){//some of these we shouldn't need this because it should deactive itself after it writes all its samples
	redSpaceshipHitSound.state = INACTIVE;
}


void activeInvaderMoveSound(){
	//assign everything so there isn't the possibility of two of these running
	//at once
	switch(aliensSoundState){
		case 1:
			invaderMove1Sound.state = ACTIVE;
			invaderMove2Sound.state = INACTIVE;
			invaderMove3Sound.state = INACTIVE;
			invaderMove4Sound.state = INACTIVE;
			break;
		case 2:
			invaderMove1Sound.state = INACTIVE;
			invaderMove2Sound.state = ACTIVE;
			invaderMove3Sound.state = INACTIVE;
			invaderMove4Sound.state = INACTIVE;
			break;
		case 3:
			invaderMove1Sound.state = INACTIVE;
			invaderMove2Sound.state = INACTIVE;
			invaderMove3Sound.state = ACTIVE;
			invaderMove4Sound.state = INACTIVE;
			break;
		default:
			invaderMove1Sound.state = INACTIVE;
			invaderMove2Sound.state = INACTIVE;
			invaderMove3Sound.state = INACTIVE;
			invaderMove4Sound.state = ACTIVE;
			break;
	}

}


void adjustVolume(int volume_direction){
	if((volume_direction == VOLUME_UP) && (volumeAttenuation <= (AC97_VOL_ATTN_46_0_DB)) && (volumeAttenuation > (AC97_VOL_ATTN_0_DB))){
		volumeAttenuation -= SOUND_INCR;
		//volumeAttenuation = AC97_VOL_ATTN_0_DB;
	}
	else if((volume_direction == VOLUME_DOWN) && (volumeAttenuation >= (AC97_VOL_ATTN_0_DB)) && (volumeAttenuation < (AC97_VOL_ATTN_46_0_DB))){ //
		volumeAttenuation += SOUND_INCR;
		xil_printf("Volume is %d, and the limit should be %d\n\r",volumeAttenuation,AC97_VOL_ATTN_46_0_DB);
		//volumeAttenuation = AC97_VOL_ATTN_46_0_DB;
	}
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, volumeAttenuation);
}

void updateSoundSampleRate(){
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
}

void fillFIFO(){
	int sample = 0;
	int counter = 0;
	//xil_printf("Well, we got into fillFIFO.\n\r");
	//xil_printf("The state of tankFireBulletSound is %d\n\r",tankFireBulletSound.state);
	while(counter < 256){//!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)

		if(tankExplodeSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankExplodeFaster_sampleRate);
			sample = tankExplodeFaster_soundData[tankExplodeSound.currentIndex];
			tankExplodeSound.currentIndex++;
		}
		else if(invaderDieSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderDie_sampleRate);
			sample = invaderDie_soundData[invaderDieSound.currentIndex];
			invaderDieSound.currentIndex++;
		}
		else if(invaderKilledSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderKilled_sampleRate);
			sample = invaderKilled_soundData[invaderKilledSound.currentIndex];
			invaderKilledSound.currentIndex++;
		}
		else if(tankFireBulletSound.state == ACTIVE){
			//xil_printf("Well, we got into tankFireBulletSound.\n\r");
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankFire_sampleRate);
			sample = tankFire_soundData[tankFireBulletSound.currentIndex];
			tankFireBulletSound.currentIndex++;
			//xil_printf("This is tankFireBulletSound's currentIndex: %d.\n\r", tankFireBulletSound.currentIndex);
		}
		else if(redSpaceshipOnScreenSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, redSpaceshipOnScreen_sampleRate);
			sample = redSpaceshipOnScreen_soundData[redSpaceshipOnScreenSound.currentIndex];
			redSpaceshipOnScreenSound.currentIndex++;
		}
		else if(redSpaceshipHitSound.state == ACTIVE){ //this had been an if before...
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, redSpaceshipHit_sampleRate);
			sample = redSpaceshipHit_soundData[redSpaceshipHitSound.currentIndex];
			redSpaceshipHitSound.currentIndex++;
		}
		else if(redSpaceshipLeaveSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, redSpaceshipLeave_sampleRate);
			sample = redSpaceshipLeave_soundData[redSpaceshipLeaveSound.currentIndex];
			redSpaceshipLeaveSound.currentIndex++;
		}

		else if(invaderMove1Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove1_sampleRate);
			sample = 8*invaderMove1_soundData[invaderMove1Sound.currentIndex];
			invaderMove1Sound.currentIndex++;
		}
		else if(invaderMove2Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove2_sampleRate);
			sample = 8*invaderMove2_soundData[invaderMove2Sound.currentIndex];
			invaderMove2Sound.currentIndex++;
		}
		else if(invaderMove3Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove3_sampleRate);
			sample = 8*invaderMove3_soundData[invaderMove3Sound.currentIndex];
			invaderMove3Sound.currentIndex++;
		}
		else if(invaderMove4Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove4_sampleRate);
			sample = 8*invaderMove4_soundData[invaderMove4Sound.currentIndex];
			invaderMove4Sound.currentIndex++;
		}
		else{
			sample = 8*127;
			//xil_printf("Ve print nozink!\n\r");
		}

		updateSoundStates();
		sample = sample | (sample<<16);
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample);
		counter++;
	}
	return;
}

void updateSoundStates(){
	if(tankFireBulletSound.currentIndex >= tankFire_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(tankExplodeSound.currentIndex >= tankExplodeFaster_numberOfSamples){
		//xil_printf("Well, we do deactive tankExplodeFasterSound.\n\r");
		tankExplodeSound.state = INACTIVE;
		tankExplodeSound.currentIndex = 0;
	}
	if(invaderDieSound.currentIndex >= invaderDie_numberOfSamples){
		invaderDieSound.state = INACTIVE;
		invaderDieSound.currentIndex = 0;
	}
	if(invaderKilledSound.currentIndex >= invaderKilled_numberOfSamples){
		invaderKilledSound.state = INACTIVE;
		invaderKilledSound.currentIndex = 0;
	}
	if(redSpaceshipLeaveSound.currentIndex >= redSpaceshipLeave_numberOfSamples){
		redSpaceshipLeaveSound.state = INACTIVE;
		redSpaceshipLeaveSound.currentIndex = 0;
	}
	if(redSpaceshipOnScreenSound.currentIndex >= redSpaceshipOnScreen_numberOfSamples){
		redSpaceshipOnScreenSound.state = INACTIVE;
		redSpaceshipOnScreenSound.currentIndex = 0;
	}
	if(invaderMove1Sound.currentIndex >= invaderMove1_numberOfSamples){
		invaderMove1Sound.state = INACTIVE;
		aliensSoundState = 2;
		invaderMove1Sound.currentIndex = 0;
		//xil_printf("aliensSoundState is now %d.\n\r",aliensSoundState);
	}
	if(invaderMove2Sound.currentIndex >= invaderMove2_numberOfSamples){
		invaderMove2Sound.state = INACTIVE;
		aliensSoundState = 3;
		invaderMove2Sound.currentIndex = 0;
		//xil_printf("aliensSoundState is now %d.\n\r",aliensSoundState);
	}
	if(invaderMove3Sound.currentIndex >= invaderMove3_numberOfSamples){
		invaderMove3Sound.state = INACTIVE;
		aliensSoundState =  4;
		invaderMove3Sound.currentIndex = 0;
		//xil_printf("aliensSoundState is now %d.\n\r",aliensSoundState);
	}
	if(invaderMove4Sound.currentIndex >= invaderMove4_numberOfSamples){
		invaderMove4Sound.state = INACTIVE;
		aliensSoundState = 1;
		invaderMove4Sound.currentIndex = 0;
		//xil_printf("aliensSoundState is now %d.\n\r",aliensSoundState);
	}
	if(redSpaceshipHitSound.currentIndex >= redSpaceshipHit_numberOfSamples){
		redSpaceshipHitSound.state = INACTIVE;
		redSpaceshipHitSound.currentIndex = 0;
		//xil_printf("redSpaceshipHit has printed all its entries.\n\r");
	}
}
