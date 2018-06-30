
#include "TMC429.h"

tmc429_t 				tmc429_My=
{
	{
		&PA4,
		SPI2,
		SPI_MODE3,
		SPI_BaudRatePrescaler_128,
		SPI_FirstBit_MSB		
	},
	
	{0,0,0},
	
	{ 
		210,   //!< VMax
		30,    //!< AMax
		3,     //!< Pulsediv
		5,     //!< Rampdiv
		
		1,		 //FlagINVDir
		1,		 //FlagINVRef
		 
		0,	 	 //XTARGET 
		0,	 	 //XACTUAL	  
		0,	   //VTARGET 
		0,		 //VACTUAL 
		 
		210,	 //HomeV1
		210,	 //HomeV2
		1000,		//POSITION_REF

		1,		 //HomeMode
		1,		 //isStop
		1		   //isReach
	 },
};

static void ReadWrite429(tmc429_t dev, 		 UCHAR *Read, 	UCHAR *Write);
static void Write429Bytes(tmc429_t dev, 	 UCHAR Address, UCHAR *Bytes);
static void Write429Datagram(tmc429_t dev, UCHAR Address, UCHAR HighByte, UCHAR MidByte, UCHAR LowByte);
static void Write429Zero(tmc429_t dev, UCHAR Address);
static void Write429Short(tmc429_t dev, UCHAR Address, int Value);
static void Write429Int(tmc429_t dev, UCHAR Address, int Value); 

static UCHAR Read429Status(tmc429_t dev);
static UCHAR Read429Bytes(tmc429_t dev, UCHAR Address, UCHAR *Bytes);
static UCHAR Read429SingleByte(tmc429_t dev, UCHAR Address, UCHAR Index);
static int Read429Short(tmc429_t dev, UCHAR Address);
static int Read429Int(tmc429_t dev, UCHAR Address);

static void Set429RampMode(tmc429_t dev, UCHAR Axis, UCHAR RampMode);
static void Set429SwitchMode(tmc429_t dev, UCHAR Axis, UCHAR SwitchMode);
static UCHAR SetAMax(tmc429_t dev, UCHAR Motor, UINT AMax);
static void HardStop(tmc429_t dev, UINT Motor);

static void Init429(tmc429_t dev);

static void TMC429_Rotate(tmc429_t dev, u8 motor,int value);
static void TMC429_Stop(tmc429_t dev, u8 motor);
static void TMC429_Position(tmc429_t dev, u8 motor,int value,u8 mode);
static void SetAxisParameter(tmc429_t dev, uint8_t Motor, uint8_t type, int Value);
static int GetAxisParameter(tmc429_t dev, uint8_t Motor, uint8_t type);

TM429_DrvTypeDef TMC429=
{
	Init429,
	TMC429_Rotate,
	TMC429_Stop,
	TMC429_Position,
	SetAxisParameter,
	GetAxisParameter,
};

/***********************WRITE****************************************//**
   \fn ReadWrite429(dev, UCHAR *Read, UCHAR *Write)
   \brief 32 bit SPI communication with TMC429
   \param Read   four byte array holding the data read from the TMC428
   \param Write  four byte array holding the data to write to the TMC428

   This is the low-level function that does all SPI communication with
   the TMC429. It sends a 32 bit SPI telegramme to the TMC429 and
   receives the 32 bit answer telegramme from the TMC429.
   It also raises the motor current when there is write access to
   a register that could cause motor movement.
********************************************************************/
static void ReadWrite429(tmc429_t dev, UCHAR *Read, UCHAR *Write)
{
  Read[0]=spi.readwrite(&dev.spi_set, Write[0], FALSE);
  Read[1]=spi.readwrite(&dev.spi_set, Write[1], FALSE);
  Read[2]=spi.readwrite(&dev.spi_set, Write[2], FALSE);
  Read[3]=spi.readwrite(&dev.spi_set, Write[3], TRUE);
}


/***************************************************************//**
   \fn Write429Bytes(UCHAR Address, UCHAR *Bytes)
   \brief TMC429 write access
   \param Address  TMC429 register address (see also TMC429.h)
   \param Bytes  Array holding three bytes to be written to the
                 TMC429 register.

   This function writes an array of  three bytes to a TMC429 register.
********************************************************************/
static void Write429Bytes(tmc429_t dev, UCHAR Address, UCHAR *Bytes)
{
  UCHAR Write[4], Read[4];

  Write[0]=Address;
  Write[1]=Bytes[0];
  Write[2]=Bytes[1];
  Write[3]=Bytes[2];
  ReadWrite429(dev, Read, Write);
}


