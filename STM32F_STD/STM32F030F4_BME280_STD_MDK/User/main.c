#include "main.h"
#include "delay.h"

#include <string.h>
#include <stdlib.h>

#include "bme280.h"
#include "bme280_selftest.h"
#include "iic_soft.h"
/*************************************/
typedef struct __FILE FILE;
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART1,(uint8_t)  ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{}
 
  return (ch);
}

#define USART_REC_LEN	50
#define HT2201_ADDR		0xA0

RCC_ClocksTypeDef RCC_Clocks;

struct bme280_dev dev;
int8_t rslt = BME280_OK;

u8 USART_RX_BUF[USART_REC_LEN];  
u16 USART_RX_STA=0; 

//float Tdata=0;

union Tdata_u
{
	float 	fdata;
	uint8_t udata[4];
}Tdata;

void user_delay_ms(uint32_t period);
int8_t user_spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t user_spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

void print_sensor_data(struct bme280_data *comp_data);
int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev);
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);

void CommandProcess(void);

void IO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
  /* Configure PC8 and PC9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = HT2201_SDA_Pin | HT2201_SCL_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,	HT2201_SDA_Pin|HT2201_SCL_Pin);
	
  GPIO_InitStructure.GPIO_Pin = SYS_LED_Pin;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF,	SYS_LED_Pin);	
}
//
void SPI_BME280_Init(void)
{
	 
  GPIO_InitTypeDef  GPIO_InitStruct;
  SPI_InitTypeDef   SPI_InitStruct;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);	
	
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP; 
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = BME280_CS_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB,	BME280_CS_Pin);
	
	SPI_I2S_DeInit(SPI1);
	
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStruct);
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  SPI_Cmd(SPI1, ENABLE); /*!< SD_SPI enable */
  
}

//
void UART_Init(void)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
					
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );
					
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);        
	/*
	*  USART1_TX -> PA9 , USART1_RX ->        PA10
	*/                                
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);        

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;//设置串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//设置停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//设置效验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//设置工作模式
	USART_Init(USART1, &USART_InitStructure); //配置入结构体
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART1, ENABLE);//使能串口1

}			


//
/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	u8 Key;
	u32 BlinkDelay;
	
  RCC_GetClocksFreq(&RCC_Clocks);
	
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	//1ms
  
	IO_Init();
	SPI_BME280_Init();
	UART_Init();
  
	dev.dev_id = 0;
	dev.intf = BME280_SPI_INTF;
	dev.read = user_spi_read;
	dev.write = user_spi_write;
	dev.delay_ms = user_delay_ms;

	rslt = bme280_init(&dev);
	
	//bme280_crc_selftest(&dev);
	
	IIC_DeviceReadBytes(HT2201_ADDR,100,&Key,1,1);
	
	//printf("Key=%c\r\n",Key);
	if(Key=='Y')
	{
		IIC_DeviceReadBytes(HT2201_ADDR,10, &Tdata.udata[0],1,1);
		IIC_DeviceReadBytes(HT2201_ADDR,11, &Tdata.udata[1],1,1);
		IIC_DeviceReadBytes(HT2201_ADDR,12, &Tdata.udata[2],1,1);
		IIC_DeviceReadBytes(HT2201_ADDR,13, &Tdata.udata[3],1,1);
		printf("Tdata=%f\r\n",Tdata.fdata);	
	}
	else Tdata.fdata=0;
	
	BlinkDelay=GetSysTimer();
	
  while (1)
  {
		stream_sensor_data_normal_mode(&dev);
		//stream_sensor_data_forced_mode(&dev);
  	if(abs((int)(GetSysTimer()-BlinkDelay))>200)
  	{
  		GPIOF->ODR ^= 0x00000002;
  		BlinkDelay=GetSysTimer();
  	}	
  }
}

