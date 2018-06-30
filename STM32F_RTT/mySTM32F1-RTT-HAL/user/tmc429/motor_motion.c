#include "motor_motion.h"
#include "tim.h"
#include "delay.h"
#include "exti.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "at24cxx.h"

#include "sys.h"
#define INPUT1	PBin(5)
#define INPUT2	PBin(6)
#define INPUT3	PBin(7)
#define INPUT4	PBin(8)
#define INPUT5	PBin(9)

#define INPUT6	PEin(0)
#define INPUT7	PEin(1)
#define INPUT8	PEin(2)
#define INPUT9	PEin(3)
#define INPUT10	PEin(4)
#define INPUT11	PEin(5)
#define INPUT12	PEin(6)

#define AXIS_X	0
#define AXIS_Y	1
#define AXIS_Z	2

#define TMC429_DEBUG

#ifdef  TMC429_DEBUG
    #define MOTION_TRACE         printf
#else
    #define MOTION_TRACE(...)
#endif

u8 act_ORG=LOW;
u8 speedFlag[3]={'0','0','0'};
u8 motor_EN=0;
u8 motor_EN_PIN=0;
u8 motor_EN_VALUE=0;

home_t HOME_temp=
{
	{FALSE,FALSE,FALSE},
	{FALSE,FALSE,FALSE},
	{DIR_NONE,DIR_NONE,DIR_NONE},
	{DIR_NONE,DIR_NONE,DIR_NONE},
	{0,0,0},
};

TMotorConfig MotorConfig[N_O_MOTORS]=
{
  1120,   //!< VMax
  600,    //!< AMax
  7,      //!< Pulsediv
  7,      //!< Rampdiv
};

UCHAR	SpeedChangedFlag[3]={FALSE,FALSE,FALSE};
void SetOrg(u8 axis) ;
void motor_RotateRight(UCHAR Motor, int speed);
void motor_RotateLeft(UCHAR Motor, int speed);
void SetAmaxAuto(u8 ActualMotor,int speed);

void TMC429_CLK_Init(void)
{
	MX_TIM3_Init();	
	delay_ms(10);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
}
//
void RampInit(void)
{
  uint8_t i;
	for(i=0; i<N_O_MOTORS; i++) 	//默认电机主要参数配置，主要为速度和加速度
	{																  
		MotorConfig[i].VMax=100;   //  3R/S  
		MotorConfig[i].AMax=500;
		MotorConfig[i].PulseDiv=5;
		MotorConfig[i].RampDiv=8;	
		SpeedChangedFlag[i]=TRUE;
	}
}

