/**
* @brief   Serially Interfaced, 8-Digit, LED Display Driver
*
* @details The MAX7219/MAX7221 are compact, serial input/output common-cathode
* display drivers that interface microprocessors (µPs) to 7-segment numeric
* LED displays of up to 8 digits, bar-graph displays, or 64 individual LEDs.
* Included on-chip are a BCD code-B decoder, multiplex scan circuitry, segment
* and digit drivers, and an 8x8 static RAM that stores each digit. Only one
* external resistor is required to set the segment current for all LEDs.
* The MAX7221 is compatible with SPI™, QSPI™, and MICROWIRE™, and has
* slew-rate-limited segment drivers to reduce EMI.
*
* @code
* int main()
* {
*     max7219_configuration_t cfg = {
*         .device_number = 1,
*         .decode_mode = 0,
*         .intensity = Max7219::MAX7219_INTENSITY_8,
*         .scan_limit = Max7219::MAX7219_SCAN_8
*     };
*
*     max7219.init_device(cfg);
*     max7219.enable_device(1);
*     max7219.set_display_test();
*     wait(1);
*     max7219.clear_display_test();
*
*     while (1) {
*         max7219.write_digit(1, Max7219::MAX7219_DIGIT_0, ...
*     }
* }
* @endcode
*/


/*On initial power-up, all control registers are reset,
the display is blanked, shutdown mode. 
scan one digit, not decode data ,intensity minimum value

10MHz Serial Interface	Serial-Data Format (16 Bits)
D15 D14 D13 D12  D11 D10 D9 D8	D7 D6 D5 D4	D3 D2 D1 D0
X		 X	 X 	 X  |		ADDRESS		 |MSB 			DATA 			 LSB

RES				|			ADDRESS				 |HEX CODE												
					D15-D12	D11	D10	D9 D8						D7 D6 D5 D4	D3 D2 D1 D0
No-Op 				X 	 0   0	 0  0 	0xX0
Digit0 				X 	 0   0	 0  0 	0xX1		0		0	 0 0	0	 0	0	 0(Decode)
Digit1 				X 	 0   0	 0  0 	0xX2		DP	A	 B C	D	 E	F	 G(No-Decode)
Digit2 				X 	 0   0	 0  0 	0xX3
Digit3 				X 	 0   0	 0  0 	0xX4
Digit4 				X 	 0   0	 0  0 	0xX5
Digit5 				X 	 0   0	 0  0 	0xX6
Digit6 				X 	 0   0	 0  0 	0xX7
Digit7 				X 	 0   0	 0  0 	0xX8
DecodeMode		X 	 0   0	 0  0 	0xX9		No decode for digits 7–0		:	0
																					Code B decode for digit 0		:	1
																					Code B decode for digit 3-0	:	0x0F
N/32																			Code B decode for digit 7-0	:	0xFF
Intensity			X 	 0   0	 0  0 	0xXA
ScanLimit			X 	 0   0	 0  0 	0xXB		0~7(8fOSC(800Hz)/N)
Shutdown			X 	 0   0	 0  0 	0xXC		X	 X  X	 X  X	X	 X	0		ShutdownMode			
																															1		NormalOp
DisplayTest		X 	 0   0	 0  0 	0xXF												0		NormalOp
																															1		DisplayTestMode									

Its minimum 9.53kΩ,the segment current at 40mA
*/



#ifndef _MAX7219_H
#define _MAX7219_H

#include "stm32f1xx_hal.h"

/**
 * @brief   Digit and Control Registers
 * @details The 14 addressable digit and control registers.
 */
typedef enum
{
		MAX7219_NO_OP = 0,
		MAX7219_DIGIT_0,
		MAX7219_DIGIT_1,
		MAX7219_DIGIT_2,
		MAX7219_DIGIT_3,
		MAX7219_DIGIT_4,
		MAX7219_DIGIT_5,
		MAX7219_DIGIT_6,
		MAX7219_DIGIT_7,
		MAX7219_DECODE_MODE,
		MAX7219_INTENSITY,
		MAX7219_SCAN_LIMIT,
		MAX7219_SHUTDOWN,
		MAX7219_DISPLAY_TEST = 15
}max7219_register_e;

