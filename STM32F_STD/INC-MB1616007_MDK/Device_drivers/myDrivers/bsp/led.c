#include "tmc429_stm32f103vet.h"
#include "led.h"

#include <rtthread.h>
#include <stm32f10x.h>

GPIP_DrvTypeDef   DIO_drv = 
{
  LED_Init,
  LED_On,
  LED_Off,
};

static uint8_t Is_LED_Initialized = 0;

void	LED_Init(void)
{
	if(Is_LED_Initialized == 0)
  {
    Is_LED_Initialized = 1;
		
		GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(led_rcc,ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = led_pin;
    GPIO_Init(led_gpio, &GPIO_InitStructure);
		
		RCC_APB2PeriphClockCmd(rgb_r_rcc|rgb_g_rcc|rgb_b_rcc,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = rgb_r_pin|rgb_g_pin|rgb_b_pin;
    GPIO_Init(rgb_gpio, &GPIO_InitStructure);
	}
}

void LED_On(uint8_t ledNum)
{
  switch (ledNum)
  {
    case 0:
        GPIO_ResetBits(led_gpio, led_pin);
        break;
    case 1:
        GPIO_ResetBits(rgb_gpio, rgb_r_pin);
				GPIO_SetBits(rgb_gpio, rgb_g_pin);
				GPIO_SetBits(rgb_gpio, rgb_b_pin);
        break;
    case 2:
        GPIO_SetBits(rgb_gpio, rgb_r_pin);
				GPIO_ResetBits(rgb_gpio, rgb_g_pin);
				GPIO_SetBits(rgb_gpio, rgb_b_pin);
        break;
    case 3:
        GPIO_SetBits(rgb_gpio, rgb_r_pin);
				GPIO_SetBits(rgb_gpio, rgb_g_pin);
				GPIO_ResetBits(rgb_gpio, rgb_b_pin);
        break;
    default:
        break;
  }	
}

void	LED_Off(uint8_t ledNum)
{
  switch (ledNum)
  {
    case 0:
        GPIO_SetBits(led_gpio, led_pin);
        break;
    case 1:
        GPIO_SetBits(rgb_gpio, rgb_r_pin);
        break;
    case 2:
				GPIO_SetBits(rgb_gpio, rgb_g_pin);
        break;
    case 3:
				GPIO_SetBits(rgb_gpio, rgb_b_pin);
        break;
    default:
        break;
  }	
}

#ifdef RT_USING_FINSH
#include <finsh.h>

void led(rt_uint32_t led, rt_uint32_t value)
{
	LED_Init();
	if(0<=led<4)
	{
		if(value)	LED_On(led);
		else			LED_Off(led);	
		rt_kprintf("[OK]@led(%d,%d)\n",led,value);	
	}
}
FINSH_FUNCTION_EXPORT(led, set led[0 - 3] on[1] or off[0].)
#endif