void  rt_hw_tmc429_init(void)
{
	TMC429_CLK_Init();//12MHZ
	
	pinMode(POSCMP1_PIN, GPIO_Mode_IPU);
  pinMode(INTOUT1_PIN, GPIO_Mode_IPU);	
	
	MX_SPI1_Init();
	RampInit();
	Init429();
}
//
void motor_rotate(UCHAR Motor, int speed)
{
	if(motor_EN==0)
	{
		if(0<Motor && Motor<N_O_MOTORS+1 && (abs(speed)<2048))
		{
			Motor=Motor-1;
			if(speed>0)	speedFlag[Motor]=DIR_R; else speedFlag[Motor]=DIR_L; 
			SpeedChangedFlag[Motor]=TRUE;
			Set429RampMode(Motor, RM_VELOCITY);
			Write429Short((IDX_VMAX|Motor<<5), 2047);
			Write429Short(IDX_VTARGET|(Motor<<5), speed);
			MOTION_TRACE("<motor is rotating>");
		}
		else MOTION_TRACE("<motor number must be 1~3, speed must be -2047~2047>");
	}
	else MOTION_TRACE("<motor is disable by input%d>",motor_EN_PIN);
}
void motor_stop(UCHAR Motor)
{
  if(0<Motor && Motor<N_O_MOTORS+1 )
  {
		Motor=Motor-1;
		HardStop(Motor);
		MOTION_TRACE("<ok>");
  }
	else MOTION_TRACE("<motor number must be 1~3>");
}
u8 motor_isLIMITED(UCHAR Motor,char* Type)
{
	if(Type[0]=='L') return (Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x02<<Motor*2)) ? 1:0;	
	else 						return (Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x01<<Motor*2)) ? 1:0;	
	
}
void motor_move(UCHAR Motor,char* Type, int Position)
{
if(motor_EN==0)
{
	if(0<Motor && Motor<N_O_MOTORS+1 && (abs(Position)<8388607))
	{
		Motor=Motor-1;
		if(Type[0]==MVP_ABS || Type[0]==MVP_REL)
		{
			if(SpeedChangedFlag[Motor])
			{
				Write429Short(IDX_VMAX|(Motor<<5), MotorConfig[Motor].VMax);
				SetAMax(Motor, MotorConfig[Motor].AMax);
				SpeedChangedFlag[Motor]=FALSE;
			}

			if(Type[0]==MVP_ABS )
			{
				if(Type[1]=='-' ) Write429Int(IDX_XTARGET|(Motor<<5), -Position);
				else Write429Int(IDX_XTARGET|(Motor<<5), Position);
				
			}
			else 
			{
				if(Type[1]=='-' ) Write429Int(IDX_XTARGET|(Motor<<5), -Position+Read429Int(IDX_XACTUAL|(Motor<<5)));		
				else Write429Int(IDX_XTARGET|(Motor<<5), Position+Read429Int(IDX_XACTUAL|(Motor<<5)));		
			}
		}
		Set429RampMode(Motor, RM_RAMP);
		MOTION_TRACE("<ok>");
		if(Read429Short(IDX_VACTUAL|(Motor<<5))>0)			speedFlag[Motor]=DIR_R;
		else if(Read429Short(IDX_VACTUAL|(Motor<<5))<0) speedFlag[Motor]=DIR_L;
		else speedFlag[Motor]=DIR_NONE;
	}
	else MOTION_TRACE("<motor number must be 1~3,position must be 0~8388607>");
}
else MOTION_TRACE("<motor is disable by input%d>",motor_EN_PIN);
}
//
void motor_set(UCHAR Motor, char* Type,long Value)
{
  UCHAR Read[4], Write[4];u8 flag=1;

  if(0<Motor && Motor<N_O_MOTORS+1 )
  {
		Motor=Motor-1;
		
    switch(Type[0])
    {
      //Write429Int(IDX_XTARGET|(Motor<<5), Value);
      //Write429Int(IDX_XACTUAL|(Motor<<5), Value);
			//Write429Short(IDX_VACTUAL|(Motor<<5), Value);
			//Write429Short(IDX_VTARGET|(Motor<<5), Value);
      case 'V':
        MotorConfig[Motor].VMax=Value;
        Write429Short(IDX_VMAX|Motor<<5, MotorConfig[Motor].VMax);
				SetAmaxAuto(Motor,abs(Value));
        break;

      case 'A':
        MotorConfig[Motor].AMax=Value;
        SetAMax(Motor, MotorConfig[Motor].AMax);
        break;

      case 'm':
        Set429RampMode(Motor, Value);
        break;

      case 'r':
        Write[0]=IDX_PULSEDIV_RAMPDIV|TMC429_READ|(Motor<<5);
        ReadWrite429(Read, Write);
        Write[1]=Read[1];
        Write[2]=(Read[2] & 0xf0) | (Value & 0x0f);
        Write[3]=Read[3];
        Write[0]=IDX_PULSEDIV_RAMPDIV|(Motor<<5);
        ReadWrite429(Read, Write);
        MotorConfig[Motor].RampDiv=Value & 0x0f;
        break;

      case 'p':
        Write[0]=IDX_PULSEDIV_RAMPDIV|TMC429_READ|(Motor<<5);
        ReadWrite429(Read, Write);
        Write[1]=Read[1];
        Write[2]=(Read[2] & 0x0f) | (Value << 4);
        Write[3]=Read[3];
        Write[0]=IDX_PULSEDIV_RAMPDIV|(Motor<<5);
        ReadWrite429(Read, Write);
        MotorConfig[Motor].PulseDiv=Value& 0x0f;
        break;

      default:	flag=0;
        break;
    }
		if(flag)	MOTION_TRACE("<ok>");
		else	  	MOTION_TRACE("<type err>");	
  } 
	else MOTION_TRACE("<motor number must be 1~3>");
}

