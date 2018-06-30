#include <stdlib.h>
#include "main.h"
#include <string.h>

#include "UART.h"
#include "Commands.h"

#include "Globals.h"
#include "TMC429.h"
#include "TMC262.h"

UCHAR  SpeedChangedFlag[N_O_MOTORS];
UCHAR  ActualMotor; 

uint8_t  CommmandLine_speed(char *Commands)
{
	if(strncmp(Commands+1,"# velocity ",11)==0 ) 
  {
		ActualMotor=Commands[0]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			char *p = NULL;
			char *s = &Commands[12];
			u8 len;		
			MotorConfig[ActualMotor].VMax=strtol(s, &p, 10);	
			len=p-s;				
			if(0<len&&len<=4)			
      {
				Write429Short(IDX_VMAX|(ActualMotor<<5), MotorConfig[ActualMotor].VMax);
				if(ProID==BUTTON_ONLINE||ProID==BUTTON_OFFLINE)
				{
					if (MotorConfig[ActualMotor].VMax==70)
					{		SetAMax(ActualMotor, 25);MotorConfig[ActualMotor].AMax=25;}
					else if (MotorConfig[ActualMotor].VMax==100)
					{		SetAMax(ActualMotor, 40);MotorConfig[ActualMotor].AMax=40;}
					else if (MotorConfig[ActualMotor].VMax==1000)
					{		SetAMax(ActualMotor, 1600);MotorConfig[ActualMotor].AMax=1600;}
					else if (MotorConfig[ActualMotor].VMax<700)
					{		SetAMax(ActualMotor, 400);MotorConfig[ActualMotor].AMax=400;}
					else if (MotorConfig[ActualMotor].VMax<840)
					{		SetAMax(ActualMotor, 700);MotorConfig[ActualMotor].AMax=700;}
					else if (MotorConfig[ActualMotor].VMax<980)
					{		SetAMax(ActualMotor, 1000);MotorConfig[ActualMotor].AMax=1000;}
					else if (MotorConfig[ActualMotor].VMax<1180)
					{		SetAMax(ActualMotor, 1300);MotorConfig[ActualMotor].AMax=1300;}
					else if (MotorConfig[ActualMotor].VMax<2047)
					{		SetAMax(ActualMotor, 1800);MotorConfig[ActualMotor].AMax=1800;}
				}
				else if(ProID==LIDOPEN)
				{
					if (MotorConfig[ActualMotor].VMax<25)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+5);	MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+5;}			
					else if (MotorConfig[ActualMotor].VMax<100)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+10);	MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+10;}
					else if (MotorConfig[ActualMotor].VMax<150)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+30);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+30;}
					else if (MotorConfig[ActualMotor].VMax<280)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+60);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+60;}
					else if (MotorConfig[ActualMotor].VMax<420)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+100);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+100;}
					else if (MotorConfig[ActualMotor].VMax<560)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+150);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+150;}
					else if (MotorConfig[ActualMotor].VMax<700)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+200);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+200;}
					else if (MotorConfig[ActualMotor].VMax<840)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+250);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+250;}
					else if (MotorConfig[ActualMotor].VMax<980)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+300);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+300;}
					else if (MotorConfig[ActualMotor].VMax<=1180)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+350);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+350;}
					else if (MotorConfig[ActualMotor].VMax<2047)
					{		SetAMax(ActualMotor, MotorConfig[ActualMotor].VMax+600);MotorConfig[ActualMotor].AMax=MotorConfig[ActualMotor].VMax+600;}
					
					
				}
				//UART_Printf("Vset[%d#][%d]<OK>\r\n",ActualMotor,MotorConfig[ActualMotor].VMax);
				UART_Printf("Vset[%d-%d]<OK>\n",ActualMotor,MotorConfig[ActualMotor].VMax);

				return 1;
			}
			else return 0;
     }
		UART_SendStr("<NA>\n");return 1;
	}
	else if(strncmp(Commands,"all# velocity ",14)==0 ) 
  {
		char *p = NULL;
		char *s = &Commands[12];
		u8 len;
			
		MotorConfig[0].VMax=strtol(s, &p, 10);
		len=p-s;				
		if(0<len&&len<=4)			
    {
			MotorConfig[1].VMax=MotorConfig[0].VMax;
			MotorConfig[2].VMax=MotorConfig[0].VMax;
			Write429Short(IDX_VMAX|(0<<5), MotorConfig[0].VMax);
			Write429Short(IDX_VMAX|(1<<5), MotorConfig[1].VMax);
			Write429Short(IDX_VMAX|(2<<5), MotorConfig[2].VMax);			
			//UART_Printf("Vset[all#][%d]<OK>\r\n",MotorConfig[ActualMotor].VMax);
			UART_Printf("Vset[%d]<OK>\n",MotorConfig[0].VMax);
			return 1;
    }
		UART_SendStr("<NA>\n");return 1;
  }
