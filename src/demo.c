//
// demo.c -- Simple demonstration program
//
//  Copyright (c) 2012-2013 Andrew Payne <andy@payne.org>
//

#include <stdio.h>

#include "system/freedom.h"
#include "system/uart.h"
#include "utils/tests.h"
#include "utils/delay.h"
#include "devices/rgb_led.h"
#include "devices/mma8451.h"
#include "devices/touch_sensor.h"
#include "system/spi.h"
#include "devices/ili9320.h"
#include "lib/AsciiLib.h"
#include "system/pit.h"

extern char *_sbrk(int len);

void lcd_test(void);
void welcome(void);
void input_demo(void);

// Main program
int main(void) {

	// Initialize all modules
	uart_init(115200);
	//accel_init();
	//touch_init((1 << 9) | (1 << 10));       // Channels 9 and 10
	// usb_init();
	setvbuf(stdin, NULL, _IONBF, 0);        // No buffering

	// Run tests
	tests();
	delay_ms(100);
	RGB_LED(100, 000, 0);

	pit_init();

	lcd_test();

	while (1)
		;
}

void lcd_test(void) {

	char input;
	uint16_t dev_code;

	//Screen cursor
	uint8_t xCur = 0;
	uint16_t yCur = 0;

	printf("Running LCD test\n\r");
	spi_init();
	initialise_controller();

	// Print the device code
	dev_code = LCD_ReadReg(0x0000);
	printf("Device code: 0x%04X\n\r", dev_code);


	// Prints char on LCD screen.
	while (1) {

		printf("\n\rEnter character:\n\r");
		uart_read(&input, 1);

		GUI_Text(xCur,yCur, &input, Red, Black);

		// Move the cursor
		xCur += ASCII_X_PX;

		// New line
		if (xCur >= MAX_X) {
			yCur += ASCII_Y_PX;
			xCur = 0;
		}

		// Back to start
		if (yCur >= MAX_Y) {
			yCur = 0;
			yCur = 0;
		}
	}
}

void welcome(void) {
	char i;
	char *heap_end;

	// Welcome banner
	iprintf("\r\n\r\n====== Freescale Freedom FRDM-LK25Z\r\n");
	iprintf("Built: %s %s\r\n\r\n", __DATE__, __TIME__);
	heap_end = _sbrk(0);
	iprintf("Heap:  %p to %p (%d bytes used)\r\n", __heap_start, heap_end,
			heap_end - (char *) __heap_start);
	iprintf("Stack: %p to %p (%d bytes used)\r\n", &i, __StackTop,
			(char *) __StackTop - &i);
	iprintf("%d bytes free\r\n", &i - heap_end);
}

void input_demo(void) {

	for (;;) {
		iprintf("monitor> ");
		getchar();
		iprintf("\r\n");
		iprintf("Inputs:  x=%5d   y=%5d   z=%5d ", accel_x(), accel_y(),
				accel_z());
		iprintf("touch=(%d,%d)\r\n", touch_data(9), touch_data(10));
		// usb_dump();
	}
}

