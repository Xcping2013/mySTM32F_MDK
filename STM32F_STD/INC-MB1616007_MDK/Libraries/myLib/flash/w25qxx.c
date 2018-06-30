#include "w25qxx.h" 
#include "delay.h"

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q32
//����Ϊ4M�ֽ�,����32��Block,1024��Sector 
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
W25QXX_DrvTypeDef w25qxx=
{
	W25Q32,
	w25qxx_init,
	w25qxx_read,
	w25qxx_fast_read,
	w25qxx_write_page,
	w25qxx_write,
	w25qxx_erase_sector,
	w25qxx_erase_chip,
	w25qxx_powerDown,
	w25qxx_wakeUP,
};
static uint16_t w25qxx_readID(SPI_CONFIG_TYPE* config)
{
	uint16_t Temp = 0;	  
	spi.readwrite(config,0x90,0);
	spi.readwrite(config,0x00,0);
	spi.readwrite(config,0x00,0);
	spi.readwrite(config,0x00,0);
	Temp |= spi.readwrite(config,0xff,0) << 8;
	Temp |= spi.readwrite(config,0xff,1);
	return Temp;
}
void w25qxx_init(SPI_CONFIG_TYPE* config)
{
	spi.init(config);
	w25qxx.id=w25qxx_readID(config);		//��ȡFLASH ID.  
}
static void w25qxx_cmd(SPI_CONFIG_TYPE* config,uint8_t cmd)
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
static uint8_t w25qxx_readSR(SPI_CONFIG_TYPE* config)
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
static void w25qxx_writeSR(SPI_CONFIG_TYPE* config,uint8_t sr)
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
void w25qxx_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read)
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
void w25qxx_fast_read(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t read_addr, uint16_t num_to_read)
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
void w25qxx_write_page(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
  w25qxx_cmd(config,W25X_WriteEnable);  						//SET WEL
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
	w25qxx_waitBusy(config);					   //�ȴ�д�����
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
void w25qxx_write_no_check(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
    uint16_t pageremain;
    pageremain = 256 - write_addr % 256; //��ҳʣ����ֽ���
    if(num_to_write <= pageremain)pageremain = num_to_write; //������256���ֽ�
    while(1)
    {
        w25qxx_write_page(config,buf, write_addr, pageremain);
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
u8 W25QXX_BUFFER[4096];		
void w25qxx_write(SPI_CONFIG_TYPE* config,uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
		u8 * W25QXX_BUF;	 
		W25QXX_BUF=W25QXX_BUFFER;	
    secpos = write_addr / 4096; //������ַ 0~511 for w25x16
    secoff = write_addr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С

    if(num_to_write <= secremain)secremain = num_to_write; //������4096���ֽ�
    while(1)
    {
        w25qxx_read(config,W25QXX_BUF, secpos * 4096, 4096); //������������������
        for(i = 0; i < secremain; i++) //У������
        {
            if(W25QXX_BUF[secoff + i] != 0XFF)break; //��Ҫ����
        }
        if(i < secremain) //��Ҫ����
        {
            w25qxx_erase_sector(config,secpos);//�����������
            for(i = 0; i < secremain; i++)	 //����
            {
                W25QXX_BUF[i + secoff] = buf[i];
            }
            w25qxx_write_no_check(config,W25QXX_BUF, secpos * 4096, 4096); //д����������

        }
        else w25qxx_write_no_check(config,buf, write_addr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
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
void w25qxx_erase_sector(SPI_CONFIG_TYPE* config,uint32_t dst_addr)
{
	dst_addr *= 4096;
  w25qxx_cmd(config,W25X_WriteEnable);  								//SET WEL
	w25qxx_waitBusy(config);
	spi.readwrite(config,W25X_SectorErase,0);	
	spi.readwrite(config,(uint8_t)((dst_addr) >> 16),0);	
	spi.readwrite(config,(uint8_t)((dst_addr) >> 8),0);	
	spi.readwrite(config,(uint8_t)dst_addr,1);	
	w25qxx_waitBusy(config);   				  							 //�ȴ��������
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
void w25qxx_erase_chip(SPI_CONFIG_TYPE* config)
{
	w25qxx_cmd(config,W25X_WriteEnable);  //SET WEL
	w25qxx_waitBusy(config);
	w25qxx_cmd(config,W25X_ChipErase); 	//����Ƭ��������             
	w25qxx_waitBusy(config);   				  //�ȴ�оƬ��������
}
//�ȴ�����
void w25qxx_waitBusy(SPI_CONFIG_TYPE* config)
{
    while ((w25qxx_readSR(config) & 0x01) == 0x01); // �ȴ�BUSYλ���
}
//�������ģʽ
void w25qxx_powerDown(SPI_CONFIG_TYPE* config)
{
	spi.readwrite(config,W25X_PowerDown,1);	
	delay_us(3);
}
//����
void w25qxx_wakeUP(SPI_CONFIG_TYPE* config)
{
	spi.readwrite(config,W25X_ReleasePowerDown,1);	//  send W25X_PowerDown command 0xAB
	delay_us(3);
}


