#ifndef  __MOTOR_MOTION_H
#define  __MOTOR_MOTION_H

#include "tmc429.h"

//EEPROM_DATA_ADDR
#define ADDR_EN_PIN		0
#define ADDR_EN_VALUE	1
#define ADDR_ORG			2
#define ADDR_LIMIT		3

extern u8 motor_EN;
extern u8 motor_EN_PIN;
extern u8 motor_EN_VALUE;

extern u8 act_ORG;
extern u8 speedFlag[3];
void  rt_hw_tmc429_init(void);
void motor_en_get(void);
void motor_enable(char* input,u8 value);

void motor_rotate(UCHAR Motor, int speed);
void motor_stop(UCHAR Motor);
void motor_move(UCHAR Motor,char* Type, int Position);

void motor_set(UCHAR Motor,char* Type,long Value);
void motor_get(UCHAR Motor,char* Type);
void motor_limitINV(void);
void motor_orgINV(void);

void exit4_handler(void);
void exit5_handler(void);
void exit6_handler(void);

void motor_isRESET(UCHAR Motor);

void motor_goLIMIT(UCHAR Motor,char* Dir,int speed);
void motor_goHOME(UCHAR Motor,char* Dir,int speed);
void motor_limit_process(void);

#endif


