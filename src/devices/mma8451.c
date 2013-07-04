// accel.c -- Accelerometer support
//
//  Copyright (c) 2012-2013 Andrew Payne <andy@payne.org>
//

// Based on demo example from Freescale

#include "../system/freedom.h"
#include "../system/i2c.h"
#include "../utils/delay.h"
#include "mma8451.h"

#define MMA8451_I2C_ADDRESS (0x1d<<1)
#define I2C_READ 1
#define I2C_WRITE 0

#define I2C0_B  I2C0_BASE_PTR

// ---------------------------------------------------------------------------
// MMA8451 control functions
//

uint8_t mma8451_read(uint8_t addr) {
	delay_ms(1);
	i2c_start(I2C0_B );
	i2c_write(I2C0_B, MMA8451_I2C_ADDRESS | I2C_WRITE);
	i2c_write(I2C0_B, addr);
	i2c_repeated_start(I2C0_B );
	i2c_write(I2C0_B, MMA8451_I2C_ADDRESS | I2C_READ);
	i2c_set_rx(I2C0_B );
	i2c_give_nack(I2C0_B );
	i2c_read(I2C0_B );
	i2c_wait(I2C0_B );
	i2c_stop(I2C0_B );
	return i2c_read(I2C0_B );
}

void mma8451_write(uint8_t addr, uint8_t data) {
	delay_ms(1);
	i2c_start(I2C0_B );
	i2c_write(I2C0_B, MMA8451_I2C_ADDRESS | I2C_WRITE);
	i2c_write(I2C0_B, addr);
	i2c_write(I2C0_B, data);
	i2c_stop(I2C0_B );
}

#define CTRL_REG1 (0x2a)
void accel_init(void) {
	uint8_t tmp;

	i2c_init(I2C0_B );
	tmp = mma8451_read(CTRL_REG1);
	mma8451_write(CTRL_REG1, tmp | 0x01);       // ACTIVE = 1
}

// Read a signed 14-bit value from (reg, reg+1)
int16_t _read_reg14(int reg) {
	return (int16_t) ((mma8451_read(reg) << 8) | mma8451_read(reg + 1)) >> 2;
}

// Read acceleration values for each axis
int16_t accel_x(void) {
	return _read_reg14(0x01);
}
int16_t accel_y(void) {
	return _read_reg14(0x03);
}
int16_t accel_z(void) {
	return _read_reg14(0x05);
}

