/*
 * interrupts.h
 *
 *  Created on: Oct 7, 2014
 *      Author: superman
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

void interrupt_handler_dispatcher();
void timer_interrupt_handler();

#endif /* INTERRUPTS_H_ */

#define COUNTDOWN_INITIAL_VALUE 0x7A120 //500,000
#define PIT_DECREMENT 0x1
#define PIT_HOLD 0x0
#define PIT_ENABLE_INTR 0x10
#define PIT_DISABLE_INTR 0x00
#define PIT_AUTO_RESET 0x100
#define PIT_NO_RESET 0x000
