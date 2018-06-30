/**
  \file 		UART.c
  \author 	Xcping2016
  \version 	1.00

  \brief 		UART functions

  This file provides all functions needed for using
  the UART interface.
*/
#include "main.h"
#include "UART.h"
#include "stdarg.h"
#include "stdio.h"

u8 	USART_RX_BUF[USART_REC_LEN];  
u16 USART_RX_STA=0;       


#if defined(UART_INTERFACE_1)

#define UART_INTR_PRI        4

#if UART_INTERFACE_1==UART_1
  #define USARTx USART1
#elif UART_INTERFACE_1==UART_2
  #define USARTx USART2
#elif UART_INTERFACE_1==UART_3
  #define USARTx USART3
#else
  #error "UART device not supported"
#endif

/*******************************************************************
   Funktion: InitUART()
   Parameter: Baudrate: Baudrate der Schnittstelle (z.B. 9600)

   Zweck: Intialisierung der UART
********************************************************************/
void InitUART(int BaudRate)
{
  USART_InitTypeDef  UART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  #if UART_INTERFACE_1 == UART_2
  //UART2 aktivieren
  USART_DeInit(USART2);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  //GPIOA aktivieren (UART2-Pins)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  GPIO_PinRemapConfig(GPIO_Remap_USART2, DISABLE);

  //UART2-Pins zuweisen (PA2 und PA3)
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  #elif UART_INTERFACE_1 == UART_1
  //UART1 aktivieren
  USART_DeInit(USART1);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  #elif UART_INTERFACE_1 == UART_3
  //UART3 aktivieren
  USART_DeInit(USART3);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


  //UART3-Pins zuweisen (PB10 und PB11)
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, DISABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  #else
  #error "UART pin mapping not defined"
  #endif

  //UART konfigurieren
  USART_StructInit(&UART_InitStructure);
  UART_InitStructure.USART_BaudRate=BaudRate;

  USART_Init(USARTx,&UART_InitStructure);

  //Interrupt f? UART aktivieren
  #if UART_INTERFACE_1 == UART_2
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  #elif UART_INTERFACE_1 == UART_1
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  #elif UART_INTERFACE_1 == UART_3
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  #else
  #error "Device not supported!"
  #endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_INTR_PRI;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  USART_ClearFlag(USARTx, USART_FLAG_CTS | USART_FLAG_LBD  | USART_FLAG_TXE  |
                          USART_FLAG_TC  | USART_FLAG_RXNE | USART_FLAG_IDLE |
                          USART_FLAG_ORE | USART_FLAG_NE   | USART_FLAG_FE |
                          USART_FLAG_PE);
  USART_ITConfig(USARTx,USART_IT_PE  ,DISABLE);
  USART_ITConfig(USARTx,USART_IT_TXE ,DISABLE);
  USART_ITConfig(USARTx,USART_IT_TC  ,DISABLE);
  USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);
  USART_ITConfig(USARTx,USART_IT_IDLE,DISABLE);
  USART_ITConfig(USARTx,USART_IT_LBD ,DISABLE);
  USART_ITConfig(USARTx,USART_IT_CTS ,DISABLE);
  USART_ITConfig(USARTx,USART_IT_ERR ,DISABLE);

  USART_Cmd(USARTx, ENABLE);
}
//串口发送一个字节数据
void UART_SendChar(u8 ch)
{  
	if(ch=='\n')
	{
		USART_SendData(USARTx, '\r');
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USARTx, ch);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
	}
	else 
	{
		USART_SendData(USARTx, ch);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
	}
}
//串口发送字符串
void UART_SendStr(char *ptr)
{
  	while(*ptr!='\0') UART_SendChar(*ptr++);
}
//串口格式打印输出
void UART_Printf(const char *fmt,...)
{   
	va_list ap;	   						//This is support the format output 
	char string[200];
	va_start(ap,fmt);
	vsprintf(string,fmt,ap); 	//Store the string in the String buffer
	UART_SendStr(string);
	va_end(ap);	 
}
/*******************************************************************
  UART-Interrupthandler
  Wird durch den NVIC aufgerufen, wenn ein UART-Interrupt auftritt.
  Dies paasiert, wenn ein Zeichen angekommen ist oder
  ein Zeichen gesendet werden kann.
  Der Aufruf dieser Funktion mu? in stm32f10x_it.c eingetragen werden.
********************************************************************/
#if UART_INTERFACE_1 == UART_1
void USART1_IRQHandler(void)
#elif UART_INTERFACE_1 == UART_2
void USART2_IRQHandler(void)
#elif UART_INTERFACE_1 == UART_3
void USART3_IRQHandler(void)
#else
#error "UART interrupt handler not defined"
#endif
{
  if(USARTx->SR & USART_FLAG_RXNE)
  {
		u8 Res = USART_ReceiveData(USARTx);									//读取接收到的数据	
		//USARTx->DR = (Res & (uint16_t)0x01FF);
		if((USART_RX_STA&0x8000)==0)												//接收未完成
		{
			if(USART_RX_STA&0x4000)															//接收到了0x0d
			{
				if(Res!=0x0a)	USART_RX_STA=0;											//接收错误,重新开始
				else 					USART_RX_STA|=0x8000;												//接收完成了 
			}
			else 																								//还没收到0X0D
			{	
				if(Res==0x0d)	USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		} 
	}
}

#endif


#if defined(UART_INTERFACE_2)

#if UART_INTERFACE_2==UART_1
   #define USARTy USART1
#elif UART_INTERFACE_2==UART_2
   #define USARTy USART2
#elif UART_INTERFACE_2==UART_3
   #define USARTy USART3
#else
  #error "UART device not supported"
#endif

/*******************************************************************
   Funktion: InitUART_2()
   Parameter: Baudrate: Baudrate der Schnittstelle (z.B. 9600)

   Zweck: Intialisierung der UART
********************************************************************/
void InitUART_2(int BaudRate)
{
  USART_InitTypeDef  UART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  #if UART_INTERFACE_2 == UART_2
  //UART2 aktivieren
  USART_DeInit(USART2);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  //GPIOA aktivieren (UART2-Pins)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  GPIO_PinRemapConfig(GPIO_Remap_USART2, DISABLE);

  //UART2-Pins zuweisen (PA2 und PA3)
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  #elif UART_INTERFACE_2 == UART_1
  //UART1 aktivieren
  USART_DeInit(USART1);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


  //UART1-Pins zuweisen (PA9 und PA10)
  GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  #elif UART_INTERFACE_2 == UART_3
  //UART3 aktivieren
  USART_DeInit(USART3);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


  //UART3-Pins zuweisen (PB10 und PB11)
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, DISABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  #else
  #error "UART pin mapping not defined"
  #endif

  //UART konfigurieren
  USART_StructInit(&UART_InitStructure);
  UART_InitStructure.USART_BaudRate=BaudRate;

  USART_Init(USARTy,&UART_InitStructure);

  //Interrupt f? UART aktivieren
  #if UART_INTERFACE_2 == UART_2
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  #elif UART_INTERFACE_2 == UART_1
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  #elif UART_INTERFACE_2 == UART_3
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  #else
  #error "Device not supported!"
  #endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_INTR_PRI;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  USART_ClearFlag(USARTy, USART_FLAG_CTS | USART_FLAG_LBD  | USART_FLAG_TXE  |
                          USART_FLAG_TC  | USART_FLAG_RXNE | USART_FLAG_IDLE |
                          USART_FLAG_ORE | USART_FLAG_NE   | USART_FLAG_FE |
                          USART_FLAG_PE);
  USART_ITConfig(USARTy,USART_IT_PE  ,DISABLE);
  USART_ITConfig(USARTy,USART_IT_TXE ,ENABLE);
  USART_ITConfig(USARTy,USART_IT_TC  ,ENABLE);
  USART_ITConfig(USARTy,USART_IT_RXNE,ENABLE);
  USART_ITConfig(USARTy,USART_IT_IDLE,DISABLE);
  USART_ITConfig(USARTy,USART_IT_LBD ,DISABLE);
  USART_ITConfig(USARTy,USART_IT_CTS ,DISABLE);
  USART_ITConfig(USARTy,USART_IT_ERR ,DISABLE);

  USART_Cmd(USARTy, ENABLE);
}


