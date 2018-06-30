/**
 *
 * @file	bme280_defs.h
 * @date	20171219
 * @version	1.0
 * @brief
 *
 */

#ifndef MCP342X_DEFS_H_
#define MCP342X_DEFS_H_

/********************************************************/
/* header includes */
#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/kernel.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

/********************************************************/
/*! @name		Common macros		        */
/********************************************************/

#if !defined(UINT8_C) && !defined(INT8_C)
#define INT8_C(x)       S8_C(x)
#define UINT8_C(x)      U8_C(x)
#endif

#if !defined(UINT16_C) && !defined(INT16_C)
#define INT16_C(x)      S16_C(x)
#define UINT16_C(x)     U16_C(x)
#endif

#if !defined(INT32_C) && !defined(UINT32_C)
#define INT32_C(x)      S32_C(x)
#define UINT32_C(x)     U32_C(x)
#endif

#if !defined(INT64_C) && !defined(UINT64_C)
#define INT64_C(x)      S64_C(x)
#define UINT64_C(x)     U64_C(x)
#endif

/**@}*/

/**\name C standard macros */
#ifndef NULL
#ifdef __cplusplus
#define NULL   0
#else
#define NULL   ((void *) 0)
#endif
#endif
/********************************************************/

#ifndef TRUE
#define TRUE                UINT8_C(1)
#endif
#ifndef FALSE
#define FALSE               UINT8_C(0)
#endif

// I2C Address of device
// MCP3421, MCP3425 & MCP3426 are factory programed for any of 0x68 thru 0x6F
#define MCP342X_DEFAULT_ADDRESS	0x68

// MCP3422, MCP3423, MCP3424, MCP3427 & MCP3428 addresses are controlled by address lines A0 and A1
// each address line can be low (GND), high (VCC) or floating (FLT)
#define MCP342X_A0GND_A1GND		0x68
#define MCP342X_A0GND_A1FLT		0x69
#define MCP342X_A0GND_A1VCC		0x6A
#define MCP342X_A0FLT_A1GND		0x6B
#define MCP342X_A0VCC_A1GND		0x6C
#define MCP342X_A0VCC_A1FLT		0x6D
#define MCP342X_A0VCC_A1VCC		0x6E
#define MCP342X_A0FLT_A1VCC		0x6F


// Conversion mode definitions
#define MCP342X_MODE_ONESHOT	0x00
#define MCP342X_MODE_CONTINUOUS	0x10


// Channel definitions
// MCP3421 & MCP3425 have only the one channel and ignore this param
// MCP3422, MCP3423, MCP3426 & MCP3427 have two channels and treat 3 & 4 as repeats of 1 & 2 respectively
// MCP3424 & MCP3428 have all four channels
#define	MCP342X_CHANNEL_1	0x00
#define	MCP342X_CHANNEL_2	0x20
#define	MCP342X_CHANNEL_3	0x40
#define	MCP342X_CHANNEL_4	0x60
#define	MCP342X_CHANNEL_MASK	0x60


// Sample size definitions - these also affect the sampling rate
// 12-bit has a max sample rate of 240sps
// 14-bit has a max sample rate of  60sps
// 16-bit has a max sample rate of  15sps
// 18-bit has a max sample rate of   3.75sps (MCP3421, MCP3422, MCP3423, MCP3424 only)
#define MCP342X_SIZE_12BIT	0x00
#define MCP342X_SIZE_14BIT	0x04
#define MCP342X_SIZE_16BIT	0x08
#define MCP342X_SIZE_18BIT	0x0C
#define MCP342X_SIZE_MASK	0x0C


// Programmable Gain definitions
#define MCP342X_GAIN_1X	0x00
#define MCP342X_GAIN_2X	0x01
#define MCP342X_GAIN_4X	0x02
#define MCP342X_GAIN_8X	0x03
#define MCP342X_GAIN_MASK 0x03


// /RDY bit definition
#define MCP342X_RDY	0x80

/*!
 * @brief Interface selection Enums
 */
enum bme280_intf {
	/*! SPI interface */
	BME280_SPI_INTF,
	/*! I2C interface */
	BME280_I2C_INTF
};

/*!
 * @brief Type definitions
 */
typedef int8_t (*bme280_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr,
		uint8_t *data, uint16_t len);

typedef void (*bme280_delay_fptr_t)(uint32_t period);

/*!
 * @brief Calibration data
 */
struct bme280_calib_data {
 /**
 * @ Trim Variables
 */
/**@{*/
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
	uint8_t  dig_H1;
	int16_t dig_H2;
	uint8_t  dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	int8_t  dig_H6;
	int32_t t_fine;
/**@}*/
};

/*!
 * @brief bme280 sensor structure which comprises of temperature, pressure and
 * humidity data
 */
#ifdef BME280_FLOAT_ENABLE
struct bme280_data {
	/*! Compensated pressure */
	double pressure;
	/*! Compensated temperature */
	double temperature;
	/*! Compensated humidity */
	double humidity;
};
#else
struct bme280_data {
	/*! Compensated pressure */
	uint32_t pressure;
	/*! Compensated temperature */
	int32_t temperature;
	/*! Compensated humidity */
	uint32_t humidity;
};
#endif /* BME280_USE_FLOATING_POINT */

/*!
 * @brief bme280 sensor structure which comprises of uncompensated temperature,
 * pressure and humidity data
 */
struct bme280_uncomp_data {
	/*! un-compensated pressure */
	uint32_t pressure;
	/*! un-compensated temperature */
	uint32_t temperature;
	/*! un-compensated humidity */
	uint32_t humidity;
};

/*!
 * @brief bme280 sensor settings structure which comprises of mode,
 * oversampling and filter settings.
 */
struct bme280_settings {
	/*! pressure oversampling */
	uint8_t osr_p;
	/*! temperature oversampling */
	uint8_t osr_t;
	/*! humidity oversampling */
	uint8_t osr_h;
	/*! filter coefficient */
	uint8_t filter;
	/*! standby time */
	uint8_t standby_time;
};

/*!
 * @brief bme280 device structure
 */
struct bme280_dev {
	/*! Chip Id */
	uint8_t chip_id;
	/*! Device Id */
	uint8_t dev_id;
	/*! SPI/I2C interface */
	enum bme280_intf intf;
	/*! Read function pointer */
	bme280_com_fptr_t read;
	/*! Write function pointer */
	bme280_com_fptr_t write;
	/*! Delay function pointer */
	bme280_delay_fptr_t delay_ms;
	/*! Trim data */
	struct bme280_calib_data calib_data;
	/*! Sensor settings */
	struct bme280_settings settings;
};

#endif /* BME280_DEFS_H_ */
/** @}*/
/** @}*/
