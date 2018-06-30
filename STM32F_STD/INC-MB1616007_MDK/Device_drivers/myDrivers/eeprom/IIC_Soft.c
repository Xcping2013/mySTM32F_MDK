#include "IIC_SOFT.h"
#include "delay.h"
#include "usart.h"
#include "gpio.h"
#include "ctype.h"

//SCL 不操作时保持0
//产生IIC起始信号  		SCL 1/n MHZ 可按需求修改延时
void IIC_Start(void)
{  	  
	IIC_SCL=0;
	
 	IIC_SDA_OUT();  //sda线输出	
	IIC_SDA=1;			//START:when CLK is high,DATA change form high to low 
	IIC_SCL=1;
	delay_us(1);
	IIC_SDA=0;
	delay_us(1);
	IIC_SCL=0;			//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SCL=0;
	
	IIC_SDA_OUT();	//sda线输出
	IIC_SCL=1;
	delay_us(1);
	IIC_SDA=0;			//STOP:when CLK is high DATA change form low to high
	delay_us(1);
	IIC_SDA=1;			//发送I2C总线结束信号	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_IN();      			//SDA设置为输入  	   
	IIC_SCL=1;
	delay_us(1);	
	IIC_SCL=0; 
	while(IIC_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	//IIC_SCL=0;						//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;					//L
	IIC_SDA_OUT();
	IIC_SDA=0;					//D
	delay_us(1);				
	IIC_SCL=1;					//H
	delay_us(1);
	IIC_SCL=0;					//钳住I2C总线，准备发送或接收数据
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT();
	IIC_SDA=1;
	delay_us(1);
	IIC_SCL=1;
	delay_us(1);
	IIC_SCL=0;				 //钳住I2C总线，准备发送或接收数据
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t; 
	IIC_SCL=0;							//拉低时钟开始数据传输  
	IIC_SDA_OUT(); 	    		

	for(t=0;t<8;t++)					//0-->1 进去一个数据
	{              						//!> 0
		IIC_SDA=(txd&0x80)>>7;	//!> 数据
		txd<<=1; 	  
		delay_us(1);  					//对于某些device这三个延时都是必须的
		IIC_SCL=1;							//!> 1			
		delay_us(1);
		IIC_SCL=0;							
		delay_us(1);
  }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();										//SDA设置为输入
	IIC_SCL=1;	
	for(i=0;i<8;i++ )
	{																//!> 1	
		delay_us(1);
		IIC_SCL=0;										//!> 0 出来一个数据
		delay_us(1); 
		receive<<=1;	
		if(IIC_READ_SDA)	receive++; 	//读数据
		IIC_SCL=1;
	}					 
	if (!ack)
			IIC_NAck();//发送nACK
	else
			IIC_Ack(); //发送ACK   
	return receive;
}


//
void IIC_ReadByte(uint8_t DeviceAddr, uint8_t Addr_Data )
{	  	    																 
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(Addr_Data);   				//发送寄存器地址
	IIC_Wait_Ack();	
	
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr | 0x01);   //进入接收模式			   
	IIC_Wait_Ack();	
	
  Addr_Data=IIC_Read_Byte(0);	
  IIC_Stop();												 //产生一个停止条件	    
}
//
void IIC_ReadBytes(uint8_t DeviceAddr, char * Addr_Data, uint8_t len )
{
	u8 i=0;		  	    																 
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(*Addr_Data);   				//发送寄存器地址
	IIC_Wait_Ack();	
	
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr | 0x01);   //进入接收模式			   
	IIC_Wait_Ack();	
	
	for(i=0;i<len;i++)
	{
		*(Addr_Data+i)=IIC_Read_Byte(0);	
	}
  IIC_Stop();												 //产生一个停止条件	    
}
//
void IIC_WriteByte(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t RegData )
{				   	  	    																 
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(RegAddr);   					//发送寄存器地址
	IIC_Wait_Ack();	
 										  		   
	IIC_Send_Byte(RegData);     				//发送字节							   
	IIC_Wait_Ack(); 
	
  IIC_Stop();													//产生一个停止条件 
	HAL_Delay(10);
}
//
void IIC_WriteBytes(uint8_t DeviceAddr, uint8_t RegAddr, char *RegData , uint8_t len)
{	
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //写器件地址 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(RegAddr);   					//发送寄存器地址
	IIC_Wait_Ack();	
 	
  while(len--)
	{
		IIC_Send_Byte(*(RegData++));     	//发送字节							   
		IIC_Wait_Ack(); 
	}
	
  IIC_Stop();													//产生一个停止条件 
	HAL_Delay(10);
}
//
void IIC_DeviceReadBytes(uint8_t DeviceAddr, u16 ReadAddr, uint8_t *RegData, uint8_t len, uint8_t DeviceType )
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
void IIC_DeviceWriteBytes(uint8_t DeviceAddr, uint16_t WriteAddr, uint8_t  *RegData , uint8_t len, uint8_t DeviceType)
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
		//HAL_Delay(10);		
	}
	
  IIC_Stop();													//产生一个停止条件 
	//HAL_Delay(10);
	HAL_Delay(10);
}
//

