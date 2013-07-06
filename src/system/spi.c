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
#include "../devices/ili9320.h"
#include "../utils/delay.h"

// Macro to manipulate a specific pin
#define GPIO_PIN(x)	((1) << (x))

void spi_test(void) {

	char ch = 'a';
	uint16_t dev_code;

	spi_init();

	printf("Running SPI test\n");

	GPIOD_PSOR = GPIO_PIN(RESET);
	delay_ms(50);
	GPIOD_PCOR = GPIO_PIN(RESET);
	delay_ms(50);
	GPIOD_PSOR = GPIO_PIN(RESET);
	delay_ms(100);

	// Set CS
	GPIOA_PSOR = GPIO_PIN(CS); //High

	while (1) {

		//printf("\n\rEnter character:\n\r");
		//uart_read(&ch, 1);

		delay_ms(100);
		//GPIOA_PCOR = GPIO_PIN(CS);
		dev_code = LCD_ReadReg(0x0000);
		printf("Device code: 0x%04X\n\r", dev_code);
		delay_ms(100);
		//GPIOA_PSOR = GPIO_PIN(CS);

	}
}


uint8_t spi_send_recv(uint8_t tx) {

	//NOTE: Match flag is ignored for now.
	uint8_t rx;

	// Wait on the SPI transmit buffer empty flag. See p666.
	while ((SPI0_S & SPI_S_SPTEF_MASK) == 0x00);

	// Write the char to the SPI data register
	SPI0_D = tx;

	while ((SPI0_S & SPI_S_SPRF_MASK) == 0x00);

	// Read the char from the SPI data register
	rx = SPI0_D;

	return rx;
}

void spi_init(void) {

	// Enable clock gate control to ports D and E
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Enable SPI0 Clock gate in the system gating control register. See p.204
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;

	//Disable the COP watchdog timeout.
	SIM_COPC &= ~SIM_COPC_COPT_MASK;

	// Disable SPI
	SPI0_C1 &= ~SPI_C1_SPE_MASK;

	// Configure the SPI pins. p164
	// DSE is drive strength enable.

	//Use PTD0 as SUDO!! SPI0_SS_b
	//PORTD_PCR0 &= ~PORT_PCR_MUX_MASK;
	//PORTD_PCR0 |= PORT_PCR_MUX(1)|PORT_PCR_DSE_MASK;
	//GPIOD_PSOR |= GPIO_PIN(CS);    // Set initial output to high
	//GPIOD_PDDR |= GPIO_PIN(CS);    // Set pin direction to output
	PORTA_PCR13 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR13 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	GPIOA_PSOR |= GPIO_PIN(CS);    // Set initial output to high
	GPIOA_PDDR |= GPIO_PIN(CS);    // Set pin direction to output

	//Use PTD3 as SPI0_MISO
	PORTD_PCR3 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR3 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	//Use PTD2 as SPI0_MOSI
	PORTD_PCR2 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR2 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	//Use PTD1 as SPI0_SCK
	PORTD_PCR1 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR1 = PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	//Use PTD5 as RST
	PORTD_PCR5 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR5 |= PORT_PCR_MUX(1)|PORT_PCR_DSE_MASK;
	GPIOD_PSOR |= GPIO_PIN(RESET);    // Set initial output to high
	GPIOD_PDDR |= GPIO_PIN(RESET);    // Set pin direction to output

	// SPI0 configuration, As per p.684 of reference manual.

	// Disable receive and mode fault interrupts, enable SS pin function
	// CPOL Active low, idles high
	//
	SPI0_C1 = 0b01011100;


	// SPI disable mode fault function for the SS pin
	SPI0_C2 = 0b10000000;

	// Set prescale divisor to 1, baud-rate divisor to 2 (6.125Mhz)
	//SPI0_BR = 0x00;
	//SPI0_BR = 0x40; //bps div = 10,---------0.8us
	//SPI0_BR = 0x77;
	SPI0_BR = 0x54;
	// NOTE: Clock is 12Mhz on scope.

}
