/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"
#include "delay.h"
#include "crc.h"

//0-������DS18B20	-1---���ִ���	1---OK
int Ds18b20_reset(Gpio *pin)
{
    uint8_t retry = 0;   
		pinMode(pin, GPIO_Mode_Out_PP);
		pinReset(pin);
    delay_us(480);
		pinMode(pin, GPIO_Mode_IPU);
    delay_us(30);//15-60us����������60-240us��ʵ��30us����
    while(pinRead(pin))//�ȴ���������,60us��ʱ
    {
        delay_us(10);
        retry++;    
        if(retry > 6) return 0;//������DS18B20
    }
    retry = 0;
    while(!pinRead(pin))//�ȴ���������
    {
        delay_us(10);
        retry++;    
        if(retry > 24) return -1;//DS18B20δ�ͷ����ߣ����ִ���
    }    
    return 1;
}
void Ds18b20_write(Gpio *pin, uint8_t data)
{
    //���䷽ʽ��LSB
    uint8_t bit;
    no_interrupts();
    for(int i = 0; i < 8; i++)
    {
				pinMode(pin, GPIO_Mode_Out_PP);
        bit = data&0x01;
				pinReset(pin);
        delay_us(10);//�������ʱ��15us
        if(bit){
            pinSet(pin);//��������������
        }
        else{
            
        }
        delay_us(40);//
        pinMode(pin, GPIO_Mode_IPU);//�ͷ�����
        delay_us(1);//д��϶�ӳ�>1us
        data>>=1;
    }
    interrupts();
}

uint8_t Ds18b20_read(Gpio *pin)
{
    u8 data=0;
    no_interrupts();
    for(int i=0;i<8;i++)								
    { 
        data>>=1;
        pinMode(pin, GPIO_Mode_Out_PP);
        pinReset(pin);
        delay_us(1);//������������1us
        pinMode(pin, GPIO_Mode_IPU);//�ͷ�����
        delay_us(1);//������ʱ���𣬵�����������ܳ���15us
        if(pinRead(pin)) data |= 0x80;
        delay_us(45);//����ϵͳ��С��ȡʱ��60usҪ��
    }    
    interrupts();
    return data;
}
float Ds18b20_get_temp(Gpio *pin)
{
    u8 flag;
    u8 temp_l,temp_h;
    float temperature;
    uint8_t buf[9];
    
    Ds18b20_reset(pin);
    Ds18b20_write(pin, 0xcc);
    Ds18b20_write(pin, 0x44);
    
    Ds18b20_reset(pin);
    Ds18b20_write(pin, 0xcc);
    Ds18b20_write(pin, 0xbe);

    for(int i = 0;i < 9; i++)
    {
      buf[i] = Ds18b20_read(pin);			//���������ٻ���
    }
    
    if (crc8(buf, 9) == 0 )
    {
      temp_l = buf[0];						//�¶�����,11λ��Ч����(����0.125��)
      temp_h = buf[1];
      if(temp_h > 7)
      {
          temp_h =~ temp_h;
          temp_l =~ temp_l; 
          flag = 0;//�¶�Ϊ��  
      }
      else
          flag = 1;//�¶�Ϊ��
      
    temperature = ((temp_h << 8) | temp_l) * 0.0625; //��ø߰�λ
    if(flag)
      return temperature; //�����¶�ֵ
    else
      return -temperature;
    
    }
    else
      return 0;
}


