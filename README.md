# Raspberry-at24c

Two-Wire Serial EEPROM Access Library for RaspberryPi.   

I forked from [eeprog-0.7.6-tear5.tar.gz](https://www.richud.com/wiki/Rasberry_Pi_I2C_EEPROM_Program)   

---

# Memory size

|Device|Number of Bits|Number of 8-bit Bytes|i2c address|
|:---|:---|:---|:---|
|24C02|2K|256|0x50-0x57|
|24C04|4K|512|0x50/0x52/0x54/0x56|
|24C08|8K|1024|0x50/0x54|
|24C16|16K|2048|0x50|
|24C32|32K|4096|0x50-0x57|
|24C64|64K|8192|0x50-0x57|
|24C128|128K|16384|0x50-0x57|
|24C256|256K|32768|0x50-0x57|
|24C512|512K|65536|0x50-0x57|

---

# Software requirement

WiringPi Library   

---

# Build
for 24C02   
cc -o main main.c at24c.c -DC02

for 24C04   
cc -o main main.c at24c.c -DC04

for 24C08   
cc -o main main.c at24c.c -DC08

for 24C16   
cc -o main main.c at24c.c -DC16

for 24C32   
cc -o main main.c at24c.c -DC32

for 24C64   
cc -o main main.c at24c.c -DC64

for 24C128   
cc -o main main.c at24c.c -DC128

for 24C256   
cc -o main main.c at24c.c -DC256

for 24C512   
cc -o main main.c at24c.c -DC512

sudo ./main [i2c-address]   
Default i2c-address is 0x50.   

---

- 24C04   
![24c04](https://user-images.githubusercontent.com/6020549/79177539-08dd1480-7e3e-11ea-82d1-0c2baf508fe8.jpg)

- 24C16   
![24c16](https://user-images.githubusercontent.com/6020549/79177549-0c709b80-7e3e-11ea-9342-39a12b610e21.jpg)

- 24C32   
![24c32](https://user-images.githubusercontent.com/6020549/79177555-0f6b8c00-7e3e-11ea-9dc1-ea0ee03f80f1.jpg)

- 24C64   
![24c64](https://user-images.githubusercontent.com/6020549/79177559-12ff1300-7e3e-11ea-811e-10c509326e99.jpg)

- 24C128   
![24c128](https://user-images.githubusercontent.com/6020549/79177565-15fa0380-7e3e-11ea-966e-85721f64bd66.jpg)

- 24C256   
![24c256](https://user-images.githubusercontent.com/6020549/79177569-198d8a80-7e3e-11ea-81fd-baaff732252f.jpg)

---

# API

```
// Open device
int eeprom_open(char *dev_fqn, int i2c_addr, int bits, int delay, struct eeprom*);

// Get EEPROM memory size
__u16 getEEPROMbytes(struct eeprom* e);

// Read data from EEPROM
int eeprom_read_byte(struct eeprom* e, __u16 mem_addr);

// Write data to EEPROM
int eeprom_write_byte(struct eeprom *e, __u16 mem_addr, __u8 data);
```

---

# Wireing
A0 A1 A2 is used to select i2c address.   
When A0 A1 A2 is GND, i2c address is 0x50.   

![24cxx](https://user-images.githubusercontent.com/6020549/59955416-fd8eb600-94c4-11e9-87f8-246db2913a04.jpg)

