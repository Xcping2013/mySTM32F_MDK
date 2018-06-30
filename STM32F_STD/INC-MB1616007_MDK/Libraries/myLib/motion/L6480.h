
#ifndef __L6480_H
#define __L6480_H

/* Includes ------------------------------------------------------------------*/
#include "l6480_config.h"
#include "mcu_spi.h"
    
/* cSPIN min speed register bit / mask */
#define L6480_LSPD_OPT				((uint16_t) ((0x1) << 12))
/* cSPIN Sync Output frequency enabling bit */
#define L6480_SYNC_EN         ((0x1) << 7)
/* cSPIN clock source monitoring enabling bit */
#define L6480_WD_EN     			((0x1) << 11)
/* Exported types ------------------------------------------------------------*/

typedef u8  bool;
/** 
  * @brief cSPIN Init structure definition
  */
typedef struct
{
  uint32_t ABS_POS;
  uint16_t EL_POS;
  uint32_t MARK;
  uint32_t SPEED;
  uint16_t ACC;
  uint16_t DEC;
  uint16_t MAX_SPEED;
  uint16_t MIN_SPEED;
  uint16_t FS_SPD;
  uint8_t  KVAL_HOLD;
  uint8_t  KVAL_RUN;
  uint8_t  KVAL_ACC;
  uint8_t  KVAL_DEC;
  uint16_t INT_SPD;
  uint8_t  ST_SLP;
  uint8_t  FN_SLP_ACC;
  uint8_t  FN_SLP_DEC;
  uint8_t  K_THERM;
  uint8_t  ADC_OUT;
  uint8_t  OCD_TH;
  uint8_t  STALL_TH;
  uint8_t  STEP_MODE;
  uint8_t  ALARM_EN;
  uint16_t GATECFG1;
  uint8_t  GATECFG2;  
  uint16_t CONFIG;
  uint16_t STATUS;
}L6480_RegsStruct_TypeDef;

/* cSPIN Low speed optimization */
typedef enum {
	L6480_LSPD_OPT_OFF		=((uint16_t)0x0000),
	L6480_LSPD_OPT_ON		=((uint16_t)L6480_LSPD_OPT)
} L6480_LSPD_OPT_TypeDef;

/* cSPIN overcurrent threshold options */
typedef enum {
	L6480_OCD_TH_31_25mV		=((uint8_t)0x00),
	L6480_OCD_TH_62_5mV		=((uint8_t)0x01),
	L6480_OCD_TH_93_75mV		=((uint8_t)0x02),
	L6480_OCD_TH_125mV		=((uint8_t)0x03),
	L6480_OCD_TH_156_25mV		=((uint8_t)0x04),
	L6480_OCD_TH_187_50mV		=((uint8_t)0x05),
	L6480_OCD_TH_218_75mV		=((uint8_t)0x06),
	L6480_OCD_TH_250mV		=((uint8_t)0x07),
	L6480_OCD_TH_281_25mV		=((uint8_t)0x08),
	L6480_OCD_TH_312_5mV		=((uint8_t)0x09),
	L6480_OCD_TH_343_75mV		=((uint8_t)0x0A),
	L6480_OCD_TH_375mV		=((uint8_t)0x0B),
	L6480_OCD_TH_406_25mV		=((uint8_t)0x0C),
	L6480_OCD_TH_437_5mV		=((uint8_t)0x0D),
	L6480_OCD_TH_468_75mV		=((uint8_t)0x0E),
	L6480_OCD_TH_500mV		=((uint8_t)0x0F),
	L6480_OCD_TH_531_25mV		=((uint8_t)0x10),
	L6480_OCD_TH_562_5mV		=((uint8_t)0x11),
	L6480_OCD_TH_593_75mV		=((uint8_t)0x12),
	L6480_OCD_TH_625mV		=((uint8_t)0x13),
	L6480_OCD_TH_656_25mV		=((uint8_t)0x14),
	L6480_OCD_TH_687_5mV		=((uint8_t)0x15),
	L6480_OCD_TH_718_75mV		=((uint8_t)0x16),
	L6480_OCD_TH_750mV		=((uint8_t)0x17),
	L6480_OCD_TH_781_25mV		=((uint8_t)0x18),
	L6480_OCD_TH_812_5mV		=((uint8_t)0x19),
	L6480_OCD_TH_843_75mV		=((uint8_t)0x1A),
	L6480_OCD_TH_875mV		=((uint8_t)0x1B),
	L6480_OCD_TH_906_25mV		=((uint8_t)0x1C),
	L6480_OCD_TH_937_75mV		=((uint8_t)0x1D),
	L6480_OCD_TH_968_75mV		=((uint8_t)0x1E),
	L6480_OCD_TH_1V		        =((uint8_t)0x1F)        
} L6480_OCD_TH_TypeDef;

