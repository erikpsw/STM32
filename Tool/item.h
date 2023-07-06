#ifndef _item_h
#define _item_h

#include "stm32f10x.h"

void Item_Init(char pinpos,uint16_t pinnum,GPIOSpeed_TypeDef speed,GPIOMode_TypeDef mode);
void Exti_Init(uint32_t pinnum,EXTIMode_TypeDef mode,EXTITrigger_TypeDef trigger);
void Nvic_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority);
void USART1_Init(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);

#endif

