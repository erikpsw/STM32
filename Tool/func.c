#include "stm32f10x.h"
#include "func.h"
#include "string.h"		

char Serial_Received[100];
uint8_t Serial_RxFlag=0;

uint8_t RxState = 0;
uint8_t pRxPacket = 0;	

void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)//每次time都自减
   {
      i=12000;  //自己定义
      while(i--) ;    //ms
   }
}

void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)//每次time都自减
   {
      i=10;  //自己定义
      while(i--) ;    //ms
   }
}



void Sound(u16 frq)//发出特定频率的声音
{
        u32 time;
        if(frq != 10000 && frq!=5000)//休止符
        {
                time = 500000/((u32)frq);//反比
                GPIO_ResetBits(GPIOB,GPIO_Pin_11);
                delay_us(time);
                GPIO_SetBits(GPIOB,GPIO_Pin_11);
                delay_us(time);
        }else if(frq == 10000)
                delay_us(200);
				else
				delay_us(80);
}

//生日歌
void play_Music_1(void)
{
        //音谱         低1  2   3   4   5   6   7  中1  2   3   4   5   6   7  高1    2   3     4    5    6    7  不发音
        uc16 tone[] = {262,294,330,349,392,440,494,523,587,659,698,784,880,988,1047,1175,1319,1397,1568,1760,1967,10000,5000};//音频数据表
        //编号          0   1   2   3   4   5   6   7   8   9   10  11  12  13  14   15   16    17   18   19   20  21
        
        //音谱
        u8 music[]={4,22,4,5,4,
                                                        7,6,21,
                                                        4,22,4,5,4,
                                                  8,7,21,
                                                        4,22,4,11,9,
                                                        7,6,5,21,
                                                        3,22,3,9,7,
                                                        8,7,21,
                                                        };
        //节拍
        u8 time[] = {2,2,2,4,4,        //时间--2代表半拍(100ms) 4代表一拍(200ms) 8代表两拍(400ms)
                                                         4,4,4, 
                                                         2,2,2,4,4,
                                                         4,4,4,
                                                         2,2,2,4,4,
                                                         4,4,4,4,
                                                         2,2,2,4,4,
                                                         4,4,4,
               };        
        u32 delayShow;
        delayShow = 10;//控制播放快慢
        for(int i=0;i<sizeof(music)/sizeof(music[0]);i++)//放歌
        {
                for(int j=0;j<((u16)time[i])*tone[music[i]]/delayShow;j++)
                {
                        Sound((u32)tone[music[i]]);
                }        
        }
}

#include "codetab.h"

//初始化硬件IIC引脚
void I2C_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		I2C_InitTypeDef I2C_InitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		//PB6——SCL PB7——SDA
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		I2C_DeInit(I2C1);
		I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStructure.I2C_ClockSpeed          = 400000;
		I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
		I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
		I2C_InitStructure.I2C_OwnAddress1         = 0x30;
		
		I2C_Init(I2C1, &I2C_InitStructure);
		I2C_Cmd(I2C1, ENABLE);
}

//向OLED寄存器地址写一个byte的数据
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
		while( I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) );
		I2C_GenerateSTART(I2C1, ENABLE);
		
		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) );
		I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);
		
		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) );
		I2C_SendData(I2C1, addr);
		
		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) );
		I2C_SendData(I2C1, data);
		
		while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) );
		I2C_GenerateSTOP(I2C1, ENABLE);
}

//写指令
void WriteCmd(unsigned char I2C_Command)
{
		I2C_WriteByte(0x00,I2C_Command);
}

//写数据
void WriteData(unsigned char I2C_Data)
{
		I2C_WriteByte(0x40,I2C_Data);
}

//厂家初始化代码
void OLED_Init(void)
{
		delay_ms(100);
		WriteCmd(0xAE); //display off
		WriteCmd(0x20);	//Set Memory Addressing Mode	
		WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
		WriteCmd(0xc8);	//Set COM Output Scan Direction
		WriteCmd(0x00); //---set low column address
		WriteCmd(0x10); //---set high column address
		WriteCmd(0x40); //--set start line address
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(0xff); //áá?èμ÷?ú 0x00~0xff
		WriteCmd(0xa1); //--set segment re-map 0 to 127
		WriteCmd(0xa6); //--set normal display
		WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
		WriteCmd(0x3F); //
		WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		WriteCmd(0xd3); //-set display offset
		WriteCmd(0x00); //-not offset
		WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
		WriteCmd(0xf0); //--set divide ratio
		WriteCmd(0xd9); //--set pre-charge period
		WriteCmd(0x22); //
		WriteCmd(0xda); //--set com pins hardware configuration
		WriteCmd(0x12);
		WriteCmd(0xdb); //--set vcomh
		WriteCmd(0x20); //0x20,0.77xVcc
		WriteCmd(0x8d); //--set DC-DC enable
		WriteCmd(0x14); //
		WriteCmd(0xaf); //--turn on oled panel

}

