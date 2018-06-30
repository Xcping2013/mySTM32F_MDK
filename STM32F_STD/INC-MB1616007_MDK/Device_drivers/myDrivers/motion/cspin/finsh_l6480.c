#include <rtthread.h>
#include <finsh.h>

#include "cspin.h"
#include "cspin_config.h"
#include "cspin_mine.h"

cSPIN_RegsStruct_TypeDef cSPIN_RegsStructArray[NUMBER_OF_SLAVES];
double   MAX_SPEED[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_MAX_SPEED;
uint8_t daisy_chain = DAISY_CHAIN;
uint8_t number_of_slaves = NUMBER_OF_SLAVES;

#define DELAY_COUNT    0x3FFFF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  RCC_ClocksTypeDef RCC_ClockFreq;
  uint32_t i;        
  uint8_t commandArray[NUMBER_OF_SLAVES];
  uint32_t argumentArray[NUMBER_OF_SLAVES];
  uint32_t responseArray[NUMBER_OF_SLAVES];
  /* straight forward definitions */
  double   ACC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_ACC;
  double   DEC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_DEC;
  double   FS_SPD[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_FS_SPD;
  uint8_t  ALARM_EN[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_ALARM_EN;
  uint8_t  OCD_TH[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_OCD_TH;
  /* OR-ed definitions */
  uint8_t  STEP_MODE[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_STEP_MODE;
  uint8_t  SYNC_MODE[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_SYNC_MODE;
  uint16_t GATECFG1_WD_EN[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_WD_EN;
  uint16_t GATECFG1_TBOOST[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TBOOST;
  uint16_t GATECFG1_IGATE[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_IGATE;
  uint16_t GATECFG1_TCC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TCC;
  uint8_t  GATECFG2_TBLANK[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TBLANK;
  uint8_t  GATECFG2_TDT[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TDT;
  uint16_t CONFIG_CLOCK_SETTING[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_CLOCK_SETTING;
  uint16_t CONFIG_SW_MODE[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_SW_MODE;   
  uint16_t CONFIG_OC_SD[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_OC_SD;
  uint16_t CONFIG_UVLOVAL[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_UVLOVAL;
  uint16_t CONFIG_VCCVAL[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_VCCVAL;
#if defined(L6480)
  /* straight forward definitions */
  double   KVAL_HOLD[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_KVAL_HOLD;
  double   KVAL_RUN[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_KVAL_RUN;
  double   KVAL_ACC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_KVAL_ACC;
  double   KVAL_DEC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_KVAL_DEC;
  double   INT_SPD[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_INT_SPD;
  double   ST_SLP[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_ST_SLP;
  double   FN_SLP_ACC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_FN_SLP_ACC;
  double   FN_SLP_DEC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_FN_SLP_DEC;
  double   K_THERM[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_K_THERM;
  double   STALL_TH[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_STALL_TH;
  /* OR-ed definitions */
  double   MIN_SPEED[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_MIN_SPEED;
  uint16_t LSPD_BIT[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_LSPD_BIT;
  uint16_t CONFIG_VS_COMP[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_VS_COMP;
  uint16_t CONFIG_PWM_DIV[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_PWM_DIV;
  uint16_t CONFIG_PWM_MUL[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_PWM_MUL;
  bool pOpenA[1];
  bool pOpenB[1];
#endif /* defined(L6480) */
#if defined(L6482)
  /* straight forward definitions */
  double   MIN_SPEED[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_MIN_SPEED;
  double   TVAL_HOLD[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TVAL_HOLD;
  double   TVAL_RUN[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TVAL_RUN;
  double   TVAL_ACC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TVAL_ACC;
  double   TVAL_DEC[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TVAL_DEC;
  double   TON_MIN[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TON_MIN;
  double   TOFF_MIN[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TOFF_MIN;
  uint8_t  TOFF_FAST[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TOFF_FAST;
  uint8_t  FAST_STEP[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_FAST_STEP;    
  /* OR-ed definitions */
  uint16_t CONFIG_TQ_REG[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TQ_REG;
  uint16_t CONFIG_TSW[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_TSW;
  uint16_t CONFIG_PRED_EN[NUMBER_OF_SLAVES] = cSPIN_DC_CONF_PARAM_PRED;
#endif /* defined(L6482) */  
  uint32_t cSPIN_rx_data = 0;
  cSPIN_RegsStruct_TypeDef cSPIN_RegsStruct;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int cSPIN_main(void) 
{
	cSPIN_Peripherals_Init();

	/* Visual LED checking */
	cSPIN_Led_Check();

	/* Resets and puts cSPIN into standby mode */
	cSPIN_Reset_And_Standby();
   
	if (daisy_chain == 0)
	{
		/* Structure initialization by default values, in order to avoid blank records */
		cSPIN_Regs_Struct_Reset(&cSPIN_RegsStruct);
		
		/* Acceleration rate settings to cSPIN_CONF_PARAM_ACC in steps/s2, range 14.55 to 59590 steps/s2 */
		cSPIN_RegsStruct.ACC 		= AccDec_Steps_to_Par(cSPIN_CONF_PARAM_ACC);
		/* Deceleration rate settings to cSPIN_CONF_PARAM_DEC in steps/s2, range 14.55 to 59590 steps/s2 */
		cSPIN_RegsStruct.DEC 		= AccDec_Steps_to_Par(cSPIN_CONF_PARAM_DEC); 
		/* Maximum speed settings to cSPIN_CONF_PARAM_MAX_SPEED in steps/s, range 15.25 to 15610 steps/s */
		cSPIN_RegsStruct.MAX_SPEED 	= MaxSpd_Steps_to_Par(cSPIN_CONF_PARAM_MAX_SPEED);
		/* Full step speed settings cSPIN_CONF_PARAM_FS_SPD in steps/s, range 7.63 to 15625 steps/s */
		cSPIN_RegsStruct.FS_SPD 	= FSSpd_Steps_to_Par(cSPIN_CONF_PARAM_FS_SPD);
	#if defined(L6480)
		/* Minimum speed settings to cSPIN_CONF_PARAM_MIN_SPEED in steps/s, range 0 to 976.3 steps/s */
		cSPIN_RegsStruct.MIN_SPEED	= cSPIN_CONF_PARAM_LSPD_BIT|MinSpd_Steps_to_Par(cSPIN_CONF_PARAM_MIN_SPEED);
					/* Acceleration duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_ACC in %, range 0 to 99.6% */
		cSPIN_RegsStruct.KVAL_ACC 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_ACC);
					/* Deceleration duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_DEC in %, range 0 to 99.6% */
		cSPIN_RegsStruct.KVAL_DEC 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_DEC);		
					/* Run duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_RUN in %, range 0 to 99.6% */
		cSPIN_RegsStruct.KVAL_RUN 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_RUN);
					/* Hold duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_HOLD in %, range 0 to 99.6% */
		cSPIN_RegsStruct.KVAL_HOLD 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_HOLD);
						/* Thermal compensation param settings to cSPIN_CONF_PARAM_K_THERM, range 1 to 1.46875 */
		cSPIN_RegsStruct.K_THERM 	= KTherm_to_Par(cSPIN_CONF_PARAM_K_THERM);
		/* Intersect speed settings for BEMF compensation to cSPIN_CONF_PARAM_INT_SPD in steps/s, range 0 to 3906 steps/s */
		cSPIN_RegsStruct.INT_SPD 	= IntSpd_Steps_to_Par(cSPIN_CONF_PARAM_INT_SPD);
		/* BEMF start slope settings for BEMF compensation to cSPIN_CONF_PARAM_ST_SLP in % step/s, range 0 to 0.4% s/step */
		cSPIN_RegsStruct.ST_SLP 	= BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_ST_SLP);
		/* BEMF final acc slope settings for BEMF compensation to cSPIN_CONF_PARAM_FN_SLP_ACC in% step/s, range 0 to 0.4% s/step */
		cSPIN_RegsStruct.FN_SLP_ACC = BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_FN_SLP_ACC);
		/* BEMF final dec slope settings for BEMF compensation to cSPIN_CONF_PARAM_FN_SLP_DEC in% step/s, range 0 to 0.4% s/step */
		cSPIN_RegsStruct.FN_SLP_DEC = BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_FN_SLP_DEC);
		/* Stall threshold settings to cSPIN_CONF_PARAM_STALL_TH in mV, range 31.25 to 1000mV */
		cSPIN_RegsStruct.STALL_TH 	= StallTh_to_Par(cSPIN_CONF_PARAM_STALL_TH);
					/* Set Config register according to config parameters */
					/* clock setting, switch hard stop interrupt mode, */
					/*  supply voltage compensation, overcurrent shutdown */
					/* UVLO threshold, VCC reg output voltage , PWM frequency */
		cSPIN_RegsStruct.CONFIG 	= (uint16_t)cSPIN_CONF_PARAM_CLOCK_SETTING | \
																						(uint16_t)cSPIN_CONF_PARAM_SW_MODE	   | \
																						(uint16_t)cSPIN_CONF_PARAM_VS_COMP       | \
																						(uint16_t)cSPIN_CONF_PARAM_OC_SD         | \
																						(uint16_t)cSPIN_CONF_PARAM_UVLOVAL       | \
																						(uint16_t)cSPIN_CONF_PARAM_VCCVAL	   | \
																						(uint16_t)cSPIN_CONF_PARAM_PWM_DIV       | \
																						(uint16_t)cSPIN_CONF_PARAM_PWM_MUL;
	#endif /* defined(L6480) */
	#if defined(L6482)
					/* Minimum speed settings to cSPIN_CONF_PARAM_MIN_SPEED in steps/s, range 0 to 976.3 steps/s */
		cSPIN_RegsStruct.MIN_SPEED	= MinSpd_Steps_to_Par(cSPIN_CONF_PARAM_MIN_SPEED);
					/* Reference voltage assigned to the torque regulation DAC during motor acceleration, range 7.8mV to 1000mV */
		cSPIN_RegsStruct.TVAL_ACC 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_ACC);
					/* Reference voltage assigned to the torque regulation DAC during motor deceleration, range 7.8mV to 1000mV */
		cSPIN_RegsStruct.TVAL_DEC 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_DEC);		
					/* Reference voltage assigned to the torque regulation DAC when motor is running, range 7.8mV to 1000mV */
		cSPIN_RegsStruct.TVAL_RUN 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_RUN);
					/* Reference voltage assigned to the torque regulation DAC when motor is stopped, range 7.8mV to 1000mV */
		cSPIN_RegsStruct.TVAL_HOLD 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_HOLD);
					/* Maximum fast decay and fall step times used by the current control system, range 2us to 32us */
					cSPIN_RegsStruct.T_FAST 	= (uint8_t)cSPIN_CONF_PARAM_TOFF_FAST | (uint8_t)cSPIN_CONF_PARAM_FAST_STEP;
					/* Minimum ON time value used by the current control system, range 0.5us to 64us */
					cSPIN_RegsStruct.TON_MIN 	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TON_MIN);
					/* Minimum OFF time value used by the current control system, range 0.5us to 64us */
					cSPIN_RegsStruct.TOFF_MIN	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TOFF_MIN);
					/* Set Config register according to config parameters */
					/* clock setting, switch hard stop interrupt mode, */
					/*  supply voltage compensation, overcurrent shutdown */
					/* UVLO threshold, VCC reg output voltage , target switching period, predictive current control */
		cSPIN_RegsStruct.CONFIG 	= (uint16_t)cSPIN_CONF_PARAM_CLOCK_SETTING | \
																						(uint16_t)cSPIN_CONF_PARAM_SW_MODE	   | \
																						(uint16_t)cSPIN_CONF_PARAM_TQ_REG        | \
																						(uint16_t)cSPIN_CONF_PARAM_OC_SD         | \
																						(uint16_t)cSPIN_CONF_PARAM_UVLOVAL       | \
																						(uint16_t)cSPIN_CONF_PARAM_VCCVAL	   | \
																						(uint16_t)cSPIN_CONF_PARAM_TSW           | \
																						(uint16_t)cSPIN_CONF_PARAM_PRED;        
	#endif /* defined(L6482) */
		/* Overcurrent threshold settings to cSPIN_CONF_PARAM_OCD_TH, range 31.25 to 1000mV */
		cSPIN_RegsStruct.OCD_TH 	= cSPIN_CONF_PARAM_OCD_TH;        
					/* Alarm settings to cSPIN_CONF_PARAM_ALARM_EN */
		cSPIN_RegsStruct.ALARM_EN 	= cSPIN_CONF_PARAM_ALARM_EN;
					/* Step mode and sycn mode settings via cSPIN_CONF_PARAM_SYNC_MODE and cSPIN_CONF_PARAM_STEP_MODE */
		cSPIN_RegsStruct.STEP_MODE 	= (uint8_t)cSPIN_CONF_PARAM_SYNC_MODE | \
																						(uint8_t)cSPIN_CONF_PARAM_STEP_MODE;
		/* Sink/source current, duration of constant current phases, duration of overboost phase settings */
		cSPIN_RegsStruct.GATECFG1       = (uint16_t)cSPIN_CONF_PARAM_IGATE | \
																			(uint16_t)cSPIN_CONF_PARAM_TCC   | \
																			(uint16_t)cSPIN_CONF_PARAM_TBOOST;
		/* Blank time, Dead time stiings */
		cSPIN_RegsStruct.GATECFG2       = (uint16_t)cSPIN_CONF_PARAM_TBLANK | \
																			 (uint16_t)cSPIN_CONF_PARAM_TDT;
		/* Program all cSPIN registers */
		cSPIN_Registers_Set(&cSPIN_RegsStruct);

#if defined(DEBUG)
		/* check the values of all cSPIN registers */
		cSPIN_rx_data = cSPIN_Registers_Check(&cSPIN_RegsStruct);
		
		/* get the values of all cSPIN registers and print them to the terminal I/O */
		cSPIN_Registers_Get(&cSPIN_RegsStruct);
#endif /* defined(DEBUG) */             
		
		/**********************************************************************/
		/* Start example of FLAG interrupt management */
		/**********************************************************************/
		/* Clear Flag pin */
		cSPIN_rx_data = cSPIN_Get_Status();       
		/* Interrupt configuration for FLAG signal */
		cSPIN_Flag_Interrupt_GPIO_Config();
		/* Run constant speed of 400 steps/s forward direction */
		cSPIN_Run(FWD, Speed_Steps_to_Par(400));
		/* Tentative to write to the current motor absolute position register */
		/* while the motor is running */
		cSPIN_Set_Param(cSPIN_ABS_POS, 100);
		cSPIN_Delay(0x004FFFFF);
		/* Get Status to clear FLAG due to non-performable command */
		cSPIN_rx_data = cSPIN_Get_Status();
		cSPIN_Delay(0x004FFFFF);        
		/* Perform SoftStop commmand */
		cSPIN_Soft_Stop();
        /* Wait until not busy - busy pin test */
		while(cSPIN_Busy_HW());
    cSPIN_Delay(0x004FFFFF);
        /**********************************************************************/
        /* End example of FLAG interrupt management */
        /**********************************************************************/

        /**********************************************************************/
        /* Start example of BUSY interrupt management */
        /**********************************************************************/
        /* Interrupt configuration for BUSY signal */
    cSPIN_Busy_Interrupt_GPIO_Config();
		/* Move by 100,000 steps reverse, range 0 to 4,194,303 */
		cSPIN_Move(REV, (uint32_t)(100000));
        /* STEVAL_PCC009V2 : during busy time the POWER LED is switched OFF */
        /* ST_cSPIN_6480H_DISCOVERY : during busy time the LED_BUSY is switched ON */
        /* Wait until not busy - busy pin test */
		while(cSPIN_Busy_HW());
        /* Disable the power bridges */
		cSPIN_Soft_HiZ();
    cSPIN_Delay(0x004FFFFF);
        /**********************************************************************/
        /* End example of BUSY interrupt management */
        /**********************************************************************/          

		/* Move by 60,000 steps rorward, range 0 to 4,194,303 */
		cSPIN_Move(FWD, (uint32_t)(60000));
	
		/* Wait until not busy - busy pin test */
		while(cSPIN_Busy_HW());

#if defined(L6480)
	/* Send cSPIN command change hold duty cycle to 0.5% */
		cSPIN_Set_Param(cSPIN_KVAL_HOLD, Kval_Perc_to_Par(0.5));
	
	/* Send cSPIN command change run duty cycle to 5% */
		cSPIN_Set_Param(cSPIN_KVAL_RUN, Kval_Perc_to_Par(5));
#endif /* defined(L6480) */
#if defined(L6482)
	/* Send cSPIN command change hold cuurent to 40mA */
		cSPIN_Set_Param(cSPIN_TVAL_HOLD, Tval_Current_to_Par(40));

	/* Send cSPIN command change run current to 200mA */
		cSPIN_Set_Param(cSPIN_TVAL_RUN, Tval_Current_to_Par(200));
#endif /* defined(L6482) */
	
		/* Run constant speed of 50 steps/s reverse direction */
		cSPIN_Run(REV, Speed_Steps_to_Par(50));

		/* Wait few seconds - motor turns */
		cSPIN_Delay(0x004FFFFF);
	
		/* Perform SoftStop commmand */
		cSPIN_Soft_Stop();

#if defined(L6480)
        /* RESET KVAL_HOLD to initial value */
		cSPIN_Set_Param(cSPIN_KVAL_HOLD, Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_HOLD));
	
		/* RESET KVAL_RUN to initial value */
		cSPIN_Set_Param(cSPIN_KVAL_RUN, Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_RUN));
#endif /* defined(L6480) */
#if defined(L6482)
        /* RESET TVAL_HOLD to initial value */
		cSPIN_Set_Param(cSPIN_TVAL_HOLD, Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_HOLD));
	
		/* RESET TVAL_RUN to initial value */
		cSPIN_Set_Param(cSPIN_TVAL_RUN, Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_RUN));
#endif /* defined(L6482) */

		/* Wait until not busy - busy status check in Status register */
		while(cSPIN_Busy_SW());
	
		/* Move by 100,000 steps forward, range 0 to 4,194,303 */
		cSPIN_Move(FWD, (uint32_t)(100000));
	
		/* Wait until not busy */
		while(cSPIN_Busy_SW());

		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);
	
		/* Test of the Flag pin by polling, wait in endless cycle if problem is detected */
		if(cSPIN_Flag()) while(1);
	
		/* Issue cSPIN Go Home command */
		cSPIN_Go_Home();
		/* Wait untill not busy - busy pin test */
		while(cSPIN_Busy_HW());

		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);

		/* Issue cSPIN Go To command */
		cSPIN_Go_To(0x0000FFFF);
		/* Wait untill not busy - busy pin test */
		while(cSPIN_Busy_HW());

		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);

		/* Issue cSPIN Go To command */
		cSPIN_Go_To_Dir(FWD, 0x0001FFFF);
		/* Wait untill not busy - busy pin test */
		while(cSPIN_Busy_HW());

		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);
	
#if defined(L6480)
		/* Read run duty cycle (cSPIN_KVAL_RUN) parameter from cSPIN */
		cSPIN_rx_data = cSPIN_Get_Param(cSPIN_KVAL_RUN);
		
		/* Read intersect speed (cSPIN_INT_SPD) parameter from cSPIN */
		cSPIN_rx_data = cSPIN_Get_Param(cSPIN_INT_SPD);
	#endif /* defined(L6480) */
	#if defined(L6482)
		/* Read run current (cSPIN_TVAL_RUN) parameter from cSPIN */
		cSPIN_rx_data = cSPIN_Get_Param(cSPIN_TVAL_RUN);
	#endif /* defined(L6482) */

		/* Read Status register content */
		cSPIN_rx_data = cSPIN_Get_Status();
		
		/* Read absolute position (cSPIN_ABS_POS) parameter from cSPIN */
		cSPIN_rx_data = cSPIN_Get_Param(cSPIN_ABS_POS);

		/* Reset position counter */
		cSPIN_Reset_Pos();

		/* Read absolute position (cSPIN_ABS_POS) parameter from cSPIN */
		cSPIN_rx_data = cSPIN_Get_Param(cSPIN_ABS_POS);

		/* Issue cSPIN Hard HiZ command - disable power stage (High Impedance) */
		cSPIN_Hard_HiZ();
 
		/**********************************************************************/
		/* Start example of GoUntil Command */
		/**********************************************************************/
		/* Configure Interrupt for switch motor in case the MCU is used to pilot the switch */
		cSPIN_Switch_Motor_Interrupt_Config();        
		/* Motion in FW direction at speed 400steps/s via GoUntil command*/
		/* When SW is closed:  */
		/*    As ACT is set to ACTION_COPY, ABS_POS is saved to MARK register */
		/*    then a soft stop is done          */
		cSPIN_Go_Until(ACTION_COPY, FWD, Speed_Steps_to_Par(400));
		
		/* Waiting for soft Stop after GoUntil command*/
		while(cSPIN_Busy_HW())
		/* User action needed to go further */
		/* User attention drawn by toggling a LED */
#if defined(STEVAL_PCC009V2)
		{
			cSPIN_Gpio_Toggle(POWER_LED_Port, POWER_LED_Pin);
			cSPIN_Delay(0x100000);
		}
		GPIO_SetBits(POWER_LED_Port, POWER_LED_Pin);
#endif        
#if defined(ST_CSPIN_6480H_DISCOVERY)
		{
			cSPIN_Gpio_Toggle(LED_SPARE_Port, LED_SPARE_Pin); 
			cSPIN_Delay(0x100000);
		}
		GPIO_ResetBits(LED_SPARE_Port, LED_SPARE_Pin);
#endif
		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);
	
		/* Move by 50,000 steps in reverse direction, range 0 to 4,194,303 */
		cSPIN_Move(REV, (uint32_t)(50000));
	 
		/* Waiting for end of move command*/
		while(cSPIN_Busy_HW());
        
		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);
		
		/* Go to Mark saved with GoUntil command */
		cSPIN_Go_Mark();
        
        /* Wait until not busy - busy pin test */
		while(cSPIN_Busy_HW());

		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);
        
		/**********************************************************************/
		/* End example of GoUntil Command */
		/**********************************************************************/
		
		/**********************************************************************/
		/* Start example of ReleaseSw Command */
		/**********************************************************************/
		/* Motion via cSPIN_Release_SW command in REV direction at minimum speed*/
		/* (or  5 steps/s is minimum speed is < 5step/s)*/
		/* When SW is opened :  */
		/*    As ACT is set to ACTION_RESET, ABS_POS is reset i.e Home position is set */
		/*    then a soft stop is done          */
		cSPIN_Release_SW(ACTION_RESET, REV);
		
		/* Waiting for soft Stop after ReleaseSw command*/
		while(cSPIN_Busy_HW())
		/* User action needed to go further */
		/* User attention drawn by toggling a LED */
#if defined(STEVAL_PCC009V2)
        {
          cSPIN_Gpio_Toggle(POWER_LED_Port, POWER_LED_Pin);
          cSPIN_Delay(0x100000);
        }
        GPIO_SetBits(POWER_LED_Port, POWER_LED_Pin);
#endif        
#if defined(ST_CSPIN_6480H_DISCOVERY)
        {
          cSPIN_Gpio_Toggle(LED_SPARE_Port, LED_SPARE_Pin); 
          cSPIN_Delay(0x100000);
        }
        GPIO_ResetBits(LED_SPARE_Port, LED_SPARE_Pin);
#endif    
        
        /* Wait a few seconds, LED busy is off */
        cSPIN_Delay(0x004FFFFF);
        
        /* Move by 100,000 steps forward, range 0 to 4,194,303 */
        cSPIN_Move(FWD, (uint32_t)(100000));
       
        /* Waiting for end of move command*/
		while(cSPIN_Busy_HW());
        
		/* Wait a few seconds, LED busy is off */
		cSPIN_Delay(0x004FFFFF);        
		
		/* Go to Home set with ReleaseSW command */
		cSPIN_Go_Home();
		
       /* Wait until not busy - busy pin test */
		while(cSPIN_Busy_HW());
        
        /* Wait a few seconds, LED busy is off */
        cSPIN_Delay(0x004FFFFF);
        /**********************************************************************/
        /* End example of ReleaseSw Command */
        /**********************************************************************/
    
         /* Get Status to clear FLAG due to switch turn-on event (falling edge on SW pin) */
        cSPIN_rx_data = cSPIN_Get_Status();       
        
        /**********************************************************************/
        /* Start example of StepClock Command */
        /**********************************************************************/
        cSPIN_Busy_Interrupt_GPIO_DeConfig();
        /* Enable Step Clock Mode */
        cSPIN_Step_Clock(FWD);
        cSPIN_Busy_Interrupt_GPIO_Config();
        /* Wait a few seconds, LED busy is off */
        cSPIN_Delay(0x004FFFFF);
        
        /* Set PWM period to 500 so PWM Frequency = 1MHz/ 500 = 2KHz */
        /* and so, motor moves at 2000 steps/s */
        cSPIN_PWM_Enable(500);
        cSPIN_Delay(0x00FFFFFF);
        cSPIN_PWM_DISABLE();
        /**********************************************************************/
        /* End example of StepClock Command */
        /**********************************************************************/
      }
      else /* (daisy_chain == 0) */
      {
        /**********************************************************************/
        /* Start example of DAISY CHAINING */
        /**********************************************************************/
        /* Structure initialization by default values, in order to avoid blank records */
        for (i=0;i<number_of_slaves;i++)
        {
					cSPIN_Regs_Struct_Reset(&cSPIN_RegsStructArray[i]);
        }
        
        /* Setting of parameters for ALL DEVICES */
        for (i=0;i<number_of_slaves;i++)
        {
          cSPIN_RegsStructArray[i].ACC 		= AccDec_Steps_to_Par(ACC[i]);
          cSPIN_RegsStructArray[i].DEC 		= AccDec_Steps_to_Par(DEC[i]);
          cSPIN_RegsStructArray[i].MAX_SPEED 	= MaxSpd_Steps_to_Par(MAX_SPEED[i]);
          cSPIN_RegsStructArray[i].FS_SPD 	= FSSpd_Steps_to_Par(FS_SPD[i]);
#if defined(L6480)
          cSPIN_RegsStructArray[i].MIN_SPEED	= LSPD_BIT[i]|MinSpd_Steps_to_Par(MIN_SPEED[i]);
          cSPIN_RegsStructArray[i].KVAL_ACC 	= Kval_Perc_to_Par(KVAL_ACC[i]);
          cSPIN_RegsStructArray[i].KVAL_DEC 	= Kval_Perc_to_Par(KVAL_DEC[i]);		
          cSPIN_RegsStructArray[i].KVAL_RUN 	= Kval_Perc_to_Par(KVAL_RUN[i]);
          cSPIN_RegsStructArray[i].KVAL_HOLD 	= Kval_Perc_to_Par(KVAL_HOLD[i]);
          cSPIN_RegsStructArray[i].K_THERM 	= KTherm_to_Par(K_THERM[i]);
          cSPIN_RegsStructArray[i].INT_SPD 	= IntSpd_Steps_to_Par(INT_SPD[i]);
          cSPIN_RegsStructArray[i].ST_SLP 	= BEMF_Slope_Perc_to_Par(ST_SLP[i]);
          cSPIN_RegsStructArray[i].FN_SLP_ACC   = BEMF_Slope_Perc_to_Par(FN_SLP_ACC[i]);
          cSPIN_RegsStructArray[i].FN_SLP_DEC   = BEMF_Slope_Perc_to_Par(FN_SLP_DEC[i]);
          cSPIN_RegsStructArray[i].STALL_TH 	= StallTh_to_Par(STALL_TH[i]);
          cSPIN_RegsStructArray[i].CONFIG 	= (uint16_t)CONFIG_CLOCK_SETTING[i] |
                                                  (uint16_t)CONFIG_SW_MODE[i] | \
                                                  (uint16_t)CONFIG_VS_COMP[i] | \
                                                  (uint16_t)CONFIG_OC_SD[i]   | \
                                                  (uint16_t)CONFIG_UVLOVAL[i] | \
                                                  (uint16_t)CONFIG_VCCVAL[i]  | \
                                                  (uint16_t)CONFIG_PWM_DIV[i] | \
                                                  (uint16_t)CONFIG_PWM_MUL[i];
#endif /* defined(L6480) */
#if defined(L6482)          
          cSPIN_RegsStructArray[i].MIN_SPEED	= MinSpd_Steps_to_Par(MIN_SPEED[i]);
          cSPIN_RegsStructArray[i].TVAL_ACC 	= Tval_Current_to_Par(TVAL_ACC[i]);
          cSPIN_RegsStructArray[i].TVAL_DEC 	= Tval_Current_to_Par(TVAL_DEC[i]);		
          cSPIN_RegsStructArray[i].TVAL_RUN 	= Tval_Current_to_Par(TVAL_RUN[i]);
          cSPIN_RegsStructArray[i].TVAL_HOLD 	= Tval_Current_to_Par(TVAL_HOLD[i]);
          cSPIN_RegsStructArray[i].TON_MIN 	= Tmin_Time_to_Par(TON_MIN[i]);		
          cSPIN_RegsStructArray[i].TOFF_MIN 	= Tmin_Time_to_Par(TOFF_MIN[i]);
          cSPIN_RegsStructArray[i].T_FAST 	= (uint8_t)TOFF_FAST[i] | (uint8_t)FAST_STEP[i];          
          cSPIN_RegsStructArray[i].CONFIG 	= (uint16_t)CONFIG_CLOCK_SETTING[i] | \
                                                  (uint16_t)CONFIG_SW_MODE[i] | \
                                                  (uint16_t)CONFIG_TQ_REG[i]  | \
                                                  (uint16_t)CONFIG_OC_SD[i]   | \
                                                  (uint16_t)CONFIG_UVLOVAL[i] | \
                                                  (uint16_t)CONFIG_VCCVAL[i]  | \
                                                  (uint16_t)CONFIG_TSW[i]     | \
                                                  (uint16_t)CONFIG_PRED_EN[i];
#endif /* defined(L6482) */
          cSPIN_RegsStructArray[i].OCD_TH 	= OCD_TH[i];
          cSPIN_RegsStructArray[i].ALARM_EN 	= ALARM_EN[i];
          cSPIN_RegsStructArray[i].STEP_MODE 	= (uint8_t)SYNC_MODE[i] | (uint8_t)STEP_MODE[i];
          cSPIN_RegsStructArray[i].GATECFG1     = (uint16_t)GATECFG1_WD_EN[i]  | \
                                                  (uint16_t)GATECFG1_TBOOST[i] | \
                                                  (uint16_t)GATECFG1_IGATE[i]  | \
                                                  (uint16_t)GATECFG1_TCC[i];
          cSPIN_RegsStructArray[i].GATECFG2     = (uint16_t)GATECFG2_TBLANK[i] | \
                                                  (uint16_t)GATECFG2_TDT[i];
        }

        /* Program all cSPIN registers of All Devices */
        cSPIN_All_Slaves_Registers_Set(number_of_slaves, &cSPIN_RegsStructArray[0]);
  
        /* Get status of all devices, clear FLAG pin */
        cSPIN_All_Slaves_Get_Status(number_of_slaves, responseArray);
        
        if (number_of_slaves > 1)
        {
          /* Initialization of command array with NOP instruction */
          for (i=0;i<number_of_slaves;i++)
          {
            commandArray[i] = cSPIN_NOP; 
          }

          /* Move DEVICE 1, keep other slaves stopped */
          cSPIN_One_Slave_Move(DEVICE_1, number_of_slaves, FWD, 150000);
         
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());

          /* Move DEVICE 1, keep other slaves stopped */
          cSPIN_One_Slave_Move(DEVICE_2, number_of_slaves, FWD, 60000);
         
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
        
          /* Move DEVICE 1 by 60000 steps in reverse direction */
          /* Run DEVICE 2 at 400 steps/s in forward direction */
          /* No operation for other slaves */
          commandArray[DEVICE_1] = (uint8_t) cSPIN_MOVE |(uint8_t) REV;
          argumentArray[DEVICE_1] = 60000;
          commandArray[DEVICE_2] = (uint8_t) cSPIN_RUN |(uint8_t) FWD;
          argumentArray[DEVICE_2] = Speed_Steps_to_Par(400);
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, argumentArray);

          /* Wait until not busy - busy pin test */
          /* DEVICE 1 and DEVICE 2 turns in opposite directions */
          while(cSPIN_Busy_HW());

          /* Wait few seconds - DEVICE 1 is stopped, DEVICE 2 turns forward */
          cSPIN_Delay(0x00FFFFFF);

          /* Move DEVICE 1 to HOME position via the shortest path */
          /* Run DEVICE 2 at 150 steps/s in reverse direction */
          /* No operation for other slaves */
          commandArray[DEVICE_1] = (uint8_t) cSPIN_GO_HOME;
          commandArray[DEVICE_2] = (uint8_t) cSPIN_RUN |(uint8_t) REV;
          argumentArray[DEVICE_2] = Speed_Steps_to_Par(150);
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, argumentArray);          
          
          /* Wait until not busy - busy pin test */
          /* DEVICE 1 goes to zero position, turning reverse in this example */
          /* DEVICE 2 changes direction to turn reverse */
          while(cSPIN_Busy_HW());        
 
          /* Wait few seconds - DEVICE 1 is stopped, DEVICE 2 turns reverse */
          cSPIN_Delay(0x00FFFFFF);
        
          /* No change for DEVICE 1 */
          /* Stop DEVICE 2 */
          /* No operation for other slaves */
          commandArray[DEVICE_1] = cSPIN_NOP;
          commandArray[DEVICE_2] = cSPIN_SOFT_STOP;         
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, argumentArray);
        
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
        }
        else /* (number_of_slaves > 1) */
        {
          /* Move by 60,000 steps rorward, range 0 to 4,194,303 */
          cSPIN_One_Slave_Move(number_of_slaves, DEVICE_1, FWD, 60000);
	
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
	
#if defined(L6480)
/* Send cSPIN command change hold duty cycle to 0.5% */
          commandArray[DEVICE_1] = cSPIN_KVAL_HOLD;
          argumentArray[DEVICE_1] = Kval_Perc_to_Par(0.5);
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);
	
          /* Send cSPIN command change run duty cycle to 5% */
          commandArray[DEVICE_1] = cSPIN_KVAL_RUN;
          argumentArray[DEVICE_1] = Kval_Perc_to_Par(5);
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);        
#endif /* defined(L6480) */
#if defined(L6482)
          /* Send cSPIN command change hold reference voltage to 16mV */
          commandArray[DEVICE_1] = cSPIN_TVAL_HOLD;
          argumentArray[DEVICE_1] = Tval_Current_to_Par(16);
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);

          /* Send cSPIN command change run reference voltage to 48mV */
          commandArray[DEVICE_1] = cSPIN_TVAL_RUN;
          argumentArray[DEVICE_1] = Tval_Current_to_Par(48);
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);        
#endif /* defined(L6482) */
          /* Run constant speed of 50 steps/s reverse direction */
          cSPIN_One_Slave_Run(DEVICE_1, number_of_slaves, REV, Speed_Steps_to_Par(50));

          /* Wait few seconds - motor turns */
          cSPIN_Delay(0x00FFFFFF);
	
          /* Perform SoftStop commmand */
          commandArray[DEVICE_1] = cSPIN_SOFT_STOP;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);
#if defined(L6480)
          /* RESET KVAL_HOLD to initial value */
          commandArray[DEVICE_1] = cSPIN_KVAL_HOLD;
          argumentArray[DEVICE_1] = cSPIN_RegsStructArray[DEVICE_1].KVAL_HOLD;
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);
	
          /* RESET KVAL_RUN to initial value */
          commandArray[DEVICE_1] = cSPIN_KVAL_RUN;
          argumentArray[DEVICE_1] = cSPIN_RegsStructArray[DEVICE_1].KVAL_RUN;
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);
#endif /* defined(L6480) */
#if defined(L6482)
          /* RESET TVAL_HOLD to initial value */
          commandArray[DEVICE_1] = cSPIN_TVAL_HOLD;
          argumentArray[DEVICE_1] = cSPIN_RegsStructArray[DEVICE_1].TVAL_HOLD;
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);
        
          /* RESET TVAL_RUN to initial value */
          commandArray[DEVICE_1] = cSPIN_TVAL_RUN;
          argumentArray[DEVICE_1] = cSPIN_RegsStructArray[DEVICE_1].TVAL_RUN;
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);
#endif /* defined(L6482) */ 
          /* Wait until not busy - busy status check in Status register */
          while(cSPIN_Busy_SW());
	
          /* Move by 100,000 steps forward, range 0 to 4,194,303 */
          cSPIN_One_Slave_Move(DEVICE_1, number_of_slaves, FWD, (uint32_t)(100000));        
	
          /* Wait until not busy */
          while(cSPIN_One_Or_More_Slaves_Busy_SW(number_of_slaves));
	
          /* Test of the Flag pin by polling, wait in endless cycle if problem is detected */
          if(cSPIN_Flag()) while(1);
	
          /* Issue cSPIN Go Home command */
          commandArray[DEVICE_1] = (uint8_t) cSPIN_GO_HOME;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);

          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
	
          /* Issue cSPIN Go To command */
          commandArray[DEVICE_1] = cSPIN_GO_TO;
          argumentArray[DEVICE_1] = 0x0000FFFF;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, argumentArray);
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
	
          /* Issue cSPIN Go To command */
          commandArray[DEVICE_1] = (uint8_t)cSPIN_GO_TO_DIR | (uint8_t)FWD;
          argumentArray[DEVICE_1] = 0x0001FFFF;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, argumentArray);
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
#if defined(L6480)
          /* Read run duty cycle (cSPIN_KVAL_RUN) parameter from cSPIN */
          commandArray[DEVICE_1] = cSPIN_KVAL_RUN;
          cSPIN_All_Slaves_Get_Param(number_of_slaves, commandArray, responseArray);

          /* Read intersect speed (cSPIN_INT_SPD) parameter from cSPIN */
          commandArray[DEVICE_1] = cSPIN_INT_SPD;
          cSPIN_All_Slaves_Get_Param(number_of_slaves, commandArray, responseArray);
