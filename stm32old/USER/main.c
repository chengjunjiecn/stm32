#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"


int main(void)
{ 
 
	u8 t;
	u8 len;	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(9600);	//串口初始化波特率为115200
	LED_Init();		  		//初始化与LED连接的硬件接口  

	while(1)
	{
		if(getSystemActive())
		{
			ledSlink();
			if(isReceivedFrame())
			{
				handData();
			}
		}
		else
		{
			gotoSleepMode();
		}
	}
}
