#include "mcu_softi2c.h"
#include "delay.h"

SoftI2c_DrvTypeDef	SoftI2c=
{
	SoftI2c_Init,
	SoftI2c_writes,
	SoftI2c_reads,	
};

void SoftI2c_start(IIC_GPIO pin);
void SoftI2c_stop(IIC_GPIO pin);
uint8_t SoftI2c_wait_ack(IIC_GPIO pin);
void SoftI2c_send_ack(IIC_GPIO pin);
void SoftI2c_send_no_ack(IIC_GPIO pin);
void SoftI2c_send_byte(IIC_GPIO pin,uint8_t byte);
uint8_t SoftI2c_read_byte(IIC_GPIO pin,uint8_t ack);

//IIC���ŵ�·δ����������ֵ����Ҫ����ΪGPIO_Mode_Out_PP  ������ʱ����ΪGPIO_Mode_Out_OD
void SoftI2c_Init(IIC_GPIO pin)
{
    pinMode(pin.sda_pin,GPIO_Mode_Out_PP);	//GPIO_Mode_Out_OD
    pinMode(pin.scl_pin,GPIO_Mode_Out_PP);	//GPIO_Mode_Out_OD
}
/*						����ʱ��						 */
//����IIC��ʼ�ź�
void SoftI2c_start(IIC_GPIO pin)
{
	pinMode(pin.sda_pin,GPIO_Mode_Out_PP);	//sda�����

  pinSet(pin.sda_pin)  ;
	pinSet(pin.scl_pin)  ;
  delay_us(4);
  pinReset(pin.sda_pin)  ;								//START:when CLK is high,DATA change form high to low 
  delay_us(4);
  pinReset(pin.scl_pin)  ;								//ǯסI2C���ߣ�׼�����ͻ�������� 
}
void SoftI2c_stop(IIC_GPIO pin)
{
    pinMode(pin.sda_pin,GPIO_Mode_Out_PP);//sda�����
    pinReset(pin.sda_pin)  ;
		delay_us(1);
    pinSet(pin.scl_pin)  ;								//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    pinSet(pin.sda_pin)  ;								//����I2C���߽����ź�
    delay_us(4);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t SoftI2c_wait_ack(IIC_GPIO pin)
{
	uint8_t ucErrTime=0;
	pinMode(pin.sda_pin,GPIO_Mode_IPU);
	delay_us(1);	
	pinSet(pin.scl_pin)  ;
	delay_us(1);
	while(pinRead(pin.sda_pin))
	{
			ucErrTime++;
			if(ucErrTime>250)
			{
					//pinMode(SoftI2c.sda_pin,GPIO_Mode_Out_PP);
					SoftI2c_stop(pin);
					return 1;

			}

	}
	pinReset(pin.scl_pin)  ;
	delay_us(1);
	return 0;
}

//����ACKӦ��
void SoftI2c_send_ack(IIC_GPIO pin)
{
	pinReset(pin.scl_pin)  ;							//L
	pinMode(pin.sda_pin,GPIO_Mode_Out_PP);
	pinReset(pin.sda_pin)  ;							//D
	delay_us(2);
	pinSet(pin.scl_pin)  ;								//H
	delay_us(2);
	pinReset(pin.scl_pin)  ;
	delay_us(2);
}
//����NACK		
void SoftI2c_send_no_ack(IIC_GPIO pin)
{
		pinReset(pin.scl_pin)  ;
    pinMode(pin.sda_pin,GPIO_Mode_Out_PP);
    pinSet(pin.sda_pin)  ;
    delay_us(2);
    pinSet(pin.scl_pin)  ;
    delay_us(2);
    pinReset(pin.scl_pin)  ;
    delay_us(1);
}
/**
 * @brief ��������.
 *
 * @param[in] byte �������͵�����.
 *
 * @return ���ͽ��.����0��ʾ���ͳɹ�������1��ʾ����ʧ��.
 */
void SoftI2c_send_byte(IIC_GPIO pin,uint8_t byte)
{
    //uint8_t ret = 0;
    u8 t;
    pinMode(pin.sda_pin,GPIO_Mode_Out_PP);
		pinReset(pin.scl_pin)  ;									//����ʱ�ӿ�ʼ���ݴ���
    delay_us(1);
		for(t=0;t<8;t++)	
    {
        pinWrite(pin.sda_pin , (byte & 0x80)>>7);
				byte<<=1;
        delay_us(1);

        pinSet(pin.scl_pin)  ;
        delay_us(2);
        pinReset(pin.scl_pin)  ;
        delay_us(1);
    }
//    ret = SoftI2c_wait_ack();
//    return ret;
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t SoftI2c_read_byte(IIC_GPIO pin,uint8_t ack)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    pinMode(pin.sda_pin,GPIO_Mode_IPU);
    for(i=0;i<8;i++ )
    {
				delay_us(2);
        pinSet(pin.scl_pin)  ;
				byte<<=1;	
				if(pinRead(pin.sda_pin))	byte++;
        delay_us(2);
				pinReset(pin.scl_pin)  ;
    }
		if (!ack)
				SoftI2c_send_no_ack(pin);//����nACK
		else
				SoftI2c_send_ack(pin); 	//����ACK  

		return byte;
}
/*						����ʱ��						 */

//��ַ1~2�ֽ��豸,��Ϊ������EEPROM,�豸REG�Ƚ���,һ�ֽ�
uint8_t SoftI2c_writes(IIC_GPIO pin, uint8_t DeviceType, uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t num_to_write)
{
  SoftI2c_start(pin);  
	switch(DeviceType)
	{
		case 0:	//�Ĵ���ֻ��һ���������ַָ��
						SoftI2c_send_byte(pin, slave_address & 0xFE);   //д������ַ 	  
						if(SoftI2c_wait_ack(pin)) return 1;
		case 1:	//һ�ֽڼĴ�����ַ			
						SoftI2c_send_byte(pin, slave_address & 0xFE);   //д������ַ 	  
						if(SoftI2c_wait_ack(pin)) return 1;
						SoftI2c_send_byte(pin , reg_address%256);  		 	//���͵͵�ַ
						if(SoftI2c_wait_ack(pin)) return 2;
			break;
		case 2:	//���ֽڼĴ�����ַ
						SoftI2c_send_byte(pin, slave_address & 0xFE);   //д������ַ 	  
						if(SoftI2c_wait_ack(pin)) return 1;
						SoftI2c_send_byte(pin, reg_address>>8);				//���͸ߵ�ַ
						if(SoftI2c_wait_ack(pin)) return 2;
						SoftI2c_send_byte(pin, reg_address%256);  		 //���͵͵�ַ
						if(SoftI2c_wait_ack(pin)) return 2;	 
			break;
		case 3:	//�Ĵ�����ַ��������ַ����
						SoftI2c_send_byte(pin, slave_address & 0xFE +((reg_address/256)<<1));   //����������ַ0XA0,д���� 
						if(SoftI2c_wait_ack(pin)) return 1;
						SoftI2c_send_byte(pin, reg_address%256);   			 //���͵͵�ַ
						if(SoftI2c_wait_ack(pin)) return 2;
			break;
		default:
			break;		
	}
		
  while(num_to_write--)
	{
		SoftI2c_send_byte(pin, *(data++));     		//�����ֽ�							   
		if(SoftI2c_wait_ack(pin)) return 3;
	}
  SoftI2c_stop(pin);													//����һ��ֹͣ���� 
	//delay_ms(10);
  return 0;
}


uint8_t 	SoftI2c_reads(IIC_GPIO pin, uint8_t DeviceType , uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t num_to_read)
{
	switch(DeviceType)
	{
		case 0:	//�Ĵ���ֻ��һ���������ַָ��
		case 1:	//һ�ֽڼĴ�����ַ			
				SoftI2c_start(pin);
				SoftI2c_send_byte(pin, slave_address & 0xFE); 		//д������ַ 	  
				if(SoftI2c_wait_ack(pin)) return 1;
				SoftI2c_send_byte(pin, reg_address %256);				//���͵͵�ַ
				if(SoftI2c_wait_ack(pin)) return 2;
			break;
		case 2:	//���ֽڼĴ�����ַ
					SoftI2c_start(pin);
					SoftI2c_send_byte(pin, slave_address & 0xFE);   //д������ַ 	  
					if(SoftI2c_wait_ack(pin)) return 1;
					SoftI2c_send_byte(pin, reg_address>>8);				 //���͸ߵ�ַ
					if(SoftI2c_wait_ack(pin)) return 2;
					SoftI2c_send_byte(pin, reg_address%256);  		 	//���͵͵�ַ
					if(SoftI2c_wait_ack(pin)) return 2;
			break;
		case 3:	//�Ĵ�����ַ��������ַ����
					SoftI2c_start(pin);
					SoftI2c_send_byte(pin, slave_address & 0xFE +((reg_address/256)<<1));   //����������ַ0XA0,д���� 
					if(SoftI2c_wait_ack(pin)) return 1;
					SoftI2c_send_byte(pin, reg_address%256);   			 //���͵͵�ַ
					if(SoftI2c_wait_ack(pin)) return 2;
			break;
		default:
			break;	
		
	}
	SoftI2c_start(pin);
	SoftI2c_send_byte(pin, slave_address | 0x01);					//�������ģʽ
	if(SoftI2c_wait_ack(pin)) return 3;

	while(num_to_read!=1)
	{
		num_to_read--;
		*data++ = SoftI2c_read_byte(pin, 1);
	}
	*data++ = SoftI2c_read_byte(pin, 0);
	SoftI2c_stop(pin);
	return 0;
}
//
