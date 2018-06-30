#ifndef __MCU_SPI_H
#define __MCU_SPI_H

#include "sys.h"

#define SPI_MODE0 0x02
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

#define SPI_CLOCK_DIV2     SPI_BaudRatePrescaler_2
#define SPI_CLOCK_DIV4     SPI_BaudRatePrescaler_4
#define SPI_CLOCK_DIV8     SPI_BaudRatePrescaler_8
#define SPI_CLOCK_DIV16    SPI_BaudRatePrescaler_16
#define SPI_CLOCK_DIV32    SPI_BaudRatePrescaler_32
#define SPI_CLOCK_DIV64    SPI_BaudRatePrescaler_64
#define SPI_CLOCK_DIV128   SPI_BaudRatePrescaler_128
#define SPI_CLOCK_DIV256   SPI_BaudRatePrescaler_256

#define SPI_BITODER_MSB		SPI_FirstBit_MSB
#define SPI_BITODER_LSB		SPI_FirstBit_LSB

typedef struct
{
	Gpio	*				cs_pin;
	SPI_TypeDef* 	spix;
	uint8_t     	mode;
	uint16_t    	prescaler;
	uint16_t    	bit_order;
} SPI_CONFIG_TYPE;

typedef struct
{
	Gpio	*				cs_pin;
	Gpio	*				sck_pin;
	Gpio	*				miso_pin;
	Gpio	*				mosi_pin;
	uint8_t     	mode;
	uint16_t    	bit_order;
	
} SOFT_SPI_CONFIG_TYPE;

typedef struct 
{
	void (*init)(SPI_CONFIG_TYPE *);
	UCHAR (*readwrite)(SPI_CONFIG_TYPE * ,UCHAR , UCHAR );
}SPI_DrvTypeDef;

extern SPI_DrvTypeDef	spi;

typedef struct 
{
	void 		(*init)(SOFT_SPI_CONFIG_TYPE *);
	UCHAR 	(*readwrite)(SOFT_SPI_CONFIG_TYPE * ,UCHAR , UCHAR );
	
	uint8_t (*readByte)(SOFT_SPI_CONFIG_TYPE * );
	void 		(*readBytes)(SOFT_SPI_CONFIG_TYPE *, uint8_t *, uint16_t );
	void 		(*writeByte)(SOFT_SPI_CONFIG_TYPE *, uint8_t );
	void  	(*writeBytes)(SOFT_SPI_CONFIG_TYPE *, uint8_t *, uint16_t );
	
}SOFT_SPI_DrvTypeDef;

extern SOFT_SPI_DrvTypeDef	soft_spi;

void   SPI_init(SPI_CONFIG_TYPE *spi_config);
UCHAR  ReadWriteSPI(SPI_CONFIG_TYPE *spi_config,UCHAR Data, UCHAR LastTransfer);

void SoftSpi_init(SOFT_SPI_CONFIG_TYPE *spi_config);
uint8_t  SoftSpi_Readwrite(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data, uint8_t LastTransfer);

uint8_t SoftSpi_readByte(SOFT_SPI_CONFIG_TYPE *spi_config );
void  SoftSpi_readBytes(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t *rcvdata, uint16_t data_length);
void  SoftSpi_writeByte(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data);
void  SoftSpi_writeBytes(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t *data, uint16_t data_length);


#endif