#endif /* defined(L6480) */
#if defined(L6482)
          /* Read run reference voltage (cSPIN_TVAL_RUN) parameter from cSPIN */
          commandArray[DEVICE_1] = cSPIN_TVAL_RUN;
          cSPIN_All_Slaves_Get_Param(number_of_slaves, commandArray, responseArray);
#endif /* defined(L6482) */
          /* Read Status register content */
          cSPIN_All_Slaves_Get_Status(number_of_slaves, responseArray);
	
          /* Read absolute position (cSPIN_ABS_POS) parameter from cSPIN */
          commandArray[DEVICE_1] = cSPIN_ABS_POS;
          cSPIN_All_Slaves_Get_Param(number_of_slaves, commandArray, responseArray);

          /* Reset position counter */
          commandArray[DEVICE_1] = cSPIN_RESET_POS;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);   

          /* Read absolute position (cSPIN_ABS_POS) parameter from cSPIN */
          commandArray[DEVICE_1] = cSPIN_ABS_POS;
          cSPIN_All_Slaves_Get_Param(number_of_slaves, commandArray, responseArray);

          /* Issue cSPIN Hard HiZ command - disable power stage (High Impedance) */
          commandArray[DEVICE_1] = cSPIN_HARD_HIZ;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);
 
          /********************************************************************/
          /* Start example of GoUntil Command */
          /********************************************************************/
          /* Configure Interrupt for switch motor in case the MCU is used to pilot the switch */
          cSPIN_Switch_Motor_Interrupt_Config();
          /* Motion in FW direction at speed 400steps/s via GoUntil command*/
          /* When SW is closed:  */
          /*    As ACT is set ot ACTION_COPY, ABS_POS is saved to MARK register */
          /*    then a soft stop is done          */
          commandArray[DEVICE_1] = (uint8_t)cSPIN_GO_UNTIL | (uint8_t)ACTION_COPY | (uint8_t)FWD;
          argumentArray[DEVICE_1] = Speed_Steps_to_Par(400);
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, argumentArray);
        
          /* Waiting for soft Stop after GoUntil command*/
          while(cSPIN_Busy_HW())
          /* User action needed to go further */
          /* User attention drawn by toggling a LED */
