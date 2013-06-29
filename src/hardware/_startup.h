/*
 * startup.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef STARTUP_H_
#define STARTUP_H_

void fault(uint32_t pattern);
#define FAULT_FAST_BLINK 	(0b10101010101010101010101010101010)
#define FAULT_MEDIUM_BLINK 	(0b11110000111100001111000011110000)
#define FAULT_SLOW_BLINK 	(0b11111111000000001111111100000000)

#endif /* STARTUP_H_ */
