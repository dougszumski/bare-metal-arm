/*
 * spi_demo.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef SPI_DEMO_H_
#define SPI_DEMO_H_

void spi_test(void);

void spi_transmit(char ch);

void spi_init(void);
void spi1_dma_master_spi0_slave(void);
void DMA0_IRQHandler(void);
void DMA1_IRQHandler(void);
void DMA2_IRQHandler(void);
void DMA3_IRQHandler(void);

#endif /* SPI_DEMO_H_ */
