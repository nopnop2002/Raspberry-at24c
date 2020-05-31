/*
  I forked from here.
  https://www.richud.com/wiki/Rasberry_Pi_I2C_EEPROM_Program

  24C02  cc -o main main.c at24c.c -DC02
  24C04  cc -o main main.c at24c.c -DC04
  24C08  cc -o main main.c at24c.c -DC08
  24C16  cc -o main main.c at24c.c -DC16
  24C32  cc -o main main.c at24c.c -DC32
  24C64  cc -o main main.c at24c.c -DC64
  24C128 cc -o main main.c at24c.c -DC128
  24C256 cc -o main main.c at24c.c -DC256
  24C512 cc -o main main.c at24c.c -DC512
*/

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "at24c.h"

void dump(char * title, uint8_t *dt, uint32_t n) {
  uint16_t clm = 0;
  uint8_t data;
  uint8_t sum;
  uint8_t vsum[16];
  uint8_t total =0;
  uint32_t saddr =0;
  uint32_t eaddr =n-1;
  
  printf("-------------------- [%s] --------------------\n", title);
  uint16_t i;
  for (i=0;i<16;i++) vsum[i]=0;  
  uint32_t addr;
  for (addr = saddr; addr <= eaddr; addr++) {
    data = dt[addr];
    if (clm == 0) {
      sum =0;
      printf("%05x: ",addr);
    }

    sum+=data;
    vsum[addr % 16]+=data;
    
    printf("%02x ",data);
    clm++;
    if (clm == 16) {
      printf("|%02x \n",sum);
      clm = 0;
    }
  }
  printf("-------------------- [%s] --------------------\n", title);
  printf("       ");
  for (i=0; i<16;i++) {
    total+=vsum[i];
    printf("%02x ",vsum[i]);
  }
  printf("|%02x \n\n",total);
}


int main(int argc, char *argv[])
{
  char device[20];
  struct stat st;
  int ret;

  // set device file name
  int adapter_nr = 0; // i2c-0
  snprintf(device, 19, "/dev/i2c-%d", adapter_nr);
  ret = stat(device, &st); 
  printf("%s=%d\n", device,ret);
  if (ret != 0) {
    adapter_nr = 1; // i2c-1
    snprintf(device, 19, "/dev/i2c-%d", adapter_nr);
    ret = stat(device, &st); 
    printf("%s=%d\n", device,ret);
    if (ret != 0) {
      printf("i2c device not found\n");
      return 1;
    }
  }

  // set i2c address
  int i2c_addr = 0x50;
  if (argc == 2) i2c_addr = strtol(argv[1],NULL,16);
  printf("i2c address=0x%x\n",i2c_addr);

  // set EEPROM memory size
  int eeprom_bits = 0;
#ifdef C02
  eeprom_bits = 2;
#endif
#ifdef C04
  eeprom_bits = 4;
#endif
#ifdef C08
  eeprom_bits = 8;
#endif
#ifdef C16
  eeprom_bits = 16;
#endif
#ifdef C32
  eeprom_bits = 32;
#endif
#ifdef C64
  eeprom_bits = 64;
#endif
#ifdef C128
  eeprom_bits = 128;
#endif
#ifdef C256
  eeprom_bits = 256;
#endif
#ifdef C512
  eeprom_bits = 512;
#endif
  printf("eeprom_bits=%d\n",eeprom_bits);
  if (eeprom_bits == 0) {
    printf("EEPROM model not found\n");
    return 1;
  }

  // open device
  int write_cycle_time = 2;
  //int write_cycle_time = 1;
  struct eeprom e;
  ret = eeprom_open(device, i2c_addr, eeprom_bits, write_cycle_time, &e);
  //printf("eeprom_open ret=%d\n",ret);
  if (ret != 0) {
    printf("eeprom_open ret=%d\n",ret);
    return 1;
  }


  // get EEPROM size(byte)
  uint32_t eeprom_bytes = getEEPROMbytes(&e);
  printf("EEPROM chip=24C%.02d bytes=%dByte\n",eeprom_bits,eeprom_bytes);

  uint16_t mem_addr;
  uint8_t data;
  uint8_t rdata[256];
  int i;

  // write first blcok
  for(i=0;i<256;i++) {
    mem_addr = i;
    data = i;
    ret = eeprom_write_byte(&e, mem_addr, data);
    if (ret != 0) printf("eeprom_write_byte ret=%d\n",ret);
  }

  // write last blcok
  for(i=0;i<256;i++) {
    mem_addr = i + (eeprom_bytes - 0x100);
    data = 255 - i;
    ret = eeprom_write_byte(&e, mem_addr, data);
    if (ret != 0) printf("eeprom_write_byte ret=%d\n",ret);
  }

  char title[64];
  sprintf(title,"address %d-%d",eeprom_bytes-0x100,eeprom_bytes-1);

  // read last blcok
  memset(rdata, 0, sizeof(rdata));
  for(i=0;i<256;i++) {
    mem_addr = i + (eeprom_bytes - 0x100);
    rdata[i] = eeprom_read_byte(&e, mem_addr);
    //ret = eeprom_24c32_read_byte(&e, mem_addr);
    if (ret != 0) printf("eeprom_read_byte ret=%d\n",ret);
  }
  dump(title, rdata, 256);

  // read first blcok
  memset(rdata, 0, sizeof(rdata));
  for(i=0;i<256;i++) {
    mem_addr = i;
    rdata[i] = eeprom_read_byte(&e, mem_addr);
    //ret = eeprom_24c32_read_byte(&e, mem_addr);
    if (ret != 0) printf("eeprom_read_byte ret=%d\n",ret);
  }
  dump("address 0-255", rdata, 256);

  eeprom_close(&e);

}

