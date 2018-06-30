#include "delay.h"
#include "TM1638.h"

#define	NONE									0
#define	DIGIT1								1
#define	DIGIT2								2
#define	DIGIT3								3
#define	DIGIT4								4

extern uint32_t  AddFactor[4];

uint8_t  _display = TM1638_DSP_ON;
uint8_t  _bright  = TM1638_BRT_DEF; 

uint32_t Display_Time[10]={C7_0,C7_1,C7_2,C7_3,C7_4,C7_5,C7_6,C7_7,C7_8,C7_9};

#define CLK_H()   HAL_GPIO_WritePin(TM1638_SCK_GPIO_Port, TM1638_SCK_Pin, GPIO_PIN_SET)
#define CLK_L()   HAL_GPIO_WritePin(TM1638_SCK_GPIO_Port, TM1638_SCK_Pin, GPIO_PIN_RESET)

#define STB_H()   HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, GPIO_PIN_SET)
#define STB_L()   HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, GPIO_PIN_RESET)

#define DIO_H()   HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port, TM1638_DIO_Pin, GPIO_PIN_SET)
#define DIO_L()   HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port, TM1638_DIO_Pin, GPIO_PIN_RESET)

void DIO_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = TM1638_DIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TM1638_DIO_GPIO_Port, &GPIO_InitStruct);
	
//{GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=0X00000030;}
}
//
uint8_t DIO_int(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = TM1638_DIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TM1638_DIO_GPIO_Port, &GPIO_InitStruct);
	
	return HAL_GPIO_ReadPin(TM1638_DIO_GPIO_Port,TM1638_DIO_Pin);
}
//
void TM1638_Write(unsigned char	DATA)			//写数据函数
{
	unsigned char i;
	DIO_Out();
	for(i=0;i<8;i++)
	{
		CLK_L();
		delay_us(1);
		if(DATA&0X01)  	DIO_H();
		else						DIO_L();
		delay_us(1);
		CLK_H();
		delay_us(1);	
		DATA>>=1;	
	}
}
//
uint8_t TM1638_Read(void)					//读数据函数
{
	uint8_t i;
	uint8_t temp=0;
	for(i=0;i<8;i++)
	{
		CLK_L();
		delay_us(1);
		temp>>=1;
		if(DIO_int())
			temp|=0x80;
		delay_us(1);
		CLK_H();
		delay_us(1);
	}
	return temp;
}
//
void TM1638_writeCmd(int cmd, int data)
{   
  STB_L();
  delay_us(1);      
  TM1638_Write( (cmd & TM1638_CMD_MSK) | (data & ~TM1638_CMD_MSK));   
	//TM1638_Write(cmd); 
  STB_H();         
}
/** Write databyte to TM1638
  *  @param  char data byte written at given address
  *  @param  int address display memory location to write byte
  *  @return none
  */ 
void TM1638_writeData(char data, int address)
{
  STB_L();
  delay_us(1);    
  TM1638_Write(TM1638_ADDR_SET_CMD | (address & TM1638_ADDR_MSK)); // Set Address cmd    
  TM1638_Write(data); // data 
  delay_us(1);
  STB_H();            
}
//
/** Set Brightness
  *
  * @param  char brightness (3 significant bits, valid range 0..7 (1/16 .. 14/14 dutycycle)  
  * @return none
  */
