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
�������� :  uint8_t SPI_Flash_ReadSR(void)
�������� : ��ȡW25X16��״̬�Ĵ���
				BIT7  6   5   4   3   2   1   0
				SPR   RV  TB BP2 BP1 BP0 WEL BUSY
				˵����
				SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
				TB,BP2,BP1,BP0:FLASH����д��������
				WEL:дʹ������
				BUSY:æ���λ(1,æ;0,����)
				Ĭ��:0x00
������� : ��
������� : ��
����ֵ   : byte	�Ĵ���״̬
***************************************************************/
static uint8_t bme280_readSR(SPI_CONFIG_TYPE* config)
{
	uint8_t byte = 0;
	spi.readwrite(config,W25X_ReadStatusReg,0);	 //���Ͷ�ȡ״̬�Ĵ�������
	byte = spi.readwrite(config,0xff,1);
	return byte;
}

/***************************************************************
�������� :  void SPI_FLASH_Write_SR(uint8_t sr)
�������� : дSPI_FLASH״̬�Ĵ����� ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
������� : sr:״̬
������� : ��
����ֵ   : ��
***************************************************************/
static void bme280_writeSR(SPI_CONFIG_TYPE* config,uint8_t sr)
{
	spi.readwrite(config,W25X_WriteStatusReg,0);	//����дȡ״̬�Ĵ�������
	spi.readwrite(config,sr,1);
}

/***************************************************************
�������� :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
�������� : ��ȡSPI FLASH�� ��ָ����ַ��ʼ��ȡָ�����ȵ�����
������� :
			pBuffer:���ݴ洢��
			ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
			NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
������� : ��
����ֵ   : ��
***************************************************************/
void bme280_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read)
{
	spi.readwrite(config,W25X_ReadData,0);									//���Ͷ�ȡ����
	spi.readwrite(config,(uint8_t)((read_addr) >> 16),0);		//����24bit��ַ    
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
�������� :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
�������� : ��ȡSPI FLASH�� ��ָ����ַ��ʼ��ȡָ�����ȵ�����
������� :
			pBuffer:���ݴ洢��
			ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
			NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
������� : ��
����ֵ   : ��
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
�������� : void SPI_Flash_Write_Page(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
�������� : SPI��һҳ(0~65535)��д������256���ֽڵ����ݣ� ��ָ����ַ��ʼд�����256�ֽڵ�����
������� :
			pBuffer:���ݴ洢��
			WriteAddr:��ʼд��ĵ�ַ(24bit)
			NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
������� : ��
����ֵ   : ��
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
	bme280_waitBusy(config);					   //�ȴ�д�����
}
/***************************************************************
�������� : void SPI_Flash_Write_NoCheck(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
�������� :  �޼���дSPI FLASH������ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!�����Զ���ҳ���ܣ�
			��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
������� :
			pBuffer:���ݴ洢��
			WriteAddr:��ʼд��ĵ�ַ(24bit)
			NumByteToWrite:Ҫд����ֽ���(���65535)
������� : ��
����ֵ   : ��
***************************************************************/
void bme280_write_no_check(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
    uint16_t pageremain;
    pageremain = 256 - write_addr % 256; //��ҳʣ����ֽ���
    if(num_to_write <= pageremain)pageremain = num_to_write; //������256���ֽ�
    while(1)
    {
        bme280_write_page(config,buf, write_addr, pageremain);
        if(num_to_write == pageremain)break; //д�������
        else //NumByteToWrite>pageremain
        {
            buf += pageremain;
            write_addr += pageremain;

            num_to_write -= pageremain;			 //��ȥ�Ѿ�д���˵��ֽ���
            if(num_to_write > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = num_to_write; 	 //����256���ֽ���
        }
    };
}

/***************************************************************
�������� : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
�������� : дSPI FLASH����ָ����ַ��ʼд��ָ�����ȵ����ݣ��ú�������������!
������� :
			pBuffer:���ݴ洢��
			WriteAddr:��ʼд��ĵ�ַ(24bit)
			NumByteToWrite:Ҫд����ֽ���(���65535)
������� : ��
����ֵ   : ��
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
    secpos = write_addr / 4096; //������ַ 0~511 for w25x16
    secoff = write_addr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С

    if(num_to_write <= secremain)secremain = num_to_write; //������4096���ֽ�
    while(1)
    {
        bme280_read(config,bme280_BUF, secpos * 4096, 4096); //������������������
        for(i = 0; i < secremain; i++) //У������
        {
            if(bme280_BUF[secoff + i] != 0XFF)break; //��Ҫ����
        }
        if(i < secremain) //��Ҫ����
        {
            bme280_erase_sector(config,secpos);//�����������
            for(i = 0; i < secremain; i++)	 //����
            {
                bme280_BUF[i + secoff] = buf[i];
            }
            bme280_write_no_check(config,bme280_BUF, secpos * 4096, 4096); //д����������

        }
        else bme280_write_no_check(config,buf, write_addr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        if(num_to_write == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0

            buf += secremain; //ָ��ƫ��
            write_addr += secremain; //д��ַƫ��
            num_to_write -= secremain;				//�ֽ����ݼ�
            if(num_to_write > 4096)secremain = 4096;	//��һ����������д����
            else secremain = num_to_write;			//��һ����������д����
        }
    }
}
/***************************************************************
�������� : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
�������� : ����һ������������һ��ɽ��������ʱ��:150ms
������� : Dst_Addr:������ַ 0~511 for w25x16
������� : ��
����ֵ   : ��
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
	bme280_waitBusy(config);   				  							 //�ȴ��������
}
/***************************************************************
�������� : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
�������� :  ��������оƬ
			��Ƭ����ʱ��:
			W25X16:25s
			W25X32:40s
			W25X64:40s
������� : ��
������� : ��
����ֵ   : ��
***************************************************************/
void bme280_erase_chip(SPI_CONFIG_TYPE* config)
{
	bme280_cmd(config,W25X_WriteEnable);  //SET WEL
	bme280_waitBusy(config);
	bme280_cmd(config,W25X_ChipErase); 	//����Ƭ��������             
	bme280_waitBusy(config);   				  //�ȴ�оƬ��������
}
//�ȴ�����
void bme280_waitBusy(SPI_CONFIG_TYPE* config)
{
    while ((bme280_readSR(config) & 0x01) == 0x01); // �ȴ�BUSYλ���
}
//�������ģʽ
void bme280_powerDown(SPI_CONFIG_TYPE* config)
{
	spi.readwrite(config,W25X_PowerDown,1);	
	delay_us(3);
}
//����
void bme280_wakeUP(SPI_CONFIG_TYPE* config)
{
	spi.readwrite(config,W25X_ReleasePowerDown,1);	//  send W25X_PowerDown command 0xAB
	delay_us(3);
}


