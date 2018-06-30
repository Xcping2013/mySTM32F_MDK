#ifndef __LED_H
#define __LED_H

#include <stdint.h>

typedef struct
{
  void     (*Init)(void);
  void		 (*On)(uint8_t);
  void     (*off)(uint8_t);
}LED_DrvTypeDef;

void     LED_Init(void);
void		 LED_On(uint8_t ledNum);
void     LED_Off(uint8_t ledNum);

#endif

