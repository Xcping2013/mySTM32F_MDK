/**
  \file 		Commands.h
  \author 	Xcping2016
  \version 	1.00

  \brief 		CharSend&Receive command definitions

  This file contains all necessary definitions for the
  PC command interpreter.
*/
#ifndef __COMMANDS_H
#define	__COMMANDS_H

uint8_t  ProcessCommand(char  *Commands);
//IO
uint8_t  CommmandLine_in(char *Commands);
uint8_t  CommmandLine_out(char *Commands);
uint8_t  CommmandLine_rgb(char *Commands);
//EEPROM
uint8_t  CommmandLine_sv(char *Commands);
uint8_t  CommmandLine_hv(char *Commands);
uint8_t  CommmandLine_fsn(char *Commands);
uint8_t  CommmandLine_baud(char *Commands);
uint8_t  CommmandLine_rom(char *Commands);
uint8_t  CommmandLine_RomPage(char *Commands);
//MOTOR
uint8_t  CommmandLine_speed(char *Commands);
uint8_t  CommmandLine_acc(char *Commands);
uint8_t  CommmandLine_rotate(char *Commands);
uint8_t  CommmandLine_move(char *Commands);
uint8_t  CommmandLine_pos(char *Commands);
uint8_t  CommmandLine_limit(char *Commands);
uint8_t  CommmandLine_reach(char *Commands);

//
uint8_t  CommmandLine_Button(char *Commands);
//
uint8_t  CommmandLine_Lidopen(char *Commands);
//
uint8_t  CommmandLine_IAC_LCR(char *Commands);
//
uint8_t  CommmandLine_JABIL_LCR(char *Commands);
//


#endif

