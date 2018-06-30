/**
  \file 			UART.h
  \author 		Xcping2016
  \version 		1.00

  \brief Motor monitoring

  This file contains the definitions of the UART functions.
*/
#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

#define USART_REC_LEN  			100 

extern u8  USART_RX_BUF[USART_REC_LEN]; 
extern u16 USART_RX_STA;

void InitUART(int BaudRate);

void UART_SendChar(u8);
void UART_SendStr(char *ptr);
void UART_Printf(const char *fmt,...);

#endif