/* cSPIN STEP_MODE register masks */
typedef enum {
	L6480_STEP_MODE_STEP_SEL		=((uint8_t)0x07),
	L6480_STEP_MODE_SYNC_SEL		=((uint8_t)0x70),
	L6480_STEP_MODE_SYNC_EN			=((uint8_t)0x80)
} L6480_STEP_MODE_Masks_TypeDef;

 /* cSPIN STEP_MODE register options */
/* cSPIN STEP_SEL options */
typedef enum {
	L6480_STEP_SEL_1		=((uint8_t)0x00),
	L6480_STEP_SEL_1_2		=((uint8_t)0x01),
	L6480_STEP_SEL_1_4		=((uint8_t)0x02),
	L6480_STEP_SEL_1_8		=((uint8_t)0x03),
	L6480_STEP_SEL_1_16		=((uint8_t)0x04),
	L6480_STEP_SEL_1_32		=((uint8_t)0x05),
	L6480_STEP_SEL_1_64		=((uint8_t)0x06),
	L6480_STEP_SEL_1_128	        =((uint8_t)0x07)
} L6480_STEP_SEL_TypeDef;

/* cSPIN SYNC_SEL options */
typedef enum {
	L6480_SYNC_SEL_DISABLED		=((uint8_t)0x00),
  L6480_SYNC_SEL_1_2		=((uint8_t)(L6480_SYNC_EN|0x00)),
	L6480_SYNC_SEL_1		=((uint8_t)(L6480_SYNC_EN|0x10)),
	L6480_SYNC_SEL_2		=((uint8_t)(L6480_SYNC_EN|0x20)),
	L6480_SYNC_SEL_4		=((uint8_t)(L6480_SYNC_EN|0x30)),
	L6480_SYNC_SEL_8		=((uint8_t)(L6480_SYNC_EN|0x40)),
	L6480_SYNC_SEL_16		=((uint8_t)(L6480_SYNC_EN|0x50)),
	L6480_SYNC_SEL_32		=((uint8_t)(L6480_SYNC_EN|0x60)),
	L6480_SYNC_SEL_64		=((uint8_t)(L6480_SYNC_EN|0x70))
} L6480_SYNC_SEL_TypeDef;

/* cSPIN ALARM_EN register options */
typedef enum {
	L6480_ALARM_EN_OVERCURRENT		=((uint8_t)0x01),
	L6480_ALARM_EN_THERMAL_SHUTDOWN		=((uint8_t)0x02),
	L6480_ALARM_EN_THERMAL_WARNING		=((uint8_t)0x04),
	L6480_ALARM_EN_UVLO     		=((uint8_t)0x08),
	L6480_ALARM_EN_ADC_UVLO 		=((uint8_t)0x10),
	L6480_ALARM_EN_STALL_DETECTION	        =((uint8_t)0x20),
	L6480_ALARM_EN_SW_TURN_ON		=((uint8_t)0x40),
	L6480_ALARM_EN_WRONG_NPERF_CMD		=((uint8_t)0x80)
} L6480_ALARM_EN_TypeDef;

