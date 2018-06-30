
#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

#define N_O_MOTORS 				 3                          			//!< number of motors supported by this module
#define MOTOR_NUMBER(a) 	(a)                								//!< extract TMC429 motor number (for future extensions)
                     				  //!< extract TMC262 motor number (for future extensions)
#define SPI_DEV_TMC429 		 0                      					//!< SPI device number of TMC429

#define SELECT_TMC429()        GPIOC->BRR=BIT4
#define DESELECT_TMC429()      GPIOC->BSRR=BIT4

void InitSPI1(void);
UCHAR ReadWriteSPI1(UCHAR DeviceNumber, UCHAR Data, UCHAR LastTransfer);

#endif

