#include "exti.h"
#include "delay.h"
#include "motor_motion.h"

#include <drivers/pin.h>

#define HOME_RTT_DEBUG
#ifdef 	HOME_RTT_DEBUG
#define HOME_TRACE         rt_kprintf
#else
#define HOME_TRACE(...)    
#endif

uint8_t org[3]={3,4,5};

void org1_hdr_callback(void *args);
void org2_hdr_callback(void *args);
void org3_hdr_callback(void *args);

void EXTI_MotorORG1_DeInit(void)
{
	rt_pin_irq_enable(org[0],PIN_IRQ_DISABLE);
}
void EXTI_MotorORG23_DeInit(void)
{
	rt_pin_irq_enable(org[1],PIN_IRQ_DISABLE);
	rt_pin_irq_enable(org[2],PIN_IRQ_DISABLE);
}
void EXTI_MotorORG1_Init(void)
{	
	if(act_ORG==LOW)  	rt_pin_attach_irq(org[0], PIN_IRQ_MODE_FALLING,org1_hdr_callback,(void*)"callback args");
	else 	
	{
			rt_pin_mode(org[0], PIN_MODE_INPUT_PULLDOWN); 						
			rt_pin_attach_irq(org[0], PIN_IRQ_MODE_RISING,org1_hdr_callback,(void*)"callback args");
	}
  rt_pin_irq_enable(org[0], PIN_IRQ_ENABLE);  
}

void EXTI_MotorORG2_Init(void)
{
	if(act_ORG==LOW)  	rt_pin_attach_irq(org[1], PIN_IRQ_MODE_FALLING,org2_hdr_callback,(void*)"callback args");
	else 	
	{
			rt_pin_mode(org[1], PIN_MODE_INPUT_PULLDOWN); 						
			rt_pin_attach_irq(org[1], PIN_IRQ_MODE_RISING,org2_hdr_callback,(void*)"callback args");
	}
  rt_pin_irq_enable(org[0], PIN_IRQ_ENABLE);  
}

void EXTI_MotorORG3_Init(void)
{
	if(act_ORG==LOW)  	rt_pin_attach_irq(org[2], PIN_IRQ_MODE_FALLING,org3_hdr_callback,(void*)"callback args");
	else 	
	{
			rt_pin_mode(org[2], PIN_MODE_INPUT_PULLDOWN); 						
			rt_pin_attach_irq(org[2], PIN_IRQ_MODE_RISING,org3_hdr_callback,(void*)"callback args");
	}
  rt_pin_irq_enable(org[2], PIN_IRQ_ENABLE);  
}

void MotorORG_TI_Enable(u8 Motor)
{
	if(Motor==1)	EXTI_MotorORG1_Init();
	if(Motor==2)	EXTI_MotorORG2_Init();
	if(Motor==3)	EXTI_MotorORG3_Init();
}
void MotorORG_TI_Disable(u8 Motor)
{
	if(Motor==1)	EXTI_MotorORG1_DeInit();
	if(Motor==2)	EXTI_MotorORG23_DeInit();
	if(Motor==3)	EXTI_MotorORG23_DeInit();
}

void org1_hdr_callback(void *args)
{
	if(HOME_temp.HomeNeed[0]==TRUE)
	{
		if(HOME_temp.HomeDir[0]==DIR_R)
		{
			if((Read429Short(IDX_VACTUAL|(0<<5)))>0);
			else if((Read429Short(IDX_VACTUAL|(0<<5)))<0)
			{
				SetOrg(0);	
			}
		}
		else if(HOME_temp.HomeDir[0]==DIR_L)
		{
			if((Read429Short(IDX_VACTUAL|(0<<5)))<0);
			else if((Read429Short(IDX_VACTUAL|(0<<5)))>0)
			{
				SetOrg(0);	
			}
		}
	}
}

void org2_hdr_callback(void *args)
{
	if(HOME_temp.HomeNeed[1]==TRUE)
	{
		if(HOME_temp.HomeDir[1]==DIR_R)
		{
			if((Read429Short(IDX_VACTUAL|(1<<5)))>0);
			else if((Read429Short(IDX_VACTUAL|(1<<5)))<0)
			{
				SetOrg(1);	
			}
		}
		else if(HOME_temp.HomeDir[1]==DIR_L)
		{
			if((Read429Short(IDX_VACTUAL|(1<<5)))<0);
			else if((Read429Short(IDX_VACTUAL|(1<<5)))>0)
			{
				SetOrg(1);	
			}
		}
	}
}

void org3_hdr_callback(void *args)
{
	if(HOME_temp.HomeNeed[2]==TRUE)
	{
		if(HOME_temp.HomeDir[2]==DIR_R)
		{
			if((Read429Short(IDX_VACTUAL|(2<<5)))>0);
			else if((Read429Short(IDX_VACTUAL|(2<<5)))<0)
			{
				SetOrg(2);	
			}
		}
		else 
		{
			if((Read429Short(IDX_VACTUAL|(2<<5)))<0);
			else if((Read429Short(IDX_VACTUAL|(2<<5)))>0)
			{
				SetOrg(2);	
			}
		}
	}	
}

 
