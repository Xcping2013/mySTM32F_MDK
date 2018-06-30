/*******************************************************************************
  Project: stepRocker Mini-TMCL

  Module:  stepRocker.c
           Main program of the stepRocker open source Mini-TMCL

   Copyright (C) 2011 TRINAMIC Motion Control GmbH & Co KG
                      Waterloohain 5
                      D - 22769 Hamburg, Germany
                      http://www.trinamic.com/

   This program is free software; you can redistribute it and/or modify it
   freely.

   This program is distributed "as is" in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

/**
  \file stepRocker.c
  \author Trinamic Motion Control GmbH & Co KG
  \version 1.00

  \brief Main file

  This file contains the main() function.
*/

#include <stdlib.h>
#include "csp.h"
#include "s3fn41f_conf.h"
#include "bits.h"
#include "stepRocker.h"
#include "SysTick.h"
#include "RS485.h"
#include "Commands.h"
#include "Globals.h"
#include "SPI.h"
#include "TMC429.h"
#include "TMC262.h"
#include "IO.h"
#include "SysControl.h"


const UCHAR VersionString[]="1110V100";


void NVIC_Init(void)
{
  CSP_NVICPriorityGroupConfig(NVIC_PRIORITY_3);
}


/*********************************//**
  \fn main(void)
  \brief Main program
  
  This is the main function. It does
  all necessary initializations and
  then enters the main loop.
*************************************/
int main(void)
{
  U32_T BlinkDelay;
  //TCanFrame TestFrame;

  CM0->PCSR=1<<2;       //enabled WDT clock
  WDT0->OMR=0x234<<4;   //switch off WDT

  CM0->CSR=1<<23;               //Enable OSC
  while((CM0->RISR & 1)==0);    //wait until OSC is stable

  CM0->MR0=0x0000082C;
  CM0->MR1=1;
  CM0->SCDR=0xACDC0000;    //SYSCLK 1:1
  CM0->PCDR=0xA3C50000;    //PCLK 1:1
  CM0->PCSR=0xffffffff;    //alle Clocks eingeschaltet
  NVIC_Init();
  EnableInterrupts();

  CSP_IFCInit(IFC0, 0);                   //initialize flash controller
  CSP_IFCConfigInterleave(IFC0, ENABLE);  //switch flash to high speed mode (needed for clock frequencies >20MHz)

  // enable PLL
  CSP_CMPLLConfig(PMS_FIN16PLL40);   //40MHz PLL output frequency
  CSP_CMPLLCmd(ENABLE);
  while(!(CM0->SR & BIT7));          //wait until PLL is stabilized 
  IFC0->MR|=BIT7;                    //switch flash to high speed mode
  CSP_CMSysclkChange(SysPLLCLK);     //use PLL for the system
  CSP_CMClockOutCmd(MdEMCLK);        //use crystal (16MHz) for clock output

#if !defined(BOOTLOADER)
  ModuleConfig.SerialBitrate=0;
  ModuleConfig.SerialModuleAddress=1;
  ModuleConfig.SerialHostAddress=2;
  MotorConfig[0].VMax=1000;
  MotorConfig[0].AMax=500;
  MotorConfig[0].PulseDiv=2;
  MotorConfig[0].RampDiv=3;
  MotorConfig[0].IRun=255;
  MotorConfig[0].IStandby=16;
  MotorConfig[0].StallVMin=0;
  MotorConfig[0].FreewheelingDelay=0;
  MotorConfig[0].SettingDelay=200;
#endif

  InitIO();
  InitSysTimer();
  InitRS485(0);
  InitSPI();
  Init429();
  InitMotorDrivers();

  InitTMCL();
  BlinkDelay=GetSysTimer();

  SetUARTTransmitDelay(5);
  
  for(;;)
  {
  	ProcessCommand();
  	SystemControl();

  	if(abs(GetSysTimer()-BlinkDelay)>1000)
  	{
  		LED1_TOGGLE();
  		BlinkDelay=GetSysTimer();
  	}
  	
  	if(ExitTMCLFlag) return 0;
  }
}
