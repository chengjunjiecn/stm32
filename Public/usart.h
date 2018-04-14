#ifndef __usart_H
#define __usart_H

#include "system.h" 
#include "stdio.h" 
#include "common.h"
#include <string.h>
void USART1_Init(u32 bound);
void handData();
void sendData();
void decode(Data * d );
void encode(Data d);
void sendDone();
void encodeTest();
#endif


