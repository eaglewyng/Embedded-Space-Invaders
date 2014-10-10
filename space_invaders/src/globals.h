/*
 * globals.h
 *
 *  Created on: Sep 29, 2014
 *      Author: superman
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#define FRAME_BUFFER_0_ADDR 0xC0000000  // Starting location in DDR where we will store the images that we display.

unsigned int * framePointer0;
unsigned int * framePointer1;

#endif /* GLOBALS_H_ */
