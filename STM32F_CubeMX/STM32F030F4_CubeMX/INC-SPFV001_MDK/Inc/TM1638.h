#ifndef _TM1638_H
#define _TM1638_H

#include "stm32f0xx_hal.h"
//#include <stm32f0xx.h>
/***
TM1638�Ǵ�����ɨ��ӿڵ�LED��������ר�õ�·
��ʾģʽ 10 �Ρ�8 λ
��ɨ�裨8��3bit��
�Զȵ��ڵ�·��ռ�ձ�8 ���ɵ���
���нӿڣ�CLK��STB��DIO��
�񵴷�ʽ��RC �񵴣�450KHz+5%��

��ʾ�Ĵ���:

SEG1	SEG2	SEG3	SEG4	SEG5	SEG6	SEG7	SEG8	SEG9	SEG10	X X X X X X
		xxHL������λ�� 				xxHU(����λ) 			xxHL(����λ) 		xxHU������λ��
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

//Reserved bits for commands
#define TM1638_CMD_MSK      0xC0

//Data setting commands
#define TM1638_DATA_SET_CMD 0x40
#define TM1638_DATA_WR      0x00
#define TM1638_KEY_RD       0x02
#define TM1638_ADDR_INC     0x00
#define TM1638_ADDR_FIXED   0x04
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

#define TM1638_BRT_DEF      TM1638_BRT4

#define TM1638_DSP_OFF      0x00
#define TM1638_DSP_ON       0x08

//
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

#define SEG_BIT4	0
#define SEG_BIT3	2
#define SEG_BIT2	4
#define SEG_BIT1	6

#define SEG_BIT0	8

void TM1638_init(void);
void TM1638_cls(void);

void TM1638_writeData(char data, int address);
void TM1638_DisplayValue(uint32_t num);
void TM1638_setBrightness(char brightness);


uint8_t TM1638_getKey(void);
void KeyScan(uint8_t PinStatus);
void KeyProcess(void);

extern uint8_t  DisValue[5];
extern uint32_t Display_Time[10];
void DIGIT4_SEG7_Display(uint8_t BitN_Flash ,uint32_t value);


#endif

