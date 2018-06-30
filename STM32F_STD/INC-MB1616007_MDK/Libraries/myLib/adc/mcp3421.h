#ifndef MCP3421_H
#define	MCP3421_H

#include "mcu_softi2c.h"
/*		//REF=2.048V	IN=+-2.048V	4.096V
REG		~RDY	C1	C0	~O/C	S1	S0	G1	G0
POR			1		0		0		1			0		0		0		0		
*/
typedef enum
{
	MCP3421_SAMPLE_RATE_240_SPS=0x00,
	MCP3421_SAMPLE_RATE_60_SPS,
	MCP3421_SAMPLE_RATE_15_SPS,
	MCP3421_SAMPLE_RATE_3_75_SPS
	
}mcp3421_sample_rate_t;
//			
typedef enum
{
	MCP3421_GAIN_1=0x00,
	MCP3421_GAIN_2,
	MCP3421_GAIN_4,
	MCP3421_GAIN_8
	
}mcp3421_gain_t;	

#define MCP3421_VALUE_IS_NOT_UPDATED    1						/**Identifies that output register is not updated with the lastest convertion.*/
//V
/**Value of the LSB for bits of resolution.*/		//2**(BIT-1) /2048 mV = 2**11
#define MCP3421_LSB_12BITS              (1E-3)
#define MCP3421_LSB_14BITS              (250E-6)
#define MCP3421_LSB_16BITS             	(62.5E-6)
#define MCP3421_LSB_18BITS         			(15.625E-6)

#define MCP3421_GAIN_1_VALUE            1
#define MCP3421_GAIN_2_VALUE            2
#define MCP3421_GAIN_4_VALUE            4
#define MCP3421_GAIN_8_VALUE            8

#define MCP342X_RDY							0x80
#define MCP342X_MODE_ONESHOT		0x00
#define MCP342X_MODE_CONTINUOUS	0x10


typedef struct
{
	IIC_GPIO	pin;
	
	mcp3421_sample_rate_t rate;
	mcp3421_gain_t gain;         /**< sensor range */
	uint8_t config;
			
} mcp3421_t;

void MCP3421StartConvertion(mcp3421_t *dev);
void MCP3421SetConvertionModeContinuous(mcp3421_t *dev);
void MCP3421SetConvertionModeOneShot(mcp3421_t *dev);
void MCP3421SetConvertionRate(mcp3421_t *dev);
void MCP3421SetPGAGain(mcp3421_t *dev);
unsigned int MCP3421GetValue(mcp3421_t *dev);
//float
#endif