/***************************************************************//**
   \fn Write429Datagram(UCHAR Address, UCHAR HighByte, UCHAR MidByte, UCHAR LowByte)
   \brief TMC429 write access
   \param Address   TMC429 register address
   \param HighByte  MSB of the TMC429 register
   \param MidByte   mid byte of the TMC429 register
   \param LowByte   LSB of the TMC429 register

   This function write three bytes to a TMC429 register.
********************************************************************/
static void Write429Datagram(tmc429_t dev, UCHAR Address, UCHAR HighByte, UCHAR MidByte, UCHAR LowByte)
{
  UCHAR Write[4], Read[4];

  Write[0]=Address;
  Write[1]=HighByte;
  Write[2]=MidByte;
  Write[3]=LowByte;
  ReadWrite429(dev, Read, Write);
}


/***************************************************************//**
   \fn Write429Zero(UCHAR Address)
   \brief Write zero to a TMC429 register
   \param Address  TMC429 register address

   This function sets a TMC429 register to zero. This can be useful
   e.g. to stop a motor quickly.
********************************************************************/
static void Write429Zero(tmc429_t dev, UCHAR Address)
{
  UCHAR Write[4], Read[4];

  Write[0]=Address;
  Write[1]=0;
  Write[2]=0;
  Write[3]=0;

  ReadWrite429(dev, Read, Write);
}


/***************************************************************//**
   \fn Write429Short(UCHAR Address, int Value)
   \brief Write 16 bit value to a TMC429 register
   \param Address  TMC429 register address
   \param Value    Value to be written

   This function writes a 16 bit value to a TMC429 register.
********************************************************************/
static void Write429Short(tmc429_t dev, UCHAR Address, int Value)
{
  UCHAR Write[4], Read[4];

  Write[0]=Address;
  Write[1]=0;
  Write[2]=Value >> 8;
  Write[3]=Value & 0xff;

  ReadWrite429(dev, Read, Write);
}


/***************************************************************//**
   \fn Write429Int(UCHAR Address, int Value)
   \brief Write 24 bit value to a TMC429 register
   \param Address  TMC429 register address
   \param Value    Value to be written

   This function writes a 24 bit value to a TMC429 register.
********************************************************************/
static void Write429Int(tmc429_t dev, UCHAR Address, int Value)
{
  UCHAR Write[4], Read[4];

  Write[0]=Address;
  Write[1]=Value >> 16;
  Write[2]=Value >> 8;
  Write[3]=Value & 0xff;

  ReadWrite429(dev, Read, Write);
}


/**************************READ*************************************//**
   \fn Read429Status
   \brief Read TMC429 status byte

   \return TMC429 status byte

   This functions reads just the status byte of the TMC429 using
   a single byte SPI access which makes this a little bit faster.
********************************************************************/
static UCHAR Read429Status(tmc429_t dev)
{
	return spi.readwrite(&dev.spi_set , 0x01, TRUE);
}


/***************************************************************//**
   \fn Read429Bytes(UCHAR Address, UCHAR *Bytes)
   \brief Read TMC429 register
   \param Address  TMC429 register address (see TMC429.h)
   \param Bytes  Pointer at array of three bytes

   \return TMC429 status byte

   This functions reads a TMC429 register and puts the result into
   an array of bytes. It also returns the TMC429 status bytes.
********************************************************************/
static UCHAR Read429Bytes(tmc429_t dev, UCHAR Address, UCHAR *Bytes)
{
  UCHAR Read[4], Write[4];

  Write[0]=Address|TMC429_READ;
  ReadWrite429(dev, Read, Write);

  Bytes[0]=Read[1];
  Bytes[1]=Read[2];
  Bytes[2]=Read[3];

  return Read[0];
}


/***************************************************************//**
   \fn Read429SingleByte(UCHAR Address, UCHAR Index)
   \brief Read TMC429 register
   \param Address  TMC429 register address (see TMC429.h)
   \param Index  TMC429 register byte to be returned (0..3)

   \return TM429 register byte

   This functions reads a TMC429 register and returns the desired
   byte of that register.
********************************************************************/
static UCHAR Read429SingleByte(tmc429_t dev, UCHAR Address, UCHAR Index)
{
  UCHAR Read[4], Write[4];

  Write[0]=Address|TMC429_READ;
  ReadWrite429(dev, Read, Write);

  //return Read[Index+1];
	return Read[Index];
}


