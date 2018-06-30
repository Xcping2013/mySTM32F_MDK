
#ifndef __L6480_CONFIG_H
#define __L6480_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "mcu_spi.h"
#include "mcu_exit.h"

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
#define cSPIN_CONF_PARAM_ACC (2008.16)

/* Register : DEC */
/* Deceleration rate in step/s2, range 14.55 to 59590 steps/s2 */
#define cSPIN_CONF_PARAM_DEC (2008.16)

/* Register : MAX_SPEED */
/* Maximum speed in step/s, range 15.25 to 15610 steps/s */
#define cSPIN_CONF_PARAM_MAX_SPEED (991.82)

/* Register : MIN_SPEED */
/* Minimum speed in step/s, range 0 to 976.3 steps/s */
#define cSPIN_CONF_PARAM_MIN_SPEED (0)

/* Register : FS_SPD */
/* Full step speed in step/s, range 7.63 to 15625 steps/s */
#define cSPIN_CONF_PARAM_FS_SPD (595.09)

/************************ Phase Current Control *****************************/
/* Register : KVAL_HOLD */
/* Hold duty cycle (torque) in %, range 0 to 99.6% */
#define cSPIN_CONF_PARAM_KVAL_HOLD (16.02)

/* Register : KVAL_RUN */
/* Run duty cycle (torque) in %, range 0 to 99.6% */
#define cSPIN_CONF_PARAM_KVAL_RUN (16.02)

/* Register : KVAL_ACC */
/* Acceleration duty cycle (torque) in %, range 0 to 99.6% */
#define cSPIN_CONF_PARAM_KVAL_ACC (16.02)

/* Register : KVAL_DEC */
/* Deceleration duty cycle (torque) in %, range 0 to 99.6% */
#define cSPIN_CONF_PARAM_KVAL_DEC (16.02)

/* Register : CONFIG - field : EN_VSCOMP */
/* Motor Supply Voltage Compensation enabling , enum cSPIN_CONFIG_EN_VSCOMP_TypeDef */
#define cSPIN_CONF_PARAM_VS_COMP (cSPIN_CONFIG_VS_COMP_DISABLE)

/* Register : MIN_SPEED - field : LSPD_OPT */
/* Low speed optimization bit, enum cSPIN_LSPD_OPT_TypeDef */
#define cSPIN_CONF_PARAM_LSPD_BIT (cSPIN_LSPD_OPT_OFF)

/* Register : K_THERM */
/* Thermal compensation param, range 1 to 1.46875 */
#define cSPIN_CONF_PARAM_K_THERM (1)

/* Register : INT_SPEED */
/* Intersect speed settings for BEMF compensation in steps/s, range 0 to 3906 steps/s */
#define cSPIN_CONF_PARAM_INT_SPD (61.512)

/* Register : ST_SLP */
/* BEMF start slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step */
#define cSPIN_CONF_PARAM_ST_SLP (0.03815)

/* Register : FN_SLP_ACC */
/* BEMF final acc slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step  */
#define cSPIN_CONF_PARAM_FN_SLP_ACC (0.06256)

/* Register : FN_SLP_DEC */
/* BEMF final dec slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step */
#define cSPIN_CONF_PARAM_FN_SLP_DEC (0.06256)

/* Register : CONFIG - field : F_PWM_INT */
/* PWM Frequency Integer division, enum cSPIN_CONFIG_F_PWM_INT_TypeDef */
#define cSPIN_CONF_PARAM_PWM_DIV (cSPIN_CONFIG_PWM_DIV_2)

/* Register : CONFIG - field : F_PWM_DEC */
/* PWM Frequency Integer Multiplier, enum cSPIN_CONFIG_F_PWM_INT_TypeDef */
#define cSPIN_CONF_PARAM_PWM_MUL (cSPIN_CONFIG_PWM_MUL_1)

/******************************* Gate Driving **********************************/
/* Register : GATECFG1 - field : TBOOST */
/* Duration of the overboost phase during gate turn-off via enum cSPIN_TBOOST_TypeDef */
#define cSPIN_CONF_PARAM_TBOOST     (cSPIN_TBOOST_0ns)

