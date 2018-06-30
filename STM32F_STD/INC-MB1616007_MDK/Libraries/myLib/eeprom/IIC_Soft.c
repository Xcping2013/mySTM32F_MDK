#include "IIC_SOFT.h"
#include "delay.h"
#include "usart.h"
#include "gpio.h"
#include "ctype.h"

//SCL ������ʱ����0
//����IIC��ʼ�ź�  		SCL 1/n MHZ �ɰ������޸���ʱ
void IIC_Start(void)
{  	  
	IIC_SCL=0;
	
 	IIC_SDA_OUT();  //sda�����	
	IIC_SDA=1;			//START:when CLK is high,DATA change form high to low 
	IIC_SCL=1;
	delay_us(1);
	IIC_SDA=0;
	delay_us(1);
	IIC_SCL=0;			//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SCL=0;
	
	IIC_SDA_OUT();	//sda�����
	IIC_SCL=1;
	delay_us(1);
	IIC_SDA=0;			//STOP:when CLK is high DATA change form low to high
	delay_us(1);
	IIC_SDA=1;			//����I2C���߽����ź�	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_IN();      			//SDA����Ϊ����  	   
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
	//IIC_SCL=0;						//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;					//L
	IIC_SDA_OUT();
	IIC_SDA=0;					//D
	delay_us(1);				
	IIC_SCL=1;					//H
	delay_us(1);
	IIC_SCL=0;					//ǯסI2C���ߣ�׼�����ͻ��������
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT();
	IIC_SDA=1;
	delay_us(1);
	IIC_SCL=1;
	delay_us(1);
	IIC_SCL=0;				 //ǯסI2C���ߣ�׼�����ͻ��������
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t; 
	IIC_SCL=0;							//����ʱ�ӿ�ʼ���ݴ���  
	IIC_SDA_OUT(); 	    		

	for(t=0;t<8;t++)					//0-->1 ��ȥһ������
	{              						//!> 0
		IIC_SDA=(txd&0x80)>>7;	//!> ����
		txd<<=1; 	  
		delay_us(1);  					//����ĳЩdevice��������ʱ���Ǳ����
		IIC_SCL=1;							//!> 1			
		delay_us(1);
		IIC_SCL=0;							
		delay_us(1);
  }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();										//SDA����Ϊ����
	IIC_SCL=1;	
	for(i=0;i<8;i++ )
	{																//!> 1	
		delay_us(1);
		IIC_SCL=0;										//!> 0 ����һ������
		delay_us(1); 
		receive<<=1;	
		if(IIC_READ_SDA)	receive++; 	//������
		IIC_SCL=1;
	}					 
	if (!ack)
			IIC_NAck();//����nACK
	else
			IIC_Ack(); //����ACK   
	return receive;
}


//
void IIC_ReadByte(uint8_t DeviceAddr, uint8_t Addr_Data )
{	  	    																 
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(Addr_Data);   				//���ͼĴ�����ַ
	IIC_Wait_Ack();	
	
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr | 0x01);   //�������ģʽ			   
	IIC_Wait_Ack();	
	
  Addr_Data=IIC_Read_Byte(0);	
  IIC_Stop();												 //����һ��ֹͣ����	    
}
//
void IIC_ReadBytes(uint8_t DeviceAddr, char * Addr_Data, uint8_t len )
{
	u8 i=0;		  	    																 
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(*Addr_Data);   				//���ͼĴ�����ַ
	IIC_Wait_Ack();	
	
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr | 0x01);   //�������ģʽ			   
	IIC_Wait_Ack();	
	
	for(i=0;i<len;i++)
	{
		*(Addr_Data+i)=IIC_Read_Byte(0);	
	}
  IIC_Stop();												 //����һ��ֹͣ����	    
}
//
void IIC_WriteByte(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t RegData )
{				   	  	    																 
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(RegAddr);   					//���ͼĴ�����ַ
	IIC_Wait_Ack();	
 										  		   
	IIC_Send_Byte(RegData);     				//�����ֽ�							   
	IIC_Wait_Ack(); 
	
  IIC_Stop();													//����һ��ֹͣ���� 
	HAL_Delay(10);
}
//
void IIC_WriteBytes(uint8_t DeviceAddr, uint8_t RegAddr, char *RegData , uint8_t len)
{	
  IIC_Start();  
	IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	   
	IIC_Wait_Ack();
	
  IIC_Send_Byte(RegAddr);   					//���ͼĴ�����ַ
	IIC_Wait_Ack();	
 	
  while(len--)
	{
		IIC_Send_Byte(*(RegData++));     	//�����ֽ�							   
		IIC_Wait_Ack(); 
	}
	
  IIC_Stop();													//����һ��ֹͣ���� 
	HAL_Delay(10);
}
//
void IIC_DeviceReadBytes(uint8_t DeviceAddr, u16 ReadAddr, uint8_t *RegData, uint8_t len, uint8_t DeviceType )
{	  	    																 
  IIC_Start();  
	if(DeviceType==3)						//�Ĵ�����ַ��������ַ���ã���AT24C02~AT24C64
	{
		IIC_Send_Byte(DeviceAddr & 0xFE +((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 
		IIC_Wait_Ack();	
	  IIC_Send_Byte(ReadAddr%256);   			 //���͵͵�ַ
	  IIC_Wait_Ack(); 
	}	
	else if(DeviceType==2)		//���ֽڼĴ�����ַ��	       ��AT24C125~AT24C512
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);				 //���͸ߵ�ַ
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr%256);  		 //���͵͵�ַ
		IIC_Wait_Ack(); 	 										  		
	}	
	else if(DeviceType==1)	//һ�ֽڼĴ�����ַ						
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr%256);  		  //���͵͵�ַ
		IIC_Wait_Ack(); 	 										  		
	}	
	
	IIC_Start();  	 	   
	IIC_Send_Byte(DeviceAddr | 0x01);    //�������ģʽ			   
	IIC_Wait_Ack();	
	
	while(len!=1)
	{
		len--;
		*(RegData++)=IIC_Read_Byte(1);	
	}
	*(RegData++)=IIC_Read_Byte(0);
  IIC_Stop();												 //����һ��ֹͣ����	    
}
//
void IIC_DeviceWriteBytes(uint8_t DeviceAddr, uint16_t WriteAddr, uint8_t  *RegData , uint8_t len, uint8_t DeviceType)
{	
  IIC_Start();  
	if(DeviceType==3)
	{
		IIC_Send_Byte(DeviceAddr & 0xFE +((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
		IIC_Wait_Ack();	
	  IIC_Send_Byte(WriteAddr%256);   			 //���͵͵�ַ
	  IIC_Wait_Ack(); 
	}	
	else if(DeviceType==2)
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);				//���͸ߵ�ַ
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr%256);  		 //���͵͵�ַ
		IIC_Wait_Ack(); 	 										  		
	}	
	else if(DeviceType==1)
	{
		IIC_Send_Byte(DeviceAddr & 0xFE);   //д������ַ 	  
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr%256);  		 //���͵͵�ַ
		IIC_Wait_Ack(); 	 										  		
	}	
 	
  while(len--)
	{
		IIC_Send_Byte(*(RegData++));     	//�����ֽ�							   
		IIC_Wait_Ack();
		//HAL_Delay(10);		
	}
	
  IIC_Stop();													//����һ��ֹͣ���� 
	//HAL_Delay(10);
	HAL_Delay(10);
}
//

