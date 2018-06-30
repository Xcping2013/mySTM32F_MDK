#ifndef __SYS_H
#define __SYS_H	

#include "stm32f1xx_hal.h"

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  
typedef __I int16_t vsc16; 
typedef __I int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 

typedef unsigned char 	UCHAR;                   //!< 8 bits unsigned
typedef unsigned short  USHORT;  	               //!< 16 bits unsigned
typedef unsigned int 		UINT;                    //!< 32 bits unsigned

/* Float Types                                                               */
typedef float		f32;
typedef double	f64;


#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define   BIT0        0x00000001
#define   BIT1        0x00000002
#define   BIT2        0x00000004
#define   BIT3        0x00000008
#define   BIT4        0x00000010
#define   BIT5        0x00000020
#define   BIT6        0x00000040
#define   BIT7        0x00000080
#define   BIT8        0x00000100
#define   BIT9        0x00000200
#define   BIT10       0x00000400
#define   BIT11       0x00000800
#define   BIT12       0x00001000
#define   BIT13       0x00002000
#define   BIT14       0x00004000
#define   BIT15       0x00008000
#define   BIT16       0x00010000
#define   BIT17       0x00020000
#define   BIT18       0x00040000
#define   BIT19       0x00080000
#define   BIT20       0x00100000
#define   BIT21       0x00200000
#define   BIT22       0x00400000
#define   BIT23       0x00800000
#define   BIT24       0x01000000
#define   BIT25       0x02000000
#define   BIT26       0x04000000
#define   BIT27       0x08000000
#define   BIT28       0x10000000
#define   BIT29       0x20000000
#define   BIT30       0x40000000
#define   BIT31       0x80000000

#ifndef FALSE
#define FALSE	((u8) 0)
#endif

#ifndef TRUE
#define TRUE	((u8) 1)
#endif

/* NULL definition */
#ifndef NULL
#define NULL  0
#endif

#ifndef HIGH
#define HIGH  0x1
#endif

#ifndef LOW
#define LOW  0x0
#endif

//#define EOK         0
//#define EWAIT       1
//#define EBUSY       2
//#define ETIMEOUT    3
//#define EPARA       4
//#define E2LONG      5
//#define EOTHER      128

#define LSB_FIRST 	0
#define MSB_FIRST 	1

#define PI 3.1415926535898
#define NOP 				__NOP

#define interrupts() 			__enable_irq()//允许所有中断
#define no_interrupts() 	__disable_irq()//禁止所有中断

#include <board.h>

#if (STM32F10X_PIN_NUMBERS == 100)

#define		PA_0				23
#define		PA_1				24
#define		PA_2				25
#define		PA_3				26
#define		PA_4				29
#define		PA_5				30
#define		PA_6				31
#define		PA_7				32
#define		PA_8				67
#define		PA_9				68
#define		PA_10				69
#define		PA_11				70
#define		PA_12				71
#define		PA_13				72
#define		PA_14				76
#define		PA_15				77

#define		PB_0				35
#define		PB_1				36
#define		PB_2				37
#define		PB_3				89
#define		PB_4				90
#define		PB_5				91
#define		PB_6				92
#define		PB_7				93
#define		PB_8				95
#define		PB_9				96
#define		PB_10				47
#define		PB_11				48
#define		PB_12				51
#define		PB_13				52
#define		PB_14				53
#define		PB_15				54

#define		PC_0				15
#define		PC_1				16
#define		PC_2				17
#define		PC_3				18
#define		PC_4				33
#define		PC_5				34
#define		PC_6				63
#define		PC_7				64
#define		PC_8				65
#define		PC_9				66
#define		PC_10				78
#define		PC_11				79
#define		PC_12				80
#define		PC_13				7
#define		PC_14				8
#define		PC_15				9

#define		PD_0				81
#define		PD_1				82
#define		PD_2				83
#define		PD_3				84
#define		PD_4				85
#define		PD_5				86
#define		PD_6				87
#define		PD_7				88
#define		PD_8				55
#define		PD_9				56
#define		PD_10				57
#define		PD_11				58
#define		PD_12				59
#define		PD_13				60
#define		PD_14				61
#define		PD_15				62

#define		PE_0				97
#define		PE_1				98
#define		PE_2				1
#define		PE_3				2
#define		PE_4				3
#define		PE_5				4
#define		PE_6				5
#define		PE_7				38
#define		PE_8				39
#define		PE_9				40
#define		PE_10				41
#define		PE_11				42
#define		PE_12				43
#define		PE_13				44
#define		PE_14				45
#define		PE_15				46

#endif

void        shift_out(rt_base_t data_pin, rt_base_t clock_pin, uint8_t bit_order, uint8_t val);
uint8_t     shift_in(rt_base_t data_pin, rt_base_t clock_pin, uint8_t bit_order);

#endif
