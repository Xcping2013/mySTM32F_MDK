/**
  \file SPI.c
  \version 1.00

  \brief SPI functions

  This file provides all functions needed for SPI
  access to the other ICs (TMC429, TMC262, EEPROM).
*/
#include "bits.h"
#include "main.h"

/****************************************************//**
  \fn InitSPI(void)
  \brief SPI intialization

  This functions initializes the SPI. It has to be called
  once at the beginning of the main() function, before
  any other things are done that need SPI access.
********************************************************/
void InitSPI(void)
{
  SPI_InitTypeDef SPIInit;
  GPIO_InitTypeDef GPIO_InitStructure;

  //Clock f? SPI1 einschalten
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  //PA5..PA7 mit SPI1 verbinden
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		//CS
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_4); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //SPI initialisieren
  SPIInit.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
  SPIInit.SPI_Mode=SPI_Mode_Master;
  SPIInit.SPI_DataSize=SPI_DataSize_8b;
  SPIInit.SPI_CPOL=SPI_CPOL_High;
  SPIInit.SPI_CPHA=SPI_CPHA_2Edge;
  SPIInit.SPI_NSS=SPI_NSS_Soft;
  SPIInit.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//1MHZ
  SPIInit.SPI_FirstBit=SPI_FirstBit_MSB;
  SPIInit.SPI_CRCPolynomial=0;
  SPI_Init(SPI1, &SPIInit);
  SPI_Cmd(SPI1, ENABLE);
}
/***************************************************************//**
   \fn ReadWriteSPI(UCHAR DeviceNumber, UCHAR Data, UCHAR LastTransfer)
   \brief SPI communication

   \param DeviceNumber  Index of the SPI device (see the constants in stepRocker.h)
   \param Data          Data byte to be sent
   \param LastTransfer  FALSE: device will be kept selected (for sending more bytes) \n
                        TRUE: the device will be deselected after the transfer \n

   \return Received byte

   This function handles SPI transfer. One byte will be sent, and the
   one byte will be received.
********************************************************************/
UCHAR ReadWriteSPI(UCHAR DeviceNumber, UCHAR Data, UCHAR LastTransfer)
{
	u8 retry=0;
  switch(DeviceNumber)
  {
    case SPI_DEV_TMC429:
      SELECT_TMC429();

      while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET)
			{
				retry++;
				if(retry>200)
				return 0;;
			}
      SPI_I2S_SendData(SPI1, Data);
			retry=0;
      while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET)
			{
				retry++;
				if(retry>200)
				return 0;;
			}
      if(LastTransfer) DESELECT_TMC429();

      return SPI_I2S_ReceiveData(SPI1);

    default:
      return 0;
  }
}

