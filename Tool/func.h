#ifndef _func_h
#define _func_h
#include "stm32f10x.h"

extern char Serial_Received[];
extern uint8_t Serial_RxFlag;

void delay_ms(u16 time);
void delay_us(u16 time);
void Sound(u16 frq);//发出特定频率的声音
void play_Music_1(void);



#define OLED_ADDRESS 0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteData(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char Fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize);
void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N);
void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(char *str);
void USART1_IRQHandler(void);

#endif

