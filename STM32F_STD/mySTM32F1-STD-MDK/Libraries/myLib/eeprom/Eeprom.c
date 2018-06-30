/**
  \file Eeprom.c
  \version 1.00

  \brief EEPROM access functions

  This file contains EEPROM access functions.
*/
#include "eeprom.h"

void WriteEepromBlockII(UINT Address, UCHAR *Block, UINT Size);
//SCL在操作后都置0

/*****************					IIC  	Emulation      ****************************************/

static void wait_us(u32 nus)
{    
   u16 i=0;  
   while(nus--)
   {
      i=100;  
      while(i--) ;    
   }
}
static void wait_ms(u16 nms)
{	
	u32 i=0;  
	while(nms--)
  {
		i=72000;  
		while(i--) ;    
  }			
//	HAL_Delay(nms);	
}
//产生IIC起始信号  		
static void IIC_Start(void)
{  	
	IIC_SDA_OUT();
  
	IIC_SDA=1;			//START:when CLK is high,DATA change form high to low 
	IIC_SCL=1;			//SDA=1	SCL=1						
	wait_us(1);
	
	IIC_SDA=0;			//SDA=0
	wait_us(1);
	
	IIC_SCL=0;			//准备发送或接收数据 
}	  
//产生IIC停止信号
static void IIC_Stop(void)
{
	IIC_SDA_OUT();	
	
	IIC_SDA=0;			//STOP:when CLK is high DATA change form low to high
	IIC_SCL=1;			//SDA=0	SCL=1	
	wait_us(1);

	IIC_SDA=1;			//SDA=1
	wait_us(1);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
static u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA=1;
	IIC_SDA_IN(); wait_us(1);	//SDA设置为输入  	   
	
	IIC_SCL=1;		wait_us(1);	//稳定数据，前面的操作会使Device输出应答
	
	while(IIC_READ_SDA)				//读取数据
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;						//钳住I2C总线，准备发送或接收数据 必须 =0 SDA数据才会变化   
	return 0;  
} 
//产生ACK应答
static void IIC_Ack(void)
{
	IIC_SCL=0;					//L
	
	IIC_SDA_OUT();
	IIC_SDA=0;					//D
	wait_us(1);
	
	IIC_SCL=1;					//H
	wait_us(1);
	
	IIC_SCL=0;					//钳住I2C总线，准备发送或接收数据 必须 SCL=0 SDA数据才会变化
}
//不产生ACK应答		    
static void IIC_NAck(void)
{
	IIC_SCL=0;				//0
	
	IIC_SDA_OUT();
	IIC_SDA=1;				//
	
	wait_us(1);
	IIC_SCL=1;				//1
	
	wait_us(1);
	IIC_SCL=0;				 //钳住I2C总线，准备发送或接收数据 必须 SCL=0 SDA数据才会变化
}	
//@@
//IIC发送一个字节
//1，有应答
//0，无应答			  
static void IIC_Send_Byte(u8 txd)
{  
	//0-->1 Out	
	u8 t; 
	IIC_SDA_OUT(); 	  
	
	IIC_SCL=0;								//!> 0
	for(t=0;t<8;t++)				
	{              						
		IIC_SDA=(txd&0x80)>>7;	//!> 数据
		txd<<=1; 	  
		wait_us(1);  						//对于某些device这三个延时都是必须的
		
		IIC_SCL=1;							//!> 1			
		wait_us(1);
		
		IIC_SCL=0;							//!> 0					
		wait_us(1);
  }	 
} 	   
//@@
static u8 IIC_Read_Byte(unsigned char ack)
{
	//!  1> 0 出来一个数据  第一个数据需先读走
	unsigned char i,receive=0;
	IIC_SDA=1; 		
	IIC_SDA_IN();										//SDA设置为输入
	for(i=0;i<8;i++ )
	{	
		IIC_SCL=0;										//0		
		wait_us(1); 
		IIC_SCL=1;										//1
		receive<<=1;	
		if(IIC_READ_SDA)	receive++; 	//读数据
		wait_us(1);	
		
	}					 
	if (!ack)
			IIC_NAck();//发送nACK
	else
			IIC_Ack(); //发送ACK   
	return receive;
}
//@@