/***************************************************************//**
   \fn Read429Short(UCHAR Address)
   \brief Read TMC429 register (12 bit)
   \param Address  TMC429 register address (see TMC429.h)

   \return TMC429 register value (sign extended)

   This functions reads a TMC429 12 bit register and sign-extends the
   register value to 32 bit.
********************************************************************/
static int Read429Short(tmc429_t dev, UCHAR Address)
{
  UCHAR Read[4], Write[4];
  int Result;

  Write[0]=Address|TMC429_READ;
  ReadWrite429(dev, Read, Write);

  Result=(Read[2]<<8)|(Read[3]);
  if(Result & 0x00000800) Result|=0xfffff000;

  return Result;
}


/***************************************************************//**
   \fn Read429Int(UCHAR Address)
   \brief Read TMC429 register (24 bit)
   \param Address  TMC429 register address (see TMC429.h)

   \return TMC429 register value (sign extended)

   This functions reads a TMC429 24 bit register and sign-extends the
   register value to 32 bit.
********************************************************************/
static int Read429Int(tmc429_t dev, UCHAR Address)
{
  UCHAR Read[4], Write[4];
  int Result;

  Write[0]=Address|TMC429_READ;
  ReadWrite429(dev, Read, Write);

  Result=(Read[1]<<16)|(Read[2]<<8)|(Read[3]);
  if(Result & 0x00800000) Result|=0xff000000;

  return Result;
}


/****************************SET***********************************//**
   \fn Set429RampMode(UCHAR Axis, UCHAR RampMode)
   \brief Set the ramping mode of an axis
   \param  Axis  Motor number (0, 1 or 2)
   \param  RampMode  ramping mode (RM_RAMP/RM_SOFT/RM_VELOCITY/RM_HOLD)

   This functions changes the ramping mode of a motor in the TMC429.
   It is some TMC429 register bit twiddling.
********************************************************************/
static void Set429RampMode(tmc429_t dev, UCHAR Axis, UCHAR RampMode)
{
  UCHAR Write[4], Read[4];

  Write[0] = MOTOR_NUMBER(Axis)<<5|IDX_REFCONF_RM|TMC429_READ;
  ReadWrite429(dev, Read, Write);

  Write[0] = MOTOR_NUMBER(Axis)<<5|IDX_REFCONF_RM;
  Write[1] = Read[1];
  Write[2] = Read[2];
  Write[3] = RampMode;
  ReadWrite429(dev, Read, Write);
}


/***************************************************************//**
   \fn Set429SwitchMode(UCHAR Axis, UCHAR SwitchMode)
   \brief Set the end switch mode
   \param  Axis  Motor number (0, 1 or 2)
   \param  SwitchMode  end switch mode

   This functions changes the end switch mode of a motor in the TMC429.
   It is some TMC429 register bit twiddling.
********************************************************************/
static void Set429SwitchMode(tmc429_t dev, UCHAR Axis, UCHAR SwitchMode)
{
  UCHAR Write[4], Read[4];

  Write[0] = MOTOR_NUMBER(Axis)<<5|IDX_REFCONF_RM|TMC429_READ;
  ReadWrite429(dev, Read, Write);

  Write[0] = MOTOR_NUMBER(Axis)<<5|IDX_REFCONF_RM;
  Write[1] = Read[1];
  Write[2] = SwitchMode;
  Write[3] = Read[3];
  ReadWrite429(dev, Read, Write);
}

