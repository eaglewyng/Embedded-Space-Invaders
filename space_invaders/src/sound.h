/*
 * sound.h
 *
 *  Created on: Oct 25, 2014
 *      Author: superman
 */



#ifndef SOUND_H_
#define SOUND_H_

#define MAX_SOUND 0x1F1F
#define MIN_SOUND 0x0
#define SOUND_INCR 0x0101;

#endif /* SOUND_H_ */

void moveAliensSound();

void moveRedSpaceshipSound();

void fireTankBulletSound();

void tankDeathSound();

void destroyRedMothershipSound();

void adjustVolume(int volume_direction);
