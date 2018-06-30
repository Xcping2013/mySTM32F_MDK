#include "mcu_spi.h"

SPI_DrvTypeDef	spi=
{
	SPI_init,
	ReadWriteSPI,
};
void SPI_init(SPI_CONFIG_TYPE *spi_config)
{
	SPI_InitTypeDef SPI_InitStructure;
	switch((uint32_t)spi_config->spix)
	{
		case	SPI1_BASE:
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
					pinMode(&PA5,GPIO_Mode_AF_PP);
					pinMode(&PA7,GPIO_Mode_AF_PP);
					pinMode(&PA6,GPIO_Mode_AF_PP);		
			break;
		case	SPI2_BASE:
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	
					pinMode(&PB13,GPIO_Mode_AF_PP);
					pinMode(&PB15,GPIO_Mode_AF_PP);
					pinMode(&PB14,GPIO_Mode_AF_PP);
			break;
		case	SPI3_BASE:
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
					pinMode(&PB3,GPIO_Mode_AF_PP);
					pinMode(&PB5,GPIO_Mode_AF_PP);
					pinMode(&PB4,GPIO_Mode_AF_PP);
			break;
		default:
			break;		
	}
	pinMode(spi_config->cs_pin ,GPIO_Mode_Out_PP);
	pinSet(spi_config->cs_pin);
	SPI_I2S_DeInit(spi_config->spix);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 	//全双工
  SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 									//8位数据模式 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 													//NSS软件管理
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
		
	if(spi_config->mode == SPI_MODE0)
	{
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	}	
	else if(spi_config->mode == SPI_MODE1)
	{
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	}
	else if(spi_config->mode == SPI_MODE2)
	{
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	}
	else if(spi_config->mode == SPI_MODE3)
	{
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	}	
	SPI_InitStructure.SPI_BaudRatePrescaler = spi_config->prescaler;
  SPI_InitStructure.SPI_FirstBit = spi_config->bit_order; 
	SPI_Init(spi_config->spix, &SPI_InitStructure);
  SPI_Cmd(spi_config->spix, ENABLE);
}
//
void Spi_writeByte(SPI_CONFIG_TYPE *spi_config, uint8_t data)
{
    __IO uint8_t dummyByte;
    while ((spi_config->spix->SR & SPI_I2S_FLAG_TXE) == RESET);
    spi_config->spix->DR = data;
    while ((spi_config->spix->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    dummyByte = spi_config->spix->DR;
}
//
void Spi_writeBytes(SPI_CONFIG_TYPE *spi_config, uint8_t *data, uint16_t data_length)
{
    __IO uint8_t dummyByte;

    while(data_length--)
    {
        while ((spi_config->spix->SR & SPI_I2S_FLAG_TXE) == RESET);
        spi_config->spix->DR = *data++;
        while ((spi_config->spix->SR & SPI_I2S_FLAG_RXNE) == RESET);
        dummyByte = spi_config->spix->DR;
    }
}
//
int8_t Spi_readByte(SPI_CONFIG_TYPE *spi_config)
{
    while ((spi_config->spix->SR & SPI_I2S_FLAG_TXE) == RESET) ;
    spi_config->spix->DR = 0xff;
    while ((spi_config->spix->SR & SPI_I2S_FLAG_RXNE) == RESET);
    return spi_config->spix->DR;
}
//
void Spi_readBytes(SPI_CONFIG_TYPE *spi_config, uint8_t *recv_data, uint16_t data_length)
{
    while(data_length--)
    {
        while ((spi_config->spix->SR & SPI_I2S_FLAG_TXE) == RESET)
            ;
        spi_config->spix->DR = 0xff;
        while ((spi_config->spix->SR & SPI_I2S_FLAG_RXNE) == RESET)
            ;
        *recv_data++ = spi_config->spix->DR;
    }
}
//


UCHAR ReadWriteSPI(SPI_CONFIG_TYPE *spi_config,UCHAR Data, UCHAR LastTransfer)
{
	u8 retry=0;
	pinReset(spi_config->cs_pin);
	while(SPI_I2S_GetFlagStatus(spi_config->spix, SPI_I2S_FLAG_TXE)==RESET)
	{
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(spi_config->spix, Data);
	retry=0;
	while(SPI_I2S_GetFlagStatus(spi_config->spix, SPI_I2S_FLAG_RXNE)==RESET)
	{
		retry++;
		if(retry>200)return 0;
	}
	retry=SPI_I2S_ReceiveData(spi_config->spix);
  if(LastTransfer) pinSet(spi_config->cs_pin);
  return retry;
}



