#ifndef __74HC595_H
#define __74HC595_H

#include "sys.h"

typedef struct
{
	u8 			out[2];
	Gpio*		clk_pin;
	Gpio*		dat_pin;
	Gpio*		lck_pin;
  void    (*init)(void);
	void 		(*write)(uint8_t *data, uint8_t data_length, uint8_t bit_oder);
	void		(*updata)(void);
	
}hc595_DrvTypeDef;

extern hc595_DrvTypeDef   hc595;

void hc595_init(void);
void hc595_write(uint8_t *data, uint8_t data_length, uint8_t bit_oder);
void hc595_updata(void);

#endif
