/***************************************************************//**
   \fn SetAMax(UCHAR Motor, UINT AMax)
   \brief Set the maximum acceleration
   \param Motor  motor number (0, 1, 2)
   \param AMax: maximum acceleration (1..2047)

   This function sets the maximum acceleration and also calculates
   the PMUL and PDIV value according to all other parameters
   (please see the TMC429 data sheet for more info about PMUL and PDIV
   values).
********************************************************************/
static UCHAR SetAMax(tmc429_t dev, UCHAR Motor, UINT AMax)
{
  int pdiv, pmul, pm, pd;
  float p, p_reduced;
  int ramp_div;
  int pulse_div;
  UCHAR PulseRampDiv;
  UCHAR Data[3];

  AMax&=0x000007ff;
  Read429Bytes(dev, IDX_PULSEDIV_RAMPDIV|MOTOR_NUMBER(Motor)<<5, Data);
  PulseRampDiv=Data[1];
  pulse_div=PulseRampDiv>>4;
  ramp_div=PulseRampDiv & 0x0f;

  pm=-1; pd=-1; // -1 indicates : no valid pair found

  if(ramp_div>=pulse_div)
    p = AMax / ( 128.0 * (1<<(ramp_div-pulse_div)));  //Exponent positive or 0
  else
    p = AMax / ( 128.0 / (1<<(pulse_div-ramp_div)));  //Exponent negative

  p_reduced = p*0.988;

  for (pdiv=0; pdiv<=13; pdiv++)
  {
    pmul = (int)(p_reduced * 8.0 * (1<<pdiv)) - 128;

    if ( (0 <= pmul) && (pmul <= 127) )
    {
      pm = pmul + 128;
      pd = pdiv;
    }
  }

  Data[0]=0;
  Data[1]=(UCHAR) pm;
  Data[2]=(UCHAR) pd;
  Write429Bytes(dev, (MOTOR_NUMBER(Motor)<<5)|IDX_PMUL_PDIV, Data);
  Write429Short(dev, (MOTOR_NUMBER(Motor)<<5)|IDX_AMAX, AMax);

  return 0;
}


/***************************************************************//**
   \fn HardStop(UINT Motor)
   \brief Stop a motor immediately
   \param Motor: motor number (0, 1, 2)

   This function stops a motor immediately (hard stop) by switching
   to velocity mode and then zeroing the V_TARGT and V_ACTUAL registers
   of that axis.
********************************************************************/
static void HardStop(tmc429_t dev, UINT Motor)
{
  Set429RampMode(dev, MOTOR_NUMBER(Motor), RM_VELOCITY);
  Write429Zero(dev, (MOTOR_NUMBER(Motor)<<5)|IDX_VTARGET);
  Write429Zero(dev, (MOTOR_NUMBER(Motor)<<5)|IDX_VACTUAL);
}