/* cSPIN TCC (Control current Time) options*/
typedef enum {
	L6480_TCC_125ns		=((uint8_t)0x00),
  L6480_TCC_250ns		=((uint8_t)0x01),
	L6480_TCC_375ns		=((uint8_t)0x02),
	L6480_TCC_500ns		=((uint8_t)0x03),
	L6480_TCC_625ns		=((uint8_t)0x04),
	L6480_TCC_750ns		=((uint8_t)0x05),
	L6480_TCC_875ns		=((uint8_t)0x06),
	L6480_TCC_1000ns	=((uint8_t)0x07),
	L6480_TCC_1125ns	=((uint8_t)0x08),
	L6480_TCC_1250ns	=((uint8_t)0x09),
	L6480_TCC_1375ns	=((uint8_t)0x0A),
	L6480_TCC_1500ns	=((uint8_t)0x0B),
	L6480_TCC_1625ns	=((uint8_t)0x0C),
	L6480_TCC_1750ns	=((uint8_t)0x0D),
	L6480_TCC_1875ns	=((uint8_t)0x0E),
	L6480_TCC_2000ns	=((uint8_t)0x0F),
	L6480_TCC_2125ns	=((uint8_t)0x10),
	L6480_TCC_2250ns	=((uint8_t)0x11),
	L6480_TCC_2375ns	=((uint8_t)0x12),
	L6480_TCC_2500ns	=((uint8_t)0x13),
	L6480_TCC_2625ns	=((uint8_t)0x14),
	L6480_TCC_2750ns	=((uint8_t)0x15),
	L6480_TCC_2875ns	=((uint8_t)0x16),
	L6480_TCC_3000ns	=((uint8_t)0x17),
	L6480_TCC_3125ns	=((uint8_t)0x18),
	L6480_TCC_3250ns	=((uint8_t)0x19),
	L6480_TCC_3375ns	=((uint8_t)0x1A),
	L6480_TCC_3500ns	=((uint8_t)0x1B),
	L6480_TCC_3625ns	=((uint8_t)0x1C),
	L6480_TCC_3750ns	=((uint8_t)0x1D),
	L6480_TCC_3750ns_bis	=((uint8_t)0x1E),
	L6480_TCC_3750ns_ter	=((uint8_t)0x1F)
} L6480_TCC_TypeDef;

/* cSPIN IGate options*/
typedef enum {
	L6480_IGATE_4mA		        =(((uint8_t)0x00)<<5),
  L6480_IGATE_4mA_Bis		=(((uint8_t)0x01)<<5),
	L6480_IGATE_8mA		        =(((uint8_t)0x02)<<5),
	L6480_IGATE_16mA		=(((uint8_t)0x03)<<5),
	L6480_IGATE_24mA		=(((uint8_t)0x04)<<5),
	L6480_IGATE_32mA		=(((uint8_t)0x05)<<5),
	L6480_IGATE_64mA		=(((uint8_t)0x06)<<5),
	L6480_IGATE_96mA		=(((uint8_t)0x07)<<5),
} L6480_IGATE_TypeDef;

/* cSPIN TBOOST (Turn off boost time) options*/
typedef enum {
	L6480_TBOOST_0ns		=(((uint8_t)0x00)<<8),
  L6480_TBOOST_62_5__83_3__125ns	=(((uint8_t)0x01)<<8),
	L6480_TBOOST_125ns		=(((uint8_t)0x02)<<8),
	L6480_TBOOST_250ns		=(((uint8_t)0x03)<<8),
	L6480_TBOOST_375ns		=(((uint8_t)0x04)<<8),
	L6480_TBOOST_500ns		=(((uint8_t)0x05)<<8),
	L6480_TBOOST_750ns		=(((uint8_t)0x06)<<8),
	L6480_TBOOST_1000ns		=(((uint8_t)0x07)<<8),
} L6480_TBOOST_TypeDef;

/* cSPIN external clock watchdog */
typedef enum {
	L6480_WD_EN_DISABLE		=((uint16_t)0x0000),
	L6480_WD_EN_ENABLE		=((uint16_t)L6480_WD_EN)
} L6480_WD_EN_TypeDef;

/* cSPIN TBLANK (Blanking time) options*/
typedef enum {
	L6480_TBLANK_125ns		=(((uint8_t)0x00)<<5),
  L6480_TBLANK_250ns      	=(((uint8_t)0x01)<<5),
	L6480_TBLANK_375ns		=(((uint8_t)0x02)<<5),
	L6480_TBLANK_500ns		=(((uint8_t)0x03)<<5),
	L6480_TBLANK_625ns		=(((uint8_t)0x04)<<5),
	L6480_TBLANK_750ns		=(((uint8_t)0x05)<<5),
	L6480_TBLANK_875ns		=(((uint8_t)0x06)<<5),
	L6480_TBLANK_1000ns		=(((uint8_t)0x07)<<5),
} L6480_TBLANK_TypeDef;