static void IIC_DeviceReadBytes(uint8_t DeviceAddr, u16 ReadAddr, uint8_t *RegData, uint8_t len, uint8_t DeviceType )
{	  	    																 
  IIC_Start();  
	if(DeviceType==3)						//寄存器地址与器件地址复用，如AT24C02~AT24C64
	{
		IIC_Send_Byte(DeviceAddr & 0xFE +((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 
		IIC_Wait_Ack();	
	  IIC_Send_Byte(ReadAddr%256);   			 //发送低地址
	  IIC_Wait_Ack(); 
	}	
	else if(DeviceType==2)		//两字节寄存器地址，	       如AT24C125~AT24C512
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);				 //发送高地址
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr%256);  		 //发送低地址
		IIC_Wait_Ack(); 	 										  		
	}	
	else if(DeviceType==1)	//一字节寄存器地址						
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr%256);  		  //发送低地址
		IIC_Wait_Ack(); 	 										  		
	}	
	
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr | 0x01);    //进入接收模式			   
	IIC_Wait_Ack();	
	
	while(len!=1)
	{
		len--;
		*(RegData++)=IIC_Read_Byte(1);	
	}
	*(RegData++)=IIC_Read_Byte(0);
  IIC_Stop();												 //产生一个停止条件	    
}
//
static void IIC_DeviceWriteBytes(uint8_t DeviceAddr, uint16_t WriteAddr, uint8_t  *RegData , uint8_t len, uint8_t DeviceType)
{	
  IIC_Start();  
	if(DeviceType==3)
	{
		IIC_Send_Byte(DeviceAddr & 0xFE +((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
		IIC_Wait_Ack();	
	  IIC_Send_Byte(WriteAddr%256);   			 //发送低地址
	  IIC_Wait_Ack(); 
	}	
	else if(DeviceType==2)
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);				//发送高地址
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr%256);  		 //发送低地址
		IIC_Wait_Ack(); 	 										  		
	}	
	else if(DeviceType==1)
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr%256);  		 //发送低地址
		IIC_Wait_Ack(); 	 										  		
	}	
 	
  while(len--)
	{
		IIC_Send_Byte(*(RegData++));     	//发送字节							   
		IIC_Wait_Ack();		
	}
	
  IIC_Stop();													//产生一个停止条件 
	wait_ms(10);
}
//
/*****************					EEPROM  	Driver      ****************************************/
/***************************************************************//**
   \fn 		WriteEepromByte(UINT Address, UCHAR Value)
   \brief Write a byte to the EEPROM
   \param Address  EEPROM location (0..16383)
   \param Value    Byte to be written

   This function writes a byte to the EEPROM at the specified
   EEPROM location.
********************************************************************/
void WriteEepromByte(UINT Address, UCHAR Value)
{
	IIC_Start(); 
	IIC_Send_Byte(EEPROM_ADDR & 0xFE);   //写器件地址 	  
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);					 //发送高地址
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  				 //发送低地址
	IIC_Wait_Ack(); 	 										  		
	
	IIC_Send_Byte(Value);     					//发送字节							   
	IIC_Wait_Ack(); 
	
	IIC_Stop();	
	wait_ms(10);
}


