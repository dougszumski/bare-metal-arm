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

// Macro to manipulate a specific pin
#define GPIO_PIN(x)	((1) << (x))

// Initialise SPI with DMA off
void spi_init(void);

// Write then read a byte
uint8_t spi_send_recv(uint8_t tx);

#endif /* SPI_DEMO_H_ */
