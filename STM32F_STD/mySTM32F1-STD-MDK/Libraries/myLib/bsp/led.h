#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define SYS_LED PDout(7)

#define RGB_R PDout(6)
#define RGB_G PDout(5)
#define RGB_B PDout(4)

void LED_Init(void);
void rgb_set(char* Value);

#endif