/*******************已测试OK*************************//**
   \fn WriteEepromBlock(UINT Address, UCHAR *Block, UINT Size)
   \brief Copy memory block to EEPROM
   \param Address  EEPROM location (0..16383)
   \param Block  pointer at memory block to be copied to the EEPROM
   \param Size   size of block to be copied (bytes)

   This function copies a memory block to the EEPROM. It is capable
   of filling the entire EEPROM just whith one function call.
********************************************************************/
void WriteEepromBlock(UINT Address, UCHAR *Block, UINT Size)
{
	u8 i;
  if(Address/64==0)			  //#从页首开始写
  {
		if(Size<=64)			
		{									
													//##小于页长度，直接写
			IIC_DeviceWriteBytes(0xA0,Address,Block,Size,2); 
		}
		else
		{
													//##大于一页长度
			for(i=0;i<(Size/64);i++)
			{				
													//##页写
				IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,64,2);
			}
													//##剩下字节
			IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,Size%64,2);
		}
	}
	else										//#从页中间开始写
	{
		if((Size+Address%64)<=64)
		{											
													//##数据长度+偏置 小于页长度，直接写
			IIC_DeviceWriteBytes(0xA0,Address,Block,Size,2); 
		}
		else
		{
			u8 StartPageSize	=	64-Address%64;
			u8 MiddlePageNum	=	(Size-StartPageSize)/64;
			u8 LastPageSize		=	(Size-StartPageSize)%64;

												 //##先写完本页剩余空间
			IIC_DeviceWriteBytes(0xA0,Address,Block,StartPageSize,2);  
												 //!> 下次写地址和数据地址
			Address=(Address/64+1)*64;Block=Block+StartPageSize;
			for(i=0;i<MiddlePageNum;i++)
			{
												//##写整页
				IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,64,2);
			}
												//##写剩下字节
			IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,LastPageSize,2);
		}
	}
}
//
void WriteEepromBlockII(UINT Address, UCHAR *Block, UINT Size)
{
	IIC_Start(); 
	IIC_Send_Byte(EEPROM_ADDR & 0xFE);   //写器件地址 	  
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);					 //发送高地址
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  				 //发送低地址
	IIC_Wait_Ack(); 	 										  		
	
  while(Size--)
	{
		IIC_Send_Byte(*(Block++));     	//发送字节							   
		IIC_Wait_Ack();		
	}
	
	IIC_Stop();	
	wait_ms(10);
}
/***************************************************************//**
   \fn 			ReadEepromByte(UINT Address)
   \brief 	Read a byte from the EEPROM
   \param 	Address  EEPROM location (0..16383)
   \return 	byte read from EEPROM

   This function reads one byte from the EEPROM.
********************************************************************/
UCHAR ReadEepromByte(UINT Address)
{
  u8 ReadData=0;
	IIC_Start(); 
	IIC_Send_Byte(EEPROM_ADDR & 0xFE); //写器件地址 	 
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);				 //发送高地址
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  			 //发送低地址
	IIC_Wait_Ack(); 
	
  IIC_Start();  	 	   
	IIC_Send_Byte(EEPROM_ADDR | 0x01);    //进入接收模式			   
	IIC_Wait_Ack();	
	
	ReadData=IIC_Read_Byte(0);	
	IIC_Stop();	
	
  return ReadData;
}


/***************************************************************//**
   \fn ReadEepromBlock(UINT Address, UCHAR *Block, UINT Size)
   \brief Copy block from EEPROM to RAM
   \param   Address  EEPROM start address (0..16383)
   \param   Block    RAM start address
   \param   Size     Length of block (bytes)

   Read a memory block from the EEPROM. This can also be the entire
   EEPROM.
********************************************************************/
void ReadEepromBlock(UINT Address, UCHAR *Block, UINT Size)
{
	IIC_Start(); 
	IIC_Send_Byte(EEPROM_ADDR & 0xFE); //写器件地址 	 
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);				 //发送高地址
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  			 //发送低地址
	IIC_Wait_Ack(); 
	
  IIC_Start();  	 	   
	IIC_Send_Byte(EEPROM_ADDR | 0x01);    //进入接收模式			   
	IIC_Wait_Ack();	
	
	while(Size!=1)
	{
		Size--;
		*(Block++)=IIC_Read_Byte(1);	
	}	
	*(Block++)=IIC_Read_Byte(0);
	IIC_Stop();	 
}

void EEPROM_Init(void)
{
	
}
//






