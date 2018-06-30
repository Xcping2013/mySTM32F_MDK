#if defined(DEBUG)
#include "stdio.h"
#include "string.h"
#endif /* defined(DEBUG) */

#include "cspin.h"

/* Private variables ---------------------------------------------------------*/
uint8_t spiTxBursts[cSPIN_CMD_ARG_MAX_NB_BYTES][NUMBER_OF_SLAVES];
uint8_t spiRxBursts[cSPIN_CMD_ARG_MAX_NB_BYTES][NUMBER_OF_SLAVES];
uint8_t arrayTxBytes[NUMBER_OF_SLAVES];
uint32_t arrayValues[NUMBER_OF_SLAVES];

void cSPIN_Delay(__IO uint32_t nCount)
{
  for(; nCount!= 0;nCount--);
}

/** 直接操作引脚
  * @brief  Resets CSPIN and puts it into standby mode
  * @param  None
  * @retval None
  */
void cSPIN_Reset_And_Standby(cSPIN_t* dev)
{
	pinReset(dev->reset_pin);
  cSPIN_Delay(10000);
  pinSet(dev->reset_pin);
}

//	芯片引脚和SPI配置
void cSPIN_Pin_Init(cSPIN_t* dev)
{
	spi.init(dev->spiConfig);
  exit_init(dev->flag_Int);	
	exit_init(dev->busy_Int);
}
/**	芯片STCK步进脉冲开关
  * @brief  Enable a PWM on the STCK pin from STM32 
  * @param  Period to be set (PWM Freq = 1MHZ/Period)
  * @retval None
  */
void cSPIN_PWM_Enable(uint16_t Period)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;        
  RCC_ClocksTypeDef RCC_Clocks;
  
  /*Get System Clock frequency */
  RCC_GetClocksFreq(&RCC_Clocks);    
  
  /* Time base configuration */
  TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
  /* Set Prescaler to have a timer clock of 1MHZ */
  TIM_TimeBaseInitStruct.TIM_Prescaler = (RCC_Clocks.SYSCLK_Frequency/1000000)-1;
  /* PWM Frequency will be equal to 1MHZ/Period */
  TIM_TimeBaseInitStruct.TIM_Period = Period-1; 

  TIM_TimeBaseInit(TIM_PWM, &TIM_TimeBaseInitStruct);

  /* PWM1 Mode configuration */
  TIM_OCStructInit( &TIM_OCInitStruct );
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = Period / 2; //range from  0 to TIM_Period, 
  /* Channel 3 */
  TIM_OC3Init(TIM_PWM, &TIM_OCInitStruct);

  /* TIM_PWM enable or disable counter */
  TIM_Cmd(TIM_PWM, ENABLE);
}

/**
  * @brief  Disable PWM on the STCK pin from STM32 
  * @param  None
  * @retval None
  */
void cSPIN_PWM_DISABLE(void)
{
	TIM_Cmd(TIM_PWM, DISABLE);
}
/**
  * @brief  Fills-in cSPIN configuration structure with default values.
  * @param  cSPIN_RegsStruct structure address (pointer to struct)
  * @retval None
  */
void cSPIN_Regs_Struct_Reset(cSPIN_RegsStruct_TypeDef* cSPIN_RegsStruct)
{
	cSPIN_RegsStruct->ABS_POS = 0;
	cSPIN_RegsStruct->EL_POS = 0;
	cSPIN_RegsStruct->MARK = 0;
	cSPIN_RegsStruct->ACC = 0x08A;
	cSPIN_RegsStruct->DEC = 0x08A;
	cSPIN_RegsStruct->MAX_SPEED = 0x041;
	cSPIN_RegsStruct->MIN_SPEED = 0;
	cSPIN_RegsStruct->FS_SPD = 0x027;
#if defined(L6480)
	cSPIN_RegsStruct->KVAL_HOLD = 0x29;
	cSPIN_RegsStruct->KVAL_RUN = 0x29;
	cSPIN_RegsStruct->KVAL_ACC = 0x29;
	cSPIN_RegsStruct->KVAL_DEC = 0x29;
	cSPIN_RegsStruct->INT_SPD = 0x0408;
	cSPIN_RegsStruct->ST_SLP = 0x19;
	cSPIN_RegsStruct->FN_SLP_ACC = 0x29;
	cSPIN_RegsStruct->FN_SLP_DEC = 0x29;
	cSPIN_RegsStruct->K_THERM = 0;
	cSPIN_RegsStruct->STALL_TH = 0x10;
#endif /* defined(L6480) */
#if defined(L6482)
	cSPIN_RegsStruct->TVAL_HOLD = 0x29;
	cSPIN_RegsStruct->TVAL_RUN = 0x29;
	cSPIN_RegsStruct->TVAL_ACC = 0x29;
	cSPIN_RegsStruct->TVAL_DEC = 0x29;
	cSPIN_RegsStruct->T_FAST = 0x19;
	cSPIN_RegsStruct->TON_MIN = 0x29;
	cSPIN_RegsStruct->TOFF_MIN = 0x29;
#endif /* defined(L6482) */
	cSPIN_RegsStruct->OCD_TH = 0x8;
	cSPIN_RegsStruct->STEP_MODE = 0x7;
	cSPIN_RegsStruct->ALARM_EN = 0xFF;
	cSPIN_RegsStruct->GATECFG1 = 0;
	cSPIN_RegsStruct->GATECFG2 = 0;
	cSPIN_RegsStruct->CONFIG = 0x2C88;
}
 