/**   读取所有电机当前速度
**					
**/	
	else if(strcmp(Commands,"velocity #all")==0 ) 
	{
		MotorConfig[0].VACTUAL=Read429Short(IDX_VACTUAL|(0<<5));
		MotorConfig[1].VACTUAL=Read429Short(IDX_VACTUAL|(1<<5));
		MotorConfig[2].VACTUAL=Read429Short(IDX_VACTUAL|(2<<5));
		UART_Printf("Vset[0]<%d>,Vset[1]<%d>,Vset[2]<%d>,V[0]<%d>,V[1]<%d>,V[2]<%d>\n",MotorConfig[0].VMax,MotorConfig[1].VMax,MotorConfig[2].VMax,MotorConfig[0].VACTUAL,MotorConfig[1].VACTUAL,MotorConfig[2].VACTUAL);		
		return 1;
	}
/**   读取单个电机当前速度
**					
**/	
	else if(strncmp(Commands,"velocity #",10)==0 ) 
	{
		ActualMotor=Commands[10]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			MotorConfig[ActualMotor].VACTUAL=Read429Short(IDX_VACTUAL|(ActualMotor<<5));
			UART_Printf("Vset[%d]<%d>,V[%d]<%d>\n",ActualMotor,MotorConfig[ActualMotor].VMax,ActualMotor,MotorConfig[ActualMotor].VACTUAL);
			return 1;
		}	
		UART_SendStr("<NA>\n");return 1;
	}
	else return 0;	
}
//
uint8_t  CommmandLine_acc(char *Commands)
{
	if(strncmp(Commands+1,"# acc ",6)==0 ) 
  {
		ActualMotor=Commands[0]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			char *p = NULL;
			char *s = &Commands[7];
			u8 len;
			MotorConfig[ActualMotor].AMax=strtol(s, &p, 10);	
			len=p-s;				
			if(0<len&&len<=4)				
      {
        SetAMax(ActualMotor, MotorConfig[ActualMotor].AMax);
				UART_Printf("A[%d-%d]<OK>\n",ActualMotor,MotorConfig[ActualMotor].AMax);				
				return 1;
      }
    }
		UART_SendStr("<NA>\n");return 1;
	}		
	else if(strncmp(Commands,"all# acc ",9)==0 ) 
  {
		char *p = NULL;
		char *s = &Commands[9];
		u8 len;
		MotorConfig[0].AMax=strtol(s, &p, 10);		
		len=p-s;				
		if(0<len&&len<=4)					
    {
			MotorConfig[1].AMax=MotorConfig[0].AMax;	
			MotorConfig[2].AMax=MotorConfig[0].AMax;	
      SetAMax(0, MotorConfig[0].AMax);
			SetAMax(1, MotorConfig[1].AMax);
			SetAMax(2, MotorConfig[2].AMax);
			UART_Printf("A[all-%d]<OK>\n",MotorConfig[0].AMax);			
			return 1;
    }
		UART_SendStr("<NA>\n");return 1;
  }
