/*
 * pit.c
 *
 *  Created on: 7 Jul 2013
 *      Author: doug
 */
#include <stdio.h>
#include "pit.h"
#include "freedom.h"
#include "irq.h"
#include "../devices/ili9320.h"
#include "../lib/AsciiLib.h"

volatile uint8_t xCur = 0;
volatile uint16_t yCur = 0;

void pit_init(void ) {

	// Clock enable
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;

	// Turn on PIT p575.
	PIT_MCR = 0x00;

	// Setup timer 1 at 1Hz
	PIT_LDVAL1 = 0x02DC6C00;

	// Enable Timer 1 interrupts
	PIT_TCTRL1 = PIT_TCTRL_TIE_MASK;

	// Start Timer 1
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK ;

	// Enable PIT interrupt
	__enable_irq();
	enable_irq(INT_PIT);

}

void PIT_IRQHandler() {

	GUI_Text(xCur++,yCur, "X", Red, Black);

	// New line
	if (xCur >= MAX_X) {
		yCur += ASCII_Y_PX;
		xCur = 0;
	}

	// Back to start
	if (yCur >= MAX_Y) {
		yCur = 0;
		yCur = 0;
	}

	// Clear the timer interrupt flag p580.
	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;
}

