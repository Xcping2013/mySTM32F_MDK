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

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	IIC_SDA_OUT();     //sda�����
	GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);	  	  
	GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	delay_us(4);
 	GPIO_ResetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SDA_OUT();//sda�����
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
	GPIO_ResetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin); 
	GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_IN();      //SDA����Ϊ����  
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
	GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		IIC_SDA_OUT(); 	    
    GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7) GPIO_SetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);
				else 							GPIO_ResetBits(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin);
        txd<<=1; 	  
		delay_us(2);  //��TEA5767��������ʱ���Ǳ����
		GPIO_SetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);
		delay_us(2);
		GPIO_ResetBits(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin);	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
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
	delay_ms(10);
}
//




















