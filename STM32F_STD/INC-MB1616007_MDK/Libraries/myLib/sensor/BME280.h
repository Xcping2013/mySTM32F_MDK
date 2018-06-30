#ifndef __BME280_H
#define __BME280_H

#include "mcu_spi.h"

#define BME280_SPI_MASK 0x7F

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
}bme280_DrvTypeDef;

extern bme280_DrvTypeDef w25qxx;

void bme280_init(SPI_CONFIG_TYPE* config);
void bme280_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
void bme280_fast_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
void bme280_write_page(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
void bme280_write(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
void bme280_erase_sector(SPI_CONFIG_TYPE* config,uint32_t dst_addr);
void bme280_erase_chip(SPI_CONFIG_TYPE* config);
void bme280_waitBusy(SPI_CONFIG_TYPE* config);
void bme280_powerDown(SPI_CONFIG_TYPE* config);
void bme280_wakeUP(SPI_CONFIG_TYPE* config);

#endif
















