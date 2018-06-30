#ifndef __LED_H
#define __LED_H

#include <sys.h>
#include <drivers/pin.h>

#define LED_RUN	PD_7

void rgb_ledInit(void);

void led_thread_entry(void* parameter);

#endif