/* cSPIN TDT (Dead time) options*/
typedef enum {
	L6480_TDT_125ns		=((uint8_t)0x00),
  L6480_TDT_250ns		=((uint8_t)0x01),
	L6480_TDT_375ns		=((uint8_t)0x02),
	L6480_TDT_500ns		=((uint8_t)0x03),
	L6480_TDT_625ns		=((uint8_t)0x04),
	L6480_TDT_750ns		=((uint8_t)0x05),
	L6480_TDT_875ns		=((uint8_t)0x06),
	L6480_TDT_1000ns	=((uint8_t)0x07),
	L6480_TDT_1125ns	=((uint8_t)0x08),
	L6480_TDT_1250ns	=((uint8_t)0x09),
	L6480_TDT_1375ns	=((uint8_t)0x0A),
	L6480_TDT_1500ns	=((uint8_t)0x0B),
	L6480_TDT_1625ns	=((uint8_t)0x0C),
	L6480_TDT_1750ns	=((uint8_t)0x0D),
	L6480_TDT_1875ns	=((uint8_t)0x0E),
	L6480_TDT_2000ns	=((uint8_t)0x0F),
	L6480_TDT_2125ns	=((uint8_t)0x10),
	L6480_TDT_2250ns	=((uint8_t)0x11),
	L6480_TDT_2375ns	=((uint8_t)0x12),
	L6480_TDT_2500ns	=((uint8_t)0x13),
	L6480_TDT_2625ns	=((uint8_t)0x14),
	L6480_TDT_2750ns	=((uint8_t)0x15),
	L6480_TDT_2875ns	=((uint8_t)0x16),
	L6480_TDT_3000ns	=((uint8_t)0x17),
	L6480_TDT_3125ns	=((uint8_t)0x18),
	L6480_TDT_3250ns	=((uint8_t)0x19),
	L6480_TDT_3375ns	=((uint8_t)0x1A),
	L6480_TDT_3500ns	=((uint8_t)0x1B),
	L6480_TDT_3625ns	=((uint8_t)0x1C),
	L6480_TDT_3750ns	=((uint8_t)0x1D),
  L6480_TDT_3875ns        =((uint8_t)0x1E),
  L6480_TDT_4000ns        =((uint8_t)0x1F)
} L6480_TDT_TypeDef;


/* cSPIN Config register masks */
typedef enum {
	L6480_CONFIG_OSC_SEL		=((uint16_t)0x0007),
	L6480_CONFIG_EXT_CLK		=((uint16_t)0x0008),
	L6480_CONFIG_SW_MODE		=((uint16_t)0x0010),
	L6480_CONFIG_EN_VSCOMP		=((uint16_t)0x0020),
	L6480_CONFIG_OC_SD		=((uint16_t)0x0080),
	L6480_CONFIG_UVLOVAL		=((uint16_t)0x0100),
	L6480_CONFIG_VCCVAL		=((uint16_t)0x0200),
	L6480_CONFIG_F_PWM_DEC		=((uint16_t)0x1C00),
	L6480_CONFIG_F_PWM_INT		=((uint16_t)0xE000)
} L6480_CONFIG_Masks_TypeDef;

/* cSPIN Config register options */
typedef enum {
	L6480_CONFIG_INT_16MHZ			=((uint16_t)0x0000),
	L6480_CONFIG_INT_16MHZ_OSCOUT_2MHZ	=((uint16_t)0x0008),
	L6480_CONFIG_INT_16MHZ_OSCOUT_4MHZ	=((uint16_t)0x0009),
	L6480_CONFIG_INT_16MHZ_OSCOUT_8MHZ	=((uint16_t)0x000A),
	L6480_CONFIG_INT_16MHZ_OSCOUT_16MHZ	=((uint16_t)0x000B),
	L6480_CONFIG_EXT_8MHZ_XTAL_DRIVE	=((uint16_t)0x0004),
	L6480_CONFIG_EXT_16MHZ_XTAL_DRIVE	=((uint16_t)0x0005),
	L6480_CONFIG_EXT_24MHZ_XTAL_DRIVE	=((uint16_t)0x0006),
	L6480_CONFIG_EXT_32MHZ_XTAL_DRIVE	=((uint16_t)0x0007),
	L6480_CONFIG_EXT_8MHZ_OSCOUT_INVERT	=((uint16_t)0x000C),
	L6480_CONFIG_EXT_16MHZ_OSCOUT_INVERT	=((uint16_t)0x000D),
	L6480_CONFIG_EXT_24MHZ_OSCOUT_INVERT	=((uint16_t)0x000E),
	L6480_CONFIG_EXT_32MHZ_OSCOUT_INVERT	=((uint16_t)0x000F)
} L6480_CONFIG_OSC_MGMT_TypeDef;

