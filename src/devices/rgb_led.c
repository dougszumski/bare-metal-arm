/*
 * rgb_led.c
 *
 *  Created on: 29 Jun 2013
 *      Author: doug
 */

#include "rgb_led.h"
#include "../system/MKL25Z4.h"

inline void RGB_LED(int red, int green, int blue) {
    TPM2_C0V  = red;
    TPM2_C1V  = green;
    TPM0_C1V  = blue;
}
