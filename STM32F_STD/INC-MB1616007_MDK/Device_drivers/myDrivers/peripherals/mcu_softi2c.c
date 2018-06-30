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

//IIC引脚电路未加上拉电阻值，需要设置为GPIO_Mode_Out_PP  有上拉时可设为GPIO_Mode_Out_OD
void SoftI2c_Init(IIC_GPIO pin)
{
    pinMode(pin.sda_pin,GPIO_Mode_Out_PP);	//GPIO_Mode_Out_OD
    pinMode(pin.scl_pin,GPIO_Mode_Out_PP);	//GPIO_Mode_Out_OD
}
/*						基本时序						 */
//产生IIC起始信号
void SoftI2c_start(IIC_GPIO pin)
{
	pinMode(pin.sda_pin,GPIO_Mode_Out_PP);	//sda线输出

  pinSet(pin.sda_pin)  ;
	pinSet(pin.scl_pin)  ;
  delay_us(4);
  pinReset(pin.sda_pin)  ;								//START:when CLK is high,DATA change form high to low 
  delay_us(4);
  pinReset(pin.scl_pin)  ;								//钳住I2C总线，准备发送或接收数据 
}
void SoftI2c_stop(IIC_GPIO pin)
{
    pinMode(pin.sda_pin,GPIO_Mode_Out_PP);//sda线输出
    pinReset(pin.sda_pin)  ;
		delay_us(1);
    pinSet(pin.scl_pin)  ;								//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    pinSet(pin.sda_pin)  ;								//发送I2C总线结束信号
    delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
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

//产生ACK应答
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
//产生NACK		
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
 * @brief 发送数据.
 *
 * @param[in] byte 将被发送的数据.
 *
 * @return 发送结果.返回0表示发送成功，返回1表示发送失败.
 */
void SoftI2c_send_byte(IIC_GPIO pin,uint8_t byte)
{
    //uint8_t ret = 0;
    u8 t;
    pinMode(pin.sda_pin,GPIO_Mode_Out_PP);
		pinReset(pin.scl_pin)  ;									//拉低时钟开始数据传输
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
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
				SoftI2c_send_no_ack(pin);//发送nACK
		else
				SoftI2c_send_ack(pin); 	//发送ACK  

		return byte;
}
/*						基本时序						 */

//地址1~2字节设备,多为大容量EEPROM,设备REG比较少,一字节
uint8_t SoftI2c_writes(IIC_GPIO pin, uint8_t DeviceType, uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t num_to_write)
{
  SoftI2c_start(pin);  
	switch(DeviceType)
	{
		case 0:	//寄存器只有一个，无需地址指明
						SoftI2c_send_byte(pin, slave_address & 0xFE);   //写器件地址 	  
						if(SoftI2c_wait_ack(pin)) return 1;
		case 1:	//一字节寄存器地址			
						SoftI2c_send_byte(pin, slave_address & 0xFE);   //写器件地址 	  
						if(SoftI2c_wait_ack(pin)) return 1;
						SoftI2c_send_byte(pin , reg_address%256);  		 	//发送低地址
						if(SoftI2c_wait_ack(pin)) return 2;
			break;
		case 2:	//两字节寄存器地址
						SoftI2c_send_byte(pin, slave_address & 0xFE);   //写器件地址 	  
						if(SoftI2c_wait_ack(pin)) return 1;
						SoftI2c_send_byte(pin, reg_address>>8);				//发送高地址
						if(SoftI2c_wait_ack(pin)) return 2;
						SoftI2c_send_byte(pin, reg_address%256);  		 //发送低地址
						if(SoftI2c_wait_ack(pin)) return 2;	 
			break;
		case 3:	//寄存器地址与器件地址复用
						SoftI2c_send_byte(pin, slave_address & 0xFE +((reg_address/256)<<1));   //发送器件地址0XA0,写数据 
						if(SoftI2c_wait_ack(pin)) return 1;
						SoftI2c_send_byte(pin, reg_address%256);   			 //发送低地址
						if(SoftI2c_wait_ack(pin)) return 2;
			break;
		default:
			break;		
	}
		
  while(num_to_write--)
	{
		SoftI2c_send_byte(pin, *(data++));     		//发送字节							   
		if(SoftI2c_wait_ack(pin)) return 3;
	}
  SoftI2c_stop(pin);													//产生一个停止条件 
	//delay_ms(10);
  return 0;
}


uint8_t 	SoftI2c_reads(IIC_GPIO pin, uint8_t DeviceType , uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t num_to_read)
{
	switch(DeviceType)
	{
		case 0:	//寄存器只有一个，无需地址指明
		case 1:	//一字节寄存器地址			
				SoftI2c_start(pin);
				SoftI2c_send_byte(pin, slave_address & 0xFE); 		//写器件地址 	  
				if(SoftI2c_wait_ack(pin)) return 1;
				SoftI2c_send_byte(pin, reg_address %256);				//发送低地址
				if(SoftI2c_wait_ack(pin)) return 2;
			break;
		case 2:	//两字节寄存器地址
					SoftI2c_start(pin);
					SoftI2c_send_byte(pin, slave_address & 0xFE);   //写器件地址 	  
					if(SoftI2c_wait_ack(pin)) return 1;
					SoftI2c_send_byte(pin, reg_address>>8);				 //发送高地址
					if(SoftI2c_wait_ack(pin)) return 2;
					SoftI2c_send_byte(pin, reg_address%256);  		 	//发送低地址
					if(SoftI2c_wait_ack(pin)) return 2;
			break;
		case 3:	//寄存器地址与器件地址复用
					SoftI2c_start(pin);
					SoftI2c_send_byte(pin, slave_address & 0xFE +((reg_address/256)<<1));   //发送器件地址0XA0,写数据 
					if(SoftI2c_wait_ack(pin)) return 1;
					SoftI2c_send_byte(pin, reg_address%256);   			 //发送低地址
					if(SoftI2c_wait_ack(pin)) return 2;
			break;
		default:
			break;	
		
	}
	SoftI2c_start(pin);
	SoftI2c_send_byte(pin, slave_address | 0x01);					//进入接收模式
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
