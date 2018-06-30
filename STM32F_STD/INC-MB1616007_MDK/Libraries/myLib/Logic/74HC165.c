#include "74HC165.h"
#include "delay.h"
 
hc165_DrvTypeDef   hc165 = 
{
	1,				//关联165个数
	&PA1,			//CLK引脚
	&PC5,			//数据引脚
	&PB0,			//时钟使能引脚
	&PC4,			//并行数据装载控制引脚
  hc165_init,
  hc165_read,
};

void hc165_init(void)
{
	pinMode(hc165.dat_pin,GPIO_Mode_IPU);
	
	pinMode(hc165.clk_pin,GPIO_Mode_Out_PP);
	pinMode(hc165.load_pin,GPIO_Mode_Out_PP);
	pinMode(hc165.clk_en_pin,GPIO_Mode_Out_PP);
	
	pinSet(hc165.clk_en_pin);
	pinSet(hc165.load_pin);
	pinSet(hc165.clk_pin);
	pinReset(hc165.clk_en_pin);
}

void 	hc165_read(uint8_t *data, uint8_t data_length, uint8_t bit_oder)
{
	u8 i;
	for(i=0;i<data_length;i++)
	{
		pinReset(hc165.load_pin);
		delay_us(2);
		pinSet(hc165.load_pin);
		delay_us(2);
		*data++=shift_in(hc165.dat_pin, hc165.clk_pin, bit_oder);
	}
}


