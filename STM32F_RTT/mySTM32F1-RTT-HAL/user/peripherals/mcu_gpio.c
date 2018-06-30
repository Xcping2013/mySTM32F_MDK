#include "mcu_gpio.h"

#include <drivers/pin.h>

void pinMode(u8 pin, u8 mode)
{
	rt_pin_mode(pin,mode);
}
void pinSet(u8 pin)
{
  rt_pin_write(pin, PIN_HIGH);
}
void pinReset(u8 pin)
{
  rt_pin_write(pin, PIN_LOW);
}
void pinWrite(u8 pin,u8 val)
{
   if(val == 0)   pinReset(pin);
   else						pinSet(pin);
}
u8 pinRead(u8 pin)
{
    if(rt_pin_read(pin)) return 1;
    else 								 return  0;
}
