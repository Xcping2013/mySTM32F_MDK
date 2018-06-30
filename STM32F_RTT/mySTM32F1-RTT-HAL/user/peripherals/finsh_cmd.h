#ifndef __FINSH_CMD_H
#define __FINSH_CMD_H

#include <rtthread.h>
#include <finsh.h>
#include <stdio.h>
#include "delay.h"


//#define CALENDAR_USED
//#define MCP3421_USED
#define AT24CXX_USED
//#define W25QXX_USED
//#define L6480_USED
//#define LOGIC_USED
//#define ISL29125_USED
//#define DS18B20_USED
//#define EXIT_USED
//#define TM1638_USED

#ifdef MCP3421_USED
	#include "MCP3421.h"
#endif

#ifdef AT24CXX_USED
	#include "at24cxx.h"
	#include "mcu_softI2c.h"
#endif

#ifdef W25QXX_USED
	#include "mcu_spi.h"
	#include "w25qxx.h" 
#endif

#ifdef L6480_USED
	#include "l6480_config.h"
	#include "l6480.h"
#endif

#ifdef LOGIC_USED
	#include "74HC165.h"
	#include "74HC595.h"
#endif

#ifdef ISL29125_USED
	#include "isl29125.h"
#endif

#ifdef CALENDAR_USED
	#include "mcu_rtc.h"
	#include "Calendar.h"
#endif

#ifdef DS18B20_USED
	#include "ds18b20.h"
#endif

#ifdef EXIT_USED
	#include "mcu_exit.h"
#endif

#ifdef TM1638_USED
	#include "tm1638.h"
#endif

#endif
