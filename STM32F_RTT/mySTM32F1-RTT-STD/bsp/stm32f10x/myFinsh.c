#include "sys_std.h"
#include "delay_rtt.h"

#include "led_rtt_std.h"
#include "input_std.h"	
#include "output_std.h"
#include "at24cxx.h" 
#include "motor_motion.h"

#include <stdlib.h>

#include <rtthread.h>
#include "bsp_debug.h"

void my_hw_init(void)
{		
	led_control_init();	
	eeprom_init();
	
	motor_EN_PIN=at24cxx.readU8(at24c256,ADDR_EN_PIN);
	motor_EN_VALUE=at24cxx.readU8(at24c256,ADDR_EN_VALUE);

	input_init();
	output_init();
	
	rt_hw_tmc429_init();
	motor_control_init();
	
	rt_kprintf("INC-MBTMC429 init ok\r\n");
	//rt_kprintf("motor_EN=%d\r\n",motor_EN);
}

INIT_APP_EXPORT(my_hw_init);

FINSH_FUNCTION_EXPORT(rgb_set,control rgb led r or g or b on);
FINSH_FUNCTION_EXPORT(input_read,read the input channel status);
FINSH_FUNCTION_EXPORT(input_reads,read all inputs status);
FINSH_FUNCTION_EXPORT(output_write,write 1 or 0 to output channel status);

FINSH_FUNCTION_EXPORT(save_data,save_data);
FINSH_FUNCTION_EXPORT(read_data,read_data);

FINSH_FUNCTION_EXPORT(motor_rotate,rgb_set)
FINSH_FUNCTION_EXPORT(motor_stop,input_read);
FINSH_FUNCTION_EXPORT(motor_move,output_write);
FINSH_FUNCTION_EXPORT(motor_set,save_data);
FINSH_FUNCTION_EXPORT(motor_get,read_data);

FINSH_FUNCTION_EXPORT(motor_limitINV,save_data);
FINSH_FUNCTION_EXPORT(motor_orgINV,read_data);
FINSH_FUNCTION_EXPORT(motor_goHOME,save_data);
FINSH_FUNCTION_EXPORT(motor_goLIMIT,read_data);
FINSH_FUNCTION_EXPORT(motor_resetOK,save_data);

FINSH_FUNCTION_EXPORT(hw_debug_start,board test);
FINSH_FUNCTION_EXPORT(motor_enable,motor enable control);

