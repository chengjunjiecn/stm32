#include "sys.h"
#include "usart.h"	


int isSystemActive = 0;
unsigned char inbuff[1024];
unsigned char outbuff[1024];
int outbufflen = 0;
int receiveStatus = 0;
int	inbufflen	 = 0 ;
int	frameFlag = 0;
int isCRCOK = 0;
//�ض���fputc���� 
int fputc(int ch,FILE *p) 
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}
 
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 r;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		r =USART_ReceiveData(USART1);
	}
	USART_ClearFlag(USART1,USART_FLAG_TC);

	if(r == 0xFE)
	{
		isSystemActive = 1;
	}
	if(r == 0x68 && receiveStatus == 0)//head
	{
		receiveStatus = 1;
		inbufflen	 = 0 ;
		frameFlag = 0;
		return ;
	}
	if(r == 0x16 )//tail
	{
		receiveStatus = 0;
		frameFlag = 1;
		return ;
	}
	if(receiveStatus ==1)
	{
		inbuff[inbufflen++] = r;
	}
} 

void gotoSleepMode(void)
{
	printf("����ϵͳ����ģʽ \n");
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,DISABLE);
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,DISABLE);
	__WFI();                                                 /* Request Wait For Interrupt */
}

void Enter_Standby_Mode(void)
{
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//ʹ��PWR����ʱ��
	PWR_ClearFlag(PWR_FLAG_WU);//���Wake-up ��־
	//PWR_WakeUpPinCmd(ENABLE);//ʹ�ܻ��ѹܽ�	ʹ�ܻ���ʧ�ܻ��ѹܽŹ���		
	PWR_EnterSTANDBYMode();//�������
	//PWR_EnterSleepMode(0);
}
void ledSlink()
{
		LED1=0;
		delay_ms(500); 
		LED1=1;
		delay_ms(500); 
}

int isReceivedFrame()
{
	return frameFlag;
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
	//encodeTest();
	sendData();
	sendDone();
}
void sendData()
{
	int i = 0;
	if(isCRCOK == 0 )
	{
		printf("У��ʧ�ܣ����ط�");
		return;
	}
	USART_SendData(USART1,0x68);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	for(i = 0;i<outbufflen;i++)
	{
		USART_SendData(USART1,outbuff[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	}
	USART_SendData(USART1,0x16);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	
}
void sendDone()
{
	memset(outbuff,0,sizeof(outbuff));
	outbufflen = 0;
	memset(inbuff,0,sizeof(inbuff));
	inbufflen = 0;
	receiveStatus = 0;
	inbufflen	 = 0 ;
	frameFlag = 0;
}
void decode(Data * d  )
{

	int i;
	int ret = 0;
	memcpy(d->addr,&inbuff[0],6);
	d->head = inbuff[6];
	d->controlCode = inbuff[7];
	d->len = inbuff[8];
	memcpy(d->data,&inbuff[9],d->len);
	for(i = 0; i < d->len;i++)
	{
		d->data[i] -= 0x33;
	}
	d->csc = inbuff[9+d->len];
	ret = crc(inbuff,inbufflen-1);
	if(ret == d->csc )
	{
		//printf("crc check is ok\n");
		isCRCOK = 1;
	}else
	{
		//printf("crc check is failed %d,%d\n",ret,d->csc);
		isCRCOK = 0;
	}
	frameFlag = 0;
}
void encodeTest()
{
	memcpy(outbuff,inbuff,inbufflen);
	outbufflen = inbufflen;
}
void encode(Data d)
{
	int ret = 0;
	memcpy(&outbuff[0],d.addr,6);
	outbuff[6] = d.head ;
	outbuff[7] = d.controlCode ;
	outbuff[8] = 0 ;
	//memcpy(&outbuff[9],d.data,d.len);
	ret = crc(outbuff,9);
	outbuff[9] = ret ;
	outbufflen = 10;
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
		sum = sum + buffer[i];
		
	}
	sum = sum % 256;
	return sum;
}
//68 A0 A1 A2 A3 A4 A5 68 0 09 31 32 33 34 35 36 2e 37 38 7A 16   send
//68 A0 A1 A2 A3 A4 A5 68 0 09 64 65 66 67 68 69 61 6A 6B 45 16  send +33h

//68 A0 A1 A2 A3 A4 A5 68 80 0 31 16   receive send


#endif	

 