typedef enum {
	L6480_CONFIG_SW_HARD_STOP		=((uint16_t)0x0000),
	L6480_CONFIG_SW_USER			=((uint16_t)0x0010)
} L6480_CONFIG_SW_MODE_TypeDef;

typedef enum {
	L6480_CONFIG_VS_COMP_DISABLE	=((uint16_t)0x0000),
	L6480_CONFIG_VS_COMP_ENABLE	=((uint16_t)0x0020)
} L6480_CONFIG_EN_VSCOMP_TypeDef;

typedef enum {
	L6480_CONFIG_OC_SD_DISABLE		=((uint16_t)0x0000),
	L6480_CONFIG_OC_SD_ENABLE		=((uint16_t)0x0080)
} L6480_CONFIG_OC_SD_TypeDef;

typedef enum {
	L6480_CONFIG_UVLOVAL_LOW        =((uint16_t)0x0000),
        L6480_CONFIG_UVLOVAL_HIGH	=((uint16_t)0x0100),
} L6480_CONFIG_UVLOVAL_TypeDef;

typedef enum {
	L6480_CONFIG_VCCVAL_7_5V        =((uint16_t)0x0200),
	L6480_CONFIG_VCCVAL_15V         =((uint16_t)0x0300)
} L6480_CONFIG_VCCVAL_TypeDef;

typedef enum {
	L6480_CONFIG_PWM_DIV_1		=(((uint16_t)0x00)<<13),
	L6480_CONFIG_PWM_DIV_2		=(((uint16_t)0x01)<<13),
	L6480_CONFIG_PWM_DIV_3		=(((uint16_t)0x02)<<13),
	L6480_CONFIG_PWM_DIV_4		=(((uint16_t)0x03)<<13),
	L6480_CONFIG_PWM_DIV_5		=(((uint16_t)0x04)<<13),
	L6480_CONFIG_PWM_DIV_6		=(((uint16_t)0x05)<<13),
	L6480_CONFIG_PWM_DIV_7		=(((uint16_t)0x06)<<13)
} L6480_CONFIG_F_PWM_INT_TypeDef;

typedef enum {
	L6480_CONFIG_PWM_MUL_0_625		=(((uint16_t)0x00)<<10),
	L6480_CONFIG_PWM_MUL_0_75		=(((uint16_t)0x01)<<10),
	L6480_CONFIG_PWM_MUL_0_875		=(((uint16_t)0x02)<<10),
	L6480_CONFIG_PWM_MUL_1			=(((uint16_t)0x03)<<10),
	L6480_CONFIG_PWM_MUL_1_25		=(((uint16_t)0x04)<<10),
	L6480_CONFIG_PWM_MUL_1_5		=(((uint16_t)0x05)<<10),
	L6480_CONFIG_PWM_MUL_1_75		=(((uint16_t)0x06)<<10),
	L6480_CONFIG_PWM_MUL_2			=(((uint16_t)0x07)<<10)
} L6480_CONFIG_F_PWM_DEC_TypeDef;

/* Status Register bit masks */
typedef enum {
	L6480_STATUS_HIZ		=(((uint16_t)0x0001)),
	L6480_STATUS_BUSY		=(((uint16_t)0x0002)),
	L6480_STATUS_SW_F		=(((uint16_t)0x0004)),
	L6480_STATUS_SW_EVN		=(((uint16_t)0x0008)),
	L6480_STATUS_DIR		=(((uint16_t)0x0010)),
	L6480_STATUS_MOT_STATUS		=(((uint16_t)0x0060)),
	L6480_STATUS_NOTPERF_CMD	=(((uint16_t)0x0080)),
	L6480_STATUS_STCK_MOD		=(((uint16_t)0x0100)),
	L6480_STATUS_UVLO		=(((uint16_t)0x0200)),
	L6480_STATUS_UVLO_ADC	        =(((uint16_t)0x0400)),
	L6480_STATUS_TH_SD		=(((uint16_t)0x1800)),
	L6480_STATUS_OCD		=(((uint16_t)0x2000)),
	L6480_STATUS_STEP_LOSS_A	=(((uint16_t)0x4000)),
	L6480_STATUS_STEP_LOSS_B	=(((uint16_t)0x8000))
} L6480_STATUS_Masks_TypeDef;

