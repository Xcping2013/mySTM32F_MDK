/*	mian.h	---��Ӧ�忨+��Ŀ����
������
	оƬ��������
	�������Ͷ���
��Χ��Դ��
	�������������GPIO����
	����оƬ��ض���
	�������оƬ���š��������ṹ�嶨��
��Ŀ��Դ��
	��Ŀ��ض���
*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
/*************������������		λ#��#��#��#	������������**************************************************************************************/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

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
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  
/*************������������		λ#��#��#��#	������������***************************************************************************************/

/*************������������		���Ͷ���	������������*******************************************************************************************/
typedef unsigned char 			UCHAR;                   //!< 8 bits unsigned
typedef unsigned short 			USHORT;                 //!< 16 bits unsigned
typedef unsigned int 				UINT;                     //!< 32 bits unsigned
/* Signed Types                                                              */
typedef signed char     		S8_T;
typedef short          	 		S16_T;
typedef long           			S32_T;

/* Unsigned Types                                                            */
typedef unsigned char   		U8_T;
typedef unsigned short  		U16_T;
typedef unsigned long   		U32_T;
typedef unsigned long long 	U64_T;

/* Float Types                                                               */
typedef float								F32_T;
typedef double							F64_T;

/* Boolean types declared as U8_T, as enums are generated as 16 bit          */
typedef U8_T								B_T;

/* Definitions for the two members of the Boolean type                       */
#ifndef FALSE
#define FALSE								((B_T) 0)
#endif

#ifndef TRUE
#define TRUE								((B_T) 1)
#endif

#ifndef UNUSED
#define UNUSED	((U8_T) 0xFF)
#endif

/* NULL definition */
#ifndef NULL
#define NULL  0
#endif

/*************������������		���Ͷ���	������������********************************************************************************************/

/***********�������������������������оƬ���������������**************************************************************************************/
#define N_O_MOTORS 				 3                          			//!< number of motors supported by this module
#define MOTOR_NUMBER(a) 	(a)                								//!< extract TMC429 motor number (for future extensions)
#define WHICH_262(a) 			(a)                      				  //!< extract TMC262 motor number (for future extensions)
#define SPI_DEV_TMC429 		 0                      					//!< SPI device number of TMC429
#define SPI_DEV_TMC262  	 1                      					//!< SPI device number of TMC262
#define SPI_DEV_EEPROM  	 2                      					//!< SPI device number of the EEPROM

#define SELECT_TMC429()        GPIOC->BRR=BIT4
#define DESELECT_TMC429()      GPIOC->BSRR=BIT4

#define DISABLE_DRIVERS() 		 GPIOD->BSRR = BIT8  					//!< turn off all motor drivers
#define ENABLE_DRIVERS() 			 GPIOD->BRR = BIT8  					//!< turn on all motor drivers

//! Motor configuration data
typedef struct
{
  UINT VMax;                   //!< maximum positioning velocity
  UINT AMax;                   //!< maximum acceleration
	UINT HomeV;
  UCHAR PulseDiv;              //!< pulse divisor (TMC429)
  UCHAR RampDiv;               //!< ramp divisor (TMC429)
	 
	int XTARGET; 
	int XACTUAL;
	
	UINT VTARGET; 
	UINT VACTUAL; 
	UCHAR REFL_FLAG;
	UCHAR REFR_FLAG;
	UCHAR STOP_FLAG;
	UCHAR POSITION_FLAG;
	
} TMotorConfig;

typedef struct
{
  UCHAR Motor;       //!< motor/bank parameter
  union
  {
    long Int32;      //!< value parameter as 32 bit integer
    UCHAR Byte[4];   //!< value parameter as 4 bytes    Byte[3]Ϊ���ֽ�
  } Value;           //!< value parameter
} TMCLCommand;

typedef union
{
	float f32;  
	//long	i32;
	u8 		Byte[4];   	
	
}Data_4Bytes;

/*************�������������������������оƬ���������������***************/

/****************����������������ģ�鹦�ܶ��������������***************/
#define UART_1 1
#define UART_2 2
#define UART_3 3
#define UART_INTERFACE_1 UART_1
//LED
#define LED1_ON()       	GPIOD->BRR = BIT7    							
#define LED1_OFF()      	GPIOD->BSRR = BIT7     						
#define LED1_TOGGLE()   	GPIOD->ODR ^= BIT7 
#define LED2_TOGGLE()   	GPIOB->ODR ^= BIT12
//RGB
#define RGB_R_ON() 				GPIOD->BSRR=BIT6;	GPIOD->BRR=BIT5|BIT4
#define RGB_G_ON() 				GPIOD->BSRR=BIT5;	GPIOD->BRR=BIT4|BIT6
#define RGB_B_ON() 				GPIOD->BSRR=BIT4;	GPIOD->BRR=BIT5|BIT6
#define RGB_OFF() 				GPIOD->BRR=BIT4|BIT5|BIT6
//INPUT
//#define INPUT(n) 					PEin((n-1))
//OUTPUT �¾ɰ忨�����л�
//#if (UART_INTERFACE_1 == UART_2)
//	#define OUTPUTL(n)   		{if(n<5)	PCout((n+5))=0;  else PAout((n+3))=0;}
//	#define OUTPUTH(n)   		{if(n<5)	PCout((n+5))=1;  else PAout((n+3))=1; }
//#elif (UART_INTERFACE_1 == UART_1)
//	#define OUTPUTL(n)   		{if(n<5)	PCout((n+5))=0;  else if((n==5)) PAout(8)=0; else if((n==6)) PAout(2)=0; else if((n==7)) PAout(3)=0; else PAout(11)=0;}
//	#define OUTPUTH(n)   		{if(n<5)	PCout((n+5))=1;  else if((n==5)) PAout(8)=1; else if((n==6)) PAout(2)=1; else if((n==7)) PAout(3)=1; else PAout(11)=1; }
//#endif
		
//functional
#define MIN(a,b) (a<b) ? (a) : (b)
#define MAX(a,b) (a>b) ? (a) : (b)

#define	EEPROM_SET	'V'						
/************��������������Ŀ���öԶ��������������***************/
#define	BUTTON_ONLINE					0			//��BUTTON_OFFLINE���𣺸�λ�����������׻�λ����
#define	BUTTON_OFFLINE				1			//��ͣ����IN3
#define	LIDOPEN								2
#define	IAC_LCR								3
#define	JABIL_LCR							4
#define	PROJECT_NONE					10

extern UCHAR ProID;	
extern UCHAR Board_debugFlag;
		
#endif

