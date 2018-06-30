
#ifndef __L6480_CONFIG_H
#define __L6480_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#define N_O_MOTORS 3  

#define L6480_CS1_PIN			PB12
#define L6480_BUSY1_PIN		PD8
#define L6480_FLAG1_PIN		PD9
#define L6480_RESET1_PIN	PD10
#define L6480_SW1_PIN			PE12

#define L6480_CS2_PIN			PE2
#define L6480_BUSY2_PIN		PE5
#define L6480_FLAG2_PIN		PE4
#define L6480_RESET2_PIN	PE3
#define L6480_SW2_PIN			PB0

#define L6480_CS3_PIN			PD7
#define L6480_BUSY3_PIN		PD4
#define L6480_FLAG3_PIN		PD5
#define L6480_RESET3_PIN	PD6

#define L6480_SW3_PIN			PD11

/****************************************************************************/
/******************No Daisy Chain Mode **************************************/
/****************************************************************************/
/**************************** Speed Profile *********************************/
/* Register : ACC */
/* Acceleration rate in step/s2, range 14.55 to 59590 steps/s2 */
#define L6480_CONF_PARAM_ACC (2008.16)

/* Register : DEC */
/* Deceleration rate in step/s2, range 14.55 to 59590 steps/s2 */
#define L6480_CONF_PARAM_DEC (2008.16)

/* Register : MAX_SPEED */
/* Maximum speed in step/s, range 15.25 to 15610 steps/s */
#define L6480_CONF_PARAM_MAX_SPEED (991.82)

/* Register : MIN_SPEED */
/* Minimum speed in step/s, range 0 to 976.3 steps/s */
#define L6480_CONF_PARAM_MIN_SPEED (0)

/* Register : FS_SPD */
/* Full step speed in step/s, range 7.63 to 15625 steps/s */
#define L6480_CONF_PARAM_FS_SPD (595.09)

/************************ Phase Current Control *****************************/
/* Register : KVAL_HOLD */
/* Hold duty cycle (torque) in %, range 0 to 99.6% */
#define L6480_CONF_PARAM_KVAL_HOLD (16.02)

/* Register : KVAL_RUN */
/* Run duty cycle (torque) in %, range 0 to 99.6% */
#define L6480_CONF_PARAM_KVAL_RUN (16.02)

/* Register : KVAL_ACC */
/* Acceleration duty cycle (torque) in %, range 0 to 99.6% */
#define L6480_CONF_PARAM_KVAL_ACC (16.02)

/* Register : KVAL_DEC */
/* Deceleration duty cycle (torque) in %, range 0 to 99.6% */
#define L6480_CONF_PARAM_KVAL_DEC (16.02)

/* Register : CONFIG - field : EN_VSCOMP */
/* Motor Supply Voltage Compensation enabling , enum L6480_CONFIG_EN_VSCOMP_TypeDef */
#define L6480_CONF_PARAM_VS_COMP (L6480_CONFIG_VS_COMP_DISABLE)

/* Register : MIN_SPEED - field : LSPD_OPT */
/* Low speed optimization bit, enum L6480_LSPD_OPT_TypeDef */
#define L6480_CONF_PARAM_LSPD_BIT (L6480_LSPD_OPT_OFF)

/* Register : K_THERM */
/* Thermal compensation param, range 1 to 1.46875 */
#define L6480_CONF_PARAM_K_THERM (1)

/* Register : INT_SPEED */
/* Intersect speed settings for BEMF compensation in steps/s, range 0 to 3906 steps/s */
#define L6480_CONF_PARAM_INT_SPD (61.512)

/* Register : ST_SLP */
/* BEMF start slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step */
#define L6480_CONF_PARAM_ST_SLP (0.03815)

/* Register : FN_SLP_ACC */
/* BEMF final acc slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step  */
#define L6480_CONF_PARAM_FN_SLP_ACC (0.06256)

/* Register : FN_SLP_DEC */
/* BEMF final dec slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step */
#define L6480_CONF_PARAM_FN_SLP_DEC (0.06256)

/* Register : CONFIG - field : F_PWM_INT */
/* PWM Frequency Integer division, enum L6480_CONFIG_F_PWM_INT_TypeDef */
#define L6480_CONF_PARAM_PWM_DIV (L6480_CONFIG_PWM_DIV_2)

/* Register : CONFIG - field : F_PWM_DEC */
/* PWM Frequency Integer Multiplier, enum L6480_CONFIG_F_PWM_INT_TypeDef */
#define L6480_CONF_PARAM_PWM_MUL (L6480_CONFIG_PWM_MUL_1)

