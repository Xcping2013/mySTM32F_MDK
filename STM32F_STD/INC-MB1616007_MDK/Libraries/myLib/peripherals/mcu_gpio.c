#include "mcu_gpio.h"

Gpio PA0 = 	{GPIOA, 0x0001,	0x00};
Gpio PA1 = 	{GPIOA, 0x0002,	0x01};
Gpio PA2 = 	{GPIOA, 0x0004,	0x02};
Gpio PA3 = 	{GPIOA, 0x0008,	0x03};
Gpio PA4 = 	{GPIOA, 0x0010,	0x04};
Gpio PA5 = 	{GPIOA, 0x0020,	0x05};
Gpio PA6 = 	{GPIOA, 0x0040,	0x06};
Gpio PA7 = 	{GPIOA, 0x0080,	0x07};
Gpio PA8 = 	{GPIOA, 0x0100,	0x08};
Gpio PA9 = 	{GPIOA, 0x0200,	0x09};
Gpio PA10 = {GPIOA,	0x0400, 0x0A};
Gpio PA11 = {GPIOA,	0x0800, 0x0B};
Gpio PA12 = {GPIOA,	0x1000, 0x0C};
Gpio PA13 = {GPIOA,	0x2000, 0x0D};
Gpio PA14 = {GPIOA,	0x4000, 0x0E};
Gpio PA15 = {GPIOA,	0x8000, 0x0F};

Gpio PB0 = 	{GPIOB, 0x0001,	0x10};
Gpio PB1 = 	{GPIOB, 0x0002,	0x11};
Gpio PB2 = 	{GPIOB, 0x0004,	0x12};
Gpio PB3 = 	{GPIOB, 0x0008,	0x13};
Gpio PB4 = 	{GPIOB, 0x0010,	0x14};
Gpio PB5 = 	{GPIOB, 0x0020,	0x15};
Gpio PB6 = 	{GPIOB, 0x0040,	0x16};
Gpio PB7 = 	{GPIOB, 0x0080,	0x17};
Gpio PB8 = 	{GPIOB, 0x0100,	0x18};
Gpio PB9 = 	{GPIOB, 0x0200,	0x19};
Gpio PB10 = {GPIOB, 0x0400, 0x1A};
Gpio PB11 = {GPIOB, 0x0800, 0x1b};
Gpio PB12 = {GPIOB, 0x1000, 0x1c};
Gpio PB13 = {GPIOB, 0x2000, 0x1d};
Gpio PB14 = {GPIOB, 0x4000, 0x1e};
Gpio PB15 = {GPIOB, 0x8000, 0x1f};


Gpio PC0 = 	{GPIOC, 0x0001,	0x20};
Gpio PC1 = 	{GPIOC, 0x0002,	0x21};
Gpio PC2 = 	{GPIOC, 0x0004,	0x22};
Gpio PC3 = 	{GPIOC, 0x0008,	0x23};
Gpio PC4 = 	{GPIOC, 0x0010,	0x24};
Gpio PC5 = 	{GPIOC, 0x0020,	0x25};
Gpio PC6 =	{GPIOC, 0x0040,	0x26};
Gpio PC7 = 	{GPIOC, 0x0080,	0x27};
Gpio PC8 = 	{GPIOC, 0x0100,	0x28};
Gpio PC9 = 	{GPIOC, 0x0200,	0x29};
Gpio PC10 = {GPIOC, 0x0400, 0x2a};
Gpio PC11 = {GPIOC, 0x0800, 0x2b};
Gpio PC12 = {GPIOC, 0x1000, 0x2c};
Gpio PC13 = {GPIOC, 0x2000, 0x2d};
Gpio PC14 = {GPIOC, 0x4000, 0x2e};
Gpio PC15 = {GPIOC, 0x8000, 0x2f};

Gpio PD0 = 	{GPIOD, 0x0001,	0x30};
Gpio PD1 = 	{GPIOD, 0x0002,	0x31};
Gpio PD2 = 	{GPIOD, 0x0004,	0x32};
Gpio PD3 = 	{GPIOD, 0x0008,	0x33};
Gpio PD4 = 	{GPIOD, 0x0010,	0x34};
Gpio PD5 = 	{GPIOD, 0x0020,	0x35};
Gpio PD6 = 	{GPIOD, 0x0040,	0x36};
Gpio PD7 = 	{GPIOD, 0x0080,	0x37};
Gpio PD8 = 	{GPIOD, 0x0100,	0x38};
Gpio PD9 = 	{GPIOD, 0x0200,	0x39};
Gpio PD10 = {GPIOD, 0x0400, 0x3a};
Gpio PD11 = {GPIOD, 0x0800, 0x3b};
Gpio PD12 = {GPIOD, 0x1000, 0x3c};
Gpio PD13 = {GPIOD, 0x2000, 0x3d};
Gpio PD14 = {GPIOD, 0x4000, 0x3e};
Gpio PD15 = {GPIOD, 0x8000, 0x3f};