/* Register : GATECFG1 - field : IGATE */
/* Gate sink/source current via enum cSPIN_IGATE_TypeDef */
#define cSPIN_CONF_PARAM_IGATE      (cSPIN_IGATE_4mA)

/* Register : GATECFG1 - field : TCC */
/* Controlled current time via enum cSPIN_TCC_TypeDef */
#define cSPIN_CONF_PARAM_TCC        (cSPIN_TCC_125ns)

/* Register : GATECFG2 - field : TBLANK */
/* Duration of the blanking time via enum cSPIN_TBLANK_TypeDef */
#define cSPIN_CONF_PARAM_TBLANK     (cSPIN_TBLANK_125ns)

/* Register : GATECFG2 - field : TDT */
/* Duration of the dead time via enum cSPIN_TDT_TypeDef */
#define cSPIN_CONF_PARAM_TDT        (cSPIN_TDT_125ns)

/* Register : CONFIG - field : VCCVAL */
/* VCC Val, enum cSPIN_CONFIG_VCCVAL_TypeDef */
#define cSPIN_CONF_PARAM_VCCVAL (cSPIN_CONFIG_VCCVAL_7_5V)

/* Register : CONFIG - field : UVLOVAL */
/* UVLO Threshold via cSPIN_CONFIG_UVLOVAL_TypeDef */
#define cSPIN_CONF_PARAM_UVLOVAL    (cSPIN_CONFIG_UVLOVAL_LOW)

/******************************* Others *************************************/
/* Register : OCD_TH */
/* Overcurrent threshold settings via enum cSPIN_OCD_TH_TypeDef*/
#define cSPIN_CONF_PARAM_OCD_TH (cSPIN_OCD_TH_281_25mV)

/* Register : STALL_TH */
/* Stall threshold settings in mV, range 31.25mV to 1000mV */
#define cSPIN_CONF_PARAM_STALL_TH (531.25)

/* Register : ALARM_EN */
/* Alarm settings via bitmap enum cSPIN_ALARM_EN_TypeDef */
#define cSPIN_CONF_PARAM_ALARM_EN (cSPIN_ALARM_EN_OVERCURRENT | cSPIN_ALARM_EN_THERMAL_SHUTDOWN | cSPIN_ALARM_EN_THERMAL_WARNING | cSPIN_ALARM_EN_UVLO | cSPIN_ALARM_EN_ADC_UVLO | cSPIN_ALARM_EN_STALL_DETECTION | cSPIN_ALARM_EN_SW_TURN_ON | cSPIN_ALARM_EN_WRONG_NPERF_CMD)

/* Register : STEP_MODE - field : STEP_MODE */
/* Step mode settings via enum cSPIN_STEP_SEL_TypeDef */
#define cSPIN_CONF_PARAM_STEP_MODE (cSPIN_STEP_SEL_1_128)

/* Register : STEP_MODE - Field : SYNC_MODE and SYNC_EN */
/* Synch. Mode settings via enum cSPIN_SYNC_SEL_TypeDef */
#define cSPIN_CONF_PARAM_SYNC_MODE (cSPIN_SYNC_SEL_DISABLED)

/* Register : GATECFG1 - field : WD_EN */
/* External clock watchdog, enum cSPIN_WD_EN_TypeDef */
#define cSPIN_CONF_PARAM_WD_EN (cSPIN_WD_EN_DISABLE)

/* Register : CONFIG - field : OC_SD */
/* Over current shutwdown enabling, enum cSPIN_CONFIG_OC_SD_TypeDef */
#define cSPIN_CONF_PARAM_OC_SD (cSPIN_CONFIG_OC_SD_DISABLE)

/* Register : CONFIG - field : SW_MODE */
/* External switch hard stop interrupt mode, enum cSPIN_CONFIG_SW_MODE_TypeDef */
#define cSPIN_CONF_PARAM_SW_MODE (cSPIN_CONFIG_SW_HARD_STOP)

/* Register : CONFIG - field : OSC_CLK_SEL */
/* Clock setting , enum cSPIN_CONFIG_OSC_MGMT_TypeDef */
#define cSPIN_CONF_PARAM_CLOCK_SETTING (cSPIN_CONFIG_INT_16MHZ_OSCOUT_2MHZ)



#endif /* __CSPIN_CONFIG_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
