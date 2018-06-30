
/* Includes ------------------------------------------------------------------*/
#include "mcu_exit.h"

#define USING_RTT 0
#if USING_RTT
	#include <rtthread.h>
#else 
	#define rt_interrupt_enter()
	#define rt_interrupt_leave()
#endif

static uint8_t    port_source;
static uint8_t    pin_source;
static uint32_t   exti_line;
static uint8_t    irq;
	
exit_t *dev;

void (*exit0_handler)(void);
void (*exit1_handler)(void);
void (*exit2_handler)(void);
void (*exit3_handler)(void);
void (*exit4_handler)(void);
void (*exit5_handler)(void);
void (*exit6_handler)(void);
void (*exit7_handler)(void);
void (*exit8_handler)(void);
void (*exit9_handler)(void);
void (*exit10_handler)(void);
void (*exit11_handler)(void);
void (*exit12_handler)(void);
void (*exit13_handler)(void);
void (*exit14_handler)(void);
void (*exit15_handler)(void);

void exit_init(exit_t *dev)
{	
	EXTI_InitTypeDef EXTI_InitStructure;
	pinMode(dev->pin,GPIO_Mode_IPU);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	switch(dev->pin->id>>4)
  {
    case 0:
        port_source = GPIO_PortSourceGPIOA;
        break;
    case 1:
        port_source = GPIO_PortSourceGPIOB;
        break;
    case 2:
        port_source = GPIO_PortSourceGPIOC;
        break;
    case 3:
        port_source = GPIO_PortSourceGPIOD;
        break;
    case 4:
        port_source = GPIO_PortSourceGPIOE;
        break;
    case 5:
        port_source = GPIO_PortSourceGPIOF;
        break;
  }
	switch(dev->pin->id&0x0f)
	{
    case 0:
        pin_source = GPIO_PinSource0;
        exti_line = EXTI_Line0;
        irq = EXTI0_IRQn;
				exit0_handler=dev->handler;
        break;

    case 1:
        pin_source = GPIO_PinSource1;
        exti_line = EXTI_Line1;
        irq = EXTI1_IRQn;
				exit1_handler=dev->handler;
        break;

    case 2:
        pin_source = GPIO_PinSource2;
        exti_line = EXTI_Line2;
        irq = EXTI2_IRQn;
				exit2_handler=dev->handler;
        break;

    case 3:
        pin_source = GPIO_PinSource3;
        exti_line = EXTI_Line3;
        irq = EXTI3_IRQn;
				exit3_handler=dev->handler;
        break;

    case 4:
        pin_source = GPIO_PinSource4;
        exti_line = EXTI_Line4;
        irq = EXTI4_IRQn;
				exit4_handler=dev->handler;
        break;

    case 5:
        pin_source = GPIO_PinSource5;
        exti_line = EXTI_Line5;
        irq = EXTI9_5_IRQn;
				exit5_handler=dev->handler;
        break;

    case 6:
        pin_source = GPIO_PinSource6;
        exti_line = EXTI_Line6;
        irq = EXTI9_5_IRQn;
				exit6_handler=dev->handler;
        break;

    case 7:
        pin_source = GPIO_PinSource7;
        exti_line = EXTI_Line7;
        irq = EXTI9_5_IRQn;
				exit7_handler=dev->handler;
        break;

    case 8:
        pin_source = GPIO_PinSource8;
        exti_line = EXTI_Line8;
        irq = EXTI9_5_IRQn;
				exit8_handler=dev->handler;
        break;

    case 9:
        pin_source = GPIO_PinSource9;
        exti_line = EXTI_Line9;
        irq = EXTI9_5_IRQn;
				exit9_handler=dev->handler;
        break;

    case 10:
        pin_source = GPIO_PinSource10;
        exti_line = EXTI_Line10;
        irq = EXTI15_10_IRQn;
				exit10_handler=dev->handler;
        break;

    case 11:
        pin_source = GPIO_PinSource11;
        exti_line = EXTI_Line11;
        irq = EXTI15_10_IRQn;
				exit11_handler=dev->handler;
        break;

    case 12:
        pin_source = GPIO_PinSource12;
        exti_line = EXTI_Line12;
        irq = EXTI15_10_IRQn;
				exit12_handler=dev->handler;
        break;

    case 13:
        pin_source = GPIO_PinSource13;
        exti_line = EXTI_Line13;
        irq = EXTI15_10_IRQn;
				exit13_handler=dev->handler;
        break;

    case 14:
        pin_source = GPIO_PinSource14;
        exti_line = EXTI_Line14;
        irq = EXTI15_10_IRQn;
				exit14_handler=dev->handler;
        break;

    case 15:
        pin_source = GPIO_PinSource15;
        exti_line = EXTI_Line15;
        irq = EXTI15_10_IRQn;
				exit15_handler=dev->handler;
        break;
	}

	GPIO_EXTILineConfig(port_source, pin_source);
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = exti_line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = dev->trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	exit_nvic_init(dev);
	//rt_kprintf("port_source=%d port_source=%d exti_line=%d irq=%d\r\n",port_source,pin_source,exti_line,irq);	
}
//
void exit_nvic_init(exit_t *dev)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = dev->sub_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = dev->en;
    NVIC_Init(&NVIC_InitStructure);
}
//
void EXTI0_IRQHandler(void)
{
	rt_interrupt_enter();
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
			exit0_handler();
			EXTI_ClearITPendingBit(EXTI_Line0);
	}
	rt_interrupt_leave();
}
void EXTI1_IRQHandler(void)
{
	rt_interrupt_enter();
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			exit1_handler();
			EXTI_ClearITPendingBit(EXTI_Line1);
	}
	rt_interrupt_leave();
}
void EXTI2_IRQHandler(void)
{
	rt_interrupt_enter();
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
			exit2_handler();
			EXTI_ClearITPendingBit(EXTI_Line2);
	}
	rt_interrupt_leave();
}
void EXTI3_IRQHandler(void)
{
	rt_interrupt_enter();
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
			exit3_handler();
			EXTI_ClearITPendingBit(EXTI_Line3);
	}
	rt_interrupt_leave();
}
void EXTI4_IRQHandler(void)
{
	rt_interrupt_enter();
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
			exit4_handler();
			EXTI_ClearITPendingBit(EXTI_Line4);
	}
	rt_interrupt_leave();
}

void EXTI9_5_IRQHandler(void)
{
	rt_interrupt_enter();
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
			exit5_handler();
			EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
			exit6_handler();
			EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
			exit7_handler();
			EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
			exit8_handler();
			pinToggle(&PE6);
			EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
			exit9_handler();
			EXTI_ClearITPendingBit(EXTI_Line9);
	}
	rt_interrupt_leave();

}

void EXTI15_10_IRQHandler(void)
{
	rt_interrupt_enter();
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
			exit10_handler();
			EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
			exit11_handler();
			EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
			exit12_handler();
			EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
			exit13_handler();
			EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
			exit14_handler();
			EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
			exit15_handler();
			EXTI_ClearITPendingBit(EXTI_Line15);
	}
	rt_interrupt_leave();

}
//



