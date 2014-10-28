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
extern int tankFireSoundRate;
extern int tankFireSoundFrames;
extern int tankFireSound[];
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
extern int tankFireBullet_soundData[];
extern int tankFireBullet_numberOfSamples;
extern int tankFireBullet_sampleRate;
GameSound tankExplodeSound;
extern int tankExplode_soundData[];
extern int tankExplode_numberOfSamples;
extern int tankExplode_sampleRate;
//int activeSound = 0;




void moveAliensSound(){
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
	int numberOfSamples = tankFireSoundFrames;
	while(1){
		while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){//!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankFireSoundRate);
			int sample = tankFireSound[i];

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


void tankDeathSound(){
	xil_printf("Huh, we do active tankExplodeSound.\n\r");
	tankExplodeSound.state = ACTIVE;
}

void deactiveTankDeathSound(){ //some of these we shouldn't need this because it should deactive itself after it writes all its samples
	tankExplodeSound.state = INACTIVE;
}

void destroyRedMothershipSound(){
	redSpaceshipLeaveSound.state = ACTIVE;
}

void deactiveDestroyRedMothershipSound(){//some of these we shouldn't need this because it should deactive itself after it writes all its samples
	redSpaceshipLeaveSound.state = INACTIVE;
}



void adjustVolume(int volume_direction){
	if(volume_direction == VOLUME_UP && volumeAttenuation <= AC97_VOL_ATTN_46_0_DB && volumeAttenuation > AC97_VOL_ATTN_0_DB){
		volumeAttenuation -= SOUND_INCR;
		//volumeAttenuation = AC97_VOL_ATTN_0_DB;
	}
	else if(volume_direction == VOLUME_DOWN && volumeAttenuation >= AC97_VOL_ATTN_0_DB && volumeAttenuation < AC97_VOL_ATTN_46_0_DB){
		volumeAttenuation += SOUND_INCR;
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
	while(counter < 250){//!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)
		if(tankFireBulletSound.state == ACTIVE){
			//xil_printf("Well, we got into tankFireBulletSound.\n\r");
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankFireBullet_sampleRate);
			sample = tankFireBullet_soundData[tankFireBulletSound.currentIndex];
			tankFireBulletSound.currentIndex++;
			//xil_printf("This is tankFireBulletSound's currentIndex: %d.\n\r", tankFireBulletSound.currentIndex);
		}
		else if(tankExplodeSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankExplode_sampleRate);
			sample = tankExplode_soundData[tankExplodeSound.currentIndex];
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
		else if(redSpaceshipLeaveSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, redSpaceshipLeave_sampleRate);
			sample = redSpaceshipLeave_soundData[redSpaceshipLeaveSound.currentIndex];
			redSpaceshipLeaveSound.currentIndex++;
		}
		else if(redSpaceshipOnScreenSound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, redSpaceshipOnScreen_sampleRate);
			sample = redSpaceshipOnScreen_soundData[redSpaceshipOnScreenSound.currentIndex];
			redSpaceshipOnScreenSound.currentIndex++;
		}
		/*else if(invaderMove1Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove1_sampleRate);
			sample = invaderMove1_soundData[invaderMove1Sound.currentIndex];
			invaderMove1Sound.currentIndex++;
		}
		else if(invaderMove2Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove2_sampleRate);
			sample = invaderMove2_soundData[invaderMove2Sound.currentIndex];
			invaderMove2Sound.currentIndex++;
		}
		else if(invaderMove3Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove3_sampleRate);
			sample = invaderMove3_soundData[invaderMove3Sound.currentIndex];
			invaderMove3Sound.currentIndex++;
		}
		else if(invaderMove4Sound.state == ACTIVE){
			XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, invaderMove4_sampleRate);
			sample = invaderMove4_soundData[invaderMove4Sound.currentIndex];
			invaderMove4Sound.currentIndex++;
		}*/
		else{
			//xil_printf("Well, the NO_SOUND is successfully being put in the FIFO.\n\r");
			sample = 0;
		}

		updateSoundStates();
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, sample | (sample<<16));
		counter++;
	}
	return;
}

void updateSoundStates(){
	if(tankFireBulletSound.currentIndex >= tankFireBullet_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(tankExplodeSound.currentIndex >= tankExplode_numberOfSamples){
		//xil_printf("Well, we do deactive tankExplodeSound.\n\r");
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(invaderDieSound.currentIndex >= invaderDie_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(invaderKilledSound.currentIndex >= invaderKilled_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(redSpaceshipLeaveSound.currentIndex >= redSpaceshipLeave_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(redSpaceshipOnScreenSound.currentIndex >= redSpaceshipOnScreen_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(invaderMove1Sound.currentIndex >= invaderMove1_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(invaderMove2Sound.currentIndex >= invaderMove2_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(invaderMove3Sound.currentIndex >= invaderMove3_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
	if(invaderMove4Sound.currentIndex >= invaderMove4_numberOfSamples){
		tankFireBulletSound.state = INACTIVE;
		tankFireBulletSound.currentIndex = 0;
	}
}
