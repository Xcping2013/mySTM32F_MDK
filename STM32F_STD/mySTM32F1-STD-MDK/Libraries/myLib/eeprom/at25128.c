#include "at25128.h"
#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif

/***************************************************************//**
   \fn at25128_WriteByte(UINT Address, UCHAR Value)
   \brief Write a byte to the EEPROM
   \param Address  EEPROM location (0..16383)
   \param Value    Byte to be written

   This function writes a byte to the EEPROM at the specified
   EEPROM location.
********************************************************************/
void at25128_WriteByte(UINT Address, UCHAR Value)
{
  ReadWriteSPI2(SPI2_DEV_AT25128, WREN, TRUE);  	 //"Write Enable"
  do
  {
    ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
  } while((ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_EN)==0x00);  //is "Write Enable"

  ReadWriteSPI2(SPI2_DEV_AT25128, WRITE, FALSE); 	//"Write"
  ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
  ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);
  ReadWriteSPI2(SPI2_DEV_AT25128, Value, TRUE);

  do
  {
    ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
  } while(ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_BUSY);
}


/***************************************************************//**
   \fn at25128_WriteBlock(UINT Address, UCHAR *Block, UINT Size)
   \brief Copy memory block to EEPROM
   \param Address  EEPROM location (0..16383)
   \param Block  pointer at memory block to be copied to the EEPROM
   \param Size   size of block to be copied (bytes)

   This function copies a memory block to the EEPROM. It is capable
   of filling the entire EEPROM just whith one function call.
********************************************************************/
void at25128_WriteBlock(UINT Address, UCHAR *Block, UINT Size)
{
	UINT i;

	ReadWriteSPI2(SPI2_DEV_AT25128, WREN, TRUE);  		//"Write Enable"
	do
	{
		ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  	//"Get Status"
	} while((ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_EN)==0x00);  //is "Write Enable"

	ReadWriteSPI2(SPI2_DEV_AT25128, WRITE, FALSE); 		//"Write"
	ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
	ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);

	for(i=0; i<Size; i++)
	{
		Address++;
		ReadWriteSPI2(SPI2_DEV_AT25128, *(Block+i), (Address & 0x0000003f)==0 || i==Size-1);
		if((Address & 0x0000003f)==0 && i<Size-1)  
		{
			do
			{
				ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
			} while(ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_BUSY);

			ReadWriteSPI2(SPI2_DEV_AT25128, WREN, TRUE);  	//"Write Enable"
			do
			{
				ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
			} while((ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_EN)==0x00);  //is "Write Enable"

			ReadWriteSPI2(SPI2_DEV_AT25128, WRITE, FALSE); //"Write"
			ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
			ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);
		}
	}

	do
	{
		ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
	} while(ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_BUSY);

}

/***************************************************************//**
   \fn at25128_ReadByte(UINT Address)
   \brief Read a byte from the EEPROM
   \param Address  EEPROM location (0..16383)
   \return byte read from EEPROM

   This function reads one byte from the EEPROM.
********************************************************************/
UCHAR at25128_ReadByte(UINT Address)
{
  ReadWriteSPI2(SPI2_DEV_AT25128, READ, FALSE);  					//"Read"
  ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
  ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);

  return ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE);
}


/***************************************************************//**
   \fn at25128_ReadBlock(UINT Address, UCHAR *Block, UINT Size)
   \brief Copy block from EEPROM to RAM
   \param   Address  EEPROM start address (0..16383)
   \param   Block    RAM start address
   \param   Size     Length of block (bytes)

   Read a memory block from the EEPROM. This can also be the entire
   EEPROM.
********************************************************************/
void at25128_ReadBlock(UINT Address, UCHAR *Block, UINT Size)
{
  UINT i;

	ReadWriteSPI2(SPI2_DEV_AT25128, READ, FALSE);  //"Read"
	ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
	ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);

	for(i=0; i<Size; i++)
		*(Block+i)=ReadWriteSPI2(SPI2_DEV_AT25128, 0, i==Size-1);  //beim letzten Byte EEPROM deselektieren

}
//
void at25128_init(void)
{
	//rt_hw_spi1_init();
	rt_hw_spi2_init();
//	// initialize the eeprom
//	if (at25128_ReadByte(ADDR_EEPROM_MAGIC) != EEPROM_MAGIC )
//	{
//		at25128_WriteByte(ADDR_EEPROM_MAGIC, 0);

//		// overwrite module specific values (0..63)
//		at25128_WriteBlock(ADDR_MODULE_CONFIG, (UCHAR *)&ModuleConfig, SIZE_MODULE_CONFIG);
//		// overwrite first motor configuration (64..319)
//		at25128_WriteBlock(ADDR_MOTOR_CONFIG+0*SIZE_MOTOR_CONFIG, (UCHAR *)&MotorConfig[0], SIZE_MOTOR_CONFIG);
//		// overwrite second motor configuration (320..575)
//		at25128_WriteBlock(ADDR_MOTOR_CONFIG+1*SIZE_MOTOR_CONFIG, (UCHAR *)&MotorConfig[0], SIZE_MOTOR_CONFIG);
//		at25128_WriteBlock(ADDR_MOTOR_CONFIG+2*SIZE_MOTOR_CONFIG, (UCHAR *)&MotorConfig[0], SIZE_MOTOR_CONFIG);

//		// update magic byte
//		at25128_WriteByte(ADDR_EEPROM_MAGIC, EEPROM_MAGIC);
//	}

//	// read the default module configuration from EEPROM
//	at25128_ReadBlock(ADDR_MODULE_CONFIG, (UCHAR *)&ModuleConfig, SIZE_MODULE_CONFIG);

//	// read the default motor configurations from EEPROM
//	UINT i;
//	for (i = 0; i < N_O_MOTORS; i++)
//		at25128_ReadBlock(ADDR_MOTOR_CONFIG+i*SIZE_MOTOR_CONFIG, (UCHAR *)&MotorConfig[i], SIZE_MOTOR_CONFIG);
	//rt_kprintf("at25128 Init ok\n");
}