//设置光标起始坐标（x,y）
void OLED_SetPos(unsigned char x,unsigned char y)
{
		WriteCmd(0xb0+y);
		WriteCmd( (x & 0xf0) >> 4 | 0x10 );
		WriteCmd( (x & 0x0f) | 0x01 );
}

//填充整个屏幕
void OLED_Fill(unsigned char Fill_Data)
{
		unsigned char m,n;
		
		for(m=0;m<8;m++)
		{
				WriteCmd(0xb0+m);
				WriteCmd(0x00);
				WriteCmd(0x10);
				
				for(n=0;n<128;n++)
				{
						WriteData(Fill_Data);
				}
		}
}

//清屏
void OLED_CLS(void)
{
		OLED_Fill(0x00);
}

//将OLED从休眠中唤醒
void OLED_ON(void)
{
		WriteCmd(0xAF);
		WriteCmd(0x8D);
		WriteCmd(0x14);
}

//让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
void OLED_OFF(void)
{
		WriteCmd(0xAE);
		WriteCmd(0x8D);
		WriteCmd(0x10);
}

void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize)
{
		unsigned char c = 0,i = 0,j = 0;
		
		switch(TextSize)
		{
			case 1:
			{
					while(ch[j] != '\0')
					{
							c = ch[j] - 32;
							if(x>126)
							{
									x = 0;
									y++;
							}
							
							OLED_SetPos(x,y);
							
							for(i=0;i<6;i++)
							{
									WriteData(F6x8[c][i]);
							}
							x+=6;
							j++;
					}
			}
			break;
			
			case 2:
			{
					while(ch[j] != '\0')
					{
							c = ch[j] - 32;
							
							if(x>120)
							{
									x = 0;
									y++;
							}
							
							OLED_SetPos(x,y);
							
							for(i=0;i<8;i++)
							{
									WriteData(F8X16[c*16+i]);	
							}
							
							OLED_SetPos(x,y+1);
							
							for(i=0;i<8;i++)
							{
									WriteData(F8X16[c*16+i+8]);
							}
							x+=8;
							j++;		
					}
			}
			break;
		}
}

void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N)
{
		unsigned char i = 0;
		unsigned char addr = 32*N;
		
		OLED_SetPos(x,y);
		
		for(i=0;i<16;i++)
		{
				WriteData(F16X16[addr]);
				addr += 1;
		}
		
		OLED_SetPos(x,y+1);
		
		for(i=0;i<16;i++)
		{
				WriteData(F16X16[addr]);
				addr += 1;
		}
}

void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
		unsigned char x,y;
		unsigned int j = 0;
		
		if(y1 % 8 == 0)
		{
				y = y1 / 8;
		}
		else
		{
				y = y1 / 8+1;
		}
		
		for(y=y0;y<y1;y++)
		{
				OLED_SetPos(x0,y);

				for(x=x0;x<x1;x++)
				{
						WriteData(BMP1[j++]);
				}
		}
}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//发送完成
}



void Serial_SendString(char *str)
{
	for (int i=0;str[i]!='\0';i++){
		Serial_SendByte(str[i]);
	}
}

void USART1_IRQHandler(void)
{

	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if (RxState == 0)
		{
			if (RxData == '@' && Serial_RxFlag == 0)
			{
				for (unsigned int i = 0; i < strlen(Serial_Received); i++)
      				Serial_Received[i] = '\0' ;	
				RxState = 1;
				pRxPacket = 0;
			}
			
		}
		else if (RxState == 1)
		{

			if (RxData == '.')
			{
				RxState = 2;

			}
			else
			{
				Serial_Received[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 2)
		{
			RxState = 0;
			Serial_Received[pRxPacket] = '\0';
			Serial_RxFlag = 1;
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

