/*
 * spi.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef SPI_DEMO_H_
#define SPI_DEMO_H_

#include "freedom.h"

// Test the SPI interface
void spi_test(void);

// Initialise SPI with DMA off
void spi_init(void);

// Write a byte
void spi_write(uint8_t  ch);

// Read a byte
uint8_t spi_read(void);

// Write then read a byte
uint8_t  spi_send_recv(uint8_t  byte) ;

#endif /* SPI_DEMO_H_ */