void TM1638_setBrightness(char brightness)
{ 
	_bright = brightness & TM1638_BRT_MSK; // mask invalid bits
	TM1638_writeCmd(TM1638_DSP_CTRL_CMD, _display | _bright );
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
  else 
	{
    _display = TM1638_DSP_OFF;
  }
   TM1638_writeCmd(TM1638_DSP_CTRL_CMD, _display | _bright );  // Display control cmd, display on/off, brightness   
  //TM1638_writeCmd(TM1638_DSP_CTRL_CMD, TM1638_DSP_ON | TM1638_DSP_OFF );  // Display control cmd, display on/off, brightness   
}
//
uint8_t TM1638_getKey(void)
{
	uint8_t c[4],i;//,key_value=0;
	STB_L();
	TM1638_Write(TM1638_DATA_SET_CMD|TM1638_KEY_RD);		           //读键扫数据 命令
	for(i=0;i<4;i++)		
	{	
		c[i]=TM1638_Read();
	}
	STB_H();					           		 //4个字节数据合成一个字节
		
	if((c[0]&0x01) && c[1]==0 && c[2]==0 && c[3]==0	&& 	(c[0]&0x10)==0) 		 		return KEY_START;
	else if((c[0]&0x10) && c[1]==0 && c[2]==0 && c[3]==0	&&	(c[0]&0x01)==0) 	return KEY_SET;
  else if(c[0]==0 && (c[1]&0x01) && c[2]==0 && c[3]==0	&& (c[1]&0x10)==0)	 	return KEY_UP;
	else if(c[0]==0 && (c[1]&0x10) && c[2]==0 && c[3]==0  && (c[1]&0x01)==0)		return KEY_DOWN;	
	//else if((c[0]&0x11) && c[2]==0 && c[3]==0 )		return KEY_BRT;	
	else return KEY_NONE;

}
//
void TM1638_DisplayValue(u32 num)
{
	static uint32_t TBlinkDelay;
	static u8 Tmode=0;
	u8 TimeValue[4]={0};
	u32 temp;
	temp = num;

	TimeValue[3] = Display_Time[temp/AddFactor[3]];			temp %= AddFactor[3];
	TimeValue[2] = Display_Time[temp/AddFactor[2]];			temp %= AddFactor[2];
	TimeValue[1] = Display_Time[temp/AddFactor[1]];			temp %= AddFactor[1];
	TimeValue[0] = Display_Time[temp/AddFactor[0]];

	if((HAL_GetTick()-TBlinkDelay)>500)
	{
		Tmode=~Tmode;
		TBlinkDelay=HAL_GetTick();
	}
	
	TM1638_writeData(TimeValue[3], SEG_BIT4);
	if(Tmode)	TM1638_writeData(TimeValue[2], SEG_BIT3);
	else 			TM1638_writeData(TimeValue[2]|S7_DP, SEG_BIT3);
	TM1638_writeData(TimeValue[1], SEG_BIT2);
	TM1638_writeData(TimeValue[0], SEG_BIT1);	

}
void TM1638_DisplayValue1(u32 num)
{
	static uint32_t TBlinkDelay;
	static u8 Tmode=0;
	if((HAL_GetTick()-TBlinkDelay)>500)
	{
		Tmode=~Tmode;
		TBlinkDelay=HAL_GetTick();
	}	
	TM1638_writeData(Display_Time[DisValue[4]], SEG_BIT4);
	if(Tmode)	TM1638_writeData(Display_Time[DisValue[3]], SEG_BIT3);
	else 			TM1638_writeData(Display_Time[DisValue[3]]|S7_DP, SEG_BIT3);
	TM1638_writeData(Display_Time[DisValue[2]], SEG_BIT2);
	TM1638_writeData(Display_Time[DisValue[1]], SEG_BIT1);	
}
/** Clear the screen and locate to 0
 */  
void TM1638_cls(void) 
{
  STB_L();
  delay_us(1);    
  TM1638_Write(TM1638_ADDR_SET_CMD | 0x00); // Address set cmd, 0
	
	TM1638_writeData(0, SEG_BIT4);
	TM1638_writeData(0, SEG_BIT3);
	TM1638_writeData(0, SEG_BIT2);
	TM1638_writeData(0, SEG_BIT1);
	
  delay_us(1);
	
  STB_H();  
}   
void TM1638_init(void)
{
  TM1638_writeCmd(TM1638_DSP_CTRL_CMD, _display | _bright );                                 // Display control cmd, display on/off, brightness   
  
  TM1638_writeCmd(TM1638_DATA_SET_CMD, TM1638_DATA_WR | TM1638_ADDR_INC | TM1638_MODE_NORM); // Data set cmd, normal mode, auto incr, write data  
	
	TM1638_cls();
	
	TM1638_writeData(Display_Time[8], SEG_BIT4);
	TM1638_writeData(Display_Time[8]|S7_DP, SEG_BIT3);
	TM1638_writeData(Display_Time[8], SEG_BIT2);
	TM1638_writeData(Display_Time[8], SEG_BIT1);

	//TM1638_DisplayValue(3600000);
}
//
//
void DIGIT4_SEG7_Display(uint8_t BitN_Flash ,uint32_t value)
{
  switch(BitN_Flash)
	{
		case DIGIT1:	
			TM1638_writeData(0, SEG_BIT1);
			break ;
		case DIGIT2:	
			TM1638_writeData(0, SEG_BIT2);
			break ;				
		case DIGIT3:	
			TM1638_writeData(0, SEG_BIT3);
			break ;		
		case DIGIT4:	
			TM1638_writeData(0, SEG_BIT4);
			break ;		
		default :
			break ;		
	}
}
