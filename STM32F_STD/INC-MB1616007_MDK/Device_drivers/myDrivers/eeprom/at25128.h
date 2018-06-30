/* 
VCC = 1.8V to 5.5V

128K (16,384 x 8)	64-byte Page Mode Block Write Protection

Supports SPI Modes 0 (0,0) and 3 (1,1):Data Sheet Describes Mode 0 Operation

20MHz Clock Rate (5V)

Self-timed Write Cycle (5ms max)

8-bit instruction registe;	MSB first and start with a high-to-low CS transition

Both Hardware and	Software Data Protection		16bits Address
*/

//Functional Description
#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "spi.h"

#define	WRSR 	0x01 //Write Status Register
#define	WRITE	0x02 //Write Data to Memory Array
#define	READ 	0x03 //Read Data from Memory Array
#define	WRDI 	0x04 //Reset Write Enable Latch
#define	RDSR 	0x05 //Read Status Register
#define WREN	0X06 //Set Write Enable Latch
//All programming instructions must therefore be preceded by a Write Enable instruction

#define WPEN_RDY	0X00
#define WPEN_BUSY	0X01
#define WPEN_EN		0X02

void at25128_init(void);

void at25128_WriteByte(UINT Address, UCHAR Value);
UCHAR at25128_ReadByte(UINT Address);

void at25128_WriteBlock(UINT Address, UCHAR *Block, UINT Size);
void at25128_ReadBlock(UINT Address, UCHAR *Block, UINT Size);

#endif
