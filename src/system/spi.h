/*
 * spi.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef SPI_DEMO_H_
#define SPI_DEMO_H_

// Test the SPI interface
void spi_test(void);

// Initialise SPI with DMA off
void spi_init(void);

// Write a byte
void spi_write(unsigned char ch);

// Read a byte
unsigned char spi_read(void);

#endif /* SPI_DEMO_H_ */
