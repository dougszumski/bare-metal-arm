/*
 * File:		twr_spi_demo.c
 * Purpose:		Main process
 *
 */

#include <stdio.h>
#include "freedom.h"
#include "irq.h"
#include "spi.h"
#include "uart.h"

uint8_t m_DMA0_IntFlag;
uint8_t m_DMA1_IntFlag;
uint8_t m_DMA2_IntFlag;
uint8_t m_DMA3_IntFlag;

uint8_t rdata8[10], m_rdata8[10], s_rdata8[10];	 //Cash: for sram is so small
uint8_t tdata8[10], m_tdata8[10], s_tdata8[10];	 //Cash: for sram is so small
int32_t dma_int_cnt;
volatile uint8_t rd, rd1, r_data[8];
uint32_t k, count;
uint32_t global_pass_count;		//Pass flag
uint32_t global_fail_count;		//Fail flag
volatile int32_t extend_cnt;
/********************************************************************/

void spi_test(void) {

	char ch;

	spi_init();

	printf("\n\rRunning SPI test\n\r");

	while (1) {
		printf("\n\rEnter character:\n\r\n\r");
		uart_read(&ch, 1);
		printf("\n\rYou entered: %c \n\r\n\r", ch);

	}
}

void spi_init(void) {

	// Enable SPI0 Clock gate in the system gating control register. See p.204
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;

	//Disable the COP watchdog timeout.
	SIM_COPC &= (~SIM_COPC_COPT_MASK);

	// Enable clock gate control to ports A, B, C, D, E
	SIM_SCGC5 |=
			SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
			SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	// Disable SPI
	SPI0_C1 &= ~SPI_C1_SPE_MASK;

	// Configure the SPI pins

	PORTD_PCR0 &= ~PORT_PCR_MUX_MASK;
	//Use PTD0 as SPI0_SS_b
	PORTD_PCR0 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;
	PORTD_PCR3 &= ~PORT_PCR_MUX_MASK;
	//Use PTD3 as SPI0_MISO
	PORTD_PCR3 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;
	PORTD_PCR2 &= ~PORT_PCR_MUX_MASK;
	//Use PTD2 as SPI0_MOSI
	PORTD_PCR2 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;
	PORTD_PCR1 &= ~PORT_PCR_MUX_MASK;
	//Use PTD1 as SPI0_SCK
	PORTD_PCR1 = PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;

	// SPI0 configuration, As per p.684 of reference manual.

	// Disable receive and mode fault interrupts
	SPI0_C1 &= ~SPI_C1_SPIE_MASK;

	// Enable SPI system
	SPI0_C1 |= SPI_C1_SPE_MASK;

	// Disable the transmit interrupt
	SPI0_C1 &= ~SPI_C1_SPTIE_MASK;

	// Set SPI module as master, bus clock 12.5mhz
	SPI0_C1 |= SPI_C1_MSTR_MASK;

	// Set SPI clock as active high
	SPI0_C1 &= ~SPI_C1_CPOL_MASK;

	// First edge on SPSCK at start of first data transfer cycle
	SPI0_C1 |= SPI_C1_CPHA_MASK;

	// Determines SS pin function on fault
	SPI0_C1 &= ~SPI_C1_SSOE_MASK;

	// MSB first for SPI serial transfers
	SPI0_C1 &= ~SPI_C1_LSBFE_MASK;

	// SPI hardware interrupt match enabled
	SPI0_C2 = 0x80;

	// Set prescale divisor to 1, baud-rate divisor to 2 (6.125Mhz)
	SPI0_BR = 0x00;

}
