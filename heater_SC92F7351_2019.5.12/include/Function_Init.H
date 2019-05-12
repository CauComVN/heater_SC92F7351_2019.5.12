#ifndef _Function_Init_H_
#define _Function_Init_H_

#include "SC92F735x_C.H"
#include "intrins.H"
#define uchar unsigned char
#define uint unsigned int

#define IapROM  0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作

extern void IO_Init(void);

extern void ADC_Test(void);
extern float ADC_Convert(void);
extern void ADC_channel(unsigned char channel);

#endif