/**
 * @brief   Intensity values
 * @details Digital control of display brightness is provided by an
 * internal pulse-width modulator, which is controlled by
 * the lower nibble of the intensity register.
 */
typedef enum
{
		MAX7219_INTENSITY_0 = 0,
		MAX7219_INTENSITY_1,
		MAX7219_INTENSITY_2,
		MAX7219_INTENSITY_3,
		MAX7219_INTENSITY_4,
		MAX7219_INTENSITY_5,
		MAX7219_INTENSITY_6,
		MAX7219_INTENSITY_7,
		MAX7219_INTENSITY_8,
		MAX7219_INTENSITY_9,
		MAX7219_INTENSITY_A,
		MAX7219_INTENSITY_B,
		MAX7219_INTENSITY_C,
		MAX7219_INTENSITY_D,
		MAX7219_INTENSITY_E,
		MAX7219_INTENSITY_F
}max7219_intensity_e;

/**
 * @brief   Scan limit for mutiplexing digits
 * @details The scan-limit register sets how many digits are 
 * displayed, from 1 to 8. They are displayed in a multiplexed
 * manner with a typical display scan rate of 800Hz with 8
 * digits displayed.
 */
typedef enum
{
		MAX7219_SCAN_1 = 0,
		MAX7219_SCAN_2,
		MAX7219_SCAN_3,
		MAX7219_SCAN_4,
		MAX7219_SCAN_5,
		MAX7219_SCAN_6,
		MAX7219_SCAN_7,
		MAX7219_SCAN_8
}max7219_scan_limit_e;
			 

/**
* @brief Structure for device configuration
* @code
* #include "max7219.h"
*
* max7219_configuration_t cfg = {
*     .device_number = 1,
*     .decode_mode = 0,
*     .intensity = Max7219::MAX7219_INTENSITY_8,
*     .scan_limit = Max7219::MAX7219_SCAN_8
* };
* @endcode
*/
typedef struct
{
    uint8_t device_number;
    uint8_t decode_mode;
    uint8_t intensity;
    uint8_t scan_limit;
}max7219_configuration_t;

typedef struct
{
	int32_t (*Set_N_O_MAX7219)(uint8_t );								//设置最大芯片个数
	
	int32_t (*Init_MAX7219)(max7219_configuration_t );	//设置模式和扫描方式、亮度
  int32_t (*Enable_MAX7219)(uint8_t );								//多个正常模式
	int32_t (*Disable_MAX7219)(uint8_t );								//多个待机模式
	
	void (*Init_MAX7219s)(max7219_configuration_t );
  void (*Enable_MAX7219s)(void );
	void (*Disable_MAX7219s)(void );
	
  void (*DisplayTest)(void);													//多个全亮
	void (*ClearDisplayTest)(void);											//正常操作
	
	int32_t (*Write_MAX7219_Digi)(uint8_t , uint8_t , uint8_t );	//单个写
	int32_t (*Clear_MAX7219_Digi)(uint8_t , uint8_t  );						//单个为0

	int32_t (*Write_MAX7219_On)(uint8_t );							//单个全亮
	int32_t (*Write_MAX7219_Off)(uint8_t );							//单个全灭
	
	void (*Write_MAX7219s_On)(void );										//多个全亮
	void (*Write_MAX7219s_Off)(void );									//多个全灭

}MAX7219_FuncDef; 


extern  MAX7219_FuncDef	max7219;
/**********************************************************//**
* @brief Sets the number of MAX7219 devices being used.
*        Defaults to one
*
* @details 
*
* On Entry:
*     @param[in] num_devices - number of MAX7219 devices being
*                              used, max of 255 
*
* On Exit:
*
* @return Returns number of devices
**************************************************************/
int32_t set_num_devices(uint8_t num_devices);

/**********************************************************//**
* @brief Tests all devices being used 
*
* @details Sets bit0 of DISPLAY_TEST regiser in all devices
*
* On Entry:
*
* On Exit:
*
* @return None
**************************************************************/
void set_display_test(void);

/**********************************************************//**
* @brief Stops test
*
* @details Clear bit0 of DISPLAY_TEST regiser in all devices
*
* On Entry:
*
* On Exit:
*
* @return None
**************************************************************/
void clear_display_test(void);

