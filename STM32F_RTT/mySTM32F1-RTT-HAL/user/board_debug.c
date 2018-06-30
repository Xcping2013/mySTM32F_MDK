#include "board_debug.h"	
#include "delay.h"
#include "gpio.h"
#include "at24cxx.h"
#include "motor_motion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys.h"

#include <rtthread.h>

#define INPUT1	PBin(5)
#define INPUT2	PBin(6)
#define INPUT3	PBin(7)
#define INPUT4	PBin(8)
#define INPUT5	PBin(9)

#define INPUT6	PEin(0)
#define INPUT7	PEin(1)
#define INPUT8	PEin(2)
#define INPUT9	PEin(3)
#define INPUT10	PEin(4)
#define INPUT11	PEin(5)
#define INPUT12	PEin(6)

#define OUTPUT1		PAout(15)
#define OUTPUT2		PCout(10)
#define OUTPUT3		PCout(11)
#define OUTPUT4		PCout(12)
#define OUTPUT5		PDout(0)
#define OUTPUT6		PDout(1)
#define OUTPUT7		PDout(2)
#define OUTPUT8		PDout(3)

#define RGB_R PDout(6)
#define RGB_G PDout(5)
#define RGB_B PDout(4)

static void hw_in_out_debug(void)
{
	u8 ret_ok=1;
													if(INPUT1==1) 	{rt_kprintf("input1 or rgb_5v ............err\r\n");ret_ok=0;}
	RGB_R=1;RGB_G=0;RGB_B=0;delay_ms(10);
													if(INPUT2==1) 	{rt_kprintf("input2 or rgb_r .............err\r\n"); ret_ok=0;}
													if(INPUT3==0) 	{rt_kprintf("input3 or rgb_g .............err\r\n"); ret_ok=0;}
													if(INPUT4==0) 	{rt_kprintf("input4 or rgb_b .............err\r\n"); ret_ok=0;}
	RGB_R=0;RGB_G=1;RGB_B=0;delay_ms(10);										        
													if(INPUT2==0) 	{rt_kprintf("input2 or rgb_r .............err\r\n"); ret_ok=0;}
													if(INPUT3==1) 	{rt_kprintf("input3 or rgb_g .............err\r\n"); ret_ok=0;}
													if(INPUT4==0) 	{rt_kprintf("input4 or rgb_b .............err\r\n"); ret_ok=0;}
	RGB_R=0;RGB_G=0;RGB_B=1;delay_ms(10);												     
													if(INPUT2==0) 	{rt_kprintf("input2 or rgb_r .............err\r\n"); ret_ok=0;}
													if(INPUT3==0) 	{rt_kprintf("input3 or rgb_g .............err\r\n"); ret_ok=0;}
													if(INPUT4==1) 	{rt_kprintf("input4 or rgb_b .............err\r\n"); ret_ok=0;}	
														
	OUTPUT1=1;delay_ms(10);	if(INPUT5==1) 	{rt_kprintf("input5 or output1 ...........err\r\n");		ret_ok=0;}
	OUTPUT1=0;delay_ms(10);	if(INPUT5==0) 	{rt_kprintf("input5 or output1 ...........err\r\n");		ret_ok=0;}
	                                                                  
	OUTPUT2=1;delay_ms(10);	if(INPUT6==1) 	{rt_kprintf("input6 or output2 ...........err\r\n");   ret_ok=0;}
	OUTPUT2=0;delay_ms(10);	if(INPUT6==0) 	{rt_kprintf("input6 or output2 ...........err\r\n");   ret_ok=0;}
	                                                                   
	OUTPUT3=1;delay_ms(10);	if(INPUT7==1) 	{rt_kprintf("input7 or output3 ...........err\r\n");   ret_ok=0;}
	OUTPUT3=0;delay_ms(10);	if(INPUT7==0) 	{rt_kprintf("input7 or output3 ...........err\r\n");   ret_ok=0;}
	                                                                  
	OUTPUT4=1;delay_ms(10);	if(INPUT8==1) 	{rt_kprintf("input8 or output4 ...........err\r\n");   ret_ok=0;}
	OUTPUT4=0;delay_ms(10);	if(INPUT8==0) 	{rt_kprintf("input8 or output4 ...........err\r\n");   ret_ok=0;}
	          
	OUTPUT5=1;delay_ms(10);	if(INPUT9==1) 	{rt_kprintf("input9 or output5 ...........err\r\n");   ret_ok=0;}
	OUTPUT5=0;delay_ms(10);	if(INPUT9==0) 	{rt_kprintf("input9 or output5 ...........err\r\n");   ret_ok=0;}
            
	OUTPUT6=1;delay_ms(10);	if(INPUT10==1) 	{rt_kprintf("input10 or output6 ..........err\r\n");   ret_ok=0;}
	OUTPUT6=0;delay_ms(10);	if(INPUT10==0) 	{rt_kprintf("input10 or output6 ..........err\r\n");   ret_ok=0;}
	          
	OUTPUT7=1;delay_ms(10);	if(INPUT11==1) 	{rt_kprintf("input11 or output7 ..........err\r\n");   ret_ok=0;}
	OUTPUT7=0;delay_ms(10);	if(INPUT11==0) 	{rt_kprintf("input11 or output7 ..........err\r\n");   ret_ok=0;}
																									 
	OUTPUT8=1;delay_ms(10);	if(INPUT12==1) 	{rt_kprintf("input12 or output8 ..........err\r\n");   ret_ok=0;}   
	OUTPUT8=0;delay_ms(10);	if(INPUT12==0) 	{rt_kprintf("input12 or output8 ..........err\r\n");   ret_ok=0;}   
	if(ret_ok)	rt_kprintf("inputs and outputs test......ok\r\n");	
	
}

static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
	while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
static void hw_eeprom_debug(void)
{
	u8 *save_info="this page data save for eeprom test";
	u8 read_info[64];
	u8 len=strlen(save_info);
					
	rt_kprintf("eeprom test..................");
	at24cxx.write(at24c256,100,save_info,len);
	delay_ms(10);
	at24cxx.read(at24c256 , 100, read_info, len);
	
  if(Buffercmp((uint8_t*)save_info,(uint8_t*)read_info,len))
  {
    rt_kprintf("err\r\n");
  }	
	else rt_kprintf("ok\r\n");
	
}

static void hw_motor_debug(void)
{
	rt_kprintf("tmc429 test..................");
	if(Read429SingleByte(IDX_REF_SWITCHES, 3)==0) 
		rt_kprintf("err");

	else 
	{		

		rt_kprintf("ok\r\n\r\n最后:检测限位采集电路");
		rt_kprintf("\r\n先发送motor_get(1,\"L\")采集数据再把L1接GND后发送motor_get(1,\"L\")返回值有变化才ok\r\n");	

	}
}

void board_debug(void)
{
	rt_kprintf("\r\n");
	hw_in_out_debug();
	hw_eeprom_debug();
	hw_motor_debug();
	
}