/**
  * @brief  Configures cSPIN internal registers with values in the config structure.
  * @param  cSPIN_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval None
  */
void cSPIN_Registers_Set(cSPIN_RegsStruct_TypeDef* cSPIN_RegsStruct)
{
	cSPIN_Set_Param(cSPIN_ABS_POS, cSPIN_RegsStruct->ABS_POS);
	cSPIN_Set_Param(cSPIN_EL_POS, cSPIN_RegsStruct->EL_POS);
	cSPIN_Set_Param(cSPIN_MARK, cSPIN_RegsStruct->MARK);
	cSPIN_Set_Param(cSPIN_ACC, cSPIN_RegsStruct->ACC);
	cSPIN_Set_Param(cSPIN_DEC, cSPIN_RegsStruct->DEC);
	cSPIN_Set_Param(cSPIN_MAX_SPEED, cSPIN_RegsStruct->MAX_SPEED);
	cSPIN_Set_Param(cSPIN_MIN_SPEED, cSPIN_RegsStruct->MIN_SPEED);
	cSPIN_Set_Param(cSPIN_FS_SPD, cSPIN_RegsStruct->FS_SPD);
#if defined(L6480)
	cSPIN_Set_Param(cSPIN_KVAL_HOLD, cSPIN_RegsStruct->KVAL_HOLD);
	cSPIN_Set_Param(cSPIN_KVAL_RUN, cSPIN_RegsStruct->KVAL_RUN);
	cSPIN_Set_Param(cSPIN_KVAL_ACC, cSPIN_RegsStruct->KVAL_ACC);
	cSPIN_Set_Param(cSPIN_KVAL_DEC, cSPIN_RegsStruct->KVAL_DEC);
	cSPIN_Set_Param(cSPIN_INT_SPD, cSPIN_RegsStruct->INT_SPD);
	cSPIN_Set_Param(cSPIN_ST_SLP, cSPIN_RegsStruct->ST_SLP);
	cSPIN_Set_Param(cSPIN_FN_SLP_ACC, cSPIN_RegsStruct->FN_SLP_ACC);
	cSPIN_Set_Param(cSPIN_FN_SLP_DEC, cSPIN_RegsStruct->FN_SLP_DEC);
	cSPIN_Set_Param(cSPIN_K_THERM, cSPIN_RegsStruct->K_THERM);
	cSPIN_Set_Param(cSPIN_STALL_TH, cSPIN_RegsStruct->STALL_TH);
#endif /* defined(L6480) */
#if defined(L6482)
	cSPIN_Set_Param(cSPIN_TVAL_HOLD, cSPIN_RegsStruct->TVAL_HOLD);
	cSPIN_Set_Param(cSPIN_TVAL_RUN, cSPIN_RegsStruct->TVAL_RUN);
	cSPIN_Set_Param(cSPIN_TVAL_ACC, cSPIN_RegsStruct->TVAL_ACC);
	cSPIN_Set_Param(cSPIN_TVAL_DEC, cSPIN_RegsStruct->TVAL_DEC);
	cSPIN_Set_Param(cSPIN_T_FAST, cSPIN_RegsStruct->T_FAST);
	cSPIN_Set_Param(cSPIN_TON_MIN, cSPIN_RegsStruct->TON_MIN);
	cSPIN_Set_Param(cSPIN_TOFF_MIN, cSPIN_RegsStruct->TOFF_MIN);
#endif /* defined(L6482) */
	cSPIN_Set_Param(cSPIN_OCD_TH, cSPIN_RegsStruct->OCD_TH);
	cSPIN_Set_Param(cSPIN_STEP_MODE, cSPIN_RegsStruct->STEP_MODE);
	cSPIN_Set_Param(cSPIN_ALARM_EN, cSPIN_RegsStruct->ALARM_EN);
	cSPIN_Set_Param(cSPIN_GATECFG1, cSPIN_RegsStruct->GATECFG1);
	cSPIN_Set_Param(cSPIN_GATECFG2, cSPIN_RegsStruct->GATECFG2);
	cSPIN_Set_Param(cSPIN_CONFIG, cSPIN_RegsStruct->CONFIG);
}

#if defined(DEBUG)  
/**
  * @brief Reads cSPIN internal registers and print them to terminal I/O
  * @param cSPIN_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval None
  */
void cSPIN_Registers_Get(cSPIN_RegsStruct_TypeDef* cSPIN_RegsStruct)
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
#if defined(L6480)
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
#endif /* defined(L6480) */
#if defined(L6482)
  PRINT_REG(TVAL_HOLD, read_reg, diff, str);
  PRINT_REG(TVAL_RUN, read_reg, diff, str);
  PRINT_REG(TVAL_ACC, read_reg, diff, str);
  PRINT_REG(TVAL_DEC, read_reg, diff, str);
  PRINT_REG(T_FAST, read_reg, diff, str);
  PRINT_REG(TON_MIN, read_reg, diff, str);
  PRINT_REG(TOFF_MIN, read_reg, diff, str);
