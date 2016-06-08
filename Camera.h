/*
 * Camera.h
 *
 *  Created on: Apr 5, 2016
 *      Author: root
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "hwlib.h"
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define regtype volatile uint8_t
#define regsize uint8_t
#define PROGMEM

struct sensor_reg {
	uint16_t reg;
	uint16_t val;
};

bool OV5642_REG_WRITE_16_8(int file, uint16_t, uint8_t);
bool OV5642_REG_WRITE_struct_16_8(int file, const struct sensor_reg reglist[]);
bool OV5642_REG_READ_16_8(int file, uint16_t, uint8_t*);

#define pgm_read_word(x)        ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))





#endif /* CAMERA_H_ */
