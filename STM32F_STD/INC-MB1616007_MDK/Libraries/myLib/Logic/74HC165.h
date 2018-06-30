#ifndef __74hc165_H
#define __74hc165_H

#include "sys.h"

typedef struct
{
	u8 			cnt;
	Gpio*		clk_pin;
	Gpio*		dat_pin;
	Gpio*		clk_en_pin;
	Gpio*		load_pin;
  void    (*init)(void);
	void		(*read)(uint8_t *, uint8_t , uint8_t );
	
}hc165_DrvTypeDef;

extern hc165_DrvTypeDef   hc165;

void   hc165_init(void);
void 	 hc165_read(uint8_t *data, uint8_t data_length, uint8_t bit_oder);


#endif
