Gpio PE0 = 	{GPIOE, 0x0001,	0x40};
Gpio PE1 = 	{GPIOE, 0x0002,	0x41};
Gpio PE2 = 	{GPIOE, 0x0004,	0x42};
Gpio PE3 = 	{GPIOE, 0x0008,	0x43};
Gpio PE4 = 	{GPIOE, 0x0010,	0x44};
Gpio PE5 = 	{GPIOE, 0x0020,	0x45};
Gpio PE6 = 	{GPIOE, 0x0040,	0x46};
Gpio PE7 = 	{GPIOE, 0x0080,	0x47};
Gpio PE8 = 	{GPIOE, 0x0100,	0x48};
Gpio PE9 = 	{GPIOE, 0x0200,	0x49};
Gpio PE10 = {GPIOE, 0x0400, 0x4a};
Gpio PE11 = {GPIOE, 0x0800, 0x4b};
Gpio PE12 = {GPIOE, 0x1000, 0x4c};
Gpio PE13 = {GPIOE, 0x2000, 0x4d};
Gpio PE14 = {GPIOE, 0x4000, 0x4e};
Gpio PE15 = {GPIOE, 0x8000, 0x4f};

Gpio PF0 = 	{GPIOF, 0x0001,	0x50};
Gpio PF1 = 	{GPIOF, 0x0002,	0x51};
Gpio PF2 = 	{GPIOF, 0x0004,	0x52};
Gpio PF3 = 	{GPIOF, 0x0008,	0x53};
Gpio PF4 = 	{GPIOF, 0x0010,	0x54};
Gpio PF5 = 	{GPIOF, 0x0020,	0x55};
Gpio PF6 = 	{GPIOF, 0x0040,	0x56};
Gpio PF7 = 	{GPIOF, 0x0080,	0x57};
Gpio PF8 = 	{GPIOF, 0x0100,	0x58};
Gpio PF9 = 	{GPIOF, 0x0200,	0x59};
Gpio PF10 = {GPIOF, 0x0400, 0x5a};
Gpio PF11 = {GPIOF, 0x0800, 0x5b};
Gpio PF12 = {GPIOF, 0x1000, 0x5c};
Gpio PF13 = {GPIOF, 0x2000, 0x5d};
Gpio PF14 = {GPIOF, 0x4000, 0x5e};
Gpio PF15 = {GPIOF, 0x8000, 0x5f};

Gpio PG0 = 	{GPIOG, 0x0001,	0x60};
Gpio PG1 = 	{GPIOG, 0x0002,	0x61};
Gpio PG2 = 	{GPIOG, 0x0004,	0x62};
Gpio PG3 = 	{GPIOG, 0x0008,	0x63};
Gpio PG4 = 	{GPIOG, 0x0010,	0x64};
Gpio PG5 = 	{GPIOG, 0x0020,	0x65};
Gpio PG6 = 	{GPIOG, 0x0040,	0x66};
Gpio PG7 = 	{GPIOG, 0x0080,	0x67};
Gpio PG8 = 	{GPIOG, 0x0100,	0x68};
Gpio PG9 = 	{GPIOG, 0x0200,	0x69};
Gpio PG10 = {GPIOG, 0x0400, 0x6a};
Gpio PG11 = {GPIOG, 0x0800, 0x6b};
Gpio PG12 = {GPIOG, 0x1000, 0x6c};
Gpio PG13 = {GPIOG, 0x2000, 0x6d};
Gpio PG14 = {GPIOG, 0x4000, 0x6e};
Gpio PG15 = {GPIOG, 0x8000, 0x6f};

void pinMode(Gpio *pin, GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  switch((uint32_t)pin->port)
  {
		case (uint32_t)GPIOA_BASE:
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
				RCC->APB2ENR|=1<<2;     //使能PORTA时钟
        break;

    case (uint32_t)GPIOB_BASE:
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   
        break;

    case (uint32_t)GPIOC_BASE:
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
				RCC->APB2ENR|=1<<4;     //使能PORTC时钟
        break;

    case (uint32_t)GPIOD_BASE:
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
				RCC->APB2ENR|=1<<5;     //使能PORTD时钟
        break;

    case (uint32_t)GPIOE_BASE:
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
				RCC->APB2ENR|=1<<6;    //使能PORTE时钟	
        break;

    case (uint32_t)GPIOF_BASE:
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
				RCC->APB2ENR|=1<<7;     //使能PORTF时钟
        break;
    case (uint32_t)GPIOG_BASE:
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
				RCC->APB2ENR|=1<<8;     //使能PORTF时钟
        break;
    }
    switch((uint8_t)mode)
    {
			case GPIO_Mode_AF_OD:
			case GPIO_Mode_AF_PP:
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
					break;
       default:
          break;
    }
		GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Pin = pin->pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(pin->port, &GPIO_InitStructure); 
}
//
void pinSet(Gpio *pin)
{
    pin->port->BSRR = pin->pin;
}
void pinReset(Gpio *pin)
{
    pin->port->BRR = pin->pin;
}
void pinWrite(Gpio *pin,uint8_t val)
{
    if(val == 0)
        pin->port->BRR = pin->pin;
    else
        pin->port->BSRR = pin->pin;
}
uint8_t pinRead(Gpio *pin)
{
    if(pin->port->IDR & pin->pin)
        return 1;
    return  0;
}
void pinToggle(Gpio *pin)
{
    (pin->port)->ODR ^= pin->pin;
}

