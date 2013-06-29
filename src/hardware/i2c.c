/*
 * i2c.c
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#include "freedom.h"
#include "i2c.h"

// ---------------------------------------------------------------------------
// I2C bus control functions
//
inline void i2c_set_tx(I2C_MemMapPtr p)     {p->C1 |= I2C_C1_TX_MASK;}
inline void i2c_set_rx(I2C_MemMapPtr p)     {p->C1 &= ~I2C_C1_TX_MASK;}
inline void i2c_set_slave(I2C_MemMapPtr p)  {p->C1 &= ~I2C_C1_MST_MASK;}
inline void i2c_set_master(I2C_MemMapPtr p) {p->C1 |=  I2C_C1_MST_MASK;}
inline void i2c_give_nack(I2C_MemMapPtr p)  {p->C1 |= I2C_C1_TXAK_MASK;}
inline void i2c_give_ack(I2C_MemMapPtr p)   {p->C1 &= ~I2C_C1_TXAK_MASK;}
inline void i2c_repeated_start(I2C_MemMapPtr p){p->C1     |= I2C_C1_RSTA_MASK;}
inline uint8_t i2c_read(I2C_MemMapPtr p)    {return p->D;}

inline void i2c_start(I2C_MemMapPtr p)
{
    i2c_set_master(p);
    i2c_set_tx(p);
}

inline void i2c_stop(I2C_MemMapPtr p)
{
    i2c_set_slave(p);
    i2c_set_rx(p);
}

inline void i2c_wait(I2C_MemMapPtr p)
{
    // Spin wait for the interrupt flag to be set
    while((p->S & I2C_S_IICIF_MASK) == 0)
        ;

    p->S |= I2C_S_IICIF_MASK;           // Clear flag
}

inline int i2c_write(I2C_MemMapPtr p, uint8_t data)
{
    // Send data, wait, and return ACK status
    p->D = data;
    i2c_wait(p);
    return ((p->S & I2C_S_RXAK_MASK) == 0);
}

inline void i2c_init(I2C_MemMapPtr p)
{
    // Enable clocks
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;

    // Configure GPIO for I2C
    PORTE_PCR24 = PORT_PCR_MUX(5);
    PORTE_PCR25 = PORT_PCR_MUX(5);

    p->F  = 0x14;                   // Baudrate settings:  ICR=0x14, MULT=0
    p->C1 = I2C_C1_IICEN_MASK;      // Enable:  IICEN=1
}