/******************************APP*********************************//**
   \fn Init429
   \brief TMC429 initialization

   This function does all necessary initializations of the TMC429.
********************************************************************/
static void Init429(tmc429_t dev)
{
  UINT addr;
  UCHAR Motor;
	u16 IFCONF_invref=0x0000;
	u16 IFCONF_invdir=0x0000;
	
	spi.init(&dev.spi_set);
	
	//电机参数设置清零
	//EEPROM_Init();
  for(Motor=0; Motor<3; Motor++)
  {
    for(addr=0; addr<=IDX_XLATCHED; addr++)
      Write429Zero(dev, addr|(Motor<<5));

  	dev.SpeedChangedFlag[Motor]=TRUE;
  }
	//	        										低电平有效			 引脚不复用		输出S\D模式		REF使能R 			脉冲极限取反（电机方向）
	if( dev.MotorConfig[0].FlagINVRef==1 ) IFCONF_invdir=0x0001;
	if( dev.MotorConfig[0].FlagINVDir==1 ) IFCONF_invref=0x0010;
	
  Write429Int(dev, IDX_IF_CONFIG_429, IFCONF_EN_SD|IFCONF_EN_REFR|IFCONF_SDO_INT|IFCONF_invref|IFCONF_INV_STEP|IFCONF_invdir);
  //mainly for SPI mode				主要设置CLK2_DIV	脉宽	 最小2us    >=500HZ 跟驱动器有关 
	Write429Datagram(dev, IDX_SMGP, 0x00, 0x04, 0x02);//(1+X)us
	//PDIV-V RampDIV-ACC 
  Write429Datagram(dev, IDX_PULSEDIV_RAMPDIV|MOTOR0, 0x00, (dev.MotorConfig[0].PulseDiv<<4)|(dev.MotorConfig[0].RampDiv & 0x0f), 0x04);
  Write429Datagram(dev, IDX_PULSEDIV_RAMPDIV|MOTOR1, 0x00, (dev.MotorConfig[1].PulseDiv<<4)|(dev.MotorConfig[1].RampDiv & 0x0f), 0x04);
  Write429Datagram(dev, IDX_PULSEDIV_RAMPDIV|MOTOR2, 0x00, (dev.MotorConfig[2].PulseDiv<<4)|(dev.MotorConfig[2].RampDiv & 0x0f), 0x04);
  //X_LATCHED(X_ACTUAL)						 RO	电机限位后按参数停止	ramp_mode 
	Write429Datagram(dev, IDX_REFCONF_RM|MOTOR0, 0x00, SOFT_REF, 0x00);
  Write429Datagram(dev, IDX_REFCONF_RM|MOTOR1, 0x00, SOFT_REF, 0x00);
  Write429Datagram(dev, IDX_REFCONF_RM|MOTOR2, 0x00, SOFT_REF, 0x00);
  //小于此值，电机停止
	Write429Short(dev, IDX_VMIN|MOTOR0, 1);
  Write429Short(dev, IDX_VMIN|MOTOR1, 1);
  Write429Short(dev, IDX_VMIN|MOTOR2, 1);
	//VMAX AMAX 
  Write429Int(dev, IDX_VMAX|MOTOR0, dev.MotorConfig[0].VMax);
  SetAMax(dev, 0, dev.MotorConfig[0].AMax);
  Write429Int(dev, IDX_VMAX|MOTOR1, dev.MotorConfig[1].VMax);
  SetAMax(dev, 1, dev.MotorConfig[1].AMax);
  Write429Int(dev, IDX_VMAX|MOTOR2, dev.MotorConfig[2].VMax);
  SetAMax(dev, 2, dev.MotorConfig[2].AMax);
}
/*****************************************************************************************************/
static void TMC429_Stop(tmc429_t dev, u8 motor)
{
  if(motor<N_O_MOTORS)
  {
    Set429RampMode(dev, 0, RM_VELOCITY);
    Write429Zero(dev, IDX_VTARGET|(motor<<5));
  }
}
//
static void TMC429_Rotate(tmc429_t dev, u8 motor,int value)
{
  if(motor<N_O_MOTORS)
  {
    dev.SpeedChangedFlag[motor]=TRUE;
    Set429RampMode(dev, motor, RM_VELOCITY);
    Write429Short(dev, IDX_VMAX|(motor<<5), 2047);
		Write429Short(dev, IDX_VTARGET|(motor<<5), value);
  }
}
//
static void TMC429_Position(tmc429_t dev, u8 motor,int value,u8 mode)
{
  if(motor<N_O_MOTORS)
  {
		if(dev.SpeedChangedFlag[motor])
		{
			Write429Short(dev, IDX_VMAX|(motor<<5), dev.MotorConfig[motor].VMax);
			SetAMax(dev, motor, dev.MotorConfig[motor].AMax);
			dev.SpeedChangedFlag[motor]=FALSE;
		}
		if(mode==MVP_ABS)
    Write429Int(dev, IDX_XTARGET|(motor<<5), value);
		else 
    Write429Int(dev, IDX_XTARGET|(motor<<5), value+Read429Int(dev, IDX_XACTUAL|(motor<<5)));
    Set429RampMode(dev, 0, RM_RAMP);
	}
}
//
static void SetAxisParameter(tmc429_t dev, uint8_t Motor, uint8_t type, int Value)
{
  UCHAR Read[4], Write[4];

  if(Motor<N_O_MOTORS)
  {
    switch(type)
    {
      case 1:			Write429Int(dev, IDX_XTARGET|(Motor<<5), Value);

				break;
      case 2:			Write429Int(dev, IDX_XACTUAL|(Motor<<5), Value);
        break;

      case 3:     Write429Short(dev, IDX_VTARGET|(Motor<<5), Value);
        break;

      case 4:     Write429Short(dev, IDX_VACTUAL|(Motor<<5), Value);
        break;

      case 5:     dev.MotorConfig[Motor].VMax=Value;
								  Write429Short(dev, IDX_VMAX|MOTOR0, dev.MotorConfig[Motor].VMax);
        break;

      case 6:     dev.MotorConfig[Motor].AMax=Value;
								  SetAMax(dev, Motor, dev.MotorConfig[Motor].AMax);
        break;

      case 7:
								Write[0]=IDX_REFCONF_RM|TMC429_READ|(Motor<<5);
								ReadWrite429(dev, Read, Write);
								Write[1]=Read[1];
								if(Value!=0)	Write[2]=Read[2]|0x02;
								else					Write[2]=Read[2]&  ~0x02;
								Write[3]=Read[3];
								Write[0]=IDX_REFCONF_RM|(Motor<<5);
								ReadWrite429(dev, Read, Write);
        break;

      case 8:
								Write[0]=IDX_REFCONF_RM|TMC429_READ|(Motor<<5);
								ReadWrite429(dev, Read, Write);
								Write[1]=Read[1];
								if(Value!=0)				Write[2]=Read[2]|0x01;
								else								Write[2]=Read[2]&  ~0x01;
								Write[3]=Read[3];
								Write[0]=IDX_REFCONF_RM|(Motor<<5);
								ReadWrite429(dev, Read, Write);
								break;

      case 9:  Write429Short(dev, IDX_VMIN|(Motor<<5), Value);
        break;

      case 10:  Set429RampMode(dev, Motor, Value);
        break;

      case 11:  Write429Short(dev, MOTOR_NUMBER(Motor)<<5|IDX_DX_REFTOLERANCE, Value);
        break;

      case 12:  Read429Bytes(dev, IDX_REFCONF_RM|MOTOR_NUMBER(Motor)<<5, Read);
								if(Value!=0)			Read[1]|=0x04;
								else							Read[1]&= ~0x04;
								Write429Bytes(dev, IDX_REFCONF_RM|MOTOR_NUMBER(Motor)<<5, Read);
        break;

      case 13:
								Write[0]=IDX_PULSEDIV_RAMPDIV|TMC429_READ|(Motor<<5);
								ReadWrite429(dev, Read, Write);
								Write[1]=Read[1];
								Write[2]=(Read[2] & 0xf0) | (Value & 0x0f);
								Write[3]=Read[3];
								Write[0]=IDX_PULSEDIV_RAMPDIV|(Motor<<5);
								ReadWrite429(dev, Read, Write);
								dev.MotorConfig[Motor].RampDiv=Value & 0x0f;
        break;

      case 14:
								Write[0]=IDX_PULSEDIV_RAMPDIV|TMC429_READ|(Motor<<5);
								ReadWrite429(dev, Read, Write);
								Write[1]=Read[1];
								Write[2]=(Read[2] & 0x0f) | (Value << 4);
								Write[3]=Read[3];
								Write[0]=IDX_PULSEDIV_RAMPDIV|(Motor<<5);
								ReadWrite429(dev, Read, Write);
								dev.MotorConfig[Motor].PulseDiv=Value& 0x0f;
        break;
			default:
        break;
		}
	}
}

