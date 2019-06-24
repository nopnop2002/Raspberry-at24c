/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _24CXX_H_
#define _24CXX_H_
#include "i2c-dev.h"

#define EEPROM_TYPE_UNKNOWN	0
#define EEPROM_TYPE_8BIT_ADDR	1
#define EEPROM_TYPE_16BIT_ADDR 	2

struct eeprom
{
	char *dev;	// device file i.e. /dev/i2c-N
	int i2c_addr;	// i2c address
	int fd;		// file descriptor
	int type;	// eeprom register size(8bit/16bit)
	int write_cycle_time;
	int bits;	// eeprom memory size(Kbits)
	__u16 bytes;	// eeprom memory size(byte)
};

/*
 * opens the eeprom device at [dev_fqn] (i.e. /dev/i2c-N)
 * whose address is [i2c_addr]
 * number of bits is [bits]
 * and set the eeprom [e]
 */
int eeprom_open(char *dev_fqn, int i2c_addr, int bits, int delay, struct eeprom*);
/*
 * closees the eeprom device [e] 
 */
int eeprom_close(struct eeprom *e);
/*
 * get EEPROM bytes
 */
__u16 getEEPROMbytes(struct eeprom* e);
/*
 * read and returns the eeprom byte at memory address [mem_addr] 
 * Note: eeprom must have been selected by ioctl(fd,I2C_SLAVE,address) 
 */
int eeprom_read_byte(struct eeprom* e, __u16 mem_addr);
/*
 * read the current byte
 * Note: eeprom must have been selected by ioctl(fd,I2C_SLAVE,address) 
 */
int eeprom_read_current_byte(struct eeprom *e);
/*
 * writes [data] at memory address [mem_addr] 
 * Note: eeprom must have been selected by ioctl(fd,I2C_SLAVE,address) 
 */
int eeprom_write_byte(struct eeprom *e, __u16 mem_addr, __u8 data);

#endif