void motor_get(UCHAR Motor,char* Type)
{
	long Value=0; u8 flag=1;
	if(0<Motor && Motor<N_O_MOTORS+1 )
	{
		Motor=Motor-1;
	
    switch(Type[0])
    {
      case 'X':
        Value=Read429Int(IDX_XACTUAL|(Motor<<5));
        break;
      case 'Y':
        Value=Read429Int(IDX_XTARGET|(Motor<<5));
        break;

      case 'V':
        Value=MotorConfig[Motor].VMax;
        break;

      case 'v':
        Value=Read429Short(IDX_VACTUAL|(Motor<<5));
        break;

      case 'A':
        Value=MotorConfig[Motor].AMax;
        break;

      case 'S':	//position reached
        if(Read429Status() & 0x01) Value=1;
        break;

      case 'L':
        Value=(Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x02<<Motor*2)) ? 1:0;
        break;

      case 'R':
        Value=(Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x01<<Motor*2)) ? 1:0;
        break;

      case 'm':
        Value=Read429SingleByte(IDX_REFCONF_RM|(Motor<<5), 3);
        break;

      case 'r':
        Value=Read429SingleByte(IDX_PULSEDIV_RAMPDIV|(Motor<<5), 2) & 0x0f;
        break;

      case 'p':
        Value=Read429SingleByte(IDX_PULSEDIV_RAMPDIV|(Motor<<5), 2) >> 4;
        break;

      default:flag=0;
        
        break;
    }
		if(flag)	MOTION_TRACE("<%d>",Value);
		else	  	MOTION_TRACE("<type err>");	
  }
	else MOTION_TRACE("<motor number must be 1~3");
}
void motor_limitINV(void)
{
	Write429Int(IDX_IF_CONFIG_429, IFCONF_EN_SD|IFCONF_EN_REFR|IFCONF_SDO_INT|IFCONF_INV_DIR|IFCONF_INV_REF);
	MOTION_TRACE("<ok>");
}
void motor_orgINV(void)
{
	act_ORG=HIGH;
	at24cxx.writeU8(at24c256,ADDR_ORG,HIGH);
	MOTION_TRACE("<ok>");
}

void motor_goHOME(UCHAR Motor,char* Dir,int speed)
{
	if(motor_EN==0)
	{
	if(0<Motor && Motor<N_O_MOTORS+1 )
	{
		Motor=Motor-1;
		HOME_temp.Homed[Motor]=FALSE;
		HOME_temp.HomeNeed[Motor]=TRUE;
		HOME_temp.HomeDir[Motor]=Dir[0];
		HOME_temp.HomeSpeed[Motor]=speed;
		HardStop(Motor);
		MotorORG_TI_Enable(Motor+1);
		
		if(Dir[0]=='R')				{motor_RotateRight(Motor,speed);speedFlag[Motor]=DIR_R;}
		else if(Dir[0]=='L')	{motor_RotateLeft(Motor,speed);speedFlag[Motor]=DIR_L;}
		MOTION_TRACE("<motor%d start go home>",Motor+1);		
	}
	
}
else MOTION_TRACE("<motor is disable by input%d>",motor_EN_PIN);		

}

void motor_goLIMIT(UCHAR Motor,char* Dir,int speed)
{
	if(motor_EN==0)
	{
	if(0<Motor && Motor<N_O_MOTORS+1 )
	{
		Motor=Motor-1;
		HOME_temp.Homed[Motor]=FALSE;
		HOME_temp.HomeNeed[Motor]=TRUE;
		HOME_temp.LimitDir[Motor]=Dir[0];
		HOME_temp.HomeSpeed[Motor]=speed;
		if(Dir[0]=='R')				motor_RotateRight(Motor,speed);
		else if(Dir[0]=='L')	motor_RotateLeft(Motor,speed);
		MOTION_TRACE("<motor%d start go limit>",Motor+1);	
	}
}
	else MOTION_TRACE("<motor is disable by input%d>",motor_EN_PIN);
	
}

//////////////////////////////////////////////////////////
void SetOrg(u8 axis) 
{
	MotorORG_TI_Disable(axis+1);
	HardStop(axis);
	delay_ms(10);
	Write429Int(IDX_XACTUAL|(axis<<5), 0);
	Write429Int(IDX_XTARGET|(axis<<5), 0);
	HOME_temp.HomeNeed[axis]=FALSE;
	HOME_temp.Homed[axis]=TRUE;
	HOME_temp.LimitDir[axis]=DIR_NONE;
	HOME_temp.HomeDir[axis]=DIR_NONE;
	speedFlag[axis]=DIR_NONE;
	MOTION_TRACE("<motor%d reset ok>\r\n>>",axis+1);
	
}
void motor_isRESET(UCHAR Motor) 
{
	if(0<Motor && Motor<N_O_MOTORS+1 )
	{
		MOTION_TRACE("<%d>",HOME_temp.Homed[Motor-1]);
	}
}

