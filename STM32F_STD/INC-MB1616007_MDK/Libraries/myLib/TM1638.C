#include "TM1638.h"
#include "spi.h"
#include "delay.h"

#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif
#define TM1638_CS	PBout(12)

char _display;
char _bright; 

u8 DisData[11]={C7_0,C7_1,C7_2,C7_3,C7_4,C7_5,C7_6,C7_7,C7_8,C7_9,0};
u8 DisBit[8]={0,2,4,6,8,10,12,14};

/** Helper to reverse all command or databits. The TM1638 expects LSB first, whereas SPI is MSB first
  为了与此SPI的其他设备正常
  *  @param  char data
  *  @return bitreversed data
  */ 
#if(1)
char TM1638_flip(char data) 
{
 char value=0;
  
 if (data & 0x01) {value |= 0x80;} ;  
 if (data & 0x02) {value |= 0x40;} ;
 if (data & 0x04) {value |= 0x20;} ;
 if (data & 0x08) {value |= 0x10;} ;
 if (data & 0x10) {value |= 0x08;} ;
 if (data & 0x20) {value |= 0x04;} ;
 if (data & 0x40) {value |= 0x02;} ;
 if (data & 0x80) {value |= 0x01;} ;
 return value;       
}
#else
char TM1638_flip(char data) 
{
 data   = (((data & 0xAA) >> 1) | ((data & 0x55) << 1));
 data   = (((data & 0xCC) >> 2) | ((data & 0x33) << 2));
 return   (((data & 0xF0) >> 4) | ((data & 0x0F) << 4));
}
#endif



/** Write command and parameter to TM1638
  *  @param  int cmd Command byte
  *  &Param  int data Parameters for command
  *  @return none
  */  
static void TM1638_writeCmd(int cmd, int data)
{   
  TM1638_CS=0;
  delay_us(1);      
  SPI2_WriteByte(TM1638_flip( (cmd & TM1638_CMD_MSK) | (data & ~TM1638_CMD_MSK)));   
  delay_us(1);
  TM1638_CS=1;          
}
/** Write databyte to TM1638
  *  @param  char data byte written at given address
  *  @param  int address display memory location to write byte
  *  @return none
  */ 
static void TM1638_writeByte(char data, int address)
{
  TM1638_CS=0;
  delay_us(1);    
  SPI2_WriteByte(TM1638_flip(TM1638_ADDR_SET_CMD | (address & TM1638_ADDR_MSK))); // Set Address cmd
      
  SPI2_WriteByte(TM1638_flip(data)); // data 
  
  delay_us(1);
  TM1638_CS=1;             
}
/** Write Display datablock to TM1638
  *  @param  DisplayData_t data Array of TM1638_DISPLAY_MEM (=16) bytes for displaydata
  *  @param  length number bytes to write (valid range 0..TM1638_DISPLAY_MEM (=16), when starting at address 0)
  *  @param  int address display memory location to write bytes (default = 0)   
  *  @return none
  */ 
/** Set Brightness
  *
  * @param  char brightness (3 significant bits, valid range 0..7 (1/16 .. 14/14 dutycycle)  
  * @return none
  */
static void TM1638_setBrightness(char brightness)
{
  _bright = brightness & TM1638_BRT_MSK; // mask invalid bits
  
  TM1638_writeCmd(TM1638_DSP_CTRL_CMD, _display | _bright );  // Display control cmd, display on/off, brightness  
}


/** Set the Display mode On/off
  *
  * @param bool display mode
  */
static void TM1638_setDisplay(u8 on)
{
  if (on)
	{
    _display = TM1638_DSP_ON;
  }
  else {
    _display = TM1638_DSP_OFF;
  }
  
  TM1638_writeCmd(TM1638_DSP_CTRL_CMD, _display | _bright );  // Display control cmd, display on/off, brightness   
}
/** Init the SPI interface and the controller
  * @param  none
  * @return none
  */ 

static void TM1638_Init(void)
{
	SPI2_Init();
	
	TM1638_CS=1;
	//TM1638 uses mode 3 (Clock High on Idle, Data latched on second (=rising) edge)
	//500khz
	//init controller  
  _display = TM1638_DSP_ON;
  _bright  = TM1638_BRT_DEF; 
  TM1638_writeCmd(TM1638_DSP_CTRL_CMD, _display | _bright );                                 // Display control cmd, display on/off, brightness   
  
  TM1638_writeCmd(TM1638_DATA_SET_CMD, TM1638_DATA_WR | TM1638_ADDR_INC | TM1638_MODE_NORM); // Data set cmd, normal mode, auto incr, write data  
}
/** Clear the screen and locate to 0
 */  
