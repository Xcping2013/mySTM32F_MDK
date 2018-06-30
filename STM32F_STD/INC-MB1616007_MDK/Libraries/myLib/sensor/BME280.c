#include "bme280.h" 
#include "delay.h"

bme280_DrvTypeDef w25qxx=
{
	W25Q32,
	bme280_init,
	bme280_read,
	bme280_fast_read,
	bme280_write_page,
	bme280_write,
	bme280_erase_sector,
	bme280_erase_chip,
	bme280_powerDown,
	bme280_wakeUP,
};
static uint16_t bme280_readID(SPI_CONFIG_TYPE* config)
{
	uint8_t chip_id;
	spi.readwrite(config,0xd0,0);
	chip_id=spi.readwrite(config,0x00,1);
	return chip_id;
}
void bme280_init(SPI_CONFIG_TYPE* config)
{
	char cmd[18];

	spi.init(config);
	w25qxx.id=bme280_readID(config);	
	
	spi.readwrite(config,0xf2 & BME280_SPI_MASK,0); // ctrl_hum
	spi.readwrite(config,0x04,1); 									// Humidity oversampling x4

  spi.readwrite(config,0xf4 & BME280_SPI_MASK,0); 					// ctrl_meas
  spi.readwrite(config,(4<<5)|(4<<2)|3,1); 				// Temparature oversampling x4, Pressure oversampling x4, Normal mode

  spi.readwrite(config,0xf5 & BME280_SPI_MASK,0); // config
  spi.readwrite(config,0xa0,1); 									// Standby 1000ms, Filter off, 4-wire SPI interface

  delay_ms(1000);

	spi.readwrite(config,0x88,1); // read dig_T regs
	for(int i = 0; i < 5; i++)
	{
			cmd[i] = spi.readwrite(config,0,0);
	}
	cmd[i] = spi.readwrite(config,0,1);
	dig_T1 = (cmd[1] << 8) | cmd[0];
	dig_T2 = (cmd[3] << 8) | cmd[2];
	dig_T3 = (cmd[5] << 8) | cmd[4];

    _cs = 0;
    spi.readwrite(0x8e); // read dig_P regs
    for(int i = 0; i < 18; i++)
        cmd[i] = spi.readwrite(0);
    _cs = 1;

    dig_P1 = (cmd[ 1] << 8) | cmd[ 0];
    dig_P2 = (cmd[ 3] << 8) | cmd[ 2];
    dig_P3 = (cmd[ 5] << 8) | cmd[ 4];
    dig_P4 = (cmd[ 7] << 8) | cmd[ 6];
    dig_P5 = (cmd[ 9] << 8) | cmd[ 8];
    dig_P6 = (cmd[11] << 8) | cmd[10];
    dig_P7 = (cmd[13] << 8) | cmd[12];
    dig_P8 = (cmd[15] << 8) | cmd[14];
    dig_P9 = (cmd[17] << 8) | cmd[16];

    DEBUG_PRINT("dig_P = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9);
    DEBUG_PRINT("dig_P = %d, %d, %d, %d, %d, %d, %d, %d, %d\n", dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9);

    _cs = 0;
    spi.readwrite(0xA1); // read dig_H1 reg
    cmd[0] = spi.readwrite(0);
    _cs = 1;

    _cs = 0;
    spi.readwrite(0xE1); // read dig_H regs
    for(int i = 0; i < 7; i++)
        cmd[1+i] = spi.readwrite(0);
    _cs = 1;

    dig_H1 = cmd[0];
    dig_H2 = (cmd[2] << 8) | cmd[1];
    dig_H3 = cmd[3];
    dig_H4 = (cmd[4] << 4) | (cmd[5] & 0x0f);
    dig_H5 = (cmd[6] << 4) | ((cmd[5]>>4) & 0x0f);
    dig_H6 = cmd[7];

    DEBUG_PRINT("dig_H = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dig_H1, dig_H2, dig_H3, dig_H4, dig_H5, dig_H6);
    DEBUG_PRINT("dig_H = %d, %d, %d, %d, 
	
}
static void bme280_cmd(SPI_CONFIG_TYPE* config,uint8_t cmd)
{
	spi.readwrite(config,cmd,1);	
}
/***************************************************************
函数名称 :  uint8_t SPI_Flash_ReadSR(void)
功能描述 : 读取W25X16的状态寄存器
				BIT7  6   5   4   3   2   1   0
				SPR   RV  TB BP2 BP1 BP0 WEL BUSY
				说明：
				SPR:默认0,状态寄存器保护位,配合WP使用
				TB,BP2,BP1,BP0:FLASH区域写保护设置
				WEL:写使能锁定
				BUSY:忙标记位(1,忙;0,空闲)
				默认:0x00
输入参数 : 无
输出参数 : 无
返回值   : byte	寄存器状态
***************************************************************/
static uint8_t bme280_readSR(SPI_CONFIG_TYPE* config)
{
	uint8_t byte = 0;
	spi.readwrite(config,W25X_ReadStatusReg,0);	 //发送读取状态寄存器命令
	byte = spi.readwrite(config,0xff,1);
	return byte;
}

/***************************************************************
函数名称 :  void SPI_FLASH_Write_SR(uint8_t sr)
功能描述 : 写SPI_FLASH状态寄存器， 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
输入参数 : sr:状态
输出参数 : 无
返回值   : 无
***************************************************************/
static void bme280_writeSR(SPI_CONFIG_TYPE* config,uint8_t sr)
{
	spi.readwrite(config,W25X_WriteStatusReg,0);	//发送写取状态寄存器命令
	spi.readwrite(config,sr,1);
}

/***************************************************************
函数名称 :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 :
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(24bit)
			NumByteToRead:要读取的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void bme280_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read)
{
	spi.readwrite(config,W25X_ReadData,0);									//发送读取命令
	spi.readwrite(config,(uint8_t)((read_addr) >> 16),0);		//发送24bit地址    
	spi.readwrite(config,(uint8_t)((read_addr) >> 8),0);	
	spi.readwrite(config,(uint8_t)read_addr,0);	
	while(num_to_read!=1)
  {
		num_to_read--;
    *buf++ = spi.readwrite(config,0xff,0);
  }
	*buf++ = spi.readwrite(config,0xff,1);
}
/***************************************************************
函数名称 :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 :
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(24bit)
			NumByteToRead:要读取的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void bme280_fast_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read)
{
	spi.readwrite(config,W25X_FastReadData,0);	
	spi.readwrite(config,(uint8_t)((read_addr) >> 16),0);	
	spi.readwrite(config,(uint8_t)((read_addr) >> 8),0);	
	spi.readwrite(config,(uint8_t)read_addr,0);	
	spi.readwrite(config,(uint8_t)0xff,0);	
	while(num_to_read!=1)
  {
		num_to_read--;
    *buf++ = spi.readwrite(config,0xff,0);
  }
	*buf++ = spi.readwrite(config,0xff,1);
}
/***************************************************************
函数名称 : void SPI_Flash_Write_Page(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 : SPI在一页(0~65535)内写入少于256个字节的数据， 在指定地址开始写入最大256字节的数据
输入参数 :
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
输出参数 : 无
返回值   : 无
0.6ms
***************************************************************/
void bme280_write_page(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
  bme280_cmd(config,W25X_WriteEnable);  						//SET WEL
	spi.readwrite(config,W25X_PageProgram,0);	
	spi.readwrite(config,(uint8_t)((write_addr) >> 16),0);	
	spi.readwrite(config,(uint8_t)((write_addr) >> 8),0);	
	spi.readwrite(config,(uint8_t)write_addr,0);	
	while(num_to_write!=1)
  {
		num_to_write--;
    spi.readwrite(config,*buf++,0);
  }
	spi.readwrite(config,*buf++,1);
	bme280_waitBusy(config);					   //等待写入结束
}
/***************************************************************
函数名称 : void SPI_Flash_Write_NoCheck(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 :  无检验写SPI FLASH，必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!具有自动换页功能，
			在指定地址开始写入指定长度的数据,但是要确保地址不越界!
输入参数 :
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void bme280_write_no_check(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
    uint16_t pageremain;
    pageremain = 256 - write_addr % 256; //单页剩余的字节数
    if(num_to_write <= pageremain)pageremain = num_to_write; //不大于256个字节
    while(1)
    {
        bme280_write_page(config,buf, write_addr, pageremain);
        if(num_to_write == pageremain)break; //写入结束了
        else //NumByteToWrite>pageremain
        {
            buf += pageremain;
            write_addr += pageremain;

            num_to_write -= pageremain;			 //减去已经写入了的字节数
            if(num_to_write > 256)pageremain = 256; //一次可以写入256个字节
            else pageremain = num_to_write; 	 //不够256个字节了
        }
    };
}

/***************************************************************
函数名称 : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 : 写SPI FLASH，在指定地址开始写入指定长度的数据，该函数带擦除操作!
输入参数 :
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
u8 bme280_BUFFER[4096];		
void bme280_write(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
		u8 * bme280_BUF;	 
		bme280_BUF=bme280_BUFFER;	
    secpos = write_addr / 4096; //扇区地址 0~511 for w25x16
    secoff = write_addr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小

    if(num_to_write <= secremain)secremain = num_to_write; //不大于4096个字节
    while(1)
    {
        bme280_read(config,bme280_BUF, secpos * 4096, 4096); //读出整个扇区的内容
        for(i = 0; i < secremain; i++) //校验数据
        {
            if(bme280_BUF[secoff + i] != 0XFF)break; //需要擦除
        }
        if(i < secremain) //需要擦除
        {
            bme280_erase_sector(config,secpos);//擦除这个扇区
            for(i = 0; i < secremain; i++)	 //复制
            {
                bme280_BUF[i + secoff] = buf[i];
            }
            bme280_write_no_check(config,bme280_BUF, secpos * 4096, 4096); //写入整个扇区

        }
        else bme280_write_no_check(config,buf, write_addr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        if(num_to_write == secremain)break; //写入结束了
        else//写入未结束
        {
            secpos++;//扇区地址增1
            secoff = 0; //偏移位置为0

            buf += secremain; //指针偏移
            write_addr += secremain; //写地址偏移
            num_to_write -= secremain;				//字节数递减
            if(num_to_write > 4096)secremain = 4096;	//下一个扇区还是写不完
            else secremain = num_to_write;			//下一个扇区可以写完了
        }
    }
}
/***************************************************************
函数名称 : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 : 擦除一个扇区，擦除一个山区的最少时间:150ms
输入参数 : Dst_Addr:扇区地址 0~511 for w25x16
输出参数 : 无
返回值   : 无
50ms
***************************************************************/
void bme280_erase_sector(SPI_CONFIG_TYPE* config,uint32_t dst_addr)
{
	dst_addr *= 4096;
  bme280_cmd(config,W25X_WriteEnable);  								//SET WEL
	bme280_waitBusy(config);
	spi.readwrite(config,W25X_SectorErase,0);	
	spi.readwrite(config,(uint8_t)((dst_addr) >> 16),0);	
	spi.readwrite(config,(uint8_t)((dst_addr) >> 8),0);	
	spi.readwrite(config,(uint8_t)dst_addr,1);	
	bme280_waitBusy(config);   				  							 //等待擦除完成
}
/***************************************************************
函数名称 : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 :  擦除整个芯片
			整片擦除时间:
			W25X16:25s
			W25X32:40s
			W25X64:40s
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void bme280_erase_chip(SPI_CONFIG_TYPE* config)
{
	bme280_cmd(config,W25X_WriteEnable);  //SET WEL
	bme280_waitBusy(config);
	bme280_cmd(config,W25X_ChipErase); 	//发送片擦除命令             
	bme280_waitBusy(config);   				  //等待芯片擦除结束
}
//等待空闲
void bme280_waitBusy(SPI_CONFIG_TYPE* config)
{
    while ((bme280_readSR(config) & 0x01) == 0x01); // 等待BUSY位清空
}
//进入掉电模式
void bme280_powerDown(SPI_CONFIG_TYPE* config)
{
	spi.readwrite(config,W25X_PowerDown,1);	
	delay_us(3);
}
//唤醒
void bme280_wakeUP(SPI_CONFIG_TYPE* config)
{
	spi.readwrite(config,W25X_ReleasePowerDown,1);	//  send W25X_PowerDown command 0xAB
	delay_us(3);
}


