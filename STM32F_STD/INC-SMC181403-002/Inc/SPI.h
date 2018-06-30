
#ifndef __SPI_H
#define __SPI_H

void InitSPI(void);
UCHAR ReadWriteSPI(UCHAR DeviceNumber, UCHAR Data, UCHAR LastTransfer);

#endif

