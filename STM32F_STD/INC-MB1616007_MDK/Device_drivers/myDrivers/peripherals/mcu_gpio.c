#include "mcu_gpio.h"

Gpio PA0 = {GPIOA, GPIO_Pin_0, 0x00};
Gpio PA1 = {GPIOA, GPIO_Pin_1, 0x01};
Gpio PA2 = {GPIOA, GPIO_Pin_2, 0x02};
Gpio PA3 = {GPIOA, GPIO_Pin_3, 0x03};
Gpio PA4 = {GPIOA, GPIO_Pin_4, 0x04};
Gpio PA5 = {GPIOA, GPIO_Pin_5, 0x05};
Gpio PA6 = {GPIOA, GPIO_Pin_6, 0x06};
Gpio PA7 = {GPIOA, GPIO_Pin_7, 0x07};
Gpio PA8 = {GPIOA, GPIO_Pin_8, 0x08};
Gpio PA9 = {GPIOA, GPIO_Pin_9, 0x09};
Gpio PA10 = {GPIOA, GPIO_Pin_10, 0x0A};
Gpio PA11 = {GPIOA, GPIO_Pin_11, 0x0B};
Gpio PA12 = {GPIOA, GPIO_Pin_12, 0x0C};
Gpio PA13 = {GPIOA, GPIO_Pin_13, 0x0D};
Gpio PA14 = {GPIOA, GPIO_Pin_14, 0x0E};
Gpio PA15 = {GPIOA, GPIO_Pin_15, 0x0F};

Gpio PB0 = {GPIOB, GPIO_Pin_0, 0x10};
Gpio PB1 = {GPIOB, GPIO_Pin_1, 0x11};
Gpio PB2 = {GPIOB, GPIO_Pin_2, 0x12};
Gpio PB3 = {GPIOB, GPIO_Pin_3, 0x13};
Gpio PB4 = {GPIOB, GPIO_Pin_4, 0x14};
Gpio PB5 = {GPIOB, GPIO_Pin_5, 0x15};
Gpio PB6 = {GPIOB, GPIO_Pin_6, 0x16};
Gpio PB7 = {GPIOB, GPIO_Pin_7, 0x17};
Gpio PB8 = {GPIOB, GPIO_Pin_8, 0x18};
Gpio PB9 = {GPIOB, GPIO_Pin_9, 0x19};
Gpio PB10 = {GPIOB, GPIO_Pin_10, 0x1A};
Gpio PB11 = {GPIOB, GPIO_Pin_11, 0x1b};
Gpio PB12 = {GPIOB, GPIO_Pin_12, 0x1c};
Gpio PB13 = {GPIOB, GPIO_Pin_13, 0x1d};
Gpio PB14 = {GPIOB, GPIO_Pin_14, 0x1e};
Gpio PB15 = {GPIOB, GPIO_Pin_15, 0x1f};


Gpio PC0 = {GPIOC, GPIO_Pin_0, 0x20};
Gpio PC1 = {GPIOC, GPIO_Pin_1, 0x21};
Gpio PC2 = {GPIOC, GPIO_Pin_2, 0x22};
Gpio PC3 = {GPIOC, GPIO_Pin_3, 0x23};
Gpio PC4 = {GPIOC, GPIO_Pin_4, 0x24};
Gpio PC5 = {GPIOC, GPIO_Pin_5, 0x25};
Gpio PC6 = {GPIOC, GPIO_Pin_6, 0x26};
Gpio PC7 = {GPIOC, GPIO_Pin_7, 0x27};
Gpio PC8 = {GPIOC, GPIO_Pin_8, 0x28};
Gpio PC9 = {GPIOC, GPIO_Pin_9, 0x29};
Gpio PC10 = {GPIOC, GPIO_Pin_10, 0x2a};
Gpio PC11 = {GPIOC, GPIO_Pin_11, 0x2b};
Gpio PC12 = {GPIOC, GPIO_Pin_12, 0x2c};
Gpio PC13 = {GPIOC, GPIO_Pin_13, 0x2d};
Gpio PC14 = {GPIOC, GPIO_Pin_14, 0x2e};
Gpio PC15 = {GPIOC, GPIO_Pin_15, 0x2f};

Gpio PD0 = {GPIOD, GPIO_Pin_0, 0x30};
Gpio PD1 = {GPIOD, GPIO_Pin_1, 0x31};
Gpio PD2 = {GPIOD, GPIO_Pin_2, 0x32};
Gpio PD3 = {GPIOD, GPIO_Pin_3, 0x33};
Gpio PD4 = {GPIOD, GPIO_Pin_4, 0x34};
Gpio PD5 = {GPIOD, GPIO_Pin_5, 0x35};
Gpio PD6 = {GPIOD, GPIO_Pin_6, 0x36};
Gpio PD7 = {GPIOD, GPIO_Pin_7, 0x37};
Gpio PD8 = {GPIOD, GPIO_Pin_8, 0x38};
Gpio PD9 = {GPIOD, GPIO_Pin_9, 0x39};
Gpio PD10 = {GPIOD, GPIO_Pin_10, 0x3a};
Gpio PD11 = {GPIOD, GPIO_Pin_11, 0x3b};
Gpio PD12 = {GPIOD, GPIO_Pin_12, 0x3c};
Gpio PD13 = {GPIOD, GPIO_Pin_13, 0x3d};
Gpio PD14 = {GPIOD, GPIO_Pin_14, 0x3e};
Gpio PD15 = {GPIOD, GPIO_Pin_15, 0x3f};

