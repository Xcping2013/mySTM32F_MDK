/**						
  \Project		IN3-SMC-3
	\file 			IO.h
  \author 		Xcping2016
  \version 		1.00

  \brief 			I/O functions
  This file contains definitions for using the I/O functions.
*/
#ifndef __IO_H
#define __IO_H

#define INPUT1	PBin(5)
#define INPUT2	PBin(6)
#define INPUT3	PBin(7)
#define INPUT4	PBin(8)
#define INPUT5	PBin(9)

#define INPUT6	PEin(0)
#define INPUT7	PEin(1)
#define INPUT8	PEin(2)
#define INPUT9	PEin(3)
#define INPUT10	PEin(4)
#define INPUT11	PEin(5)
#define INPUT12	PEin(6)

#define INPUT13	PBin(1)
#define INPUT14	PDin(8)
#define INPUT15	PDin(12)
#define INPUT16	PDin(9)
#define INPUT17	PBin(15)
#define INPUT18	PBin(13)

#define OUTPUT1		PAout(15)
#define OUTPUT2		PCout(10)
#define OUTPUT3		PCout(11)
#define OUTPUT4		PCout(12)
#define OUTPUT5		PDout(0)
#define OUTPUT6		PDout(1)
#define OUTPUT7		PDout(2)
#define OUTPUT8		PDout(3)

#define OUTPUT9		PBout(14)
#define OUTPUT10	PDout(10)
#define OUTPUT11	PDout(13)
#define OUTPUT12	PDout(11)
#define OUTPUT13	PEout(14)
#define OUTPUT14	PEout(15)

void 		InitPinAsGPIO(void);
void 		SetOutput(uint8_t ch, uint8_t val);
uint8_t GetInput(uint8_t ch);

void EnableInterrupts(void);
void DisableInterrupts(void);
void ResetCPU(UCHAR ResetPeripherals);

void 	SetMotorCurrent(UCHAR Motor, UCHAR Current);
UCHAR GetStallState(UCHAR Motor);

void Debug_In_Out(void);

#endif