/*******************************************************************
  UART-Interrupthandler
  Wird durch den NVIC aufgerufen, wenn ein UART-Interrupt auftritt.
  Dies paasiert, wenn ein Zeichen angekommen ist oder
  ein Zeichen gesendet werden kann.
  Der Aufruf dieser Funktion mu? in stm32f10x_it.c eingetragen werden.
********************************************************************/
#if UART_INTERFACE_2 == UART_1
void USART1_IRQHandler(void)
#elif UART_INTERFACE_2 == UART_2
void USART2_IRQHandler(void)
#elif UART_INTERFACE_2 == UART_3
void USART3_IRQHandler(void)
#else
#error "UART interrupt handler not defined"
#endif
{
  int i;

  //Ist ein Zeichen  abgekommen?
  if(USARTy->SR & USART_FLAG_RXNE)
  {
    //Wenn RS485 gerade auf Senden geschaltet ist, dann ist
    //es ein Echo, das hier ignoriert wird.
    if(IS_RS485_SENDING_2())
    {
      i=USARTy->DR;
    }
    else
    {
      //Zeichen in den Empfangspuffer kopieren
      i=UART2RxWritePtr+1;
      if(i==UART_BUFFER_SIZE) i=0;

      if(i!=UART2RxReadPtr)
      {
        UART2RxBuffer[UART2RxWritePtr]=USARTy->DR;
        UART2RxWritePtr=i;
      }

      //Empfangs-Timeout auf Startwert setzen
      UART2TimeoutTimer=UART_TIMEOUT_VALUE;

      //Sendeverz?erung auf Startwert setzen
      UART2TransmitDelayTimer=UART2TransmitDelay;
    }
  }

  //Kann das n?hste Zeichen gesendet werden?
  if(USARTy->SR & USART_FLAG_TXE)
  {
    if(UART2TransmitDelayTimer==0)
    {
      if(UART2TxWritePtr!=UART2TxReadPtr)
      {
        SET_RS485_SEND_MODE_2();
        USARTy->DR=UART2TxBuffer[UART2TxReadPtr++];
        if(UART2TxReadPtr==UART_BUFFER_SIZE) UART2TxReadPtr=0;
      }
      else
      {
        //Sendeinterrupt deaktivieren, wenn kein Zeichen im Sendepuffer ist
        USART_ITConfig(USARTy, USART_IT_TXE ,DISABLE);
      }
    }
  }

  //Allerletztes Bit gesendet?
  if(USARTy->SR & USART_FLAG_TC)
  {
    USART_ClearITPendingBit(USARTy, USART_IT_TC);
    if(UART2TxReadPtr==UART2TxWritePtr) SET_RS485_RECEIVE_MODE_2();
  }
}


