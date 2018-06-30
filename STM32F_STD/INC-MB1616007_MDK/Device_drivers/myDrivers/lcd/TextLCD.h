/*
TextLCD Library, for a 4-bit LCD based on HD44780

#include "TextLCD.h"
 
SetUp GPIO for drive the LCD

LCD (RS, E, D4, D5, D6, D7)

 ---------------------------------------------
  - DB4               : D4  -> PB_5
  - DB5               : D5  -> PB_4
  - DB6               : D6  -> PB_10
  - DB7               : D7  -> PA_8
  - RS                : D8  -> PA_9
  - Enable            : D9  -> ptd5
  - Backlight Control : D10 -> PB_6
 ---------------------------------------------

 
#define ON  1
#define OFF 0
 
int main()
{
 
    backlight = ON;  // ON: 1, OFF: 0
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("By: www.emcu.it");
    wait(2);
 
    while (1)
    {
        myled = !myled;
 
 
        // READ BUTTON VALUE
        unsigned long value = button.read_u16();
 
        // LCD OUTPUT
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("By: www.emcu.it");
 
        lcd.locate(0,1);
        lcd.printf("BUTTON : %04x", value);  // button value
        
        // Test the Push Button on LCD
        if (value < 0x3ff)       // up
            backlight = ON;
        else if (value < 0x7ff)  // down
            backlight = OFF;            
        else;
        wait(0.2);
    }
}
 */


#ifndef _TEXT_LCD_H
#define _TEXT_LCD_H
#include "sys.h"

/**  A TextLCD interface for driving 4-bit HD44780-based LCDs
 *
 * Currently supports 16x2, 20x2 and 20x4 panels
 *
 * @code
 * #include "mbed.h"
 * #include "TextLCD.h"
 * 
 * TextLCD lcd(p10, p12, p15, p16, p29, p30); // rs, e, d4-d7
 * 
 * int main() {
 *     lcd.printf("Hello World!\n");
 * }
 * @endcode
 */



//    /** Locate to a screen column and row
//     *
//     * @param column  The horizontal position from the left, indexed from 0
//     * @param row     The vertical position from the top, indexed from 0
//     */
//    void locate(u8 column, u8 row);

//    /** Clear the screen and locate to 0,0 */
//    void cls();

//    u16 rows();
//    u16 columns();

//    u16 address(u8 column, u8 row);
//    void character(u8 column, u8 row, u8 c);
//    void writeByte(u8 value);
//    void writeCommand(u8 command);
//    void writeData(u8 data);
typedef struct
{
	u8 type;
	Gpio * rs_pin;
	Gpio * en_pin;
	Gpio * d4_pin;
	Gpio * d5_pin;
	Gpio * d6_pin;
	Gpio * d7_pin;
	
}TextLCD_t;

typedef struct
{		
  void (*init)(void );
	void (*clear)(void );
	
  void (*location)(uint8_t,uint8_t);													
	void (*printf)(const char *,...);																			

}TextLCD_FuncDef; 

extern TextLCD_FuncDef LCD1602;

#endif
