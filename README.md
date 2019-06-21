# Raspberry-24Cxx

Two-Wire Serial EEPROM Access Library for RaspberryPi.   

I forked from here.   
https://www.richud.com/wiki/Rasberry_Pi_I2C_EEPROM_Program
eeprog-0.7.6-tear5.tar.gz

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

24C64 with Raspberry Pi   
![24cxx-raspberry](https://user-images.githubusercontent.com/6020549/59955201-e0a5b300-94c3-11e9-96a0-36d694c5dec9.jpg)

24C04 with Orange Pi   
![24cxx-orangepi](https://user-images.githubusercontent.com/6020549/59955202-e0a5b300-94c3-11e9-97c5-d980e950d726.jpg)

---

# API

// Open device   
int eeprom_open(char *dev_fqn, int i2c_addr, int bits, int delay, struct eeprom*);   

// Get EEPROM memory size   
__u16 getEEPROMbytes(struct eeprom* e);

// Read data from EEPROM   
int eeprom_read_byte(struct eeprom* e, __u16 mem_addr);

// Write data to EEPROM   
int eeprom_write_byte(struct eeprom *e, __u16 mem_addr, __u8 data);

---

# Wireing

![24cxx](https://user-images.githubusercontent.com/6020549/59955416-fd8eb600-94c4-11e9-87f8-246db2913a04.jpg)

