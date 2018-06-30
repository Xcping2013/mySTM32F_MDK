#include "IIC_SOFT.h"
#include "main.h"

void IIC_SDA_IN(void)	//PA13
{
	GPIO_InitTypeDef	GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = HT2201_SDA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
};

void IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = HT2201_SDA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
};

//产生IIC起始信号
void IIC_Start(void)
{
	IIC_SDA_OUT();     //sda线输出
	GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);	  	  
	GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	delay_us(4);
 	GPIO_ResetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SDA_OUT();//sda线输出
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	GPIO_ResetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin); 
	GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_IN();      //SDA设置为输入  
	GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);delay_us(1);	   
	GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);delay_us(1);	 
	while(GPIO_ReadInputDataBit(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	IIC_SDA_OUT();
	GPIO_ResetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);
	delay_us(2);
	GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	delay_us(2);
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	IIC_SDA_OUT();
	GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);
	delay_us(2);
	GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	delay_us(2);
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		IIC_SDA_OUT(); 	    
    GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7) GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);
				else 							GPIO_ResetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);
        txd<<=1; 	  
		delay_us(2);  //对TEA5767这三个延时都是必须的
		GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
		delay_us(2);
		GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
        GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin); 
        delay_us(2);
				GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
		
        receive<<=1;
        if(GPIO_ReadInputDataBit(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin))	receive++;   
				delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
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
	delay_ms(10);
}
//




















