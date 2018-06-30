#ifndef __AT24CX_H
#define __AT24CX_H

//10 kO for 100 kHz, 2 kO for 400 kHz
#include "sys.h"
#include "mcu_softI2c.h"
//	     		Kbit							Byte				n*pageSize(byte)		MaxFreq	 		Write Cycle   data 	word 			address 
#define AT24C01		0x001		//	127					//16*8		2**7			400K	    	
#define AT24C02		0x002		//	255					//32*8		2**8   		1MHZ				8-bit 
#define AT24C04		0x004		//	511					//32*16							400k		 		3ms       								1010A2A1P0W/R		2_blcok*256Byte
#define AT24C08		0x008	  //	1023				//64*16																										1010A2P1P0W/R
#define AT24C16		0x010		//	2047				//128*16																									1010P2P1P0W/R
#define AT24C32		0x020		//	4095				//128*32																									1010A2A1A0W/R
#define AT24C64		0x040		//	8191
#define AT24C128	0x080		//	16383				//256*64Byte				12MHZ			5ms Max/100W 		14-bit
#define AT24C256	0x100		//	32767  
#define AT24C512	0x200		//

typedef struct 
{
	uint16_t type;
	int maxAddress;
	int pageSize;
	int blockNumbers;
	
}device_t;

extern const device_t C24LC01  ;
extern const device_t C24LC02  ;
extern const device_t C24LC04  ;
extern const device_t C24LC08  ;
extern const device_t C24LC16  ;
extern const device_t C24LC32  ;
extern const device_t C24LC64  ;
extern const device_t C24LC128 ;
extern const device_t C24LC256 ;
extern const device_t C24LC512 ;

typedef struct 
{
	u8 data[8];
	IIC_GPIO	pins;
	uint8_t devAddress;
	device_t eeprom;	
	
}at24cxx_t;

typedef struct
{		
  void (*init)		(at24cxx_t);
	void (*read)		(at24cxx_t,  u16 , u8 *,	u16 );		
	void (*write)		(at24cxx_t,  u16 , u8 *,	u16 );	
	
	u8 	 (*readU8)	(at24cxx_t , u16 );
	void (*writeU8)	(at24cxx_t , u16 , u8 );
	
	u16	 (*readU16)	(at24cxx_t , u16 );
	void (*writeU16)(at24cxx_t , u16 , u16 );
	
	u32	 (*readU32)	(at24cxx_t , u16 );
	void (*writeU32)(at24cxx_t , u16 , u32 );

	f32  (*readF32)	(at24cxx_t , u16 );	
	void (*writeF32)(at24cxx_t , u16 , f32 );

}AT24CXX_DrvTypeDef; 

extern AT24CXX_DrvTypeDef at24cxx;

extern at24cxx_t at24c256;
					  
#endif
