#endif /* defined(L6482) */
  PRINT_REG(ADC_OUT, read_reg, diff, str);  
  PRINT_REG(OCD_TH, read_reg, diff, str);
  PRINT_REG(FS_SPD, read_reg, diff, str);
  PRINT_REG(STEP_MODE, read_reg, diff, str);
  PRINT_REG(ALARM_EN, read_reg, diff, str);
  PRINT_REG(CONFIG, read_reg, diff, str);
  PRINT_REG(STATUS, read_reg, diff, str);
}
#endif /* defined(DEBUG) */

#if defined(DEBUG)  
/**
  * @brief  Reads cSPIN internal writable registers and compares with the values in the code
  * @param  cSPIN_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval Bitmap with the bits, corresponding to the unmatched registers, set
  */
uint32_t cSPIN_Registers_Check(cSPIN_RegsStruct_TypeDef* cSPIN_RegsStruct)
{
  uint32_t result = 0;
  
  CHECK_REG(ABS_POS, result);
  CHECK_REG(EL_POS, result);
  CHECK_REG(MARK, result);
  CHECK_REG(ACC, result);
  CHECK_REG(DEC, result);
  CHECK_REG(MAX_SPEED, result);
  CHECK_REG(MIN_SPEED, result);  
#if defined(L6480)
  CHECK_REG(KVAL_HOLD, result);
  CHECK_REG(KVAL_RUN, result);
  CHECK_REG(KVAL_ACC, result);
  CHECK_REG(KVAL_DEC, result);
  CHECK_REG(INT_SPD, result);
  CHECK_REG(ST_SLP, result);
  CHECK_REG(FN_SLP_ACC, result);
  CHECK_REG(FN_SLP_DEC, result);
  CHECK_REG(K_THERM, result);
  CHECK_REG(STALL_TH, result);
#endif /* defined(L6480) */
#if defined(L6482)
  CHECK_REG(TVAL_HOLD, result);
  CHECK_REG(TVAL_RUN, result);
  CHECK_REG(TVAL_ACC, result);
  CHECK_REG(TVAL_DEC, result);
  CHECK_REG(T_FAST, result);
  CHECK_REG(TON_MIN, result);
  CHECK_REG(TOFF_MIN, result);
#endif /* defined(L6482) */  
  CHECK_REG(OCD_TH, result);
  CHECK_REG(FS_SPD, result);
  CHECK_REG(STEP_MODE, result);
  CHECK_REG(ALARM_EN, result);
  CHECK_REG(CONFIG, result);
  
  return result;
}
#endif /* defined(DEBUG) */

/**
  * @brief  Issues cSPIN NOP command.
  * @param  None
  * @retval None
  */
