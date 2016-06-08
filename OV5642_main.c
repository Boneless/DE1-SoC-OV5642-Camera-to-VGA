/*
 * OV5642.c
 *
 *  Created on: Apr 5, 2016
 *      Author: Jonathan Campbell
 */


#define soc_cv_av

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

#include "Camera.h"
#include "ov5642_regs.h"





int main(int argc, char **argv){
	uint8_t reg_val;
	bool writeSuccess;

	int file;
	const char *filename = "/dev/i2c-1";

	//-------------------------------------------------
	// open i2c bus
	//-------------------------------------------------

	if ((file = open(filename, O_RDWR)) < 0) {
	  	  /* ERROR HANDLING: you can check errno to see what went wrong */
		  perror("Failed to open the i2c bus of camera");
	  	  exit(1);
	}

	int addr = 0x3c;
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
	  	  printf("Failed to acquire bus access and/or talk to slave.\n");
		    /* ERROR HANDLING; you can check errno to see what went wrong */
	  	  exit(1);
	}
	//-------------------------------------------------------
	printf("\nI2C bus successfuly opened\n");
	//-------------------------------------
	//Reset the camera and delay set period (100ms)
	//-------------------------------------


	writeSuccess = OV5642_REG_WRITE_16_8(file, 0x3008, 0x80);
	uint8_t test;
	OV5642_REG_READ_16_8(file, 0x4300, &test);
	usleep(100000);
	//-------------------------------------

	OV5642_REG_WRITE_struct_16_8(file, OV5642_RGB_QVGA);
	OV5642_REG_READ_16_8(file, 0x3818, &reg_val);
	OV5642_REG_WRITE_16_8(file, 0x3818, (reg_val | 0x60) & 0xff );
	OV5642_REG_READ_16_8(file, 0x3621, &reg_val);
	OV5642_REG_WRITE_16_8(file, 0x3621, reg_val & 0xdf);

	if(writeSuccess == true)
	{
		while(1){
			printf("good\n");
			usleep(1000000);
		}
	}

	return 0;
}
//----------------------------------------------------------------------

bool OV5642_REG_WRITE_16_8(int file, uint16_t address, uint8_t regData){
	bool writeSuccess = false;
	uint8_t regH, regL;


	regH = (address >> 8) & 0x00FF;
	regL = address & 0x00FF;
	uint8_t writeBuffer[3];
	writeBuffer[0] = regH;
	writeBuffer[1] = regL;
	writeBuffer[2] = regData;

	ssize_t test = write(file, writeBuffer, 3);

	if( test == sizeof(writeBuffer)){
		writeSuccess = true;
	}

	return writeSuccess;

}

bool OV5642_REG_WRITE_struct_16_8(int file, const struct sensor_reg reglist[])
{
	bool writeSuccess = false;

	unsigned int reg_addr, reg_val;
	const struct sensor_reg *next = reglist;

	while((reg_addr != 0xffff) | (reg_val != 0xff))
	{
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		writeSuccess = OV5642_REG_WRITE_16_8(file, reg_addr, reg_val);
		next++;
	}

	return writeSuccess;
}

bool OV5642_REG_READ_16_8(int file, uint16_t address, uint8_t *regData){
	bool writeSuccess = false;
	uint8_t regH, regL, readData;


	regH = (address >> 8);
	regL = address;
	uint8_t readBuffer[2] = {regH, regL};
	//reposition file pointer

	if(write(file, &readBuffer, sizeof(address)) == sizeof(address)){
		//Read register value
		if(read(file, &readData, sizeof(readData)) == sizeof(readData)){
			*regData = readData;
			writeSuccess = true;
		}
	}

	write(file, readBuffer, 2);
	read(file, regData, 1);


	return writeSuccess;
}
