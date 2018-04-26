#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "led.h"
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
typedef struct
{
	unsigned char addr[6];
	unsigned char head;
	unsigned char controlCode;
	unsigned char len;
	unsigned char data[200];
	unsigned char csc;

}Data;
void uart_init(u32 bound);
void gotoSleepMode(void);
void Enter_Standby_Mode(void);
void ledSlink(void);
void handData(void);
void sendData(void);
void decode(Data * d );
void encode(Data d);
void sendDone(void);
void encodeTest(void);
int crc(unsigned char buffer[], int len);
#endif


