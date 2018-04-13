#include "usart.h"
#include "led.h"
#include "wkup.h"
int isSystemActive = 0;
unsigned char buff[1024];
unsigned char outbuff[1024];
int outbufflen = 0;
int recstatu = 0;
int	ccnt	 = 0 ;
int	packerflag = 0;

int fputc(int ch,FILE *p)  //º¯ÊýÄ¬ÈÏµÄ£¬ÔÚÊ¹ÓÃprintfº¯ÊýÊ±×Ô¶¯µ÷ÓÃ
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* º¯ Êý Ãû         : USART1_Init
* º¯Êý¹¦ÄÜ		   : USART1³õÊ¼»¯º¯Êý
* Êä    Èë         : bound:²¨ÌØÂÊ
* Êä    ³ö         : ÎÞ
*******************************************************************************/
void USART1_Init(u32 bound)
{
   //GPIO¶Ë¿ÚÉèÖÃ
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //´ò¿ªÊ±ÖÓ


	/*  ÅäÖÃGPIOµÄÄ£Ê½ºÍIO¿Ú */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //´®¿ÚÊä³öPA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //¸´ÓÃÍÆÍìÊä³ö
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ³õÊ¼»¯´®¿ÚÊäÈëIO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //´®¿ÚÊäÈëPA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //Ä£ÄâÊäÈë
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ³õÊ¼»¯GPIO */


   //USART1 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
	USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1

	USART_Cmd(USART1, ENABLE);  //Ê¹ÄÜ´®¿Ú1

	USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖÐ¶Ï

	//Usart1 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//´®¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
}

/*******************************************************************************
* º¯ Êý Ãû         : USART1_IRQHandler
* º¯Êý¹¦ÄÜ		   : USART1ÖÐ¶Ïº¯Êý
* Êä    Èë         : ÎÞ
* Êä    ³ö         : ÎÞ
*******************************************************************************/
void USART1_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	u8 r;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï
	{
		r =USART_ReceiveData(USART1);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
	}
	USART_ClearFlag(USART1,USART_FLAG_TC);

	if(r == 0xFE)
	{
		isSystemActive = 1;
	}
	if(r == 0x68 && recstatu == 0)//head
	{
		recstatu = 1;
		ccnt	 = 0 ;
		packerflag = 0;
		return ;
	}
	if(r == 0x16 )//tail
	{
		recstatu = 0;
		packerflag = 1;
		return ;
	}
	if(recstatu ==1)
	{
		buff[ccnt++] = r;
	}
}

int isReceivedFrame()
{
	return packerflag;
}
int getSystemActive()
{
	return isSystemActive;
}
void setSystemActive(int state )
{
	isSystemActive = state;
}
void handData()
{
	Data d;
	memset(&d,0,sizeof(d));
	decode(&d);
	encode(d);
	sendData();

}
void sendData()
{
	int i = 0;

	USART_SendData(USART1,0x68);
	for(i = 0;i<outbufflen;i++)
	{
		USART_SendData(USART1,outbufflen[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	}
	memset(outbufflen,0,sizeof(outbufflen));
	outbufflen = 0;
}

void decode(Data * d )
{

	int i;
	memcpy(d->addr,&buff[0],6);
	d->head = buff[6];
	d->controlCode = buff[7];
	d->len = buff[8];
	memcpy(d->data,&buff[9],d->len);
	for(i = 0; i < d->len;i++)
	{
		d->data[i] -= 0x33H;
	}
	d->csc = buff[9+d->len];
	int ret = crc(buff,ccnt);
	if(ret == d->csc )
	{
		printf("crc check is ok");
	}else
	{
		printf("crc check is failed %d",ret);
	}
}
void encode(Data d)
{
	memcpy(&outbuff[0],d->addr,6);
	outbuff[6] = d->head ;
	outbuff[7] = d->controlCode ;
	outbuff[8] = d->len ;
	memcpy(&outbuff[9],d->data,d->len);
	int ret = crc(outbuff,9+d->len);
	outbuff[9+d->len] = ret ;

}
void bit_set(unsigned char *p_data, unsigned char pos, int flag)
{
	if (flag == 1)
	{
	    *p_data |= (1 << (pos - 1));
	}
	if (flag == 0)
	{
	    *p_data &= ~(1 << (pos - 1));
	}
}
//A&(1<<i)
int bit_get(int data, int index)
{
	int ret = 0;
	ret = data&(1<<(index - 1));
	if(ret > 0)
	{
		return 1;
	} else
	{
		return 0;
	}
}
int crc(unsigned char buffer[], int len)
{
	int sum = 0x68;
	int i = 0;
	for (i = 0; i < len; i++)
	{
		sum += buffer[i];
	}
	sum = sum % 256;
	return sum;
}
//68 A0 A1 A2 A3 A4 A5 68 0 09 31 32 33 34 35 36 2e 37 38 7A 16   send
//68 A0 A1 A2 A3 A4 A5 68 0 09 64 65 66 67 68 69 61 70 71 81 16   send +33h

//68 A0 A1 A2 A3 A4 A5 68 80 0 31 16   receive send

