#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "mcu_gpio.h"
//250K		CLK delay_us(2)*2 1/4=250K

typedef struct
{
	Gpio *		scl_pin;
	Gpio *		sda_pin;
} IIC_GPIO;

typedef struct
{
//	Gpio *		scl_pin;
//	Gpio *		sda_pin;
  void      (*pin_init)(IIC_GPIO);
  uint8_t 	(*writes)(IIC_GPIO, uint8_t , uint8_t , uint16_t , uint8_t *, uint16_t );
	uint8_t		(*reads)(IIC_GPIO, uint8_t  , uint8_t , uint16_t , uint8_t *, uint16_t );

}SoftI2c_DrvTypeDef;

extern SoftI2c_DrvTypeDef	SoftI2c;

void SoftI2c_Init(IIC_GPIO pin);
uint8_t SoftI2c_writes(IIC_GPIO pin, uint8_t DeviceType , uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t num_to_write);
uint8_t SoftI2c_reads(IIC_GPIO pin, uint8_t DeviceType , uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t num_to_read);

#endif
