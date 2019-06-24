/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include "24cXX.h"

static int i2c_write_1b(struct eeprom *e, __s8 i2c_addr, __u8 buf)
{
	// set working device
	int r;
	if( ( r = ioctl(e->fd, I2C_SLAVE, i2c_addr)) < 0)
	{
		fprintf(stderr, "Error i2c_write_1b: %s\n", strerror(errno));
		return r;
	}

	// we must simulate a plain I2C byte write with SMBus functions
	r = i2c_smbus_write_byte(e->fd, buf);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_1b: %s\n", strerror(errno));
	usleep(10);
	return r;
}

static int i2c_write_2b(struct eeprom *e, __s8 i2c_addr, __u8 buf[2])
{
	// set working device
	int r;
	if( ( r = ioctl(e->fd, I2C_SLAVE, i2c_addr)) < 0)
	{
		fprintf(stderr, "Error i2c_write_2b: %s\n", strerror(errno));
		return r;
	}

	// we must simulate a plain I2C byte write with SMBus functions
	r = i2c_smbus_write_byte_data(e->fd, buf[0], buf[1]);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_2b: %s\n", strerror(errno));
	usleep(10);
	return r;
}

static int i2c_write_3b(struct eeprom *e, __s8 i2c_addr, __u8 buf[3])
{
	// set working device
	int r;
	if( ( r = ioctl(e->fd, I2C_SLAVE, i2c_addr)) < 0)
	{
		fprintf(stderr, "Error i2c_write_3b: %s\n", strerror(errno));
		return r;
	}

	// we must simulate a plain I2C byte write with SMBus functions
	// the __u16 data field will be byte swapped by the SMBus protocol
	r = i2c_smbus_write_word_data(e->fd, buf[0], buf[2] << 8 | buf[1]);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_3b: %s\n", strerror(errno));
	usleep(10);
	return r;
}


#define CHECK_I2C_FUNC( var, label ) \
	do { 	if(0 == (var & label)) { \
		fprintf(stderr, "\nError: " \
			#label " function is required. Program halted.\n\n"); \
		exit(1); } \
	} while(0);

int eeprom_open(char *dev_fqn, int i2c_addr, int bits, int write_cycle_time, struct eeprom* e)
{
	int funcs, fd, r;
	e->fd = e->i2c_addr = 0;
	e->dev = 0;
	
	fd = open(dev_fqn, O_RDWR);
	if(fd <= 0)
	{
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	// get funcs list
	if((r = ioctl(fd, I2C_FUNCS, &funcs) < 0))
	{
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	
	// check for req funcs
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_WORD_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA );

#if 0
	// set working device
	if( ( r = ioctl(fd, I2C_SLAVE, addr)) < 0)
	{
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}
#endif
	e->fd = fd;
	e->i2c_addr = i2c_addr;
	e->dev = dev_fqn;
	e->bits = bits;
	e->type = EEPROM_TYPE_8BIT_ADDR;
	if (bits > 16) e->type = EEPROM_TYPE_16BIT_ADDR;
	e->bytes = 128 * bits;
	e->write_cycle_time = write_cycle_time;
	//printf("bits=%d bytes=%d type=%d\n",e->bits, e->bytes, e->type);
	return 0;
}

int eeprom_close(struct eeprom *e)
{
	close(e->fd);
	e->fd = -1;
	e->dev = 0;
	e->type = EEPROM_TYPE_UNKNOWN;
	return 0;
}

__u16 getEEPROMbytes(struct eeprom* e)
{
	return(e->bytes);
}

int eeprom_read_current_byte(struct eeprom* e)
{
	ioctl(e->fd, BLKFLSBUF); // clear kernel read buffer
	return i2c_smbus_read_byte(e->fd);
}

int eeprom_read_byte(struct eeprom* e, __u16 mem_addr)
{
	if (mem_addr >= e->bytes) {
		fprintf(stderr, "Your EEPROM is %d bytes\n", e->bytes);
		fprintf(stderr, "The address (%d) exceeds this.\n", mem_addr);
		return -1;
	}

	int r;
	ioctl(e->fd, BLKFLSBUF); // clear kernel read buffer
	if(e->type == EEPROM_TYPE_8BIT_ADDR)
	{
		__u8 blockNumber = mem_addr / 256;
		__u8 _mem_addr =  mem_addr - (blockNumber * 256);
		__s8 i2c_addr = e->i2c_addr + blockNumber;
		__u8 buf =  _mem_addr;
		//printf("read_byte mem_addr=%x i2c_addr=%x _mem_addr=%x\n",mem_addr, i2c_addr, _mem_addr);
		r = i2c_write_1b(e, i2c_addr, buf);
	} else if(e->type == EEPROM_TYPE_16BIT_ADDR) {
		__s8 i2c_addr = e->i2c_addr;
		__u8 buf[2] = { (mem_addr >> 8) & 0x0ff, mem_addr & 0x0ff };
		r = i2c_write_2b(e, i2c_addr, buf);
	} else {
		fprintf(stderr, "ERR: unknown eeprom type\n");
		return -1;
	}
	if (r < 0)
		return r;
	r = i2c_smbus_read_byte(e->fd);
	return r;
}

int eeprom_write_byte(struct eeprom *e, __u16 mem_addr, __u8 data)
{
	if (mem_addr >= e->bytes) {
		fprintf(stderr, "Your EEPROM is %d bytes\n", e->bytes);
		fprintf(stderr, "The address (%d) exceeds this.\n", mem_addr);
		return -1;
	}

	int ret;
	if(e->type == EEPROM_TYPE_8BIT_ADDR) {
		__u8 blockNumber = mem_addr / 256;
		__u8 _mem_addr =  mem_addr - (blockNumber * 256);
		__s8 i2c_addr = e->i2c_addr + blockNumber;
		//__u8 buf[2] = { mem_addr & 0x00ff, data };
		__u8 buf[2] = { _mem_addr, data };
		//printf("write_byte mem_addr=%x i2c_addr=%x _mem_addr=%x\n",mem_addr, i2c_addr, _mem_addr);
		ret = i2c_write_2b(e, i2c_addr, buf);
		if (ret == 0 && e->write_cycle_time != 0) {
			usleep(1000 * e->write_cycle_time);
		}
		return ret;
	} else if(e->type == EEPROM_TYPE_16BIT_ADDR) {
		__s8 i2c_addr = e->i2c_addr;
		__u8 buf[3] = 
			{ (mem_addr >> 8) & 0x00ff, mem_addr & 0x00ff, data };
		ret = i2c_write_3b(e, i2c_addr, buf);
		if (ret == 0 && e->write_cycle_time != 0) {
			usleep(1000 * e->write_cycle_time);
		}
		return ret;
	} 
	fprintf(stderr, "ERR: unknown eeprom type\n");
	return -1;
}

