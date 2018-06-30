#include "usmart.h"
#include "usmart_str.h" 


#include "delay.h"		
#include "sys.h"
#include "led.h"
#include "tim.h"

#include "delay.h"
#include "led.h"
#include "input.h"
#include "output.h"
#include "at24cxx.h"
#include "motor_motion.h"
#include "board_debug.h"

extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta);
 
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	
	(void*)read_addr,				"u32 read_addr(u32 addr)",
	(void*)write_addr,			"void write_addr(u32 addr,u32 val)",	 
#endif
//	(void*)delay_ms,				"void delay_ms(u16 nms)",
//	(void*)delay_us,				"void delay_us(u32 nus)",	
		
	(void*)rgb_set, 				"void rgb_set(char* Value)",	
	(void*)input_read,			"void input_read(uint8_t channel)",
	(void*)input_reads,			"void input_reads(void)",
	(void*)output_write,		"void output_write(uint8_t channel, uint8_t value)",
			
	(void*)board_debug,			"void board_debug(void)",
		
	(void*)save_data,			  "void save_data(uint8_t pageNum , char *Commands)",	
	(void*)read_data,				"void read_data(u8 pageNum)",
	
	(void*)motor_rotate,		"void motor_rotate(UCHAR Motor, int speed)",
	(void*)motor_stop,			"void motor_stop(UCHAR Motor)",
	(void*)motor_move,			"void motor_move(UCHAR Motor,char* Type, int Position)",
		
	(void*)motor_set,  			"void motor_set(UCHAR Motor,char* Type,long Value)",
	(void*)motor_get,				"void motor_get(UCHAR Motor,char* Type)",
	(void*)motor_limitINV,	"void motor_limitINV(void)",
	(void*)motor_orgINV,		"void motor_orgINV(void)",
		
	(void*)motor_goHOME,		"void motor_goHOME(UCHAR Motor,char* Dir,int speed)",
	(void*)motor_goLIMIT,		"void motor_goLIMIT(UCHAR Motor,char* Dir,int speed)",
	(void*)motor_isRESET,		"void motor_isRESET(UCHAR Motor)",
	(void*)motor_enable,		"void motor_enable(char* input,u8 value)",
		
	//(void*)tim_get_cfg,			"void tim_get_cfg(void)",
};						  

struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