/**********************************************************//**
* @brief Initializes specific device in display with given 
*        config data
*
* @details 
*
* On Entry:
*     @param[in] config - Structure containing configuration 
*                         data of device
*
* On Exit:
*
* @return  Returns  0 on success\n 
*          Returns -1 if device number is > _num_devices\n
*          Returns -2 if device number is 0\n
**************************************************************/
int32_t init_device(max7219_configuration_t config);


/**********************************************************//**
* @brief Initializes all devices with given config data
*
* @details All devices are configured with given data
*
* On Entry:
*     @param[in] config - Structure containing configuration 
*                         data
* On Exit:
*
* @return None
**************************************************************/
void init_display(max7219_configuration_t config);

/**********************************************************//**
* @brief Enables specific device in display
*
* @details 
*
* On Entry:
*     @param[in] device_number - device to enable
*
* On Exit:
*
* @return Returns  0 on success\n 
*         Returns -1 if device number is > _num_devices\n
*         Returns -2 if device number is 0\n
**************************************************************/
int32_t enable_device(uint8_t device_number);

/**********************************************************//**
* @brief Enables all device in display 
*
* @details 
*
* On Entry:
*
* On Exit:
*
* @return None
**************************************************************/
void enable_display(void);

/**********************************************************//**
* @brief Disables specific device in display
*
* @details 
*
* On Entry:
*     @param[in] device_number - device to disable
*
* On Exit:
*    @return Returns  0 on success\n 
*            Returns -1 if device number is > _num_devices\n
*            Returns -2 if device number is 0\n
**************************************************************/
int32_t disable_device(uint8_t device_number);

/**********************************************************//**
* @brief Disables all devices in display
*
* @details 
*
* On Entry:
*
* On Exit:
*
* @return None
**************************************************************/
void disable_display(void);

/**********************************************************//**
* @brief Writes digit of given device with given data, user
*        must enter correct data for decode_mode chosen
*
* @details 
*
* On Entry:
*     @param[in] device_number - device to write too
*     @param[in] digit - digit to write
*     @param[in] data - data to write
*
* On Exit:
*
* @return Returns  0 on success\n 
*         Returns -1 if device number is > _num_devices\n
*         Returns -2 if device number is 0\n
*         Returns -3 if digit > 8\n
*         Returns -4 if digit < 1\n
**************************************************************/
int32_t write_digit(uint8_t device_number, uint8_t digit, uint8_t data);

/**********************************************************//**
* @brief Clears digit of given device
*
* @details 
*
* On Entry:
*     @param[in] device_number - device to write too
*     @param[in] digit - digit to clear
*
* On Exit:
*
* @return Returns  0 on success\n 
*         Returns -1 if device number is > _num_devices\n
*         Returns -2 if device number is 0\n
*         Returns -3 if digit > 8\n
*         Returns -4 if digit < 1\n
**************************************************************/
int32_t clear_digit(uint8_t device_number, uint8_t digit);

/**********************************************************//**
* @brief Turns on all segments/digits of given device
*
* @details 
*
* On Entry:
*     @param[in] device_number - device to write too
*
* On Exit:
*
* @return Returns  0 on success\n 
*         Returns -1 if device number is > _num_devices\n
*         Returns -2 if device number is 0\n
**************************************************************/
int32_t device_all_on(uint8_t device_number);

/**********************************************************//**
* @brief Turns off all segments/digits of given device
*
* @details 
*
* On Entry:
*     @param[in] device_number - device to write too
*
* On Exit:
*
* @return Returns 0 on success\n 
*         Returns -1 if device number is > _num_devices\n
*         Returns -2 if device number is 0\n
**************************************************************/
int32_t device_all_off(uint8_t device_number);

/**********************************************************//**
* @brief Turns on all segments/digits of display
*
* @details 
*
* On Entry:
*
* On Exit:
*
* @return None
**************************************************************/
void display_all_on(void);

/**********************************************************//**
* @brief Turns off all segments/digits of display
*
* @details 
*
* On Entry:
*
* On Exit:
*
* @return None
**************************************************************/
void display_all_off(void);
// 
void MAX7219_FunctionTest(void);

#endif /* MAX7219_H*/
