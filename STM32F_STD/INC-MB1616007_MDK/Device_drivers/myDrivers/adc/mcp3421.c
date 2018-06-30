#include "MCP3421.h"

#define MCP3421_DEVICE_ADDRESS      		0xd0		//1101AAA-			

float lsb = MCP3421_LSB_16BITS;
char gain = MCP3421_GAIN_8_VALUE;

//static uint8_t MCP3421_set[1]={0x0a};				//0 0 0 0	1 0	1	0

static uint8_t MCP3421_configReg(mcp3421_t *dev)
{
	return SoftI2c.writes(dev->pin,0,MCP3421_DEVICE_ADDRESS,0x00,&dev->config,1); 
}
/**
 * Sends a command to the MCP3421 to initiate a new convertion. One-Shot convertion
 * must be configured. In continuous mode this funtion as no effect on the ADC.
 */
void MCP3421StartConvertion(mcp3421_t *dev)
{
	u8	data;
	data=dev->config|0x80;
	SoftI2c.writes(dev->pin,0,MCP3421_DEVICE_ADDRESS,0x00,&data,1); 
}

/**
 * Sets the MCP3421 convertion mode to Continuous convertion mode.
 */
void MCP3421SetConvertionModeContinuous(mcp3421_t *dev)
{
	dev->config |=	MCP342X_MODE_CONTINUOUS	;
	MCP3421_configReg(dev);
}

/**
 * Sets the MCP3421 convertion mode to One-Shot mode.
 */
void MCP3421SetConvertionModeOneShot(mcp3421_t *dev)
{
	dev->config |=	MCP342X_MODE_ONESHOT	;
	MCP3421_configReg(dev);
}

/**
 * Sets the convertion rate on the MCP3421.
 * @param value Value to select the convertion rate.
 * @warning Must select one of the following values:
 * #MCP3421_SAMPLE_RATE_240_SPS ,
 * #MCP3421_SAMPLE_RATE_60_SPS , 
 * #MCP3421_SAMPLE_RATE_15_SPS and
 * #MCP3421_SAMPLE_RATE_3_75_SPS.
 */
void MCP3421SetConvertionRate(mcp3421_t *dev)
{
	switch (dev->rate)
	{
		case MCP3421_SAMPLE_RATE_240_SPS:
			lsb = MCP3421_LSB_12BITS;
			dev->config |=	0x00;
			break;

		case MCP3421_SAMPLE_RATE_60_SPS:
			lsb = MCP3421_LSB_14BITS;
			dev->config |=	0x04;
			break;

		case MCP3421_SAMPLE_RATE_15_SPS:
			lsb = MCP3421_LSB_16BITS;
			dev->config |=	0x08;
			break;

		case MCP3421_SAMPLE_RATE_3_75_SPS:
			lsb = MCP3421_LSB_18BITS;
			dev->config |=	0x0c;
			break;

		default:
			lsb = MCP3421_LSB_16BITS;
			dev->config |=	0x08;
			break;
	}
	MCP3421_configReg(dev);
}

/**
 * Sets the gain of the internal PGA.
 * @param value Value to select the gain internally.
 * @warning Must select one of the following values:
 * #MCP3421_GAIN_1,
 * #MCP3421_GAIN_2,
 * #MCP3421_GAIN_4 and
 * #MCP3421_GAIN_8.
 */
void MCP3421SetPGAGain(mcp3421_t *dev)
{
	switch (dev->gain)
	{
		case MCP3421_GAIN_1:
			gain = MCP3421_GAIN_1_VALUE;
			dev->config |=	0x00;
			break;

		case MCP3421_GAIN_2:
			gain = MCP3421_GAIN_2_VALUE;
			dev->config |=	0x01;
			break;

		case MCP3421_GAIN_4:
			gain = MCP3421_GAIN_4_VALUE;
			dev->config |=	0x02;
			break;

		case MCP3421_GAIN_8:
			gain = MCP3421_GAIN_8_VALUE;
			dev->config |=	0x03;
			break;

		default:
			gain = MCP3421_GAIN_1_VALUE;
			dev->config |=	0x03;
			break;
	}
	MCP3421_configReg(dev);
}

/**
 * This funtion gets the value from the ADC. The funtion as blocking properties,
 *  if the convertion is not completed the device is continuous asked if the
 * convertion is completed. When is completed the value in volts is returned.
 * @return The voltage on the terminals of the ADC.
 */
//#include <rtthread.h>
unsigned int MCP3421GetValue(mcp3421_t *dev)
{
	//float voltage = 0.0;
	//16BITS	
	u8 data[3];//,data2,regRead;

	do
	{
		SoftI2c.reads(dev->pin,0,MCP3421_DEVICE_ADDRESS,0x00,data,3);
	}
  while ((data[2] & 0x80) == 0x80);
//	rt_kprintf("\r\ndata1=%d\r\n",data1);
//	rt_kprintf("data2=%d\r\n",data2);
//	rt_kprintf("regRead=%d\r\n",regRead);
//	rt_kprintf("regSet=%d\r\n",MCP3421_set);
  //voltage = data.value * (lsb*1000 / gain);	
	//mV
	//先从ADC多次读取数字量取平均后 再把最后结构装换为电压值，这样数据误差小
	//voltage = (int)((data1<<8)+data2) * (lsb*1000.0 / gain);	//mV
	return (u16)((u16)data[0]<<8 | data[1]);	
  //return voltage;
}
