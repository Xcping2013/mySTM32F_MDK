#include "stdio.h"
#include "string.h"

#include "l6480.h"

#define L6480_DEBUG (0)

#if L6480_DEBUG

#define CHECK_REG(reg, result) if (L6480_RegsStruct-> ## reg != L6480_Get_Param(cSPIN_ ## reg)) result |= 1 << ((uint8_t) L6480_ ## reg)
#define PRINT_REG(reg_name, read_reg, diff, reg_name_string)  strcpy(reg_name_string,"          "); \
  strncpy(reg_name_string, #reg_name, strlen(#reg_name)); \
  strcpy(diff," "); \
  read_reg = L6480_Get_Param(L6480_ ## reg_name); \
  if (read_reg != L6480_RegsStruct-> ## reg_name) strcpy(diff,"x"); \
  printf("%s %s mcu: 0x%-6X, cspin: 0x%-6X\n", diff, reg_name_string, L6480_RegsStruct-> ## reg_name, read_reg)
#else
#define CHECK_REG(...)
#define PRINT_REG(...)
#endif



L6480_t	L6480s=
{
	&PB12,
	SPI2,
	SPI_MODE3,
	SPI_CLOCK_DIV16,				//设置为18M时钟,高速模式
	SPI_BITODER_MSB,
	&PD10,
	&PD8,
	&PD9,
	1,
};

/*	小延时
*/
void L6480_Delay(__IO uint32_t nCount)
{
  for(; nCount!= 0;nCount--);
}

/** 芯片复位进入待机模式
  * @brief  Resets CSPIN and puts it into standby mode
  * @param  None
  * @retval None
  */
void L6480_Reset_And_Standby(void)
{
	pinReset(L6480s.reset_pin);
  L6480_Delay(10000);
  pinSet(L6480s.reset_pin);
}
/*	芯片寄存器结构体数据默认值设置
*/
void L6480_Regs_Struct_Reset(L6480_RegsStruct_TypeDef* L6480_RegsStruct)
{
	L6480_RegsStruct->ABS_POS = 0;
	L6480_RegsStruct->EL_POS = 0;
	L6480_RegsStruct->MARK = 0;
	L6480_RegsStruct->ACC = 0x08A;
	L6480_RegsStruct->DEC = 0x08A;
	L6480_RegsStruct->MAX_SPEED = 0x041;
	L6480_RegsStruct->MIN_SPEED = 0;
	L6480_RegsStruct->FS_SPD = 0x027;

	L6480_RegsStruct->KVAL_HOLD = 0x29;
	L6480_RegsStruct->KVAL_RUN = 0x29;
	L6480_RegsStruct->KVAL_ACC = 0x29;
	L6480_RegsStruct->KVAL_DEC = 0x29;
	L6480_RegsStruct->INT_SPD = 0x0408;
	L6480_RegsStruct->ST_SLP = 0x19;
	L6480_RegsStruct->FN_SLP_ACC = 0x29;
	L6480_RegsStruct->FN_SLP_DEC = 0x29;
	L6480_RegsStruct->K_THERM = 0;
	L6480_RegsStruct->STALL_TH = 0x10;

	L6480_RegsStruct->OCD_TH = 0x8;
	L6480_RegsStruct->STEP_MODE = 0x7;
	L6480_RegsStruct->ALARM_EN = 0xFF;
	L6480_RegsStruct->GATECFG1 = 0;
	L6480_RegsStruct->GATECFG2 = 0;
	L6480_RegsStruct->CONFIG = 0x2C88;
}
 
/*	芯片SPI读写字节
*/
uint8_t L6480_Write_Byte(uint8_t byte)
{
	if(L6480s.motor==1) 
	{
		L6480s.spiConfig.cs_pin=&L6480_CS1_PIN;
		L6480s.reset_pin=&L6480_RESET1_PIN;
		L6480s.reset_pin=&L6480_BUSY1_PIN;
		L6480s.flag_pin=&L6480_FLAG1_PIN;
	}
	else 	if(L6480s.motor==2) 
	{
		L6480s.spiConfig.cs_pin=&L6480_CS2_PIN;
		L6480s.reset_pin=&L6480_RESET2_PIN;
		L6480s.reset_pin=&L6480_BUSY2_PIN;
		L6480s.flag_pin=&L6480_FLAG2_PIN;
	}
	else 	if(L6480s.motor==3) 
	{
		L6480s.spiConfig.cs_pin=&L6480_CS3_PIN;
		L6480s.reset_pin=&L6480_RESET3_PIN;
		L6480s.reset_pin=&L6480_BUSY3_PIN;
		L6480s.flag_pin=&L6480_FLAG3_PIN;
	}
	return spi.readwrite(&L6480s.spiConfig, byte, TRUE);
}
/**	把寄存器结构体数据写入芯片中
  * @brief  Configures cSPIN internal registers with values in the config structure.
  * @param  L6480_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval None
  */
void L6480_Registers_Set(L6480_RegsStruct_TypeDef* L6480_RegsStruct)
{
	L6480_Set_Param( L6480_ABS_POS, L6480_RegsStruct->ABS_POS);
	L6480_Set_Param( L6480_EL_POS, L6480_RegsStruct->EL_POS);
	L6480_Set_Param( L6480_MARK, L6480_RegsStruct->MARK);
	L6480_Set_Param( L6480_ACC, L6480_RegsStruct->ACC);
	L6480_Set_Param( L6480_DEC, L6480_RegsStruct->DEC);
	L6480_Set_Param( L6480_MAX_SPEED, L6480_RegsStruct->MAX_SPEED);
	L6480_Set_Param( L6480_MIN_SPEED, L6480_RegsStruct->MIN_SPEED);
	L6480_Set_Param( L6480_FS_SPD, L6480_RegsStruct->FS_SPD);

	L6480_Set_Param( L6480_KVAL_HOLD, L6480_RegsStruct->KVAL_HOLD);
	L6480_Set_Param( L6480_KVAL_RUN, L6480_RegsStruct->KVAL_RUN);
	L6480_Set_Param( L6480_KVAL_ACC, L6480_RegsStruct->KVAL_ACC);
	L6480_Set_Param( L6480_KVAL_DEC, L6480_RegsStruct->KVAL_DEC);
	L6480_Set_Param( L6480_INT_SPD, L6480_RegsStruct->INT_SPD);
	L6480_Set_Param( L6480_ST_SLP, L6480_RegsStruct->ST_SLP);
	L6480_Set_Param( L6480_FN_SLP_ACC, L6480_RegsStruct->FN_SLP_ACC);
	L6480_Set_Param( L6480_FN_SLP_DEC, L6480_RegsStruct->FN_SLP_DEC);
	L6480_Set_Param( L6480_K_THERM, L6480_RegsStruct->K_THERM);
	L6480_Set_Param( L6480_STALL_TH, L6480_RegsStruct->STALL_TH);

	L6480_Set_Param( L6480_OCD_TH, L6480_RegsStruct->OCD_TH);
	L6480_Set_Param( L6480_STEP_MODE, L6480_RegsStruct->STEP_MODE);
	L6480_Set_Param( L6480_ALARM_EN, L6480_RegsStruct->ALARM_EN);
	L6480_Set_Param( L6480_GATECFG1, L6480_RegsStruct->GATECFG1);
	L6480_Set_Param( L6480_GATECFG2, L6480_RegsStruct->GATECFG2);
	L6480_Set_Param( L6480_CONFIG, L6480_RegsStruct->CONFIG);
}

/**	读取芯片寄存器内容并打印
  * @brief Reads cSPIN internal registers and print them to terminal I/O
  * @param L6480_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval None
  */
void L6480_Registers_Get(L6480_RegsStruct_TypeDef* L6480_RegsStruct)
{
  uint32_t read_reg;
  char diff[1];
  char str[10];
  
  PRINT_REG(ABS_POS, read_reg, diff, str);
  PRINT_REG(EL_POS, read_reg, diff, str);
  PRINT_REG(MARK, read_reg, diff, str);
  PRINT_REG(SPEED, read_reg, diff, str);
  PRINT_REG(ACC, read_reg, diff, str);
  PRINT_REG(DEC, read_reg, diff, str);
  PRINT_REG(MAX_SPEED, read_reg, diff, str);
  PRINT_REG(MIN_SPEED, read_reg, diff, str);

  PRINT_REG(KVAL_HOLD, read_reg, diff, str);
  PRINT_REG(KVAL_RUN, read_reg, diff, str);
  PRINT_REG(KVAL_ACC, read_reg, diff, str);
  PRINT_REG(KVAL_DEC, read_reg, diff, str);
  PRINT_REG(INT_SPD, read_reg, diff, str);
  PRINT_REG(ST_SLP, read_reg, diff, str);
  PRINT_REG(FN_SLP_ACC, read_reg, diff, str);
  PRINT_REG(FN_SLP_DEC, read_reg, diff, str);
  PRINT_REG(K_THERM, read_reg, diff, str);
  PRINT_REG(STALL_TH, read_reg, diff, str);

  PRINT_REG(ADC_OUT, read_reg, diff, str);  
  PRINT_REG(OCD_TH, read_reg, diff, str);
  PRINT_REG(FS_SPD, read_reg, diff, str);
  PRINT_REG(STEP_MODE, read_reg, diff, str);
  PRINT_REG(ALARM_EN, read_reg, diff, str);
  PRINT_REG(CONFIG, read_reg, diff, str);
  PRINT_REG(STATUS, read_reg, diff, str);
}

/**
  * @brief  Reads cSPIN internal writable registers and compares with the values in the code
  * @param  L6480_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval Bitmap with the bits, corresponding to the unmatched registers, set
  */
uint32_t L6480_Registers_Check(L6480_RegsStruct_TypeDef* L6480_RegsStruct)
{
  uint32_t result = 0;
  
  CHECK_REG( ABS_POS, result);
  CHECK_REG( EL_POS, result);
  CHECK_REG( MARK, result);
  CHECK_REG( ACC, result);
  CHECK_REG( DEC, result);
  CHECK_REG( MAX_SPEED, result);
  CHECK_REG( MIN_SPEED, result);  

  CHECK_REG( KVAL_HOLD, result);
  CHECK_REG( KVAL_RUN, result);
  CHECK_REG( KVAL_ACC, result);
  CHECK_REG( KVAL_DEC, result);
  CHECK_REG( INT_SPD, result);
  CHECK_REG( ST_SLP, result);
  CHECK_REG( FN_SLP_ACC, result);
  CHECK_REG( FN_SLP_DEC, result);
  CHECK_REG( K_THERM, result);
  CHECK_REG( STALL_TH, result);


  CHECK_REG( OCD_TH, result);
  CHECK_REG( FS_SPD, result);
  CHECK_REG( STEP_MODE, result);
  CHECK_REG( ALARM_EN, result);
  CHECK_REG( CONFIG, result);
  
  return result;
}

/**
  * @brief  Issues cSPIN NOP command.
  * @param  None
  * @retval None
  */
void L6480_Nop(void)
{
	/* Send NOP operation code to cSPIN */
	L6480_Write_Byte(L6480_NOP);
}

/**
  * @brief  Issues cSPIN Set Param command.
  * @param  param cSPIN register address
  * @param  value to be set
  * @retval None
  */
void L6480_Set_Param(L6480_Registers_TypeDef param, uint32_t value)
{
	/* Send SetParam operation code to cSPIN */
	L6480_Write_Byte((uint8_t)L6480_SET_PARAM | (uint8_t)param);
	switch (param)
	{
		case L6480_ABS_POS: ;
		case L6480_MARK: ;
			/* Send parameter - byte 2 to cSPIN */
			L6480_Write_Byte((uint8_t)(value >> 16));
		case L6480_EL_POS: ;
		case L6480_ACC: ;
		case L6480_DEC: ;
		case L6480_MAX_SPEED: ;
		case L6480_MIN_SPEED: ;
		case L6480_FS_SPD: ;
		case L6480_INT_SPD: ;
		case L6480_CONFIG: ;
		case L6480_STATUS:
			/* Send parameter - byte 1 to cSPIN */
		   	L6480_Write_Byte((uint8_t)(value >> 8));
		default:
			/* Send parameter - byte 0 to cSPIN */
		   	L6480_Write_Byte( (uint8_t)(value));
	}
}

/**
  * @brief  Issues cSPIN Get Param command.
  * @param  param cSPIN register address
  * @retval Register value - 1 to 3 bytes (depends on register)
  */
//
uint32_t L6480_Get_Param(L6480_Registers_TypeDef param)
{
	uint32_t temp = 0;
	uint32_t rx = 0;

	/* Send GetParam operation code to cSPIN */
	temp = L6480_Write_Byte( (uint8_t)L6480_GET_PARAM | (uint8_t)param);
	/* MSB which should be 0 */
	temp = temp << 24;
	rx |= temp;
	switch (param)
	{
		case L6480_ABS_POS: ;
		case L6480_MARK: ;
		case L6480_SPEED:
		   	temp = L6480_Write_Byte( (uint8_t)(0x00));
			temp = temp << 16;
			rx |= temp;
		case L6480_EL_POS: ;
		case L6480_ACC: ;
		case L6480_DEC: ;
		case L6480_MAX_SPEED: ;
		case L6480_MIN_SPEED: ;
		case L6480_FS_SPD: ;
#if defined(L6480)
		case L6480_INT_SPD: ;
#endif /* defined(L6480) */
		case L6480_CONFIG: ;
		case L6480_STATUS:
		   	temp = L6480_Write_Byte( (uint8_t)(0x00));
			temp = temp << 8;
			rx |= temp;
		default:
		   	temp = L6480_Write_Byte( (uint8_t)(0x00));
			rx |= temp;
	}
	return rx;
}

/**
  * @brief  Issues cSPIN Run command.
  * @param  direction Movement direction (FWD, REV)
  * @param  speed over 3 bytes
  * @retval None
  */
void L6480_Run(L6480_Direction_TypeDef direction, uint32_t speed)
{
	/* Send RUN operation code to cSPIN */
	L6480_Write_Byte( (uint8_t)L6480_RUN | (uint8_t)direction);
	/* Send speed - byte 2 data cSPIN */
	L6480_Write_Byte( (uint8_t)(speed >> 16));
	/* Send speed - byte 1 data cSPIN */
	L6480_Write_Byte( (uint8_t)(speed >> 8));
	/* Send speed - byte 0 data cSPIN */
	L6480_Write_Byte( (uint8_t)(speed));
}

/**
  * @brief  Issues cSPIN Step Clock command.
  * @param  direction Movement direction (FWD, REV)
  * @retval None
  */
void L6480_Step_Clock(L6480_Direction_TypeDef direction)
{
	/* Send StepClock operation code to cSPIN */
	L6480_Write_Byte( (uint8_t)L6480_STEP_CLOCK | (uint8_t)direction);
}

/**
  * @brief  Issues cSPIN Move command.
  * @param  direction Movement direction
  * @param  n_step number of steps
  * @retval None
  */
void L6480_Move(L6480_Direction_TypeDef direction, uint32_t n_step)
{
	/* Send Move operation code to cSPIN */
	L6480_Write_Byte( (uint8_t)L6480_MOVE | (uint8_t)direction);
	/* Send n_step - byte 2 data cSPIN */
	L6480_Write_Byte( (uint8_t)(n_step >> 16));
	/* Send n_step - byte 1 data cSPIN */
	L6480_Write_Byte( (uint8_t)(n_step >> 8));
	/* Send n_step - byte 0 data cSPIN */
	L6480_Write_Byte( (uint8_t)(n_step));
}

/**
  * @brief  Issues cSPIN Go To command.
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void L6480_Go_To(uint32_t abs_pos)
{
	/* Send GoTo operation code to cSPIN */
	L6480_Write_Byte( L6480_GO_TO);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go To Dir command.
  * @param  direction movement direction
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void L6480_Go_To_Dir(L6480_Direction_TypeDef direction, uint32_t abs_pos)
{
	/* Send GoTo_DIR operation code to cSPIN */
	L6480_Write_Byte( (uint8_t)L6480_GO_TO_DIR | (uint8_t)direction);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go Until command.
  * @param  action
  * @param  direction movement direction
  * @param  speed
  * @retval None
  */
void L6480_Go_Until(L6480_Action_TypeDef action, L6480_Direction_TypeDef direction, uint32_t speed)
{
	/* Send GoUntil operation code to cSPIN */
	L6480_Write_Byte( (uint8_t)L6480_GO_UNTIL | (uint8_t)action | (uint8_t)direction);
	/* Send speed parameter - byte 2 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(speed >> 16));
	/* Send speed parameter - byte 1 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(speed >> 8));
	/* Send speed parameter - byte 0 data to cSPIN */
	L6480_Write_Byte( (uint8_t)(speed));
}

/**
  * @brief  Issues cSPIN Release SW command.
  * @param  action
  * @param  direction movement direction
  * @retval None
  */
void L6480_Release_SW(L6480_Action_TypeDef action, L6480_Direction_TypeDef direction)
{
	/* Send ReleaseSW operation code to cSPIN */
	L6480_Write_Byte( (uint8_t)L6480_RELEASE_SW | (uint8_t)action | (uint8_t)direction);
}

/**
  * @brief  Issues cSPIN Go Home command. (Shorted path to zero position)
  * @param  None
  * @retval None
  */
void L6480_Go_Home(void)
{
	/* Send GoHome operation code to cSPIN */
	L6480_Write_Byte( L6480_GO_HOME);
}

/**	go to targer position
  * @brief  Issues cSPIN Go Mark command.
  * @param  None
  * @retval None
  */
void L6480_Go_Mark(void)
{
	/* Send GoMark operation code to cSPIN */
	L6480_Write_Byte( L6480_GO_MARK);
}

/**
  * @brief  Issues cSPIN Reset Pos command.
  * @param  None
  * @retval None
  */
void L6480_Reset_Pos(void)
{
	/* Send ResetPos operation code to cSPIN */
	L6480_Write_Byte( L6480_RESET_POS);
}

/**
  * @brief  Issues cSPIN Reset Device command.
  * @param  None
  * @retval None
  */
void L6480_Reset_Device(void)
{
	/* Send ResetDevice operation code to cSPIN */
	L6480_Write_Byte( L6480_RESET_DEVICE);
}

/**
  * @brief  Issues cSPIN Soft Stop command.
  * @param  None
  * @retval None
  */
void L6480_Soft_Stop(void)
{
	/* Send SoftStop operation code to cSPIN */
	L6480_Write_Byte( L6480_SOFT_STOP);
}

/**
  * @brief  Issues cSPIN Hard Stop command.
  * @param  None
  * @retval None
  */
void L6480_Hard_Stop(void)
{
	/* Send HardStop operation code to cSPIN */
	L6480_Write_Byte( L6480_HARD_STOP);
}

/**
  * @brief  Issues cSPIN Soft HiZ command.
  * @param  None
  * @retval None
  */
void L6480_Soft_HiZ(void)
{
	/* Send SoftHiZ operation code to cSPIN */
	L6480_Write_Byte( L6480_SOFT_HIZ);
}

/**
  * @brief  Issues cSPIN Hard HiZ command.
  * @param  None
  * @retval None
  */
void L6480_Hard_HiZ(void)
{
	/* Send HardHiZ operation code to cSPIN */
	L6480_Write_Byte( L6480_HARD_HIZ);
}

/**
  * @brief  Issues cSPIN Get Status command.
  * @param  None
  * @retval Status Register content
  */
uint16_t L6480_Get_Status(void)
{
	uint16_t temp = 0;
	uint16_t rx = 0;

	/* Send GetStatus operation code to cSPIN */
	L6480_Write_Byte( L6480_GET_STATUS);
	/* Send zero byte / receive MSByte from cSPIN */
	temp = L6480_Write_Byte( (uint8_t)(0x00));
	temp = temp << 8;
	rx |= temp;
	/* Send zero byte / receive LSByte from cSPIN */
	temp = L6480_Write_Byte( (uint8_t)(0x00));
	rx |= temp;
	return rx;
}

/**
  * @brief  Checks if the cSPIN is Busy by hardware - active Busy signal.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t L6480_Busy_HW(void)
{
	if(!(pinRead(L6480s.busy_pin))) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks if the cSPIN is Busy by SPI - Busy flag bit in Status Register.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t L6480_Busy_SW(void)
{
	if(!(L6480_Get_Status( ) & L6480_STATUS_BUSY)) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks cSPIN Flag signal.
  * @param  None
  * @retval one if Flag signal is active, otherwise zero
  */
uint8_t L6480_getFlag(void)
{
	if(!(pinRead(L6480s.flag_pin ))) return 0x01;
	else return 0x00;
}