void motor_RotateRight(UCHAR Motor, int speed)
{
	if(motor_EN==0)
	{
  if(Motor<N_O_MOTORS && (abs(speed)<2048))
  {
//		Motor=Motor-1;
    SpeedChangedFlag[Motor]=TRUE;
    Set429RampMode(Motor, RM_VELOCITY);
		Write429Short((IDX_VMAX|Motor<<5), 2047);
		Write429Short(IDX_VTARGET|(Motor<<5), speed);
		speedFlag[Motor]=DIR_R;
  }
}
}
void motor_RotateLeft(UCHAR Motor, int speed)
{
	if(motor_EN==0)
	{
  if(Motor<N_O_MOTORS && (abs(speed)<2048))
  {
//		Motor=Motor-1;
    SpeedChangedFlag[Motor]=TRUE;
    Set429RampMode(Motor, RM_VELOCITY);
		Write429Short((IDX_VMAX|Motor<<5), 2047);
		Write429Short(IDX_VTARGET|(Motor<<5),-speed);
		speedFlag[Motor]=DIR_L;
  }
}
}
void motor_limit_process(void)
{
	u8 SwitchStatus;
	u8 i;
	if(motor_EN==0)
	{
	SwitchStatus=Read429SingleByte(IDX_REF_SWITCHES, 3); 											//读取限位状态	
	for(i=0;i<3;i++)
	{
		if(((SwitchStatus & (0x02<<i*2))?1:0) ==TRUE)														//触发左限位
		{
			if(HOME_temp.HomeNeed[i]==TRUE)																					//收到回原点命令
			{
				if(HOME_temp.LimitDir[i]==DIR_L) {SetOrg(i);}													//找左限位，回原点OK
				else motor_RotateRight(i,HOME_temp.HomeSpeed[i]);											//否则右转
			}
			else if(speedFlag[i]==DIR_L)	{speedFlag[i]=DIR_NONE;	HardStop(i);} 	
		}
		if(((SwitchStatus & (0x01<<i*2))?1:0) ==TRUE)		//右限位
		{
			if(HOME_temp.HomeNeed[i]==TRUE)
			{
				if(HOME_temp.LimitDir[i]==DIR_R) {SetOrg(i);}
				else motor_RotateLeft(i,HOME_temp.HomeSpeed[i]);		
			}
			else if(speedFlag[i]==DIR_R)	{speedFlag[i]=DIR_NONE;	HardStop(i);} 
		}		
	}
}
	else HardStop(0);
}