#if defined(STEVAL_PCC009V2)
          {
            cSPIN_Gpio_Toggle(POWER_LED_Port, POWER_LED_Pin);
            cSPIN_Delay(0x00100000);
          }
          GPIO_SetBits(POWER_LED_Port, POWER_LED_Pin);
#endif        
#if defined(ST_CSPIN_6480H_DISCOVERY)
          {
            cSPIN_Gpio_Toggle(LED_SPARE_Port, LED_SPARE_Pin); 
            cSPIN_Delay(0x00100000);
          }
          GPIO_ResetBits(LED_SPARE_Port, LED_SPARE_Pin);
#endif           

          /* Move by 50,000 steps in reverse direction, range 0 to 4,194,303 */
          cSPIN_One_Slave_Move(DEVICE_1, number_of_slaves, REV, (uint32_t)(50000));
       
          /* Waiting for end of move command*/
          while(cSPIN_Busy_HW());
        
          /* Go to Mark saved with GoUntil command */
          commandArray[DEVICE_1] = cSPIN_GO_MARK;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);
        
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
        
          /********************************************************************/
          /* End example of GoUntil Command */
          /********************************************************************/
        
          cSPIN_Delay(0x00FFFFFF);
        
          /********************************************************************/
          /* Start example of ReleaseSw Command */
          /********************************************************************/
          /* Motion via cSPIN_Release_SW command in REV direction at minimum speed*/
          /* (or  5 steps/s is minimum speed is < 5step/s)*/
          /* When SW is opened:  */
          /*    As ACT is set ot ACTION_RESET, ABS_POS is reset i.e Home position is set */
          /*    then a soft stop is done          */
          commandArray[DEVICE_1] = (uint8_t)cSPIN_RELEASE_SW | (uint8_t)ACTION_RESET | (uint8_t)REV;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);          
        
          /* Waiting for soft Stop after ReleaseSw command*/
          while(cSPIN_Busy_HW())
          /* User action needed to go further */
          /* User attention drawn by toggling a LED */
