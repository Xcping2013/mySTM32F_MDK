#include "mcu_spi.h"
#include "delay.h"

SOFT_SPI_DrvTypeDef	soft_spi=
{
	SoftSpi_init,
	SoftSpi_Readwrite,
	
	0,
	0,
	0,
	0,
};

void SoftSpi_init(SOFT_SPI_CONFIG_TYPE *spi_config)
{
		pinMode(spi_config->cs_pin, GPIO_Mode_Out_PP);
		pinSet(spi_config->cs_pin);
	
		pinMode(spi_config->sck_pin, GPIO_Mode_Out_PP);
		pinMode(spi_config->mosi_pin, GPIO_Mode_Out_PP);
		pinMode(spi_config->miso_pin, GPIO_Mode_IPU);

    switch(spi_config->mode)
    {
				case SPI_MODE0:
						pinReset(spi_config->sck_pin);
					break;
				case SPI_MODE1:
						pinReset(spi_config->sck_pin);
					break;
				case SPI_MODE2:
						pinSet(spi_config->sck_pin);
					break;
				case SPI_MODE3:
						pinSet(spi_config->sck_pin);
					break;
    }
}

uint8_t SoftSpi_transfer0(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //时钟空闲输出：0；
    //第一个是上升沿：读取数据；
    //第二个是下降沿：输出数据；
    for (i = 0; i < 8; i++)
    {
        if (spi_config->bit_order == SPI_BITODER_LSB)
        {
            RcvData |= pinRead(spi_config->miso_pin) << i;
            pinWrite(spi_config->mosi_pin, !!(data & (1 << i)));
        }
        else
        {
            RcvData |= (pinRead(spi_config->miso_pin) << (7 - i));
            pinWrite(spi_config->mosi_pin, !!(data & (1 << (7 - i))));
        }
        delay_us(1);
        pinSet(spi_config->sck_pin);
        delay_us(1);
        pinReset(spi_config->sck_pin);
    }
    return RcvData;
}
uint8_t SoftSpi_transfer1(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //时钟空闲输出：0；
    //第一个是上升沿：输出数据；
    //第二个是下降沿：读取数据；
    for (i = 0; i < 8; i++)
    {
        ///////////////////上升沿输出//////////
        if (spi_config->bit_order == SPI_BITODER_LSB)
        {
            pinWrite(spi_config->mosi_pin, !!(data & (1 << i)));
        }
        else
        {
            RcvData |= (pinRead(spi_config->miso_pin) << (7 - i));
            pinWrite(spi_config->mosi_pin, !!(data & (1 << (7 - i))));
        }
        delay_us(1);
        pinSet(spi_config->sck_pin);
        /////////////////下降沿采样////////////
        delay_us(1);
        pinReset(spi_config->sck_pin);
        if (spi_config->bit_order == LSB_FIRST)
        {
            pinWrite(spi_config->mosi_pin, !!(data & (1 << i)));
        }
        else
        {
            RcvData |= (pinRead(spi_config->miso_pin) << (7 - i));
            pinWrite(spi_config->mosi_pin, !!(data & (1 << (7 - i))));
        }
    }

    return RcvData;
}
uint8_t SoftSpi_transfer2(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //时钟空闲输出：1；
    //第一个是下降沿：读取数据；
    //第二个是上升沿：输出数据；
    for (i = 0; i < 8; i++)
    {
        pinReset(spi_config->sck_pin);
        delay_us(1);
        if (spi_config->bit_order == SPI_BITODER_LSB)
        {
            RcvData |= pinRead(spi_config->miso_pin) << i;
            pinWrite(spi_config->mosi_pin, !!(data & (1 << i)));
        }
        else
        {
            RcvData |= (pinRead(spi_config->miso_pin) << (7 - i));
            pinWrite(spi_config->mosi_pin, !!(data & (1 << (7 - i))));
        }
        delay_us(1);
        pinSet(spi_config->sck_pin);
    }

    return RcvData;
}
uint8_t SoftSpi_transfer3(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //时钟空闲输出：1；
    //第一个是下降沿：输出数据；
    //第二个是上升沿：读取数据；
    for (i = 0; i < 8; i++)
    {
        ///////////////////下降沿沿输出
        pinReset(spi_config->sck_pin);
        delay_us(1);
        if (spi_config->bit_order == SPI_BITODER_LSB)
        {
            pinWrite(spi_config->mosi_pin, !!(data & (1 << i)));
        }
        else
        {
            RcvData |= (pinRead(spi_config->miso_pin) << (7 - i));
            pinWrite(spi_config->mosi_pin, !!(data & (1 << (7 - i))));
        }
        /////////////////上升沿采样////////////
        pinSet(spi_config->sck_pin);
        delay_us(1);
        if (spi_config->bit_order == LSB_FIRST)
        {
            pinWrite(spi_config->mosi_pin, !!(data & (1 << i)));
        }
        else
        {
            RcvData |= (pinRead(spi_config->miso_pin) << (7 - i));
            pinWrite(spi_config->mosi_pin, !!(data & (1 << (7 - i))));
        }
    }

    return RcvData;
}
uint8_t SoftSpi_transfer(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data)
{
    uint8_t RcvData = 0 ;
    switch(spi_config->mode)
    {
			case SPI_MODE0:
					RcvData = SoftSpi_transfer0(spi_config, data);
					break;
			case SPI_MODE1:
					RcvData = SoftSpi_transfer1(spi_config, data);
					break;
			case SPI_MODE2:
					RcvData = SoftSpi_transfer2(spi_config, data);
					break;
			case SPI_MODE3:
					RcvData = SoftSpi_transfer3(spi_config, data);
					break;
			default :
					//return 0xff;
					break;
		}
    return RcvData;

}

void  SoftSpi_writeByte(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data)
{
	pinReset(spi_config->cs_pin);
  SoftSpi_transfer(spi_config, data);
	pinSet(spi_config->cs_pin);
}
void  SoftSpi_writeBytes(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t *data, uint16_t data_length)
{
	pinReset(spi_config->cs_pin);
	while(data_length--)
	{
			SoftSpi_transfer(spi_config, *data++);
	}
	pinSet(spi_config->cs_pin);
}

uint8_t SoftSpi_readByte(SOFT_SPI_CONFIG_TYPE *spi_config )
{
	u8 data;
	pinReset(spi_config->cs_pin);
  data=SoftSpi_transfer(spi_config, 0xff);
	pinSet(spi_config->cs_pin);
	return data;
}

void  SoftSpi_readBytes(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t *rcvdata, uint16_t data_length)
{
	pinReset(spi_config->cs_pin);
	while(data_length--)
	{
			*rcvdata++ = SoftSpi_transfer(spi_config, 0xff);
	}
	pinSet(spi_config->cs_pin);
}

uint8_t  SoftSpi_Readwrite(SOFT_SPI_CONFIG_TYPE *spi_config, uint8_t data, uint8_t LastTransfer)
{
	pinReset(spi_config->cs_pin);
	SoftSpi_writeByte(spi_config, data);
	data=SoftSpi_readByte(spi_config);
	
  if(LastTransfer) pinSet(spi_config->cs_pin);
  return data;
}



