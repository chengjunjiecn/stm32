#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "led.h"
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
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