//12MHZ|10000steps//2047-->17.57/M;					0.29R/S			100--0.01R/S
void SetAmaxAuto(u8 ActualMotor,int speed)
{
	if(MotorConfig[ActualMotor].PulseDiv==7&&	MotorConfig[ActualMotor].RampDiv==10)
	{
		if (speed<25)
		{		SetAMax(ActualMotor, speed+5);	MotorConfig[ActualMotor].AMax=speed+5;}			
		else if (speed<100)
		{		SetAMax(ActualMotor, speed+10);	MotorConfig[ActualMotor].AMax=speed+10;}
		else if (speed<150)
		{		SetAMax(ActualMotor, speed+30);MotorConfig[ActualMotor].AMax=speed+30;}
		else if (speed<280)
		{		SetAMax(ActualMotor, speed+60);MotorConfig[ActualMotor].AMax=speed+60;}
		else if (speed<420)
		{		SetAMax(ActualMotor, speed+100);MotorConfig[ActualMotor].AMax=speed+100;}
		else if (speed<560)
		{		SetAMax(ActualMotor, speed+150);MotorConfig[ActualMotor].AMax=speed+150;}
		else if (speed<700)
		{		SetAMax(ActualMotor, speed+200);MotorConfig[ActualMotor].AMax=speed+200;}
		else if (speed<840)
		{		SetAMax(ActualMotor, speed+250);MotorConfig[ActualMotor].AMax=speed+250;}
		else if (speed<980)
		{		SetAMax(ActualMotor, speed+300);MotorConfig[ActualMotor].AMax=speed+300;}
		else if (speed<=1180)
		{		SetAMax(ActualMotor, speed+350);MotorConfig[ActualMotor].AMax=speed+350;}
		else if (speed<2047)
		{		SetAMax(ActualMotor, speed+600);MotorConfig[ActualMotor].AMax=speed+600;}	
	}
	else if(MotorConfig[ActualMotor].PulseDiv==5 &&	MotorConfig[ActualMotor].RampDiv==8)
	{
		if (speed<25)	//1~24
		{		SetAMax(ActualMotor, speed);	MotorConfig[ActualMotor].AMax=speed;}			
		else if (speed<50)
		{		SetAMax(ActualMotor, speed+5);	MotorConfig[ActualMotor].AMax=speed+5;}
		else if (speed<100)
		{		SetAMax(ActualMotor, 100);	MotorConfig[ActualMotor].AMax=100;}
		else if (speed<150)
		{		SetAMax(ActualMotor, 500);MotorConfig[ActualMotor].AMax=500;}
		else if (speed<250)
		{		SetAMax(ActualMotor, 1000);MotorConfig[ActualMotor].AMax=1000;}
		else if (speed<2047)
		{		SetAMax(ActualMotor, 2047);MotorConfig[ActualMotor].AMax=2047;}	
	}
}
//-----------------------------------------------------------
void motor_enable(char* input,u8 value)
{
	if(strcmp("input1",input)==0) 		 motor_EN_PIN=1;	
	else if(strcmp("input2",input)==0) motor_EN_PIN=2;	
	else if(strcmp("input3",input)==0) motor_EN_PIN=3;	
	else if(strcmp("input4",input)==0) motor_EN_PIN=4;	
	else if(strcmp("input5",input)==0) motor_EN_PIN=5;	
	else if(strcmp("input6",input)==0) motor_EN_PIN=6;	
	else if(strcmp("input7",input)==0) motor_EN_PIN=7;	
	else if(strcmp("input8",input)==0) motor_EN_PIN=8;	
	else if(strcmp("input9",input)==0) motor_EN_PIN=9;
	else 															 motor_EN_PIN=0;		
	at24cxx.writeU8(at24c256,ADDR_EN_PIN,motor_EN_PIN);
	at24cxx.writeU8(at24c256,ADDR_EN_VALUE,value);
	MOTION_TRACE("<ok>");
}
void motor_en_get(void)
{
	switch(motor_EN_PIN)
	{
		case 1:		if(motor_EN_VALUE==0){if(INPUT1==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT1==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		case 2:		if(motor_EN_VALUE==0){if(INPUT2==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT2==0) motor_EN=1;else motor_EN=0;}	
			break;	
														
		case 3:		if(motor_EN_VALUE==0){if(INPUT3==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT3==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		case 4:		if(motor_EN_VALUE==0){if(INPUT4==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT4==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		case 5:		if(motor_EN_VALUE==0){if(INPUT5==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT5==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		case 6:		if(motor_EN_VALUE==0){if(INPUT6==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT6==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		case 7:		if(motor_EN_VALUE==0){if(INPUT7==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT7==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		case 8:		if(motor_EN_VALUE==0){if(INPUT8==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT8==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		case 9:		if(motor_EN_VALUE==0){if(INPUT9==1) motor_EN=1;else motor_EN=0;}
							if(motor_EN_VALUE==1){if(INPUT9==0) motor_EN=1;else motor_EN=0;}	
			break;
							
		default:	motor_EN=0;
			break;
		
	}
}

//
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t motor_stack[ 512 ];

static struct rt_thread motor_thread;


void motor_thread_entry(void *parameter)
{
    while (1)
    {  
			motor_en_get();
  		motor_limit_process();
			rt_thread_delay(1);
    }
}

int motor_thread_init(void)
{
    rt_err_t result;

    /* init led thread */
    result = rt_thread_init(&motor_thread,
                            "motor",
                            motor_thread_entry,
                            RT_NULL,
                            (rt_uint8_t *)&motor_stack[0],
                            sizeof(motor_stack),
                            20,
                            5);
    if (result == RT_EOK)
    {
       rt_thread_startup(&motor_thread);
    }
    return 0;
}
//



