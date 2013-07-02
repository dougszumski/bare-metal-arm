/*
 * spi.c
 *
 * Basic SPI routines
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#include <stdio.h>
#include "freedom.h"
#include "irq.h"
#include "spi.h"
#include "uart.h"

void spi_test(void) {

	char ch = 'a';

	spi_init();

	printf("\n\rRunning SPI test\n\r");

	while (1) {
		//printf("\n\rEnter character:\n\r\n\r");
		//uart_read(&ch, 1);
		//printf("\n\rYou entered: %c \n\r\n\r", ch);

		//Transmit but SPI
		spi_write(ch);

	}
}

void spi_write(unsigned char byte) {

	// Wait on the SPI transmit buffer empty flag. See p666.
	while(!(SPI0_S && SPI_S_SPTEF_MASK)) {
	};

	// Write the char to the SPI data register
	SPI0_D = byte;
}

unsigned char spi_read(void) {

	// Wait on the read buffer full flag. See p666.
	while(!(SPI0_S && SPI_S_SPRF_MASK)) {
	};

	// Read the char from the SPI data register
	return SPI0_D;
}

void spi_init(void) {

	// Enable clock gate control to ports D
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

	// Enable SPI0 Clock gate in the system gating control register. See p.204
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;

	//Disable the COP watchdog timeout.
	SIM_COPC &= ~SIM_COPC_COPT_MASK;

	// Disable SPI
	SPI0_C1 &= ~SPI_C1_SPE_MASK;

	// Configure the SPI pins. p164

	//Use PTD0 as SPI0_SS_b
	PORTD_PCR0 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR0 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	//Use PTD3 as SPI0_MISO
	PORTD_PCR3 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR3 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	//Use PTD2 as SPI0_MOSI
	PORTD_PCR2 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR2 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	//Use PTD1 as SPI0_SCK
	PORTD_PCR1 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR1 = PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	// SPI0 configuration, As per p.684 of reference manual.

	// Disable receive and mode fault interrupts
	SPI0_C1 = 0x54;

	// SPI hardware interrupt match enabled
	SPI0_C2 = 0x80;

	// Set prescale divisor to 1, baud-rate divisor to 2 (6.125Mhz)
	SPI0_BR = 0x00;
	// NOTE: Clock is 12Mhz on scope.

}
