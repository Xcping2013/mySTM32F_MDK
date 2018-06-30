#include "74HC595.h"
#include "delay.h"
 
hc595_DrvTypeDef   hc595 = 
{
	{0xff,0},
	&PB13,			//CLK
	&PB15,			//DATA
	&PA4,				//SCP
  hc595_init,
  hc595_write,
	hc595_updata,
};

void hc595_init(void)
{
	pinMode(hc595.dat_pin,GPIO_Mode_Out_PP);
	pinMode(hc595.clk_pin,GPIO_Mode_Out_PP);
	pinMode(hc595.lck_pin,GPIO_Mode_Out_PP);
	
	pinSet(hc595.dat_pin);
	pinSet(hc595.clk_pin);
	pinSet(hc595.lck_pin);
}

//Updates the output register
void hc595_write(uint8_t *data, uint8_t data_length, uint8_t bit_oder)
{
	int i;
	pinReset(hc595.lck_pin);
	for(i = 0; i < data_length; i++)
	{
		shift_out(hc595.dat_pin, hc595.clk_pin, bit_oder, data[i]);
	}
}
void hc595_updata(void)
{
	pinReset(hc595.lck_pin);
	delay_us(2);
	pinSet(hc595.lck_pin);
}

