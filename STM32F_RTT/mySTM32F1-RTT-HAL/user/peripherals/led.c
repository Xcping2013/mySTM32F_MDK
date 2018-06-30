#include <rtthread.h>
#include <led.h>
 
#define LED_B	PD_4
#define LED_G	PD_5
#define LED_R	PD_6

void led_thread_entry(void* parameter)
{
    rt_pin_mode(LED_RUN,PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED_RUN,PIN_HIGH);
        rt_thread_delay( RT_TICK_PER_SECOND/2 ); /* sleep 0.5 second and switch to other thread */

        rt_pin_write(LED_RUN,PIN_LOW);
        rt_thread_delay( RT_TICK_PER_SECOND/2 );
    }
}

void rgb_ledInit(void)
{
	rt_pin_mode(LED_R,PIN_MODE_OUTPUT);
	rt_pin_mode(LED_G,PIN_MODE_OUTPUT);
	rt_pin_mode(LED_B,PIN_MODE_OUTPUT);
}
void rgb_set(char rgb)
{
	if(rgb=='r') 
	{
		rt_pin_write(LED_R,PIN_HIGH);rt_pin_write(LED_G,PIN_LOW);rt_pin_write(LED_B,PIN_LOW);
	}
	else if(rgb=='g') 
	{
		rt_pin_write(LED_G,PIN_HIGH);rt_pin_write(LED_R,PIN_LOW);rt_pin_write(LED_B,PIN_LOW);
	}	
	else if(rgb=='b') 
	{
		rt_pin_write(LED_B,PIN_HIGH);rt_pin_write(LED_R,PIN_LOW);rt_pin_write(LED_G,PIN_LOW);
	}	
	
}

