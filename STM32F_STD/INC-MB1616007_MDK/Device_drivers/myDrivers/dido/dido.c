#include "dido.h"
#include "74HC165.h"
#include "74HC595.h"

#include "string.h"

static void input_init(void);
static uint8_t input_read(uint8_t ch);
static void input_reads(void);

static void output_init(void);
static void output_write(uint8_t ch , uint8_t state);
static void output_writeAll(uint8_t * states);

INPUT_DrvTypeDef Int=
{
	{0},
	input_init,
	input_read,
	input_reads,
};


OUTPUT_DrvTypeDef Out=
{
	output_init,
	output_write,
	output_writeAll,
};

static void input_init(void)
{
		pinMode(&PE0,GPIO_Mode_IPU);
		pinMode(&PE1,GPIO_Mode_IPU);
		pinMode(&PE2,GPIO_Mode_IPU);
		pinMode(&PE3,GPIO_Mode_IPU);
		pinMode(&PE4,GPIO_Mode_IPU);
		pinMode(&PE5,GPIO_Mode_IPU);
		pinMode(&PE6,GPIO_Mode_IPU);
		pinMode(&PE7,GPIO_Mode_IPU);
		hc165.init();
}
//
static void input_reads(void)
{
	uint8_t i;
	uint8_t	hc165_in;
	Int.data_read[0]=pinRead(&PE0)? '1':'0';
	Int.data_read[1]=pinRead(&PE1)? '1':'0';
	Int.data_read[2]=pinRead(&PE2)? '1':'0';
	Int.data_read[3]=pinRead(&PE3)? '1':'0';
	Int.data_read[4]=pinRead(&PE4)? '1':'0';
	Int.data_read[5]=pinRead(&PE5)? '1':'0';
	Int.data_read[6]=pinRead(&PE6)? '1':'0';
	Int.data_read[7]=pinRead(&PE7)? '1':'0';
	hc165.read(&hc165_in,1,LSB_FIRST);
	for(i=0;i<8;i++)	Int.data_read[i+8]=(hc165_in>>(7-i))&0x01 ? '1':'0';		
	
}
//
static uint8_t input_read(uint8_t ch)
{
	input_reads();
	return Int.data_read[ch-1]=='0' ? 0:1;
}
//
static void output_init(void)
{
	pinMode(&PB11,GPIO_Mode_Out_PP);
	pinMode(&PB10,GPIO_Mode_Out_PP);
	
	pinMode(&PC13,GPIO_Mode_Out_PP);
	pinMode(&PC14,GPIO_Mode_Out_PP);
	pinMode(&PC15,GPIO_Mode_Out_PP);
	
	pinMode(&PC0,GPIO_Mode_Out_PP);
	pinMode(&PC1,GPIO_Mode_Out_PP);
	pinMode(&PC2,GPIO_Mode_Out_PP);
	
	hc595.init();
	hc595.write(&hc595.out[0],2,MSB_FIRST);
	hc595.updata();		
}
static void output_write(uint8_t ch , uint8_t state)
{
	switch(ch)
	{
		case 1:			pinWrite(&PB11,state);
			break;	
		case 2:			pinWrite(&PB10,state);
			break;	
		case 3:			pinWrite(&PC13,state);
			break;	
		case 4:			pinWrite(&PC14,state);
			break;				
		case 5:			pinWrite(&PC15,state);
			break;	
		case 6:			pinWrite(&PC0,state);
			break;		
		case 7:			pinWrite(&PC1,state);
			break;
		case 8:			pinWrite(&PC2,state);
			break;	
		
		case 9:			
		case 10:		
		case 11:		
		case 12:						
		case 13:			
		case 14:				
		case 15:			
		case 16:		if(state) hc595.out[1] |= (0x80>>(16-ch));
								else   		hc595.out[1] &= ((0x7f >> (16-ch)) | (0x7f << (8 -(16-ch))));//Ñ­»·ÓÒÒÆ
								hc595.write(&hc595.out[0],2,MSB_FIRST);
								hc595.updata();
			break;
		
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
							if(state==0)   hc595.out[0] |= (0x80>>(24-ch));
								else   		hc595.out[0] &= ((0x7f >> (24-ch)) | (0x7f << (8 -(24-ch))));//Ñ­»·ÓÒÒÆ
								hc595.write(&hc595.out[0],2,MSB_FIRST);
								hc595.updata();
			break;
	
		default:
			break;		
	}
}
static void output_writeAll(uint8_t * states)
{
	uint8_t i;
	pinWrite(&PB11,states[0]-'0');
	pinWrite(&PB10,states[1]-'0');
	pinWrite(&PC13,states[2]-'0');
	pinWrite(&PC14,states[3]-'0');
	pinWrite(&PC15,states[4]-'0');
	pinWrite(&PC0,states[5]-'0');
	pinWrite(&PC1,states[6]-'0');
	pinWrite(&PC2,states[7]-'0');
	for(i=0;i<8;i++)
	{
		if(states[8+i]=='1') 	hc595.out[0] |= (0x80>>(7-i));
		else   							hc595.out[0] &= (0x7f>>(7-i));
		hc595.write(&hc595.out[0],1,MSB_FIRST);	
		hc595.updata();
	}
}
//