/*******************************************************************
   Funktion: WriteUART()
   Parameter: ch: Zu schreibendes Zeichen

   Zweck: Senden eines Zeichens ?er UART3
          (Einstellen in den Sendepuffer)
********************************************************************/
void WriteUART_2(UCHAR ch)
{
  int i;

  //Zeichen in die Warteschlange stellen
  i=UART2TxWritePtr+1;
  if(i==UART_BUFFER_SIZE) i=0;

  if(i!=UART2TxReadPtr)
  {
    UART2TxBuffer[UART2TxWritePtr]=ch;
    UART2TxWritePtr=i;

    //Sendeinterrupt aktivieren
    USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);
  }
}


/*******************************************************************
   Funktion: ReadUART()
   Parameter: *ch: Zeiger auf Variable f? zu lesendes Zeichen

   R?kgabewert: TRUE bei Erfolg
                 FALSE wenn kein Zeichen da war
   Zweck: Lesen eines Zeichens aus dem Empfangspuffer der UART3
          (Einstellen in den Sendepuffer)
********************************************************************/
UCHAR ReadUART_2(UCHAR *ch)
{
  //Kein Zeichen vorhanden?
  if(UART2RxReadPtr==UART2RxWritePtr) return FALSE;

  //Zeichen aus dem Puffer holen
  *ch=UART2RxBuffer[UART2RxReadPtr++];
  if(UART2RxReadPtr==UART_BUFFER_SIZE)  UART2RxReadPtr=0;

  return TRUE;
}


/*******************************************************************
   Funktion: SetUARTTransmitDelay()
   Parameter: Delay: Sendeverz?erung in ms

   R?kgabewert: ---

   Zweck: Setzen der Sendeverz?erung
          (f? einige RS485-Adapter wichtig)
********************************************************************/
void SetUARTTransmitDelay_2(UINT Delay)
{
  UART2TransmitDelay=Delay;
}


/*******************************************************************
   Funktion: CheckUARTTimeout()
   Parameter: ---

   R?kgabewert: TRUE wenn zwischenzeitlich ein Timeout aufgetreten ist
                 FALSE wenn kein Timeout aufgetreten ist
   Zweck: Pr?en, ob beim Empfangen ein Timeout aufgetreten ist (also
          Zeit zwischen zwei Bytes >5ms) und Zur?ksetzen des
          Timeout-Flag.
********************************************************************/
UINT CheckUARTTimeout_2(void)
{
  if(UART2TimeoutFlag)
  {
    UART2TimeoutFlag=FALSE;
    return TRUE;
  }
  else return FALSE;
}


/*******************************************************************
   Funktion: ClearUARTRxBuffer()
   Parameter: ---

   R?kgabewert: ---

   Zweck: L?chen des Empfangspuffers
********************************************************************/
void ClearUARTRxBuffer_2(void)
{
  DisableInterrupts();
  UART2RxReadPtr=0;
  UART2RxWritePtr=0;
  EnableInterrupts();
}


/*******************************************************************
   Funktion: DisableRS485()
   Parameter: ---

   R?kgabewert: ---

   Zweck: Abschalten der RS485-Schnittstelle zum TMC562
********************************************************************/
void DisableRS485(void)
{
  USART_ITConfig(USARTy,USART_IT_TXE ,DISABLE);
  USART_ITConfig(USARTy,USART_IT_TC  ,DISABLE);
  USART_ITConfig(USARTy,USART_IT_RXNE,DISABLE);
  SET_RS485_RECEIVE_MODE_2();
}


/*******************************************************************
   Funktion: DisableRS485()
   Parameter: ---

   R?kgabewert: ---

   Zweck: Wiedereinschalten der RS485-Schnittstelle zum TMC562
********************************************************************/
void EnableRS485(void)
{
  USART_ITConfig(USARTy,USART_IT_TXE ,ENABLE);
  USART_ITConfig(USARTy,USART_IT_TC  ,ENABLE);
  USART_ITConfig(USARTy,USART_IT_RXNE,ENABLE);
}
#endif
