# Raspberry-24Cxx

Two-Wire Serial EEPROM Access Library for RaspberryPi.   

I forked from here.   
https://www.richud.com/wiki/Rasberry_Pi_I2C_EEPROM_Program

---

# Build
for 24C02   
cc -o main main.c 24cXX.c -DC02

for 24C04   
cc -o main main.c 24cXX.c -DC04

for 24C08   
cc -o main main.c 24cXX.c -DC08

for 24C16   
cc -o main main.c 24cXX.c -DC16

for 24C32   
cc -o main main.c 24cXX.c -DC32

for 24C64   
cc -o main main.c 24cXX.c -DC64

for 24C128   
cc -o main main.c 24cXX.c -DC128

for 24C256   
cc -o main main.c 24cXX.c -DC256

for 24C512   
cc -o main main.c 24cXX.c -DC512

sudo ./main [i2c-address]   
Default i2c-address is 0x50.   

---

# API

// Open device   
int eeprom_open(char *dev_fqn, int i2c_addr, int type, int delay, struct eeprom*);   

// Get EEPROM memory size   
__u16 getEEPROMbytes(struct eeprom* e);

// Read data from EEPROM   
int eeprom_read_byte(struct eeprom* e, __u16 mem_addr);

// Write data to EEPROM   
int eeprom_write_byte(struct eeprom *e, __u16 mem_addr, __u8 data);


