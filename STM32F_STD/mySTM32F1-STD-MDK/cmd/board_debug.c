#include "board_debug.h"	

#include "delay.h"
#include "led.h"
#include "input.h"
#include "output.h"
#include "at24cxx.h"
#include "motor_motion.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//out1~4  to input1~4	
//out5~8  to input5~8
//RGB     to input9~12
static void hw_in_out_debug(void)
{
	u8 ret_ok=1;
													if(INPUT1==1) 	{printf("input1 or rgb_5v ............err\r\n");ret_ok=0;}
	RGB_R=1;RGB_G=0;RGB_B=0;delay_ms(10);
													if(INPUT2==1) 	{printf("input2 or rgb_r .............err\r\n"); ret_ok=0;}
													if(INPUT3==0) 	{printf("input3 or rgb_g .............err\r\n"); ret_ok=0;}
													if(INPUT4==0) 	{printf("input4 or rgb_b .............err\r\n"); ret_ok=0;}
	RGB_R=0;RGB_G=1;RGB_B=0;delay_ms(10);										        
													if(INPUT2==0) 	{printf("input2 or rgb_r .............err\r\n"); ret_ok=0;}
													if(INPUT3==1) 	{printf("input3 or rgb_g .............err\r\n"); ret_ok=0;}
													if(INPUT4==0) 	{printf("input4 or rgb_b .............err\r\n"); ret_ok=0;}
	RGB_R=0;RGB_G=0;RGB_B=1;delay_ms(10);												     
													if(INPUT2==0) 	{printf("input2 or rgb_r .............err\r\n"); ret_ok=0;}
													if(INPUT3==0) 	{printf("input3 or rgb_g .............err\r\n"); ret_ok=0;}
													if(INPUT4==1) 	{printf("input4 or rgb_b .............err\r\n"); ret_ok=0;}	
														
	OUTPUT1=1;delay_ms(10);	if(INPUT5==1) 	{printf("input5 or output1 ...........err\r\n");		ret_ok=0;}
	OUTPUT1=0;delay_ms(10);	if(INPUT5==0) 	{printf("input5 or output1 ...........err\r\n");		ret_ok=0;}
	                                                                  
	OUTPUT2=1;delay_ms(10);	if(INPUT6==1) 	{printf("input6 or output2 ...........err\r\n");   ret_ok=0;}
	OUTPUT2=0;delay_ms(10);	if(INPUT6==0) 	{printf("input6 or output2 ...........err\r\n");   ret_ok=0;}
	                                                                   
	OUTPUT3=1;delay_ms(10);	if(INPUT7==1) 	{printf("input7 or output3 ...........err\r\n");   ret_ok=0;}
	OUTPUT3=0;delay_ms(10);	if(INPUT7==0) 	{printf("input7 or output3 ...........err\r\n");   ret_ok=0;}
	                                                                  
	OUTPUT4=1;delay_ms(10);	if(INPUT8==1) 	{printf("input8 or output4 ...........err\r\n");   ret_ok=0;}
	OUTPUT4=0;delay_ms(10);	if(INPUT8==0) 	{printf("input8 or output4 ...........err\r\n");   ret_ok=0;}
	          
	OUTPUT5=1;delay_ms(10);	if(INPUT9==1) 	{printf("input9 or output5 ...........err\r\n");   ret_ok=0;}
	OUTPUT5=0;delay_ms(10);	if(INPUT9==0) 	{printf("input9 or output5 ...........err\r\n");   ret_ok=0;}
            
	OUTPUT6=1;delay_ms(10);	if(INPUT10==1) 	{printf("input10 or output6 ..........err\r\n");   ret_ok=0;}
	OUTPUT6=0;delay_ms(10);	if(INPUT10==0) 	{printf("input10 or output6 ..........err\r\n");   ret_ok=0;}
	          
	OUTPUT7=1;delay_ms(10);	if(INPUT11==1) 	{printf("input11 or output7 ..........err\r\n");   ret_ok=0;}
	OUTPUT7=0;delay_ms(10);	if(INPUT11==0) 	{printf("input11 or output7 ..........err\r\n");   ret_ok=0;}
																									 
	OUTPUT8=1;delay_ms(10);	if(INPUT12==1) 	{printf("input12 or output8 ..........err\r\n");   ret_ok=0;}   
	OUTPUT8=0;delay_ms(10);	if(INPUT12==0) 	{printf("input12 or output8 ..........err\r\n");   ret_ok=0;}   
	if(ret_ok)	printf("inputs and outputs test......ok\r\n");	
	
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
					
	printf("eeprom test..................");
	at24cxx.write(at24c256,100*64,save_info,len);
	delay_ms(10);
	at24cxx.read(at24c256 , 100*64, read_info, len);
	
  if(Buffercmp((uint8_t*)save_info,(uint8_t*)read_info,len))
  {
    printf("err\r\n");
  }	
	else printf("ok\r\n");
	
}

static void hw_motor_debug(void)
{
	printf("tmc429 test..................");
	if(Read429SingleByte(IDX_REF_SWITCHES, 3)==0) 
		printf("err");

	else 
	{
		printf("ok\r\n\r\n最后:检测限位采集电路");
		printf("\r\n先发送motor_get(1,\"L\")采集数据再把L1接GND后发送motor_get(1,\"L\")返回值有变化才ok");

	}
}

void board_debug(void)
{
	printf("\r\n");
	hw_in_out_debug();
	hw_eeprom_debug();
	hw_motor_debug();
	
}
