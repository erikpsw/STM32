#include "stm32f10x.h"

void Item_Init(char pinpos,uint16_t GPIO_Pin,GPIOSpeed_TypeDef GPIO_Speed,GPIOMode_TypeDef GPIO_Mode){
    if(pinpos=='A'){
        
        GPIO_InitTypeDef GPIO_InitStructure={GPIO_Pin,GPIO_Speed,GPIO_Mode};
        GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
    }
    else if(pinpos=='B'){
         GPIO_InitTypeDef GPIO_InitStructure={GPIO_Pin,GPIO_Speed,GPIO_Mode};
        GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化
    }
    else if(pinpos=='C'){
        GPIO_InitTypeDef GPIO_InitStructure={GPIO_Pin,GPIO_Speed,GPIO_Mode};
        GPIO_Init(GPIOC,&GPIO_InitStructure);//初始化
    }

}

void Exti_Init(uint32_t EXTI_Line,EXTIMode_TypeDef EXTI_Mode,EXTITrigger_TypeDef EXTI_Trigger){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//C外设时钟控制函数
        EXTI_InitTypeDef EXTI_InitStructure={EXTI_Line,EXTI_Mode,EXTI_Trigger,ENABLE};
        EXTI_Init(&EXTI_InitStructure);//初始化
}

void Nvic_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority){
        NVIC_InitTypeDef NVIC_InitStructure={NVIC_IRQChannel,NVIC_IRQChannelPreemptionPriority,NVIC_IRQChannelSubPriority,ENABLE};
        NVIC_Init(&NVIC_InitStructure);//初始化
}

void USART1_Init(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl){
        USART_InitTypeDef USART_InitStructure={USART_BaudRate,USART_WordLength,USART_StopBits,USART_Parity,USART_Mode,USART_HardwareFlowControl};
        USART_Init(USART1,&USART_InitStructure);//初始化
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_Init(&NVIC_InitStructure);
        USART_Cmd(USART1,ENABLE);
}

