/*
 * touch_sensor.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef TOUCH_SENSOR_H_
#define TOUCH_SENSOR_H_

// From touch.c
int touch_data(int channel);
void touch_init(uint32_t channel_mask);

#endif /* TOUCH_SENSOR_H_ */
