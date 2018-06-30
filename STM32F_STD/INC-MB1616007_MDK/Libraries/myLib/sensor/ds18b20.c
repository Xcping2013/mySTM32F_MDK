/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"
#include "delay.h"
#include "crc.h"

//0-不存在DS18B20	-1---出现错误	1---OK
int Ds18b20_reset(Gpio *pin)
{
    uint8_t retry = 0;   
		pinMode(pin, GPIO_Mode_Out_PP);
		pinReset(pin);
    delay_us(480);
		pinMode(pin, GPIO_Mode_IPU);
    delay_us(30);//15-60us后，拉低总线60-240us（实测30us）。
    while(pinRead(pin))//等待总线拉低,60us后超时
    {
        delay_us(10);
        retry++;    
        if(retry > 6) return 0;//不存在DS18B20
    }
    retry = 0;
    while(!pinRead(pin))//等待总线拉高
    {
        delay_us(10);
        retry++;    
        if(retry > 24) return -1;//DS18B20未释放总线，出现错误
    }    
    return 1;
}
void Ds18b20_write(Gpio *pin, uint8_t data)
{
    //传输方式：LSB
    uint8_t bit;
    no_interrupts();
    for(int i = 0; i < 8; i++)
    {
				pinMode(pin, GPIO_Mode_Out_PP);
        bit = data&0x01;
				pinReset(pin);
        delay_us(10);//最大拉低时间15us
        if(bit){
            pinSet(pin);//电阻上拉起作用
        }
        else{
            
        }
        delay_us(40);//
        pinMode(pin, GPIO_Mode_IPU);//释放总线
        delay_us(1);//写间隙延迟>1us
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
        delay_us(1);//拉低总线至少1us
        pinMode(pin, GPIO_Mode_IPU);//释放总线
        delay_us(1);//从拉低时刻起，到读数据最大不能超过15us
        if(pinRead(pin)) data |= 0x80;
        delay_us(45);//满足系统最小读取时间60us要求
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
      buf[i] = Ds18b20_read(pin);			//连续读高速缓存
    }
    
    if (crc8(buf, 9) == 0 )
    {
      temp_l = buf[0];						//温度数据,11位有效数字(精度0.125度)
      temp_h = buf[1];
      if(temp_h > 7)
      {
          temp_h =~ temp_h;
          temp_l =~ temp_l; 
          flag = 0;//温度为负  
      }
      else
          flag = 1;//温度为正
      
    temperature = ((temp_h << 8) | temp_l) * 0.0625; //获得高八位
    if(flag)
      return temperature; //返回温度值
    else
      return -temperature;
    
    }
    else
      return 0;
}