/* Status Register options */
typedef enum {
	L6480_STATUS_MOT_STATUS_STOPPED		=(((uint16_t)0x0000)<<5),
	L6480_STATUS_MOT_STATUS_ACCELERATION	=(((uint16_t)0x0001)<<5),
	L6480_STATUS_MOT_STATUS_DECELERATION	=(((uint16_t)0x0002)<<5),
	L6480_STATUS_MOT_STATUS_CONST_SPD	=(((uint16_t)0x0003)<<5)
} L6480_STATUS_TypeDef;

/* 芯片寄存器地址
cSPIN internal register addresses */
typedef enum {
	L6480_ABS_POS			=((uint8_t)0x01),
	L6480_EL_POS			=((uint8_t)0x02),
	L6480_MARK			=((uint8_t)0x03),	
	L6480_SPEED			=((uint8_t)0x04),
	L6480_ACC			=((uint8_t)0x05),
	L6480_DEC			=((uint8_t)0x06),
	L6480_MAX_SPEED			=((uint8_t)0x07),
	L6480_MIN_SPEED			=((uint8_t)0x08),
	L6480_FS_SPD			=((uint8_t)0x15),
	L6480_KVAL_HOLD			=((uint8_t)0x09),
	L6480_KVAL_RUN			=((uint8_t)0x0A),
	L6480_KVAL_ACC			=((uint8_t)0x0B),
	L6480_KVAL_DEC			=((uint8_t)0x0C),
	L6480_INT_SPD			=((uint8_t)0x0D),
	L6480_ST_SLP			=((uint8_t)0x0E),
	L6480_FN_SLP_ACC		=((uint8_t)0x0F),
	L6480_FN_SLP_DEC		=((uint8_t)0x10),
	L6480_K_THERM			=((uint8_t)0x11),
	L6480_ADC_OUT			=((uint8_t)0x12),
	L6480_OCD_TH			=((uint8_t)0x13),
	L6480_STALL_TH			=((uint8_t)0x14),
	L6480_STEP_MODE			=((uint8_t)0x16),
	L6480_ALARM_EN			=((uint8_t)0x17),
	L6480_GATECFG1			=((uint8_t)0x18),
	L6480_GATECFG2			=((uint8_t)0x19),
	L6480_CONFIG			=((uint8_t)0x1A),
	L6480_STATUS			=((uint8_t)0x1B)
} L6480_Registers_TypeDef;

/* 芯片操作命令类型定义
cSPIN command set */
typedef enum {
	L6480_NOP		=((uint8_t)0x00),
	L6480_SET_PARAM		=((uint8_t)0x00),
	L6480_GET_PARAM		=((uint8_t)0x20),
	L6480_RUN		=((uint8_t)0x50),
	L6480_STEP_CLOCK	=((uint8_t)0x58),
	L6480_MOVE		=((uint8_t)0x40),
	L6480_GO_TO		=((uint8_t)0x60),
	L6480_GO_TO_DIR		=((uint8_t)0x68),
	L6480_GO_UNTIL		=((uint8_t)0x82),
  L6480_GO_UNTIL_ACT_CPY  =((uint8_t)0x8A),
	L6480_RELEASE_SW	=((uint8_t)0x92),
	L6480_GO_HOME		=((uint8_t)0x70),
	L6480_GO_MARK		=((uint8_t)0x78),
	L6480_RESET_POS		=((uint8_t)0xD8),
	L6480_RESET_DEVICE	=((uint8_t)0xC0),
	L6480_SOFT_STOP		=((uint8_t)0xB0),
	L6480_HARD_STOP		=((uint8_t)0xB8),
	L6480_SOFT_HIZ		=((uint8_t)0xA0),
	L6480_HARD_HIZ		=((uint8_t)0xA8),
	L6480_GET_STATUS	=((uint8_t)0xD0),
	L6480_RESERVED_CMD1	=((uint8_t)0xEB),
	L6480_RESERVED_CMD2	=((uint8_t)0xF8)
} L6480_Commands_TypeDef;

/* 运动方向定义
cSPIN direction options */
typedef enum {
	FWD		=((uint8_t)0x01),
	REV		=((uint8_t)0x00)
} L6480_Direction_TypeDef;