/**   读取所有电机当前加速度
**					
**/		
	else if(strcmp(Commands,"acc #all")==0 ) 
	{
		UART_Printf("A[0]<%d>,A[1]<%d>,A[2]<%d>\n",MotorConfig[0].AMax,MotorConfig[1].AMax,MotorConfig[2].AMax);
		return 1;
	}
	else return 0;
}
//
uint8_t  CommmandLine_rotate(char *Commands)
{
	if(strncmp(Commands+1,"# rotate ",9)==0 ) 
  {
		if( Commands[10]=='+' || Commands[10]=='-' )
		{
			ActualMotor=Commands[0]-'0';
			if(ActualMotor<N_O_MOTORS)
			{
				char *p = NULL;
				char *s = &Commands[11];		
				u8 len;				
				MotorConfig[ActualMotor].VTARGET=strtol(s, &p, 10);		
				len=p-s;				
				if(0<len&&len<=4)					
				{
					SpeedChangedFlag[ActualMotor]=TRUE;
					Set429RampMode(ActualMotor, RM_VELOCITY);
					Write429Short(IDX_VMAX|(ActualMotor<<5), 2047);
					if( Commands[10]=='+' )
					{
						Write429Short(IDX_VTARGET|(ActualMotor<<5), MotorConfig[ActualMotor].VTARGET);
					}
					else Write429Short(IDX_VTARGET|(ActualMotor<<5), -MotorConfig[ActualMotor].VTARGET);

					UART_SendStr("<OK>\n");
					return 1;
				}
			}
		}
		UART_SendStr("<NA>\n");return 1;		
	}
	else return 0;
}
//
uint8_t  CommmandLine_move(char *Commands)
{
/**   绝对运动
**					
**/		
	if(strncmp(Commands+1,"# move to ",10)==0 ) 
  {
		if( Commands[11]=='+' || Commands[11]=='-' )
		{
			ActualMotor=Commands[0]-'0';
			if(ActualMotor<N_O_MOTORS)
			{
				char *p = NULL;
				char *s = &Commands[12];	
				u8 len;
				MotorConfig[ActualMotor].XTARGET=(int)strtol(s, &p, 10);			
				len=p-s;				
				if(0<len&&len<=8)	
				{
					if(SpeedChangedFlag[ActualMotor])
					{
						Write429Short(IDX_VMAX|(ActualMotor<<5), MotorConfig[ActualMotor].VMax);
						SetAMax(ActualMotor, MotorConfig[ActualMotor].AMax);
						SpeedChangedFlag[ActualMotor]=FALSE;
					}
					Set429RampMode(ActualMotor, RM_VELOCITY);
					if( Commands[11]=='+' )
					{
						Write429Int(IDX_XTARGET|(ActualMotor<<5), MotorConfig[ActualMotor].XTARGET);
					}
					else Write429Int(IDX_XTARGET|(ActualMotor<<5), -MotorConfig[ActualMotor].XTARGET);
					
					Set429RampMode(ActualMotor, RM_RAMP);	
					UART_SendStr("<OK>\n");					
					return 1;
				}
			}
		}
		UART_SendStr("<NA>\n");return 1;
	}
/**   相对运动
**					
**/		
	else	if(strncmp(Commands+1,"# move ",7)==0 ) 
  {
		if( Commands[8]=='+' || Commands[8]=='-' )
		{
			ActualMotor=Commands[0]-'0';
			if(ActualMotor<N_O_MOTORS)
			{
				char *p = NULL;
				char *s = &Commands[9];	
				u8 len;
				MotorConfig[ActualMotor].XTARGET=(int)strtol(s, &p, 10);							
				len=p-s;				
				if(0<len&&len<=8)	
				{
					if(SpeedChangedFlag[ActualMotor])
					{
						Write429Short(IDX_VMAX|(ActualMotor<<5), MotorConfig[ActualMotor].VMax);
						SetAMax(ActualMotor, MotorConfig[ActualMotor].AMax);
						SpeedChangedFlag[ActualMotor]=FALSE;
					}
					Set429RampMode(ActualMotor, RM_VELOCITY);
					if( Commands[8]=='+' )
					{
						Write429Int(IDX_XTARGET|(ActualMotor<<5), Read429Int(IDX_XACTUAL|(ActualMotor<<5))+MotorConfig[ActualMotor].XTARGET);
					}
					else Write429Int(IDX_XTARGET|(ActualMotor<<5), Read429Int(IDX_XACTUAL|(ActualMotor<<5))-MotorConfig[ActualMotor].XTARGET);
					
					Set429RampMode(ActualMotor, RM_RAMP);	
					UART_SendStr("<OK>\r\n");	
					return 1;
				}
			}
		}
		UART_SendStr("<NA>\n");return 1;
	}
/**   停止运动
**					
**/	
	else 	if(strncmp(Commands+1,"# stop",6)==0 ) 
  {
		ActualMotor=Commands[0]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			Set429RampMode(ActualMotor, RM_VELOCITY);
			Write429Zero(IDX_VTARGET|(ActualMotor<<5));			
			UART_Printf("STOP[%d]<OK>\r\n",ActualMotor);			
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;
	}
	else return 0;
}
//
uint8_t  CommmandLine_pos(char *Commands)
{
	/**   读取位置
**					
**/	
	if(strcmp(Commands,"position #all")==0 ) 
	{
		MotorConfig[0].XACTUAL=Read429Int(IDX_XACTUAL|(0<<5));
		MotorConfig[1].XACTUAL=Read429Int(IDX_XACTUAL|(1<<5));
		MotorConfig[2].XACTUAL=Read429Int(IDX_XACTUAL|(2<<5));
		UART_Printf("P[0]<%d>,P[1]<%d>,P[2]<%d>\n",MotorConfig[0].XACTUAL,MotorConfig[1].XACTUAL,MotorConfig[2].XACTUAL);
		//UART_Printf("P[0]=%d,P[1]=%d,P[2]=%d\r\n",MotorConfig[0].XACTUAL,MotorConfig[1].XACTUAL,MotorConfig[2].XACTUAL);	
		return 1;
	}
	else 	if(strncmp(Commands,"position #",10)==0 ) 
	{
		ActualMotor=Commands[10]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			MotorConfig[ActualMotor].XACTUAL=Read429Int(IDX_XACTUAL|(ActualMotor<<5));
			UART_Printf("P[%d]<%d>\n",ActualMotor,MotorConfig[ActualMotor].XACTUAL);
			//UART_Printf("P[%d]=%d\r\n",ActualMotor,MotorConfig[ActualMotor].XACTUAL);
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;
	}
	else return 0;
}
//
uint8_t  CommmandLine_limit(char *Commands)
{
/**   读取指定轴左极限状态
**					
**/	
	if(strncmp(Commands,"is left #",9)==0 ) 
	{
		ActualMotor=Commands[9]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			MotorConfig[ActualMotor].REFL_FLAG=(Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x02<<ActualMotor*2)) ? 1:0;
			UART_Printf("REFL[%d]<%d>\n",ActualMotor,MotorConfig[ActualMotor].REFL_FLAG);
			//UART_Printf("REFL[%d]=%d\r\n",ActualMotor,MotorConfig[ActualMotor].REFL_FLAG);
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;
	}
