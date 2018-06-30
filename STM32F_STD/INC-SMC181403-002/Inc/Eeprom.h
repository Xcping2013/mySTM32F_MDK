#ifndef __Eeprom_H
#define __Eeprom_H

#include "24cxx.h" 
	/**				EEPROM	ADDRESS
**
**/
	#define	ADDR_PROJECT_ID	16383
	//PAGE0	Number	:Basic
	#define	ADDR_Update			0				//'Y'|'N'
	#define	ADDR_BT					1				//4Byte BaudRate
	#define	ADDR_SW					5				//1Byte 
	#define	ADDR_HW					6				//1Byte 
	#define	ADDR_DE					7				//1Byte 
	//PAGE1 String	:User
	#define	ADDR_FSN				64			//10Byte 
	
	//#define	ADDR_Next			74	
/*	[SW]			*/			void PrintSoftVersionFromEEPROM(void);
/*	[HW]			*/			void PrintHardVersionFromEEPROM(void);
/*	[FSN]			*/			void PrintFSNFromEEPROM(void);

/*	SW[110]			*/		void WriteSWToEEPROM(uint8_t SoftWareNum);
/*	HW[110]			*/		void WriteHWToEEPROM(uint8_t HardWareNum);
/*	HW[100]			*/		void WriteFSNToEEPROM(uint8_t *Fsn);
/*	BaudRate[115200]*/void WriteBaudRateToEEPROM(uint32_t BaudRate);

uint8_t DefaultSettingsToEEPROM(void);

//uint8_t ROM_WriteDatas(char *Commands);
//uint8_t ROM_ReadDatas(char *Commands);

#endif

