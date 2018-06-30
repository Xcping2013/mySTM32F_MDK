
#ifndef _DIDO_H
#define _DIDO_H
#include "sys.h"

typedef struct
{		
	uint8_t data_read[20];
  void (*init)(void );
	uint8_t (*read)(uint8_t);		
	void (*readAll)(void);			

}INPUT_DrvTypeDef; 

typedef struct
{		
  void (*init)(void );
	void (*write)(uint8_t, uint8_t);			
	void (*writeAll)(uint8_t*);			

}OUTPUT_DrvTypeDef; 


extern INPUT_DrvTypeDef Int;
extern OUTPUT_DrvTypeDef Out;

#endif