/**   读取指定轴右极限状态
**					
**/		
	else if(strncmp(Commands,"is right #",10)==0 ) 
	{
		ActualMotor=Commands[10]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			MotorConfig[ActualMotor].REFR_FLAG=(Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x01<<ActualMotor*2)) ? 1:0;
			UART_Printf("REFR[%d]<%d>\n",ActualMotor,MotorConfig[ActualMotor].REFR_FLAG);
			//UART_Printf("REFR[%d]=%d\r\n",ActualMotor,MotorConfig[ActualMotor].REFR_FLAG);
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;
	}
/**   读取所有轴极限开关状态
**					
**/	
	else if(strcmp(Commands,"is limit")==0 ) 
	{
		u8 limit_char[6];u8 i;
		u8 limit = Read429SingleByte(IDX_REF_SWITCHES, 3);
		for(i=0;i<6;i++)
		{
			limit_char[i]=(limit&(0x20>>i)) ? '1':'0';
		}
		UART_Printf("LIMIT<%.6s>\n",limit_char);
		//UART_Printf("limit=%6s\r\n",limit_char);		
		return 1;
	}
	else return 0;
}
//
uint8_t  CommmandLine_reach(char *Commands)
{
	/**   判断指定轴是否运动到位
**					
**/	
	if(strncmp(Commands,"is reach #",10)==0 ) 
	{
		ActualMotor=Commands[10]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			MotorConfig[ActualMotor].POSITION_FLAG=(Read429Status() & (0x01<<ActualMotor*2)) ? 1:0;
			UART_Printf("REACH[%d]<%d>\n",ActualMotor,MotorConfig[ActualMotor].POSITION_FLAG);
			//UART_Printf("REACH[%d]=%d\r\n",ActualMotor,MotorConfig[ActualMotor].POSITION_FLAG);
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;
	}
/**   判断指定轴是否停止
**					
**/	
	else 	if(strncmp(Commands,"is stop #",9)==0 ) 
	{
		u8 Swith=Read429SingleByte(IDX_REF_SWITCHES, 3);
		ActualMotor=Commands[9]-'0';
		if(ActualMotor<N_O_MOTORS)
		{
			if( Read429Short(IDX_VACTUAL|(ActualMotor<<5))==0 )  MotorConfig[ActualMotor].STOP_FLAG=1;
			else MotorConfig[ActualMotor].STOP_FLAG=0;
			
			if((Swith&0x03)==0x03) MotorConfig[0].STOP_FLAG=1;
			if((Swith&0x0c)==0x0c) MotorConfig[1].STOP_FLAG=1;
			if((Swith&0x30)==0x30) MotorConfig[2].STOP_FLAG=1;	

			UART_Printf("STOP[%d]<%d>\n",ActualMotor,MotorConfig[ActualMotor].STOP_FLAG);
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;
	}							
	else return 0;
}
//