#if defined(STEVAL_PCC009V2)
          {
            cSPIN_Gpio_Toggle(POWER_LED_Port, POWER_LED_Pin);
            cSPIN_Delay(0x00100000);
          }
          GPIO_SetBits(POWER_LED_Port, POWER_LED_Pin);
#endif        
#if defined(ST_CSPIN_6480H_DISCOVERY)
          {
            cSPIN_Gpio_Toggle(LED_SPARE_Port, LED_SPARE_Pin); 
            cSPIN_Delay(0x00100000);
          }
          GPIO_ResetBits(LED_SPARE_Port, LED_SPARE_Pin);
#endif

          /* Move by 100,000 steps forward, range 0 to 4,194,303 */
          cSPIN_One_Slave_Move(DEVICE_1, number_of_slaves, FWD, (uint32_t)(100000));
       
          /* Waiting for end of move command*/
          while(cSPIN_Busy_HW());
        
          /* Go to Home set with ReleaseSW command */
          commandArray[DEVICE_1] = (uint8_t) cSPIN_GO_HOME;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);
        
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
          /********************************************************************/
          /* End example of ReleaseSw Command */
          /********************************************************************/
    
          /********************************************************************/
          /* Start example of StepClock Command */
          /********************************************************************/
        
          /* Enable Step Clock Mode */
          commandArray[DEVICE_1] = (uint8_t)cSPIN_STEP_CLOCK | (uint8_t)FWD;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);
        
          /* Set PWM period to 500 so PWM Frequency = 1MHz/ 500 = 2KHz */
          /* and so, motor moves at 2000 steps/s */
          cSPIN_PWM_Enable(500);
          cSPIN_Delay(0x00FFFFFF);
          cSPIN_PWM_DISABLE();
        
          /* Get Status to clear FLAG due to step clock mode */
          cSPIN_All_Slaves_Get_Status(number_of_slaves, responseArray);
        
          /********************************************************************/
          /* End example of StepClock Command */
          /********************************************************************/

          /********************************************************************/
          /* Start example of FLAG interrupt management */
          /********************************************************************/
          /* Interrupt configuration for FLAG signal */
          cSPIN_Flag_Interrupt_GPIO_Config();
          /* Run constant speed of 400 steps/s forward direction */
          cSPIN_One_Slave_Run(DEVICE_1, number_of_slaves, FWD, Speed_Steps_to_Par(400));        
        
          /* Tentative to write to the current motor absolute position register */
          /* while the motor is running */
          commandArray[DEVICE_1] = cSPIN_ABS_POS;
          argumentArray[DEVICE_1] = 100;
          cSPIN_All_Slaves_Set_Param(number_of_slaves, commandArray, argumentArray);       
          cSPIN_Delay(0x00FFFFFF);
          /* Get Status to clear FLAG due to non-performable command */
          cSPIN_All_Slaves_Get_Status(number_of_slaves, responseArray);
          /* Perform SoftStop commmand */
          commandArray[DEVICE_1] = cSPIN_SOFT_STOP;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);
          /* Wait until not busy - busy pin test */
          while(cSPIN_Busy_HW());
          /********************************************************************/
          /* End example of FLAG interrupt management */
          /********************************************************************/
      
          /********************************************************************/
          /* Start example of BUSY interrupt management */
          /********************************************************************/
          /* Interrupt configuration for BUSY signal */
          cSPIN_Busy_Interrupt_GPIO_Config();
          /* Move by 100,000 steps forward, range 0 to 4,194,303 */
          cSPIN_One_Slave_Move(DEVICE_1, number_of_slaves, REV, (uint32_t)(100000));
          /* During busy time the POWER LED is switched OFF */
          /* Wait until not busy - busy pin test */
	  while(cSPIN_Busy_HW());
          /* Disable the power bridges */
          commandArray[DEVICE_1] = cSPIN_SOFT_HIZ;
          cSPIN_All_Slaves_Send_Command(number_of_slaves, commandArray, 0);
          /********************************************************************/
          /* End example of BUSY interrupt management */
          /********************************************************************/
        }  /* (number_of_slaves > 1) */
        /**********************************************************************/
        /* End example of DAISY CHAINING */
        /**********************************************************************/
      }  /* (daisy_chain == 0) */

      /************************************************************************/
      /* Start example of motor control using button */
      /************************************************************************/
#if defined(STEVAL_PCC009V2)
      cSPIN_Busy_Interrupt_GPIO_DeConfig();
#endif /* defined(STEVAL_PCC009V2) */
      cSPIN_Buttons_Interrupts_GPIO_Config();
      /************************************************************************/
      /* End example of motor control using button */
      /************************************************************************/        
      while(1);
}

void L6480_Reset_And_Standby(uint8_t motor)
{
	switch(motor)
	{
		case 1:				GPIO_ResetBits(cSPIN_STBY_RESET1_Port, cSPIN_STBY_RESET1_Pin);
									cSPIN_Delay(10000);
									GPIO_SetBits(cSPIN_STBY_RESET1_Port, cSPIN_STBY_RESET1_Pin);
			break;
		case 1:				GPIO_ResetBits(cSPIN_STBY_RESET1_Port, cSPIN_STBY_RESET1_Pin);
									cSPIN_Delay(10000);
									GPIO_SetBits(cSPIN_STBY_RESET1_Port, cSPIN_STBY_RESET1_Pin);
			break;
		case 3:
			break;
		

	}
}

