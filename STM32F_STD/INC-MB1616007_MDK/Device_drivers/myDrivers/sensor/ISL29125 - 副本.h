#ifndef _ISL29125_H
#define _ISL29125_H

// Common Controls                                                       Used with
//                                                          Operating       IRQ        Status
//                                                          Mode  ADC    assignment    request
#define ISL29125_G      0x01    // Green                        X            X            X
#define ISL29125_R      0x02    // Red                          X            X            X
#define ISL29125_B      0x03    // Blue                         X            X            X
#define ISL29125_RG     0x06    // Red and Green                X            -            -
#define ISL29125_BG     0x07    // Blue and Green               X            -            -
#define ISL29125_RGB    0x05    // Red, Green and Blue          X            -            X
#define ISL29125_STBY   0x04    // Standby                      X            -            -
#define ISL29125_OFF    0x00    // Switch OFF a control         X            X            -
// Unique Controls
#define ISL29125_LTH_W  0x04    // Low interrupt threshold register
#define ISL29125_HTH_W  0x06    // High interrupt threshold register
#define ISL29125_LTH_R  0x02    // Low interrupt threshold register
#define ISL29125_HTH_R  0x03    // High interrupt threshold register
#define ISL29125_375LX  0x00    // Full scale range = 375 lux
#define ISL29125_10KLX  0x08    // Full scale range = 10K lux
#define ISL29125_16BIT  0x00    // ADC resolution = 16 bit
#define ISL29125_12BIT  0x10    // ADC resolution = 12 bit
#define ISL29125_PERS1  0x00    // IRQ when threshold is reached once
#define ISL29125_PERS2  0x04    // IRQ when threshold is reached twice
#define ISL29125_PERS4  0x08    // IRQ when threshold is reached 4 times
#define ISL29125_PERS8  0x0C    // IRQ when threshold is reached 8 times

#endif
