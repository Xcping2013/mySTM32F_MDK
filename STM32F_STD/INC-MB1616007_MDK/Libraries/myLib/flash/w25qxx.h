#ifndef __W25QXX_H
#define __W25QXX_H

#include "mcu_spi.h"

#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
//cmd
#define W25X_WriteEnable			0x06
#define W25X_WriteDisable			0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadData					0x03
#define W25X_FastReadData			0x0B
#define W25X_FastReadDual			0x3B
#define W25X_PageProgram			0x02
#define W25X_BlockErase				0xD8
#define W25X_SectorErase			0x20
#define W25X_ChipErase				0xC7
#define W25X_PowerDown				0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID					0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F

typedef struct 
{
	u16	id;
	void (*init)(SPI_CONFIG_TYPE* );
	void (*read)(SPI_CONFIG_TYPE* ,uint8_t *, uint32_t , uint16_t );
	void (*fast_read)(SPI_CONFIG_TYPE* ,uint8_t *, uint32_t , uint16_t );
	void (*write_page)(SPI_CONFIG_TYPE* ,uint8_t *, uint32_t , uint16_t );
	void (*write)(SPI_CONFIG_TYPE* ,uint8_t *, uint32_t , uint16_t );
	void (*erase_sector)(SPI_CONFIG_TYPE* ,uint32_t );
	void (*erase_chip)(SPI_CONFIG_TYPE* );
	void (*powerDown)(SPI_CONFIG_TYPE* );
	void (*wakeUp)(SPI_CONFIG_TYPE* );
}W25QXX_DrvTypeDef;

extern W25QXX_DrvTypeDef w25qxx;

void w25qxx_init(SPI_CONFIG_TYPE* config);
void w25qxx_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
void w25qxx_fast_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
void w25qxx_write_page(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
void w25qxx_write(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
void w25qxx_erase_sector(SPI_CONFIG_TYPE* config,uint32_t dst_addr);
void w25qxx_erase_chip(SPI_CONFIG_TYPE* config);
void w25qxx_waitBusy(SPI_CONFIG_TYPE* config);
void w25qxx_powerDown(SPI_CONFIG_TYPE* config);
void w25qxx_wakeUP(SPI_CONFIG_TYPE* config);

#endif
















