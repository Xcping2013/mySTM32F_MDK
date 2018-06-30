#ifndef __MCU_GPIO_H
#define __MCU_GPIO_H

#include "sys.h"
#include <drivers/pin.h>

#define GPIO_Mode_Out_PP					PIN_MODE_OUTPUT
#define GPIO_Mode_IN_FLOATING			PIN_MODE_INPUT
#define GPIO_Mode_IPU							PIN_MODE_INPUT_PULLUP
#define GPIO_Mode_IPD							PIN_MODE_INPUT_PULLDOWN
#define GPIO_Mode_Out_OD				  PIN_MODE_OUTPUT_OD


void pinMode(u8 pin, u8 mode);
void pinSet(u8  pin);
void pinReset(u8 pin);
void pinWrite(u8 pin,u8 val);
u8   pinRead(u8 pin);
//void pinToggle(u8 pin);

#endif