/******************************* Gate Driving **********************************/
/* Register : GATECFG1 - field : TBOOST */
/* Duration of the overboost phase during gate turn-off via enum L6480_TBOOST_TypeDef */
#define L6480_CONF_PARAM_TBOOST     (L6480_TBOOST_0ns)

/* Register : GATECFG1 - field : IGATE */
/* Gate sink/source current via enum L6480_IGATE_TypeDef */
#define L6480_CONF_PARAM_IGATE      (L6480_IGATE_4mA)

/* Register : GATECFG1 - field : TCC */
/* Controlled current time via enum L6480_TCC_TypeDef */
#define L6480_CONF_PARAM_TCC        (L6480_TCC_125ns)

/* Register : GATECFG2 - field : TBLANK */
/* Duration of the blanking time via enum L6480_TBLANK_TypeDef */
#define L6480_CONF_PARAM_TBLANK     (L6480_TBLANK_125ns)

/* Register : GATECFG2 - field : TDT */
/* Duration of the dead time via enum L6480_TDT_TypeDef */
#define L6480_CONF_PARAM_TDT        (L6480_TDT_125ns)

/* Register : CONFIG - field : VCCVAL */
/* VCC Val, enum L6480_CONFIG_VCCVAL_TypeDef */
#define L6480_CONF_PARAM_VCCVAL (L6480_CONFIG_VCCVAL_7_5V)

/* Register : CONFIG - field : UVLOVAL */
/* UVLO Threshold via L6480_CONFIG_UVLOVAL_TypeDef */
#define L6480_CONF_PARAM_UVLOVAL    (L6480_CONFIG_UVLOVAL_LOW)

/******************************* Others *************************************/
/* Register : OCD_TH */
/* Overcurrent threshold settings via enum L6480_OCD_TH_TypeDef*/
#define L6480_CONF_PARAM_OCD_TH (L6480_OCD_TH_281_25mV)

/* Register : STALL_TH */
/* Stall threshold settings in mV, range 31.25mV to 1000mV */
#define L6480_CONF_PARAM_STALL_TH (531.25)

/* Register : ALARM_EN */
/* Alarm settings via bitmap enum L6480_ALARM_EN_TypeDef */
#define L6480_CONF_PARAM_ALARM_EN (L6480_ALARM_EN_OVERCURRENT | L6480_ALARM_EN_THERMAL_SHUTDOWN | L6480_ALARM_EN_THERMAL_WARNING | L6480_ALARM_EN_UVLO | L6480_ALARM_EN_ADC_UVLO | L6480_ALARM_EN_STALL_DETECTION | L6480_ALARM_EN_SW_TURN_ON | L6480_ALARM_EN_WRONG_NPERF_CMD)

/* Register : STEP_MODE - field : STEP_MODE */
/* Step mode settings via enum L6480_STEP_SEL_TypeDef */
#define L6480_CONF_PARAM_STEP_MODE (L6480_STEP_SEL_1_128)

/* Register : STEP_MODE - Field : SYNC_MODE and SYNC_EN */
/* Synch. Mode settings via enum L6480_SYNC_SEL_TypeDef */
#define L6480_CONF_PARAM_SYNC_MODE (L6480_SYNC_SEL_DISABLED)

/* Register : GATECFG1 - field : WD_EN */
/* External clock watchdog, enum L6480_WD_EN_TypeDef */
#define L6480_CONF_PARAM_WD_EN (L6480_WD_EN_DISABLE)

/* Register : CONFIG - field : OC_SD */
/* Over current shutwdown enabling, enum L6480_CONFIG_OC_SD_TypeDef */
#define L6480_CONF_PARAM_OC_SD (L6480_CONFIG_OC_SD_DISABLE)

/* Register : CONFIG - field : SW_MODE */
/* External switch hard stop interrupt mode, enum L6480_CONFIG_SW_MODE_TypeDef */
#define L6480_CONF_PARAM_SW_MODE (L6480_CONFIG_SW_HARD_STOP)

/* Register : CONFIG - field : OSC_CLK_SEL */
/* Clock setting , enum L6480_CONFIG_OSC_MGMT_TypeDef */
#define L6480_CONF_PARAM_CLOCK_SETTING (L6480_CONFIG_INT_16MHZ_OSCOUT_2MHZ)



#endif /* __L6480_CONFIG_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
