/*
 * mma8451.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef MMA8451_H_
#define MMA8451_H_

void accel_init(void);
int16_t accel_x(void);
int16_t accel_y(void);
int16_t accel_z(void);

#endif /* MMA8451_H_ */