//
void user_delay_ms(uint32_t period)
{
    /*
     * Return control or wait,
     * for a period amount of milliseconds
     */
	delay_ms(period);
}
//
int8_t user_spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */
    /*
     * Data on the bus should be like
     * |----------------+---------------------+-------------|
     * | MOSI           | MISO                | Chip Select |
     * |----------------+---------------------|-------------|
     * | (don't care)   | (don't care)        | HIGH        |
     * | (reg_addr)     | (don't care)        | LOW         |
     * | (don't care)   | (reg_data[0])       | LOW         |
     * | (....)         | (....)              | LOW         |
     * | (don't care)   | (reg_data[len - 1]) | LOW         |
     * | (don't care)   | (don't care)        | HIGH        |
     * |----------------+---------------------|-------------|
     */
		uint8_t i;
		switch(dev_id)
		{
			case 0:		
								GPIO_ResetBits(BME280_CS_GPIO_Port, BME280_CS_Pin);
			
								while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
								SPI_SendData8(SPI1,	reg_addr);
								while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
								SPI_ReceiveData8(SPI1);			
		
								for(i=0;i<len;i++)
								{
									while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
									SPI_SendData8(SPI1,	reg_addr);
									while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
									reg_data[i]=SPI_ReceiveData8(SPI1);
									
								}
								GPIO_SetBits(BME280_CS_GPIO_Port, BME280_CS_Pin);

				break;
			default:
				break;		
		}		
    return rslt;
}
//
int8_t user_spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */

    /*
     * Data on the bus should be like
     * |---------------------+--------------+-------------|
     * | MOSI                | MISO         | Chip Select |
     * |---------------------+--------------|-------------|
     * | (don't care)        | (don't care) | HIGH        |
     * | (reg_addr)          | (don't care) | LOW         |
     * | (reg_data[0])       | (don't care) | LOW         |
     * | (....)              | (....)       | LOW         |
     * | (reg_data[len - 1]) | (don't care) | LOW         |
     * | (don't care)        | (don't care) | HIGH        |
     * |---------------------+--------------|-------------|
     */
		uint8_t i;
		switch(dev_id)
		{
			case 0:		
								GPIO_ResetBits(BME280_CS_GPIO_Port, BME280_CS_Pin);
								while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
								SPI_SendData8(SPI1,	reg_addr);
								while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
								SPI_ReceiveData8(SPI1);
								for(i=0;i<len;i++)
								{
									while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
									SPI_SendData8(SPI1,	reg_data[i]);
									while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
									SPI_ReceiveData8(SPI1);

								}
								GPIO_SetBits(BME280_CS_GPIO_Port, BME280_CS_Pin);

				break;
			default:
				break;		
		}		
    return rslt;
}
//
void print_sensor_data(struct bme280_data *comp_data)
{
	printf("%0.2f,%0.2f,%0.2f\r\n",comp_data->temperature-Tdata.fdata, comp_data->humidity, comp_data->pressure/1000);
}
//
int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev)
{
	int8_t rslt;
	uint8_t settings_sel;
	struct bme280_data comp_data;

	u32 BlinkDelay;
	/* Recommended mode of operation: Indoor navigation */
	dev->settings.osr_h = BME280_OVERSAMPLING_1X;				  //BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_OVERSAMPLING_16X;   				//BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;    			//BME280_OVERSAMPLING_2X;
	dev->settings.filter = BME280_FILTER_COEFF_16;   			//BME280_FILTER_COEFF_16
	dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;	//BME280_STANDBY_TIME_62_5_MS

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, dev);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

	printf("Temperature, Humidity, Pressure\r\n");
	BlinkDelay=GetSysTimer();
	
	while (1) 
	{
		/* Delay while the sensor completes a measurement */
		//CommandProcess();
		//dev->delay_ms(200);
		CommandProcess();
		if(abs((int)(GetSysTimer()-BlinkDelay))>250)
		{
			rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
			print_sensor_data(&comp_data);
			GPIOF->ODR ^= 0x00000002;
  		BlinkDelay=GetSysTimer();
		}
	}

	return rslt;
}
//
void CommandProcess(void)
{
	u8 len;
	if(USART_RX_STA&0x8000)									//接收到\r\n						
	{
		len=USART_RX_STA&0x3fff;							//得到此次接收到的数据长度
		USART_RX_BUF[len]='\0';								//在末尾加入结束符. 

		if(USART_RX_BUF[0]=='\0')	;

		else if(strncmp("Temperature=",	(char*)USART_RX_BUF,12)==0)	
		//else if(USART_RX_BUF[0]=='T' && USART_RX_BUF[1]=='=')	
		{		
			char *p = NULL;
			char *s = (char*)&USART_RX_BUF[12];			
			float Cal_T=strtod(s, &p);	
			//if(*p=='\0')									
			{	
				struct bme280_data comp_data;

				bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);				
				Tdata.fdata	=comp_data.temperature-Cal_T;

				//不能连续写
				//IIC_DeviceWriteBytes(HT2201_ADDR,10,	 (u8*)&Tdata,4,1);	
				IIC_DeviceWriteBytes(HT2201_ADDR,10,	 &Tdata.udata[0],1,1);	
				IIC_DeviceWriteBytes(HT2201_ADDR,11,	 &Tdata.udata[1],1,1);	
				IIC_DeviceWriteBytes(HT2201_ADDR,12,	 &Tdata.udata[2],1,1);	
				IIC_DeviceWriteBytes(HT2201_ADDR,13,	 &Tdata.udata[3],1,1);	
				
				IIC_DeviceWriteBytes(HT2201_ADDR,100,(u8*)"Y",1,1);
				printf("Get_T=%f\r\n",comp_data.temperature);
				printf("Cal_T=%f\r\n",Cal_T);
				printf("Tdata=%f\r\n",Tdata.fdata);
			}
		}							
		USART_RX_STA=0;
	}	
}
//
void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)
  {
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		Res = USART_ReceiveData(USART1);									//读取接收到的数据	
		//USARTx->DR = (Res & (uint16_t)0x01FF);
		if((USART_RX_STA&0x8000)==0)												//接收未完成
		{
			if(USART_RX_STA&0x4000)															//接收到了0x0d
			{
				if(Res!=0x0a)	USART_RX_STA=0;											//接收错误,重新开始
				else 					USART_RX_STA|=0x8000;												//接收完成了 
			}
			else 																								//还没收到0X0D
			{	
				if(Res==0x0d)	USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		} 
	}
}
//
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{
    int8_t rslt;
    uint8_t settings_sel;
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    rslt = bme280_set_sensor_settings(settings_sel, dev);

    printf("Temperature, Pressure, Humidity\r\n");
    /* Continuously stream sensor data */
    while (1) {
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        /* Wait for the measurement to complete and print data @25Hz */
        dev->delay_ms(500);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        print_sensor_data(&comp_data);
    }
    return rslt;
}
