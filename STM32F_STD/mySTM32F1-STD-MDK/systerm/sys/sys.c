#include "sys.h"
#include "delay.h"

void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

//
void ResetCPU(void)
{
	//NVIC_GenerateSystemReset();
	//SCB->AIRCR = SCB_AIRCR_VECTKEY_Msk | (u32)0x04;
	//NVIC_GenerateCoreReset();
	SCB->AIRCR = SCB_AIRCR_VECTKEY_Msk | (u32)0x01;
}

uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
       //pinWrite(	clock_pin,	HIGH	);	delay_us(1);
       if (bit_order == LSB_FIRST)      value |= pinRead(data_pin) << i;
       else									            value |= pinRead(data_pin) << (7 - i);
       pinWrite(	clock_pin,	LOW		);	delay_us(1);
			 pinWrite(	clock_pin,	HIGH	);	delay_us(1);
    }
    return value;
}

void shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val)
{
	int i;
	for (i = 0; i < 8; i++)
	{
			if (bit_order == LSB_FIRST)
					pinWrite(	data_pin,	(!!(val & (1 << i))));
			else
					pinWrite(	data_pin,(!!(val & (1 << (7 - i))))	);

			pinWrite(	clock_pin,	HIGH	);
			pinWrite(	clock_pin,	LOW		);
	}
}
//

