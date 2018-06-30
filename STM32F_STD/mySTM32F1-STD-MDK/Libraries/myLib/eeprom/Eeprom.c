/**
  \file Eeprom.c
  \version 1.00

  \brief EEPROM access functions

  This file contains EEPROM access functions.
*/
#include "eeprom.h"

void WriteEepromBlockII(UINT Address, UCHAR *Block, UINT Size);
//SCL�ڲ�������0

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
//����IIC��ʼ�ź�  		
static void IIC_Start(void)
{  	
	IIC_SDA_OUT();
  
	IIC_SDA=1;			//START:when CLK is high,DATA change form high to low 
	IIC_SCL=1;			//SDA=1	SCL=1						
	wait_us(1);
	
	IIC_SDA=0;			//SDA=0
	wait_us(1);
	
	IIC_SCL=0;			//׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
static void IIC_Stop(void)
{
	IIC_SDA_OUT();	
	
	IIC_SDA=0;			//STOP:when CLK is high DATA change form low to high
	IIC_SCL=1;			//SDA=0	SCL=1	
	wait_us(1);

	IIC_SDA=1;			//SDA=1
	wait_us(1);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
static u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA=1;
	IIC_SDA_IN(); wait_us(1);	//SDA����Ϊ����  	   
	
	IIC_SCL=1;		wait_us(1);	//�ȶ����ݣ�ǰ��Ĳ�����ʹDevice���Ӧ��
	
	while(IIC_READ_SDA)				//��ȡ����
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;						//ǯסI2C���ߣ�׼�����ͻ�������� ���� =0 SDA���ݲŻ�仯   
	return 0;  
} 
//����ACKӦ��
static void IIC_Ack(void)
{
	IIC_SCL=0;					//L
	
	IIC_SDA_OUT();
	IIC_SDA=0;					//D
	wait_us(1);
	
	IIC_SCL=1;					//H
	wait_us(1);
	
	IIC_SCL=0;					//ǯסI2C���ߣ�׼�����ͻ�������� ���� SCL=0 SDA���ݲŻ�仯
}
//������ACKӦ��		    
static void IIC_NAck(void)
{
	IIC_SCL=0;				//0
	
	IIC_SDA_OUT();
	IIC_SDA=1;				//
	
	wait_us(1);
	IIC_SCL=1;				//1
	
	wait_us(1);
	IIC_SCL=0;				 //ǯסI2C���ߣ�׼�����ͻ�������� ���� SCL=0 SDA���ݲŻ�仯
}	
//@@
//IIC����һ���ֽ�
//1����Ӧ��
//0����Ӧ��			  
static void IIC_Send_Byte(u8 txd)
{  
	//0-->1 Out	
	u8 t; 
	IIC_SDA_OUT(); 	  
	
	IIC_SCL=0;								//!> 0
	for(t=0;t<8;t++)				
	{              						
		IIC_SDA=(txd&0x80)>>7;	//!> ����
		txd<<=1; 	  
		wait_us(1);  						//����ĳЩdevice��������ʱ���Ǳ����
		
		IIC_SCL=1;							//!> 1			
		wait_us(1);
		
		IIC_SCL=0;							//!> 0					
		wait_us(1);
  }	 
} 	   
//@@
static u8 IIC_Read_Byte(unsigned char ack)
{
	//!  1> 0 ����һ������  ��һ���������ȶ���
	unsigned char i,receive=0;
	IIC_SDA=1; 		
	IIC_SDA_IN();										//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{	
		IIC_SCL=0;										//0		
		wait_us(1); 
		IIC_SCL=1;										//1
		receive<<=1;	
		if(IIC_READ_SDA)	receive++; 	//������
		wait_us(1);	
		
	}					 
	if (!ack)
			IIC_NAck();//����nACK
	else
			IIC_Ack(); //����ACK   
	return receive;
}
//@@

static void IIC_DeviceReadBytes(uint8_t DeviceAddr, u16 ReadAddr, uint8_t *RegData, uint8_t len, uint8_t DeviceType )
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
static void IIC_DeviceWriteBytes(uint8_t DeviceAddr, uint16_t WriteAddr, uint8_t  *RegData , uint8_t len, uint8_t DeviceType)
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
	}
	
  IIC_Stop();													//����һ��ֹͣ���� 
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
	IIC_Send_Byte(EEPROM_ADDR & 0xFE);   //д������ַ 	  
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);					 //���͸ߵ�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  				 //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		
	
	IIC_Send_Byte(Value);     					//�����ֽ�							   
	IIC_Wait_Ack(); 
	
	IIC_Stop();	
	wait_ms(10);
}


/*******************�Ѳ���OK*************************//**
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
  if(Address/64==0)			  //#��ҳ�׿�ʼд
  {
		if(Size<=64)			
		{									
													//##С��ҳ���ȣ�ֱ��д
			IIC_DeviceWriteBytes(0xA0,Address,Block,Size,2); 
		}
		else
		{
													//##����һҳ����
			for(i=0;i<(Size/64);i++)
			{				
													//##ҳд
				IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,64,2);
			}
													//##ʣ���ֽ�
			IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,Size%64,2);
		}
	}
	else										//#��ҳ�м俪ʼд
	{
		if((Size+Address%64)<=64)
		{											
													//##���ݳ���+ƫ�� С��ҳ���ȣ�ֱ��д
			IIC_DeviceWriteBytes(0xA0,Address,Block,Size,2); 
		}
		else
		{
			u8 StartPageSize	=	64-Address%64;
			u8 MiddlePageNum	=	(Size-StartPageSize)/64;
			u8 LastPageSize		=	(Size-StartPageSize)%64;

												 //##��д�걾ҳʣ��ռ�
			IIC_DeviceWriteBytes(0xA0,Address,Block,StartPageSize,2);  
												 //!> �´�д��ַ�����ݵ�ַ
			Address=(Address/64+1)*64;Block=Block+StartPageSize;
			for(i=0;i<MiddlePageNum;i++)
			{
												//##д��ҳ
				IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,64,2);
			}
												//##дʣ���ֽ�
			IIC_DeviceWriteBytes(0xA0,Address+64*i,Block+64*i,LastPageSize,2);
		}
	}
}
//
void WriteEepromBlockII(UINT Address, UCHAR *Block, UINT Size)
{
	IIC_Start(); 
	IIC_Send_Byte(EEPROM_ADDR & 0xFE);   //д������ַ 	  
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);					 //���͸ߵ�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  				 //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		
	
  while(Size--)
	{
		IIC_Send_Byte(*(Block++));     	//�����ֽ�							   
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
	IIC_Send_Byte(EEPROM_ADDR & 0xFE); //д������ַ 	 
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);				 //���͸ߵ�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  			 //���͵͵�ַ
	IIC_Wait_Ack(); 
	
  IIC_Start();  	 	   
	IIC_Send_Byte(EEPROM_ADDR | 0x01);    //�������ģʽ			   
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
	IIC_Send_Byte(EEPROM_ADDR & 0xFE); //д������ַ 	 
	IIC_Wait_Ack();
	
	IIC_Send_Byte(Address>>8);				 //���͸ߵ�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(Address%256);  			 //���͵͵�ַ
	IIC_Wait_Ack(); 
	
  IIC_Start();  	 	   
	IIC_Send_Byte(EEPROM_ADDR | 0x01);    //�������ģʽ			   
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






