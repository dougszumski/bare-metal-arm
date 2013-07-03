/*
 * Read Device code on SPI interfaced LCD Controller, with Start byte. Particularly the HY28A.
 *
 * Wire up the SPI bus: SCLK, MOSI and MISO. LCDB_CS => CE0
 * Wire up RESET to GPIO25
 *
 * SPI code taken from: https://raw.github.com/torvalds/linux/master/Documentation/spi/spidev_test.c
 *
 * GPIO code from: http://elinux.org/RPi_Low-level_peripherals#C (Dom and Gert)
 *            and: https://github.com/torvalds/linux/blob/master/drivers/gpio/gpiolib.c
 *
 * Ref:
 * http://www.raspberrypi.org/phpBB3/viewtopic.php?f=64&t=34007&start=25
 *
 *
 *
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>

/* LCD Controller Start byte definitions */
#define SPI_START (0x70)   /* Start byte for SPI transfer */
#define SPI_RD (0x01)      /* WR bit 1 within start */
#define SPI_WR (0x00)      /* WR bit 0 within start */
#define SPI_DATA (0x02)    /* RS bit 1 within start byte */
#define SPI_INDEX (0x00)   /* RS bit 0 within start byte */

//unsigned read_devicecode(int fd);

//int main(int argc, char **argv) {

//	int spifd;

	/* reset lcd controller */
	//gpio_set_value(RESET_GPIO, 0);
	//gpio_set_value(RESET_GPIO, 1);

	//printf("\nDevice code: 0x%04X\n", read_devicecode(spifd));

//	return 0;
//}

//unsigned read_devicecode(int fd) {
	//int ret;
	//uint8_t regtxbuf[16] = { SPI_START | SPI_WR | SPI_INDEX, 0, 0, };
	//uint8_t txbuf[16] = { SPI_START | SPI_RD | SPI_DATA, 0, 0, 0, };
	//uint8_t rxbuf[16] = { 0, };

	/* Set index register 0x0000 */
	//transfer(fd, regtxbuf, NULL, 3);

	/* Read register contents */
	//transfer(fd, txbuf, rxbuf, 16);

	//return (rxbuf[2] << 8) | rxbuf[3];
//}

