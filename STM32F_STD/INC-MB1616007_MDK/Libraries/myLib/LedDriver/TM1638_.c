#include "TM1638.h"
#include "spi.h"
#include "delay.h"

#define TM1638_CS	PBout(12)

typedef char DisplayData_t[TM1638_DISPLAY_MEM];
char _display;
char _bright; 

/** Helper to reverse all command or databits. The TM1638 expects LSB first, whereas SPI is MSB first
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


void SPI2_WriteByte(u8 TxData)
{
	u8 TxTemp[1];
	TxTemp[0]=TxData;
	HAL_SPI_Transmit(&hspi2,&TxTemp[0],1,5);
}
/** Write command and parameter to TM1638
  *  @param  int cmd Command byte
  *  &Param  int data Parameters for command
  *  @return none
  */  
void TM1638_writeCmd(int cmd, int data)
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
void TM1638_writeByte(char data, int address)
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
void TM1638_writeBytes(DisplayData_t data, int length, int address)
{
	int idx;
  TM1638_CS=0;
  delay_us(1);    
       
// sanity check
  address &= TM1638_ADDR_MSK;
  if (length < 0) {length = 0;}
  if ((length + address) > TM1638_DISPLAY_MEM) {length = (TM1638_DISPLAY_MEM - address);}
  SPI2_WriteByte(TM1638_flip(TM1638_ADDR_SET_CMD | address)); // Set Address

  for (idx=0; idx<length; idx++) 
	{    
    SPI2_WriteByte(TM1638_flip(data[address + idx])); // data 
  }
  
  delay_us(1);
  TM1638_CS=1;             
}
/** Set Brightness
  *
  * @param  char brightness (3 significant bits, valid range 0..7 (1/16 .. 14/14 dutycycle)  
  * @return none
  */
void TM1638_setBrightness(char brightness)
{
  _bright = brightness & TM1638_BRT_MSK; // mask invalid bits
  
  TM1638_writeCmd(TM1638_DSP_CTRL_CMD, _display | _bright );  // Display control cmd, display on/off, brightness  
}


/** Set the Display mode On/off
  *
  * @param bool display mode
  */
void TM1638_setDisplay(u8 on)
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

void TM1638_Init(void)
{
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
      
  for (cnt=0; cnt<TM1638_DISPLAY_MEM; cnt++)
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
}



