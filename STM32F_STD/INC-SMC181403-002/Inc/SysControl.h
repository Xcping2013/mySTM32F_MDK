/**
  \file SysControl.h

  \version 1.00

  \brief Motor monitoring 

  This file contains the definitions of the functions from the SysControl.c
  module.
*/
#ifndef __SYSCONTROL_H
#define	__SYSCONTROL_H

#define AXIS_X	0
#define AXIS_Y	1
#define AXIS_Z	2
//°´¼ü
extern u8 KeySTART_ON;
extern u8 KeyEmergency_ON;
extern u8 KeyRESET_ON;
extern u8 MotorIsHomed;

void MotorHoming(void);
void CommandCheckAndExe(void);

void EXTIX_Init(void);

//-----------------------------------------------------------Lidopen
extern u8 HomeStep;

void MotorHoming_Lidopen(void);
//-----------------------------------------------------------Lidopen
u8 	KeyEmergency_EXE(void);
void KeySTART_EXE(void);
void KeyRESET_EXE_ONLINE(void);
void KeyRESET_EXE_OFFLINE(void);
void KeyRESET_EXE_IAC_LCR(void);
void MotorHoming_Button(void);
void Cylinder_Reset_check(void);

#endif
