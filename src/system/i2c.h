/*
 * i2c.h
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#ifndef I2C_H_
#define I2C_H_

#include "freedom.h"

inline void i2c_set_tx(I2C_MemMapPtr p);

inline void i2c_set_rx(I2C_MemMapPtr p);

inline void i2c_set_slave(I2C_MemMapPtr p);

inline void i2c_set_master(I2C_MemMapPtr p);

inline void i2c_give_nack(I2C_MemMapPtr p);

inline void i2c_give_ack(I2C_MemMapPtr p);

inline void i2c_repeated_start(I2C_MemMapPtr p);

inline uint8_t i2c_read(I2C_MemMapPtr p);

inline void i2c_start(I2C_MemMapPtr p);

inline void i2c_stop(I2C_MemMapPtr p);

inline void i2c_wait(I2C_MemMapPtr p);

inline int i2c_write(I2C_MemMapPtr p, uint8_t data);

inline void i2c_init(I2C_MemMapPtr p);


#endif /* I2C_H_ */
