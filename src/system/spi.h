/*
 * spi.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef SPI_DEMO_H_
#define SPI_DEMO_H_

#include "freedom.h"

// Reset pin position (port D)
#define RESET 5
#define CS 13

// Test the SPI interface
void spi_test(void);

// Initialise SPI with DMA off
void spi_init(void);

// Write then read a byte
uint8_t spi_send_recv(uint8_t tx);

#endif /* SPI_DEMO_H_ */