void cSPIN_Nop(cSPIN_t* dev)
{
	/* Send NOP operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_NOP);
}

/**
  * @brief  Issues cSPIN Set Param command.
  * @param  param cSPIN register address
  * @param  value to be set
  * @retval None
  */
void cSPIN_Set_Param((cSPIN_t* dev, cSPIN_Registers_TypeDef param, uint32_t value)
{
	/* Send SetParam operation code to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)cSPIN_SET_PARAM | (uint8_t)param);
	switch (param)
	{
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
			/* Send parameter - byte 2 to cSPIN */
			cSPIN_Write_Byte(dev, (uint8_t)(value >> 16));
		case cSPIN_EL_POS: ;
		case cSPIN_ACC: ;
		case cSPIN_DEC: ;
		case cSPIN_MAX_SPEED: ;
		case cSPIN_MIN_SPEED: ;
		case cSPIN_FS_SPD: ;
#if defined(L6480)
		case cSPIN_INT_SPD: ;
#endif /* defined(L6480) */
		case cSPIN_CONFIG: ;
		case cSPIN_STATUS:
			/* Send parameter - byte 1 to cSPIN */
		   	cSPIN_Write_Byte(dev, (uint8_t)(value >> 8));
		default:
			/* Send parameter - byte 0 to cSPIN */
		   	cSPIN_Write_Byte(dev, (uint8_t)(value));
	}
}

/**
  * @brief  Issues cSPIN Get Param command.
  * @param  param cSPIN register address
  * @retval Register value - 1 to 3 bytes (depends on register)
  */
uint32_t cSPIN_Get_Param((cSPIN_t* dev, cSPIN_Registers_TypeDef param)
{
	uint32_t temp = 0;
	uint32_t rx = 0;

	/* Send GetParam operation code to cSPIN */
	temp = cSPIN_Write_Byte(dev, (uint8_t)cSPIN_GET_PARAM | (uint8_t)param);
	/* MSB which should be 0 */
	temp = temp << 24;
	rx |= temp;
	switch (param)
	{
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
		case cSPIN_SPEED:
		   	temp = cSPIN_Write_Byte(dev, (uint8_t)(0x00));
			temp = temp << 16;
			rx |= temp;
		case cSPIN_EL_POS: ;
		case cSPIN_ACC: ;
		case cSPIN_DEC: ;
		case cSPIN_MAX_SPEED: ;
		case cSPIN_MIN_SPEED: ;
		case cSPIN_FS_SPD: ;
#if defined(L6480)
		case cSPIN_INT_SPD: ;
#endif /* defined(L6480) */
		case cSPIN_CONFIG: ;
		case cSPIN_STATUS:
		   	temp = cSPIN_Write_Byte(dev, (uint8_t)(0x00));
			temp = temp << 8;
			rx |= temp;
		default:
		   	temp = cSPIN_Write_Byte(dev, (uint8_t)(0x00));
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
void cSPIN_Run(cSPIN_t* dev, cSPIN_Direction_TypeDef direction, uint32_t speed)
{
	/* Send RUN operation code to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)cSPIN_RUN | (uint8_t)direction);
	/* Send speed - byte 2 data cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(speed >> 16));
	/* Send speed - byte 1 data cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(speed >> 8));
	/* Send speed - byte 0 data cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(speed));
}

/**
  * @brief  Issues cSPIN Step Clock command.
  * @param  direction Movement direction (FWD, REV)
  * @retval None
  */
void cSPIN_Step_Clock(cSPIN_t* dev, cSPIN_Direction_TypeDef direction)
{
	/* Send StepClock operation code to cSPIN */
	cSPIN_Write_Byte((uint8_t)cSPIN_STEP_CLOCK | (uint8_t)direction);
}

/**
  * @brief  Issues cSPIN Move command.
  * @param  direction Movement direction
  * @param  n_step number of steps
  * @retval None
  */
void cSPIN_Move(cSPIN_t* dev, cSPIN_Direction_TypeDef direction, uint32_t n_step)
{
	/* Send Move operation code to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)cSPIN_MOVE | (uint8_t)direction);
	/* Send n_step - byte 2 data cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(n_step >> 16));
	/* Send n_step - byte 1 data cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(n_step >> 8));
	/* Send n_step - byte 0 data cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(n_step));
}

/**
  * @brief  Issues cSPIN Go To command.
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void cSPIN_Go_To(cSPIN_t* dev, uint32_t abs_pos)
{
	/* Send GoTo operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_GO_TO);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go To Dir command.
  * @param  direction movement direction
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void cSPIN_Go_To_Dir(cSPIN_t* dev, cSPIN_Direction_TypeDef direction, uint32_t abs_pos)
{
	/* Send GoTo_DIR operation code to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)cSPIN_GO_TO_DIR | (uint8_t)direction);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go Until command.
  * @param  action
  * @param  direction movement direction
  * @param  speed
  * @retval None
  */
void cSPIN_Go_Until(cSPIN_t* dev, cSPIN_Action_TypeDef action, cSPIN_Direction_TypeDef direction, uint32_t speed)
{
	/* Send GoUntil operation code to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)cSPIN_GO_UNTIL | (uint8_t)action | (uint8_t)direction);
	/* Send speed parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(speed >> 16));
	/* Send speed parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(speed >> 8));
	/* Send speed parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)(speed));
}

/**
  * @brief  Issues cSPIN Release SW command.
  * @param  action
  * @param  direction movement direction
  * @retval None
  */
void cSPIN_Release_SW(cSPIN_t* dev, cSPIN_Action_TypeDef action, cSPIN_Direction_TypeDef direction)
{
	/* Send ReleaseSW operation code to cSPIN */
	cSPIN_Write_Byte(dev, (uint8_t)cSPIN_RELEASE_SW | (uint8_t)action | (uint8_t)direction);
}

/**
  * @brief  Issues cSPIN Go Home command. (Shorted path to zero position)
  * @param  None
  * @retval None
  */
void cSPIN_Go_Home(cSPIN_t* dev)
{
	/* Send GoHome operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_GO_HOME);
}

/**	go to targer position
  * @brief  Issues cSPIN Go Mark command.
  * @param  None
  * @retval None
  */
void cSPIN_Go_Mark(cSPIN_t* dev)
{
	/* Send GoMark operation code to cSPIN */
	cSPIN_Write_Byte(cSPIN_GO_MARK);
}

/**
  * @brief  Issues cSPIN Reset Pos command.
  * @param  None
  * @retval None
  */
void cSPIN_Reset_Pos(cSPIN_t* dev)
{
	/* Send ResetPos operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_RESET_POS);
}

/**
  * @brief  Issues cSPIN Reset Device command.
  * @param  None
  * @retval None
  */
void cSPIN_Reset_Device(cSPIN_t* dev)
{
	/* Send ResetDevice operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_RESET_DEVICE);
}

/**
  * @brief  Issues cSPIN Soft Stop command.
  * @param  None
  * @retval None
  */
void cSPIN_Soft_Stop(cSPIN_t* dev)
{
	/* Send SoftStop operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_SOFT_STOP);
}

/**
  * @brief  Issues cSPIN Hard Stop command.
  * @param  None
  * @retval None
  */
void cSPIN_Hard_Stop(cSPIN_t* dev)
{
	/* Send HardStop operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_HARD_STOP);
}

/**
  * @brief  Issues cSPIN Soft HiZ command.
  * @param  None
  * @retval None
  */
void cSPIN_Soft_HiZ(cSPIN_t* dev)
{
	/* Send SoftHiZ operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_SOFT_HIZ);
}

/**
  * @brief  Issues cSPIN Hard HiZ command.
  * @param  None
  * @retval None
  */
void cSPIN_Hard_HiZ(cSPIN_t* dev)
{
	/* Send HardHiZ operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_HARD_HIZ);
}

/**
  * @brief  Issues cSPIN Get Status command.
  * @param  None
  * @retval Status Register content
  */
uint16_t cSPIN_Get_Status(cSPIN_t* dev)
{
	uint16_t temp = 0;
	uint16_t rx = 0;

	/* Send GetStatus operation code to cSPIN */
	cSPIN_Write_Byte(dev, cSPIN_GET_STATUS);
	/* Send zero byte / receive MSByte from cSPIN */
	temp = cSPIN_Write_Byte(dev, (uint8_t)(0x00));
	temp = temp << 8;
	rx |= temp;
	/* Send zero byte / receive LSByte from cSPIN */
	temp = cSPIN_Write_Byte(dev, (uint8_t)(0x00));
	rx |= temp;
	return rx;
}

/**
  * @brief  Checks if the cSPIN is Busy by hardware - active Busy signal.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t cSPIN_Busy_HW(cSPIN_t* dev)
{
	if(!(pinRead(dev->busy_Int->pin)) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks if the cSPIN is Busy by SPI - Busy flag bit in Status Register.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t cSPIN_Busy_SW(cSPIN_t* dev)
{
	if(!(cSPIN_Get_Status(dev) & cSPIN_STATUS_BUSY)) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks cSPIN Flag signal.
  * @param  None
  * @retval one if Flag signal is active, otherwise zero
  */
uint8_t cSPIN_getFlag(cSPIN_t* dev)
{
	if(!(pinRead(dev->flag_Int->pin ))) return 0x01;
	else return 0x00;
}

/**
  * @brief  Transmits/Receives one byte to/from cSPIN over SPI.
  * @param  byte Transmited byte
  * @retval Received byte
  */
uint8_t cSPIN_Write_Byte(cSPIN_t* dev, uint8_t byte)
{
	return spi.readwrite(dev, byte, TRUE);
}
//////////////////////////////////////////////////////////////////////
/**通过一个CS引脚 一个SPI 写动态链设备
  * @brief  Transmits/Receives several bytes to cSPIN over SPI
  * @param  pTxByte pTxBytePointer to TX bytes
  * @param  pRxByte Pointer to RX bytes
  * @param  nBytes Number of TX = RX bytes
  * @retval None
  */
void cSPIN_Write_Daisy_Chain_Bytes(cSPIN_t* dev, uint8_t *pTxByte, uint8_t *pRxByte, uint8_t nBytes)
{
  uint32_t index;
  for (index = 0; index < nBytes-1; index++)
  {
		*pRxByte++=spi.readwrite(dev, *pTxByte++, FALSE)
  }
	*pRxByte++=spi.readwrite(dev, *pTxByte++, TRUE);
}

/**
  * @brief  Issues cSPIN Set Param command to each device (slave).
  * @param  slaves_number number of slaves
  * @param  pParam Pointer to an array of cSPIN register address
  * @param  pValue Pointer to an array of cSPIN parameter value
  * @retval None
  */
void cSPIN_All_Slaves_Set_Param(cSPIN_t* dev, uint8_t slaves_number, uint8_t *pParam, uint32_t *pValue)
{
  uint32_t i;
  uint8_t maxArgumentNbBytes = 0;

  for (i = 0; i < slaves_number; i++)
  {
     switch (*pParam)
     {
				case cSPIN_ABS_POS: ;
				case cSPIN_MARK: ;
				case cSPIN_SPEED:
						 spiTxBursts[0][i] = *pParam;
						 spiTxBursts[1][i] = (uint8_t)(*pValue >> 16);
						 spiTxBursts[2][i] = (uint8_t)(*pValue >> 8);
						 maxArgumentNbBytes = 3;
					break;
				case cSPIN_EL_POS: ;
				case cSPIN_ACC: ;
				case cSPIN_DEC: ;
				case cSPIN_MAX_SPEED: ;
				case cSPIN_MIN_SPEED: ;
				case cSPIN_FS_SPD: ;
#if defined(L6480)
				case cSPIN_INT_SPD: ;
#endif /* defined(L6480) */
				case cSPIN_CONFIG: ;
				case cSPIN_STATUS:
					 spiTxBursts[0][i] = cSPIN_NOP;
					 spiTxBursts[1][i] = *pParam;
					 spiTxBursts[2][i] = (uint8_t)(*pValue >> 8);           
					 if (maxArgumentNbBytes < 2)
					 {
							maxArgumentNbBytes = 2;
					 }
					break;
				default:
						 spiTxBursts[0][i] = cSPIN_NOP;
						 spiTxBursts[1][i] = cSPIN_NOP;
						 spiTxBursts[2][i] = *pParam;
						 if (maxArgumentNbBytes < 1)
						 {                 
								maxArgumentNbBytes = 1;
						 }
			}
     spiTxBursts[3][i] = (uint8_t)(*pValue);
     pParam++;
     pValue++;
  }
  for (i = cSPIN_CMD_ARG_MAX_NB_BYTES-1-maxArgumentNbBytes; i < cSPIN_CMD_ARG_MAX_NB_BYTES; i++)
  {
     cSPIN_Write_Daisy_Chain_Bytes(DEV, &spiTxBursts[i][0], &spiRxBursts[i][0], slaves_number);
  }
}

/**
  * @brief  Issues cSPIN Get Param command to each device (slave).
  * @param  slaves_number number of slaves
  * @param  pParam Pointer to an array of cSPIN register address
  * @param  pValue Pointer to an array of cSPIN parameter value
  * @retval None
  */
void cSPIN_All_Slaves_Get_Param(cSPIN_t* dev, uint8_t slaves_number, uint8_t *pParam, uint32_t *pValue)
{
  uint32_t i;
  uint8_t maxArgumentNbBytes = 0;
  
  for (i = 0; i < slaves_number; i++)
  {
     switch (*pParam)
     {
	case cSPIN_ABS_POS: ;
	case cSPIN_MARK: ;
	case cSPIN_SPEED:
                 spiTxBursts[0][i] = ((uint8_t)cSPIN_GET_PARAM )| (*pParam);
                 spiTxBursts[1][i] = cSPIN_NOP;
                 spiTxBursts[2][i] = cSPIN_NOP;
	         maxArgumentNbBytes = 3;
                 break;
        case cSPIN_EL_POS: ;
	case cSPIN_ACC: ;
	case cSPIN_DEC: ;
	case cSPIN_MAX_SPEED: ;
	case cSPIN_MIN_SPEED: ;
	case cSPIN_FS_SPD: ;
#if defined(L6480)
	case cSPIN_INT_SPD: ;
#endif /* defined(L6480) */
	case cSPIN_CONFIG: ;
	case cSPIN_STATUS:
                 spiTxBursts[0][i] = cSPIN_NOP;
                 spiTxBursts[1][i] = ((uint8_t)cSPIN_GET_PARAM )| (*pParam);
                 spiTxBursts[2][i] = cSPIN_NOP;
                 if (maxArgumentNbBytes < 2)
                 {
                    maxArgumentNbBytes = 2;
                 }
                 break;
	default:
                 spiTxBursts[0][i] = cSPIN_NOP;
                 spiTxBursts[1][i] = cSPIN_NOP;
                 spiTxBursts[2][i] = ((uint8_t)cSPIN_GET_PARAM )| (*pParam);
                 if (maxArgumentNbBytes < 1)
                 {                 
                    maxArgumentNbBytes = 1;
                 }
     }
     spiTxBursts[3][i] = cSPIN_NOP;
     spiRxBursts[1][i] = 0;
     spiRxBursts[2][i] = 0;
     spiRxBursts[3][i] = 0;
     pParam++;
  }
  for (i = cSPIN_CMD_ARG_MAX_NB_BYTES-1-maxArgumentNbBytes; i < cSPIN_CMD_ARG_MAX_NB_BYTES; i++)
  {
     cSPIN_Write_Daisy_Chain_Bytes(dev, &spiTxBursts[i][0], &spiRxBursts[i][0], slaves_number);
  }
  for (i = 0; i < slaves_number; i++)
  {
    *pValue = (spiRxBursts[1][i] << 16) | (spiRxBursts[2][i] << 8) | (spiRxBursts[3][i]);
    pValue++;
  }
}

/**
  * @brief  Configures cSPIN slaves internal registers with values in the config structure.
  * @param  slaves_number number of slaves 
  * @param  cSPIN_RegsStructArray Configuration structure array address (pointer to configuration structure array)
  * @retval None
  */
void cSPIN_All_Slaves_Registers_Set(cSPIN_t* dev, uint8_t slaves_number, cSPIN_RegsStruct_TypeDef *cSPIN_RegsStructArray)
{
    uint32_t i;
    
    /* ABS_POS */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_ABS_POS;
      arrayValues[i] = cSPIN_RegsStructArray[i].ABS_POS;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* EL_POS */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_EL_POS;
      arrayValues[i] = cSPIN_RegsStructArray[i].EL_POS;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* MARK */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_MARK;
      arrayValues[i] = cSPIN_RegsStructArray[i].MARK;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* ACC */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_ACC;
      arrayValues[i] = cSPIN_RegsStructArray[i].ACC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* DEC*/
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_DEC;
      arrayValues[i] = cSPIN_RegsStructArray[i].DEC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);  
    /* MAX_SPEED */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_MAX_SPEED;
      arrayValues[i] = cSPIN_RegsStructArray[i].MAX_SPEED;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* MIN_SPEED */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_MIN_SPEED;
      arrayValues[i] = cSPIN_RegsStructArray[i].MIN_SPEED;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* FS_SPD */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_FS_SPD;
      arrayValues[i] = cSPIN_RegsStructArray[i].FS_SPD;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues); 
#if defined(L6480)
    /* KVAL_HOLD */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_KVAL_HOLD;
      arrayValues[i] = cSPIN_RegsStructArray[i].KVAL_HOLD;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* KVAL_RUN */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_KVAL_RUN;
      arrayValues[i] = cSPIN_RegsStructArray[i].KVAL_RUN;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* KVAL_ACC */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_KVAL_ACC;
      arrayValues[i] = cSPIN_RegsStructArray[i].KVAL_ACC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues); 
    /* KVAL_DEC */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_KVAL_DEC;
      arrayValues[i] = cSPIN_RegsStructArray[i].KVAL_DEC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* INT_SPD */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_INT_SPD;
      arrayValues[i] = cSPIN_RegsStructArray[i].INT_SPD;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* ST_SLP */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_ST_SLP;
      arrayValues[i] = cSPIN_RegsStructArray[i].ST_SLP;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* FN_SLP_ACC */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_FN_SLP_ACC;
      arrayValues[i] = cSPIN_RegsStructArray[i].FN_SLP_ACC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* FN_SLP_DEC */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_FN_SLP_DEC;
      arrayValues[i] = cSPIN_RegsStructArray[i].FN_SLP_DEC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* K_THERM */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_K_THERM;
      arrayValues[i] = cSPIN_RegsStructArray[i].K_THERM;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* STALL_TH */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_STALL_TH;
      arrayValues[i] = cSPIN_RegsStructArray[i].STALL_TH;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
#endif /* defined(L6480) */
#if defined(L6482)
    /* TVAL_HOLD */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_TVAL_HOLD;
      arrayValues[i] = cSPIN_RegsStructArray[i].TVAL_HOLD;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* TVAL_RUN */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_TVAL_RUN;
      arrayValues[i] = cSPIN_RegsStructArray[i].TVAL_RUN;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* TVAL_ACC */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_TVAL_ACC;
      arrayValues[i] = cSPIN_RegsStructArray[i].TVAL_ACC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues); 
    /* TVAL_DEC */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_TVAL_DEC;
      arrayValues[i] = cSPIN_RegsStructArray[i].TVAL_DEC;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* T_FAST */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_T_FAST;
      arrayValues[i] = cSPIN_RegsStructArray[i].T_FAST;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* TON_MIN */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_TON_MIN;
      arrayValues[i] = cSPIN_RegsStructArray[i].TON_MIN;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* TOFF_MIN */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_TOFF_MIN;
      arrayValues[i] = cSPIN_RegsStructArray[i].TOFF_MIN;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
#endif /* defined(L6482) */
    /* OCD_TH */
     for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_OCD_TH;
      arrayValues[i] = cSPIN_RegsStructArray[i].OCD_TH;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* STEP_MODE */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_STEP_MODE;
      arrayValues[i] = cSPIN_RegsStructArray[i].STEP_MODE;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* ALARM_EN */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_ALARM_EN;
      arrayValues[i] = cSPIN_RegsStructArray[i].ALARM_EN;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* GATECFG1 */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_GATECFG1;
      arrayValues[i] = cSPIN_RegsStructArray[i].GATECFG1;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* GATECFG2 */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_GATECFG2;
      arrayValues[i] = cSPIN_RegsStructArray[i].GATECFG2;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues);
    /* CONFIG */
    for (i=0;i<slaves_number;i++)
    {
      arrayTxBytes[i] = cSPIN_CONFIG;
      arrayValues[i] = cSPIN_RegsStructArray[i].CONFIG;
    }
    cSPIN_All_Slaves_Set_Param(dev, slaves_number, arrayTxBytes, arrayValues); 
}

/**
  * @brief  Issues cSPIN Move command to one slave device
  * @param  slaves_number number of slaves 
  * @param  slaveNumber slave number
  * @param  direction movement direction
  * @param  n_step number of steps
  * @retval None
  */
void cSPIN_One_Slave_Move(cSPIN_t* dev, uint8_t slaves_number, uint8_t slaveNumber, cSPIN_Direction_TypeDef direction, uint32_t n_step)
{
  cSPIN_One_Slave_Send_Command(dev, slaveNumber, slaves_number, (uint8_t)cSPIN_MOVE | (uint8_t)direction, n_step);
}

/**
  * @brief  Issues cSPIN Run command to one slave device
  * @param  slaveNumber slave number
  * @param  slaves_number number of slaves 
  * @param  direction movement direction
  * @param  speed
  * @retval None
  */
void cSPIN_One_Slave_Run(cSPIN_t* dev, uint8_t slaveNumber, uint8_t slaves_number, cSPIN_Direction_TypeDef direction, uint32_t speed)
{
  cSPIN_One_Slave_Send_Command(dev, slaveNumber, slaves_number, (uint8_t)cSPIN_RUN | (uint8_t)direction, speed);
}

/**
  * @brief  Issues a command to one slave device
  * @param  slaveNumber slave number
  * @param  slaves_number number of slaves 
  * @param  param command to issue
  * @param  value command argument 
  * @retval None
  */
void cSPIN_One_Slave_Send_Command(cSPIN_t* dev, uint8_t slaveNumber, uint8_t slaves_number, uint8_t param, uint32_t value)
{
  uint32_t i;
  
  for (i = 0; i < slaves_number; i++)
  {
    if (i == slaveNumber)
    {
      spiTxBursts[0][i] = (param);
      spiTxBursts[1][i] = (uint8_t)(value >> 16);
      spiTxBursts[2][i] = (uint8_t)(value >> 8);
      spiTxBursts[3][i] = (uint8_t)(value);
    }
    else
    {
      spiTxBursts[0][i] = cSPIN_NOP;
      spiTxBursts[1][i] = cSPIN_NOP;
      spiTxBursts[2][i] = cSPIN_NOP;
      spiTxBursts[3][i] = cSPIN_NOP;     
    }
  }
  for (i = cSPIN_CMD_ARG_MAX_NB_BYTES-cSPIN_CMD_ARG_NB_BYTES_MOVE; i < cSPIN_CMD_ARG_MAX_NB_BYTES; i++)
  {
     cSPIN_Write_Daisy_Chain_Bytes(dev, &spiTxBursts[i][0], &spiRxBursts[i][0], slaves_number);
  }
}

/**
  * @brief Issues commands to the slave devices for synchronous execution
  * @param slaves_number number of slaves
  * @param pParam Pointer to an array of cSPIN commands
  * @param pValue Pointer to an array of cSPIN arguments

  * @retval None
  */
void cSPIN_All_Slaves_Send_Command(cSPIN_t* dev, uint8_t slaves_number, uint8_t *pParam, uint32_t *pValue)
{
  uint32_t i;
  uint8_t maxArgumentNbBytes = 0;

  for (i = 0; i < slaves_number; i++)
  {
     switch ((*pParam) & DAISY_CHAIN_COMMAND_MASK)
     {
        case cSPIN_RUN: ;
        case cSPIN_MOVE: ;
        case cSPIN_GO_TO: ;
        case cSPIN_GO_TO_DIR: ;
        case cSPIN_GO_UNTIL: ;
        case cSPIN_GO_UNTIL_ACT_CPY:
                 spiTxBursts[0][i] = *pParam;
                 spiTxBursts[1][i] = (uint8_t)(*pValue >> 16);
                 spiTxBursts[2][i] = (uint8_t)(*pValue >> 8);
                 spiTxBursts[3][i] = (uint8_t)(*pValue);
	         maxArgumentNbBytes = 3;
                 break;
	default:
                 spiTxBursts[0][i] = cSPIN_NOP;
                 spiTxBursts[1][i] = cSPIN_NOP;
                 spiTxBursts[2][i] = cSPIN_NOP;
                 spiTxBursts[3][i] = *pParam;
     }
     pParam++;
     pValue++;
  }
  for (i = cSPIN_CMD_ARG_MAX_NB_BYTES-1-maxArgumentNbBytes; i < cSPIN_CMD_ARG_MAX_NB_BYTES; i++)
  {
     cSPIN_Write_Daisy_Chain_Bytes(dev, &spiTxBursts[i][0], &spiRxBursts[i][0], slaves_number);
  }
}

/**
  * @brief Issues cSPIN Get Status command to each device (slave)
  * @param slaves_number number of slaves
  * @param pValue pointer to an array of Status Register content
  * @retval None
  */
void cSPIN_All_Slaves_Get_Status(cSPIN_t* dev, uint8_t slaves_number, uint32_t *pValue)
{
  uint32_t i;

  for (i = 0; i < slaves_number; i++)
  {
     spiTxBursts[0][i] = cSPIN_GET_STATUS;
     spiTxBursts[1][i] = cSPIN_NOP;
     spiTxBursts[2][i] = cSPIN_NOP;
     spiRxBursts[1][i] = 0;
     spiRxBursts[2][i] = 0;
  }
  for (i = 0; i < cSPIN_CMD_ARG_NB_BYTES_GET_STATUS+cSPIN_RSP_NB_BYTES_GET_STATUS; i++)
  {
     cSPIN_Write_Daisy_Chain_Bytes(dev, &spiTxBursts[i][0], &spiRxBursts[i][0], slaves_number);
  }
  for (i = 0; i < slaves_number; i++)
  {
    *pValue = (spiRxBursts[1][i] << 8) | (spiRxBursts[2][i]);
    pValue++;
  }
}

/**
  * @brief  Checks if one of the cSPIN device (slave) is Busy by SPI - Busy flag bit in Status Register.
  * @param  slaves_number number of slaves
  * @retval one if there is a busy chip, otherwise zero
  */
uint8_t cSPIN_One_Or_More_Slaves_Busy_SW(cSPIN_t* dev, uint8_t slaves_number)
{
  uint32_t i;
  uint16_t status;
  cSPIN_All_Slaves_Get_Status(dev, slaves_number, arrayValues);
  for (i = 0; i < slaves_number; i++)
  {
    status |= arrayValues[i];
  }
  if(!(status & cSPIN_STATUS_BUSY)) return 0x01;
  else return 0x00;
}

/** @} */  
/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