INIT_DEVICE_EXPORT(at25128_init);
#include <finsh.h>
static void at25128_Read(UINT Address, UINT Size)
{
	UINT i;
	UCHAR Block[101];
	UCHAR Temp;
	if(	( Address+Size )<= 16384 && Size<=100)
	{
		for(i=0; i<101; i++)
		{
			Block[i]='\0';
		}
		ReadWriteSPI2(SPI2_DEV_AT25128, READ, FALSE);  					
		ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
		ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);

		for(i=0; i<Size; i++)
		{
			Temp=ReadWriteSPI2(SPI2_DEV_AT25128, 0, i==Size-1); 
			if(Temp<=127) Block[i]=Temp;
//*(Block+i)=Temp; 		
//			rt_kprintf("ReadWriteSPI2=%.1s\n",Block[i]);
//			rt_kprintf("ReadWriteSPI2=%d\n",Block[i]);
		}
		
		rt_kprintf("[%.100s]@ReadInfo(%d,%d)\n",Block,Address,Size);
	}
	
}

static void at25128_Write(UINT Address, UCHAR *Datas)
{
	UINT i;
	UINT Size=strlen(Datas);	
	if(	( Address+Size )<= 16384 && Size<=100)
	{
		UCHAR Block[100]={'\0'};	
		for(i=0; i<Size; i++)
		{
			Block[i]=Datas[i];	
			Block[i+1]='\0';		
		}
		ReadWriteSPI2(SPI2_DEV_AT25128, WREN, TRUE);  		//"Write Enable"
		do
		{
			//rt_kprintf("WPEN_EN_1\n");

			ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  	//"Get Status"
		} while((ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_EN)==0x00);  //is "Write Enable"

		ReadWriteSPI2(SPI2_DEV_AT25128, WRITE, FALSE); 		//"Write"
		ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
		ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);

		for(i=0; i<Size; i++)
		{
			Address++;
			ReadWriteSPI2(SPI2_DEV_AT25128, *(Block+i), (Address & 0x0000003f)==0 || i==Size-1);
			if((Address & 0x0000003f)==0 && i<Size-1)  
			{
				do
				{
					//rt_kprintf("WPEN_BUSY_1\n");
					ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
				} while(ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_BUSY);

				ReadWriteSPI2(SPI2_DEV_AT25128, WREN, TRUE);  	//"Write Enable"
				do
				{
					//rt_kprintf("WPEN_EN_2\n");
					ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
				} while((ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_EN)==0x00);  //is "Write Enable"

				ReadWriteSPI2(SPI2_DEV_AT25128, WRITE, FALSE); //"Write"
				ReadWriteSPI2(SPI2_DEV_AT25128, Address >> 8, FALSE);
				ReadWriteSPI2(SPI2_DEV_AT25128, Address & 0xff, FALSE);
			}
		}

		do
		{
			//rt_kprintf("WPEN_BUSY_2\n");
			ReadWriteSPI2(SPI2_DEV_AT25128, RDSR, FALSE);  //"Get Status"
		} while(ReadWriteSPI2(SPI2_DEV_AT25128, 0x00, TRUE) & WPEN_BUSY);
		rt_kprintf("[OK]@SaveInfo\n");
	}
}
//FINSH_FUNCTION_EXPORT(at25128_WriteBlock,eeprom)
FINSH_FUNCTION_EXPORT_ALIAS(at25128_Write, SaveInfo, write datas to at25128)
FINSH_FUNCTION_EXPORT_ALIAS(at25128_Read, ReadInfo, read dataa from at25128)
//