/***************************************************************//**
  \fn GetAxisParameter(void)
  \brief Command GAP

  GAP (Get Axis Parameter) command (see TMCL manual).
********************************************************************/
static int GetAxisParameter(tmc429_t dev, uint8_t Motor, uint8_t type)
{
	int Value=0;
  if(Motor<N_O_MOTORS)
  {
    switch(type)
    {
      case 1:   Value=Read429Int(dev,   IDX_XTARGET|(Motor<<5));
				break;
      case 2:   Value=Read429Int(dev, 	 IDX_XACTUAL|(Motor<<5));
				break;
      case 3:		Value=Read429Short(dev, IDX_VTARGET|(Motor<<5));
				break;
      case 4:		Value=Read429Short(dev, IDX_VACTUAL|(Motor<<5));
				break;
      case 5:		Value=dev.MotorConfig[Motor].VMax;
				break;
      case 6:		Value=dev.MotorConfig[Motor].AMax;
				break;
      case 7:   if(Read429Status(dev) & 0x01) Value=1;
        break;
      case 8:
        Value=(Read429SingleByte(dev, IDX_REF_SWITCHES, 3) & (0x02<<Motor*2)) ? 1:0;
        break;
      case 9:
        Value=(Read429SingleByte(dev, IDX_REF_SWITCHES, 3) & (0x01<<Motor*2)) ? 1:0;
        break;
      case 10:
        Value=(Read429SingleByte(dev,IDX_REFCONF_RM|(Motor<<5), 2) & 0x02) ? 1:0;
        break;
      case 11:
        Value=(Read429SingleByte(dev, IDX_REFCONF_RM|(Motor<<5), 2) & 0x01) ? 1:0;
        break;
      case 12:
        Value=Read429Short(dev, IDX_VMIN|(Motor<<5));
        break;
      case 13:
        Value=Read429SingleByte(dev, IDX_REFCONF_RM|(Motor<<5), 3);
        break;
      case 14:
        Value=Read429Short(dev, MOTOR_NUMBER(Motor)<<5|IDX_DX_REFTOLERANCE);
        break;
      case 15:
        Value=(Read429SingleByte(dev, IDX_REFCONF_RM|MOTOR_NUMBER(Motor)<<5, 0) & 0x04) ? 1:0;
        break;
      case 16:
        Value=Read429SingleByte(dev, IDX_PULSEDIV_RAMPDIV|(Motor<<5), 2) & 0x0f;
        break;
      case 17:
        Value=Read429SingleByte(dev, IDX_PULSEDIV_RAMPDIV|(Motor<<5), 2) >> 4;
        break;

      default:
        break;
    }
  }
	return Value;
}
//
















