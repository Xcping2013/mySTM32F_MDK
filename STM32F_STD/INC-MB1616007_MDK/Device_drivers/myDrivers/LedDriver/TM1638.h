#ifndef  __TM1638_H
#define  __TM1638_H

#include "sys.h"

/***		读取和接收1个BIT都在时钟的上升沿操作
TM1638是带键盘扫描接口的LED驱动控制专用电路
显示模式 10 段×8 位
键扫描（8×3bit）
辉度调节电路（占空比8 级可调）
串行接口（CLK，STB，DIO）
振荡方式：RC 振荡（450KHz+5%）

显示寄存器:

SEG1	SEG2	SEG3	SEG4	SEG5	SEG6	SEG7	SEG8	SEG9	SEG10	X X X X X X
		xxHL（低四位） 				xxHU(高四位) 			xxHL(低四位) 		xxHU（高四位）
	 B0 B1 B2 B3 						B4	B5 	B6 	B7 				B0 B1 B2 B3 B4 B5 B6 B7	
	 
			00HL 										00HU 0					1HL 0						1HU 					GRID1
			02HL 										02HU 						03HL 						03HU 					GRID2
			04HL							 			04HU 						05HL 						05HU				  GRID3
			06H   									06HU 						07HL 						07HU 					GRID4
			08HL 										08HU 						09HL 						09HU 					GRID5
			0AHL 										0AHU 						0BHL 						0BHU 					GRID6
			0CHL 										0CHU					  0DHL 						0DHU 					GRID7
			0EHL 										0EHU 						0FHL 						0FHU 					GRID8
***/
/* 		key read	8*3
					KS1	KS2		KS3		KS4		KS5	KS6	KS7	KS8  4Byte
			K1 B0~2	B4~5	B0~2	B4~5
			K2  
			K3  


*/

//only display 8 bits without key scan	2*4BITS SEG

//Reserved bits for commands
#define TM1638_CMD_MSK      0xC0
//Data setting commands
#define TM1638_DATA_SET_CMD 0x40
#define TM1638_DATA_WR      0x00
#define TM1638_KEY_RD       0x02
#define TM1638_ADDR_INC     0x00			//wirteBytes							MAX 14BYTE
#define TM1638_ADDR_FIXED   0x04			//wirteByte one by one	
#define TM1638_MODE_NORM    0x00
#define TM1638_MODE_TEST    0x08
//Address setting commands
#define TM1638_ADDR_SET_CMD 0xC0
#define TM1638_ADDR_MSK     0x0F
//Display control commands
#define TM1638_DSP_CTRL_CMD 0x80
#define TM1638_BRT_MSK      0x07
#define TM1638_BRT0         0x00 //Pulsewidth 1/16
#define TM1638_BRT1         0x01
#define TM1638_BRT2         0x02
#define TM1638_BRT3         0x03
#define TM1638_BRT4         0x04
#define TM1638_BRT5         0x05
#define TM1638_BRT6         0x06
#define TM1638_BRT7         0x07 //Pulsewidth 14/16

#define TM1638_BRT_DEF      TM1638_BRT3

#define TM1638_DSP_OFF      0x00
#define TM1638_DSP_ON       0x08

//COMMON_A
//            A
//          -----
//         |     |     
//       F |     | B    
//         |  G  |     
//          -----
//         |     |     
//       E |     | C    
//         |     |     
//          -----   * DP
//            D  
//
#define S7_A    0x0001
#define S7_B    0x0002
#define S7_C    0x0004
#define S7_D    0x0008
#define S7_E    0x0010
#define S7_F    0x0020
#define S7_G    0x0040 
#define S7_DP   0x0080 
// ASCII Font definitions for segments in each character
//48 0x30  Digits
#define C7_0    (S7_A | S7_B | S7_C | S7_D | S7_E | S7_F)
#define C7_1    (S7_B | S7_C)
#define C7_2    (S7_A | S7_B | S7_D | S7_E | S7_G)
#define C7_3    (S7_A | S7_B | S7_C | S7_D | S7_G)
#define C7_4    (S7_B | S7_C | S7_F | S7_G)
#define C7_5    (S7_A | S7_C | S7_D | S7_F | S7_G)
#define C7_6    (S7_A | S7_C | S7_D | S7_E | S7_F | S7_G)
#define C7_7    (S7_A | S7_B | S7_C)
#define C7_8    (S7_A | S7_B | S7_C | S7_D | S7_E | S7_F | S7_G)
#define C7_9    (S7_A | S7_B | S7_C | S7_D | S7_F | S7_G)

typedef struct 
{
	Gpio* dio_pin;
	Gpio* stb_pin;
	Gpio* clk_pin;
	
	uint8_t bright;
	uint8_t display;
	
}tm1638_t;

typedef struct
{		
  void 		 (*init)			(tm1638_t);
	uint16_t (*readkey)		(tm1638_t);		
	void 		 (*show)			(tm1638_t,  char *);	

}TM1638_DrvTypeDef; 

extern tm1638_t tm1638_8bit;
extern TM1638_DrvTypeDef tm1638;

#endif


