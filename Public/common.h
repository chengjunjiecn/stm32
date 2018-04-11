#ifndef _common_H
#define _common_H

#include "usart.h"		 
#include "led.h"
#include "wkup.h"

typedef struct
{
	unsigned char addr[6];
	unsigned char head;
	unsigned char controlCode;
	unsigned char len;
	//unsigned char* data;
	//unsigned char csc;

}Data;

#endif