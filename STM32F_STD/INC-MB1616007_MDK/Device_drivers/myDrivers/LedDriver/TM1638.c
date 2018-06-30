#include "TM1638.h"
#include "delay.h"
#include "string.h"

uint32_t showNum[10]={C7_0,C7_1,C7_2,C7_3,C7_4,C7_5,C7_6,C7_7,C7_8,C7_9};

static void tm1638_init(tm1638_t dev);

static void tm1638_writeByte(tm1638_t dev, uint8_t DATA);
static void tm1638_writeCmd(tm1638_t dev, int cmd, int data );

static void tm1638_setBrightness(tm1638_t dev, char brightness);
static void TM1638_setDisplay(tm1638_t dev, u8 on);
static void tm1638_writeData(tm1638_t dev, char data, int address);
static void tm1638_cls(tm1638_t dev);
static void tm1638_show(tm1638_t dev, char *data);

static uint8_t 	tm1638_readByte(tm1638_t dev);
static uint16_t tm1638_readkey(tm1638_t dev);

tm1638_t tm1638_8bit=
{
	&PE12,
	&PE14,
	&PE15,
	
	TM1638_BRT_DEF,
	TM1638_DSP_ON,
};
//
TM1638_DrvTypeDef	tm1638=
{
	tm1638_init,
	tm1638_readkey,
	tm1638_show,
};

static void tm1638_init(tm1638_t dev)
{
	pinMode(dev.dio_pin, GPIO_Mode_Out_PP);
	pinMode(dev.clk_pin, GPIO_Mode_Out_PP);
	pinMode(dev.stb_pin, GPIO_Mode_Out_PP);

	pinWrite( dev.dio_pin , HIGH);
	pinWrite( dev.clk_pin , HIGH);
	pinWrite( dev.stb_pin , HIGH);
	
  tm1638_writeCmd(dev, TM1638_DSP_CTRL_CMD, dev.display | dev.bright );                           // Display control cmd, display on/off, brightness   
  tm1638_writeCmd(dev, TM1638_DATA_SET_CMD, TM1638_DATA_WR | TM1638_ADDR_FIXED | TM1638_MODE_NORM); // Data set cmd, normal mode, auto incr, write data  
	
	tm1638_cls(dev);
}

//shift_out(data_pin,sck_pin,bit_oder,Cmd);
static void tm1638_writeByte(tm1638_t dev, uint8_t DATA)
{
    uint8_t i;
		pinMode(dev.dio_pin, GPIO_Mode_Out_PP);
    for(i = 0; i < 8; i++)
    {
        pinWrite( dev.clk_pin , LOW);
        delay_us(1);
        if(DATA & 0X01)
            pinWrite( dev.dio_pin , HIGH);
        else
            pinWrite( dev.dio_pin , LOW);
        DATA >>= 1;
        delay_us(1);
        pinWrite( dev.clk_pin , HIGH);
        delay_us(1);
    }
}
static void tm1638_writeCmd(tm1638_t dev, int cmd, int data )
{
    pinWrite( dev.stb_pin , LOW);
    delay_us(1);
    tm1638_writeByte(dev, (cmd & TM1638_CMD_MSK) | (data & ~TM1638_CMD_MSK));
    pinWrite(dev.stb_pin , HIGH);
}
/** Write databyte to TM1638
  *  @param  char data byte written at given address
  *  @param  int address display memory location to write byte
  *  @return none
  */ 
static void tm1638_writeData(tm1638_t dev, char data, int address)
{
  pinWrite( dev.stb_pin , LOW);
  delay_us(1);    
  tm1638_writeByte(dev, TM1638_ADDR_SET_CMD | (address & TM1638_ADDR_MSK)); // Set Address cmd    
  tm1638_writeByte(dev, data); // data 
  delay_us(1);
  pinWrite( dev.stb_pin , HIGH);            
}
/** Set Brightness
  *
  * @param  char brightness (3 significant bits, valid range 0..7 (1/16 .. 14/14 dutycycle)  
  * @return none
  */
static void tm1638_setBrightness(tm1638_t dev, char brightness)
{
	dev.bright = brightness & TM1638_BRT_MSK; // mask invalid bits
	tm1638_writeCmd(dev, TM1638_DSP_CTRL_CMD, dev.display | dev.bright );
}
/** Set the Display mode On/off
  *
  * @param bool display mode
  */
static void TM1638_setDisplay(tm1638_t dev, u8 on)
{
	if (on) 
	{
    dev.display = TM1638_DSP_ON;
  }
  else 
	{
    dev.display = TM1638_DSP_OFF;
  }
   tm1638_writeCmd(dev, TM1638_DSP_CTRL_CMD, dev.display | dev.bright );  // Display control cmd, display on/off, brightness   
}
static uint8_t tm1638_readByte(tm1638_t dev)
{
    uint8_t ReadValue = 0;
    uint8_t i;
		pinMode(dev.dio_pin, GPIO_Mode_IPU);
    for(i = 0; i < 8; i++)
    {
        pinWrite( dev.clk_pin , LOW);
        ReadValue = ReadValue << 1;
        delay_us(1);
        ReadValue |= pinRead(dev.dio_pin);
        pinWrite(dev.clk_pin , HIGH);
        delay_us(1);
    }
    return ReadValue;
}

static uint16_t tm1638_readkey(tm1638_t dev)
{
    uint8_t c[4], i;
    uint16_t key_value = 0;
    pinWrite( dev.stb_pin , LOW);
    delay_us(1);
    tm1638_writeByte(dev, TM1638_DATA_SET_CMD|TM1638_KEY_RD);
    delay_us(1);
    for(i = 0; i < 4; i++)
        c[i] = tm1638_readByte(dev);
    delay_us(1);
    pinMode(dev.dio_pin, GPIO_Mode_Out_PP);
    delay_us(1);
    pinWrite( dev.stb_pin , HIGH);					//4个字节数据合成一个字节
//    key_value = c[1];
//    key_value = ((key_value) << 8) | c[0];
		key_value= (c[3]<<12)	| (c[2]<< 8) | (c[1]<< 4) | c[0];
    return key_value;
}



/** Clear the screen and locate to 0
 */  
static void tm1638_cls(tm1638_t dev)
{
  pinWrite( dev.stb_pin , LOW);
  delay_us(1); 
	
  tm1638_writeByte(dev, TM1638_ADDR_SET_CMD | 0x00); // Address set cmd, 0
	
	tm1638_writeData(dev, 0, 0);
	tm1638_writeData(dev, 0, 2);
	tm1638_writeData(dev, 0, 4);
	tm1638_writeData(dev, 0, 6);
	tm1638_writeData(dev, 0, 8);
	tm1638_writeData(dev, 0, 10);
	tm1638_writeData(dev, 0, 12);
	tm1638_writeData(dev, 0, 14);
	
  delay_us(1);
  pinWrite( dev.stb_pin , HIGH);  
} 

static void tm1638_show(tm1638_t dev, char *data)
{
	u8 i;
	u8 len=strlen(data);
	if(len>0 && len<9 )
	{
		for(i=len;i>0;i--)
		{
			tm1638_writeData(dev, showNum[(data[i-1]-'0')], (len-i)*2);
		}	
		for(i=len;i<8;i++)
		{
			tm1638_writeData(dev, 0, i*2);
		}	
	}
}

