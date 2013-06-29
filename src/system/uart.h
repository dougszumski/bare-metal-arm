/*
 * uart.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef UART_H_
#define UART_H_

void UART0_IRQHandler() __attribute__((interrupt("IRQ")));
int uart_write(char *p, int len);
int uart_write_err(char *p, int len);
int uart_read(char *p, int len);
void uart_init(int baud_rate);

#endif /* UART_H_ */
