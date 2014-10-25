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

void moveAliensSound(){

/*	int i;

	for(i = 0; i < 10000; i++){
		int mySound = i%10000;//alienSound1_soundData[i%4];
		mySound = mySound | (mySound << 16); //puts in both left and right ear buds
		mySound = mySound << 6; //TAs told me to do this
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, mySound);
	}*/


}

void moveRedSpaceshipSound(){

}

void fireTankBulletSound(){
	//XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);

	int i;

	for(i = 0; i < 10000; i++){
		int mySound = i%500;//alienSound1_soundData[i%4];
		mySound = mySound | (mySound << 16); //puts in both left and right ear buds
		mySound = mySound << 6; //TAs told me to do this
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, mySound);
	}

	XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);
}


void tankDeathSound(){

}


void destroyRedMothershipSound(){

}

void adjustVolume(int volume_direction){
	if(volume_direction == VOLUME_UP && volumeAttenuation <= AC97_VOL_ATTN_46_0_DB){
		//volumeAttenuation -= SOUND_INCR;
		volumeAttenuation = AC97_VOL_ATTN_46_0_DB;
	}
	else if(volume_direction == VOLUME_DOWN && volumeAttenuation >= AC97_VOL_ATTN_0_DB){
		//volumeAttenuation += SOUND_INCR;
		volumeAttenuation = AC97_VOL_ATTN_0_DB;
	}
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, volumeAttenuation);
}

void updateSoundSampleRate(){
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);
}
