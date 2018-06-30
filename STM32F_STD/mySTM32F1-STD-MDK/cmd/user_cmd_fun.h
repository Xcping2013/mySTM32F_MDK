#ifndef __USER_BSP_H
#define __USER_BSP_H

#include "dido.h"
#include "at24cxx.h"
	
extern at24cxx_t at24c256;

uint8_t  CMD_int(char *Commands);
uint8_t  CMD_out(char *Commands);

void 		 eeprom_init(void);

uint8_t  CMD_fsn(char *Commands);
uint8_t  CMD_rom(char *Commands);
uint8_t  CMD_RomPage(char *Commands);

#endif