Gpio PE0 = {GPIOE, GPIO_Pin_0, 0x40};
Gpio PE1 = {GPIOE, GPIO_Pin_1, 0x41};
Gpio PE2 = {GPIOE, GPIO_Pin_2, 0x42};
Gpio PE3 = {GPIOE, GPIO_Pin_3, 0x43};
Gpio PE4 = {GPIOE, GPIO_Pin_4, 0x44};
Gpio PE5 = {GPIOE, GPIO_Pin_5, 0x45};
Gpio PE6 = {GPIOE, GPIO_Pin_6, 0x46};
Gpio PE7 = {GPIOE, GPIO_Pin_7, 0x47};
Gpio PE8 = {GPIOE, GPIO_Pin_8, 0x48};
Gpio PE9 = {GPIOE, GPIO_Pin_9, 0x49};
Gpio PE10 = {GPIOE, GPIO_Pin_10, 0x4a};
Gpio PE11 = {GPIOE, GPIO_Pin_11, 0x4b};
Gpio PE12 = {GPIOE, GPIO_Pin_12, 0x4c};
Gpio PE13 = {GPIOE, GPIO_Pin_13, 0x4d};
Gpio PE14 = {GPIOE, GPIO_Pin_14, 0x4e};
Gpio PE15 = {GPIOE, GPIO_Pin_15, 0x4f};

Gpio PF0 = {GPIOF, GPIO_Pin_0, 0x50};
Gpio PF1 = {GPIOF, GPIO_Pin_1, 0x51};
Gpio PF2 = {GPIOF, GPIO_Pin_2, 0x52};
Gpio PF3 = {GPIOF, GPIO_Pin_3, 0x53};
Gpio PF4 = {GPIOF, GPIO_Pin_4, 0x54};
Gpio PF5 = {GPIOF, GPIO_Pin_5, 0x55};
Gpio PF6 = {GPIOF, GPIO_Pin_6, 0x56};
Gpio PF7 = {GPIOF, GPIO_Pin_7, 0x57};
Gpio PF8 = {GPIOF, GPIO_Pin_8, 0x58};
Gpio PF9 = {GPIOF, GPIO_Pin_9, 0x59};
Gpio PF10 = {GPIOF, GPIO_Pin_10, 0x5a};
Gpio PF11 = {GPIOF, GPIO_Pin_11, 0x5b};
Gpio PF12 = {GPIOF, GPIO_Pin_12, 0x5c};
Gpio PF13 = {GPIOF, GPIO_Pin_13, 0x5d};
Gpio PF14 = {GPIOF, GPIO_Pin_14, 0x5e};
Gpio PF15 = {GPIOF, GPIO_Pin_15, 0x5f};

Gpio PG0 = {GPIOG, GPIO_Pin_0, 0x60};
Gpio PG1 = {GPIOG, GPIO_Pin_1, 0x61};
Gpio PG2 = {GPIOG, GPIO_Pin_2, 0x62};
Gpio PG3 = {GPIOG, GPIO_Pin_3, 0x63};
Gpio PG4 = {GPIOG, GPIO_Pin_4, 0x64};
Gpio PG5 = {GPIOG, GPIO_Pin_5, 0x65};
Gpio PG6 = {GPIOG, GPIO_Pin_6, 0x66};
Gpio PG7 = {GPIOG, GPIO_Pin_7, 0x67};
Gpio PG8 = {GPIOG, GPIO_Pin_8, 0x68};
Gpio PG9 = {GPIOG, GPIO_Pin_9, 0x69};
Gpio PG10 = {GPIOG, GPIO_Pin_10, 0x6a};
Gpio PG11 = {GPIOG, GPIO_Pin_11, 0x6b};
Gpio PG12 = {GPIOG, GPIO_Pin_12, 0x6c};
Gpio PG13 = {GPIOG, GPIO_Pin_13, 0x6d};
Gpio PG14 = {GPIOG, GPIO_Pin_14, 0x6e};
Gpio PG15 = {GPIOG, GPIO_Pin_15, 0x6f};

void pinMode(Gpio *pin, GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  switch((uint32_t)pin->port)
  {
		case (uint32_t)GPIOA_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        break;

    case (uint32_t)GPIOB_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        break;

    case (uint32_t)GPIOC_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        break;

    case (uint32_t)GPIOD_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        break;

    case (uint32_t)GPIOE_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
        break;

    case (uint32_t)GPIOF_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
        break;
    case (uint32_t)GPIOG_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
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