void TM1638_cls(void) 
{
	int cnt;
  TM1638_CS=0;
  delay_us(1);    
  SPI2_WriteByte(TM1638_flip(TM1638_ADDR_SET_CMD | 0x00)); // Address set cmd, 0
      
  for (cnt=0; cnt<16; cnt++)
	{
    SPI2_WriteByte(0x00); // data 
  }
  delay_us(1);
  TM1638_CS=1;      
}   
//
void DispalyInit(void)
{
	TM1638_Init();
	TM1638_cls();
	TM1638_writeByte(C7_0, 14);
	TM1638_writeByte(C7_1, 12);
	TM1638_writeByte(C7_2, 10);
	TM1638_writeByte(C7_3, 8);
	TM1638_writeByte(C7_4, 6);
	TM1638_writeByte(C7_5, 4);
	TM1638_writeByte(C7_6, 2);
	TM1638_writeByte(C7_7, 0);
}
void DisplayBit(u8 bit, u8 data)
{
	if(1<=bit&&bit<=8)
	{
		if(data<=10)
		{
			TM1638_writeByte(DisData[data],DisBit[8-bit]);	
		}
		
	}
}
void DisplayBits(u32 data)
{
	u8 bit[8];
	u8 i;
	if(data>99999999)	data=99999999;
	bit[7]=data/10000000;
	bit[6]=(data/1000000)&0x01;
	bit[5]=(data/100000)&0x01;
	bit[4]=(data/10000)&0x01;
	bit[3]=(data/1000)&0x01;
	bit[2]=(data/100)&0x01;
	bit[1]=(data/10)&0x01;
	bit[0]=(data|0x01)&0x01;
	
	if(bit[7])
	{
		for(i=1;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}
	else if(bit[6])
	{
		TM1638_writeByte(0,DisBit[7]);	
		for(i=2;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}
	else if(bit[5])
	{
		TM1638_writeByte(0,DisBit[7]);	
		TM1638_writeByte(0,DisBit[6]);	
		for(i=3;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}
	else if(bit[4])
	{
		TM1638_writeByte(0,DisBit[7]);	
		TM1638_writeByte(0,DisBit[6]);	
		TM1638_writeByte(0,DisBit[5]);	
		for(i=4;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}
	else if(bit[3])
	{
		TM1638_writeByte(0,DisBit[7]);	
		TM1638_writeByte(0,DisBit[6]);	
		TM1638_writeByte(0,DisBit[5]);	
		TM1638_writeByte(0,DisBit[4]);	
		for(i=5;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}
	else if(bit[3])
	{
		TM1638_writeByte(0,DisBit[7]);	
		TM1638_writeByte(0,DisBit[6]);	
		TM1638_writeByte(0,DisBit[5]);	
		TM1638_writeByte(0,DisBit[4]);	
		TM1638_writeByte(0,DisBit[3]);
		for(i=6;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}
	else if(bit[2])
	{
		TM1638_writeByte(0,DisBit[7]);	
		TM1638_writeByte(0,DisBit[6]);	
		TM1638_writeByte(0,DisBit[5]);	
		TM1638_writeByte(0,DisBit[4]);	
		TM1638_writeByte(0,DisBit[3]);
		TM1638_writeByte(0,DisBit[2]);
		for(i=7;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}		
	else if(bit[1])
	{
		TM1638_writeByte(0,DisBit[7]);	
		TM1638_writeByte(0,DisBit[6]);	
		TM1638_writeByte(0,DisBit[5]);	
		TM1638_writeByte(0,DisBit[4]);	
		TM1638_writeByte(0,DisBit[3]);
		TM1638_writeByte(0,DisBit[2]);
		TM1638_writeByte(0,DisBit[1]);
		for(i=8;i<9;i++)
		{
			TM1638_writeByte(DisData[bit[i-1]],DisBit[8-i]);	
		}
	}	
	else TM1638_writeByte(DisData[bit[0]],DisBit[0]);	
	
}
#ifdef RT_USING_FINSH
#include <finsh.h>
//INIT_DEVICE_EXPORT(DispalyInit);
FINSH_FUNCTION_EXPORT(TM1638_setBrightness, ...)
FINSH_FUNCTION_EXPORT(DisplayBits, ...)
#endif