/* cSPIN action options */
typedef enum {
	ACTION_RESET	=((uint8_t)0x00),
	ACTION_COPY	=((uint8_t)0x08)
} L6480_Action_TypeDef;
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
#define Speed_Steps_to_Par(steps) ((uint32_t)(((steps)*67.108864)+0.5))			/* Speed conversion, range 0 to 15625 steps/s */
#define AccDec_Steps_to_Par(steps) ((uint16_t)(((steps)*0.068719476736)+0.5))	/* Acc/Dec rates conversion, range 14.55 to 59590 steps/s2 */
#define MaxSpd_Steps_to_Par(steps) ((uint16_t)(((steps)*0.065536)+0.5))			/* Max Speed conversion, range 15.25 to 15610 steps/s */
#define MinSpd_Steps_to_Par(steps) ((uint16_t)(((steps)*4.194304)+0.5))			/* Min Speed conversion, range 0 to 976.3 steps/s */
#define FSSpd_Steps_to_Par(steps) ((uint16_t)((steps)*0.065536))				/* Full Step Speed conversion, range 7.63 to 15625 steps/s */

#define IntSpd_Steps_to_Par(steps) ((uint16_t)(((steps)*4.194304)+0.5))			/* Intersect Speed conversion, range 0 to 3906 steps/s */
#define Kval_Perc_to_Par(perc) ((uint8_t)(((perc)/0.390625)+0.5))				/* KVAL conversions, range 0.4% to 99.6% */
#define BEMF_Slope_Perc_to_Par(perc) ((uint8_t)(((perc)/0.00156862745098)+0.5))	/* BEMF compensation slopes, range 0 to 0.4% s/step */
#define KTherm_to_Par(KTherm) ((uint8_t)(((KTherm - 1)/0.03125)+0.5))			/* K_THERM compensation conversion, range 1 to 1.46875 */
#define StallTh_to_Par(StallTh) ((uint8_t)(((StallTh - 31.25)/31.25)+0.5))		/* Stall Threshold conversion, range 31.25mV to 1000mV */

typedef struct
{
	SPI_CONFIG_TYPE spiConfig ;

	Gpio 		*reset_pin;	
	Gpio 		*busy_pin;	
	Gpio 		*flag_pin;	
	
	uint8_t motor;
	
} L6480_t;

extern L6480_t	L6480s;

void L6480_Reset_And_Standby(void);
void L6480_Regs_Struct_Reset(L6480_RegsStruct_TypeDef* L6480_RegsStruct);

uint8_t L6480_Write_Byte(uint8_t byte);
void L6480_Nop(void);

void L6480_Set_Param(L6480_Registers_TypeDef param, uint32_t value);
uint32_t L6480_Get_Param(L6480_Registers_TypeDef param);

uint8_t L6480_getFlag(void);
uint8_t L6480_Busy_HW(void);
uint16_t L6480_Get_Status(void);
uint8_t L6480_Busy_SW(void);

void L6480_Go_Home(void);
void L6480_Go_Mark(void);

void L6480_Reset_Pos(void);
void L6480_Reset_Device(void);

void L6480_Soft_Stop(void);
void L6480_Hard_Stop(void);
void L6480_Soft_HiZ(void);
void L6480_Hard_HiZ(void);

uint32_t L6480_Registers_Check(L6480_RegsStruct_TypeDef* L6480_RegsStruct);
void L6480_Registers_Get(L6480_RegsStruct_TypeDef* L6480_RegsStruct);

//rotate
void L6480_Run(L6480_Direction_TypeDef direction, uint32_t speed);
void L6480_Move(L6480_Direction_TypeDef direction, uint32_t n_step);
void L6480_Go_To(uint32_t abs_pos);

void L6480_Step_Clock( L6480_Direction_TypeDef direction);
void L6480_Go_To_Dir(L6480_Direction_TypeDef direction, uint32_t abs_pos);
void L6480_Go_Until(L6480_Action_TypeDef action, L6480_Direction_TypeDef direction, uint32_t speed);
void L6480_Release_SW(L6480_Action_TypeDef action, L6480_Direction_TypeDef direction);

	
void L6480_Registers_Set(L6480_RegsStruct_TypeDef* L6480_RegsStruct);

#endif /* __L6480_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
