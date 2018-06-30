#include "IIC_SOFT.h"

void IIC_SDA_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = HT2201_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
};

void IIC_SDA_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = HT2201_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
};

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	IIC_SDA_OUT();     //sda�����
	HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_SET);	  	  
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_SET);
	delay_us(4);
 	HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SDA_OUT();//sda�����
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_SET); 
	HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_SET);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SDA_IN();      //SDA����Ϊ����  
	HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_SET);delay_us(1);	   
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_SET);delay_us(1);	 
	while(HAL_GPIO_ReadPin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);
	IIC_SDA_OUT();
	HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);
	IIC_SDA_OUT();
	HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		IIC_SDA_OUT(); 	    
    HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7) HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_SET);
				else 							HAL_GPIO_WritePin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin,GPIO_PIN_RESET);
        txd<<=1; 	  
		delay_us(2);  //��TEA5767��������ʱ���Ǳ����
		HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_SET);
		delay_us(2);
		HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET);	
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
        HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_RESET); 
        delay_us(2);
				HAL_GPIO_WritePin(HT2201_SCL_GPIO_Port,HT2201_SCL_Pin,GPIO_PIN_SET);
		
        receive<<=1;
        if(HAL_GPIO_ReadPin(HT2201_SDA_GPIO_Port,HT2201_SDA_Pin))receive++;   
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




















