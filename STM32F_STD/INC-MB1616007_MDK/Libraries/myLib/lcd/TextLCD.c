/* TextLCD Library, for a 4-bit LCD based on HD44780
 */
#include "TextLCD.h"
#include "delay.h"
#include <stdarg.h>
/** Create a TextLCD interface
*
* @param rs    Instruction/data control line
* @param e     Enable line (clock)
* @param d4-d7 Data lines for using as a 4-bit interface
* @param type  Sets the panel size/addressing mode (default = LCD16x2)
*/
#define _rs					PAout(1)
#define _e					PAout(2)
#define d4					PAout(4)
#define d5					PAout(5)
#define d6					PAout(6)
#define d7					PAout(7)
//#define d7_in()  		{GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)8<<28;}
//#define d7_out() 		{GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)3<<28;}
#define STRING_STACK_LIMIT    30
/** LCD panel format */
enum LCDType
{
		LCD16x2,    /**< 16x2 LCD panel (default) */
		LCD16x2B, 	/**< 16x2 LCD panel alternate addressing */
		LCD20x2,   	/**< 20x2 LCD panel */
		LCD20x4,    /**< 20x4 LCD panel */
};

u8 _type=LCD16x2;
u8 _column;
u8 _row;

static u16 columns(void) 
{
    switch (_type) {
        case LCD20x4:
        case LCD20x2:
            return 20;
        case LCD16x2:
        case LCD16x2B:
        default:
            return 16;
    }
}

static u16 rows(void) 
{
    switch (_type) {
        case LCD20x4:
            return 4;
        case LCD16x2:
        case LCD16x2B:
        case LCD20x2:
        default:
            return 2;
    }
}
//
static void locate(u8 column, u8 row) {
    _column = column;
    _row = row;
}

static void writeByte(u8 value) 
{

	if(value&0x10)	d4=1	;	else	d4=0 	;
	if(value&0x20)	d5=1	;	else	d5=0 	;
	if(value&0x40)	d6=1	;	else	d6=0 	;
	if(value&0x80)	d7=1	;	else	d7=0 	;
	
	delay_us(40); // most instructions take 40us
	_e = 0;
	delay_us(40);
	_e = 1;

	if(value&0x01)	d4=1	;	else	d4=0 	;
	if(value&0x02)	d5=1	;	else	d5=0 	;
	if(value&0x04)	d6=1	;	else	d6=0 	;
	if(value&0x08)	d7=1	;	else	d7=0 	;
	
	delay_us(40);
	_e = 0;
	delay_us(40);  // most instructions take 40us
	_e = 1;
}
//
static void writeCommand(u8 command) 
{
    _rs = 0;
    writeByte(command);
}

static void writeData(u8 data) 
{
    _rs = 1;
    writeByte(data);
}
//
static u16 address(u8 column, u8 row) 
{
	switch (_type) 
	{
		case LCD20x4:
				switch (row) {
						case 0:
								return 0x80 + column;
						case 1:
								return 0xc0 + column;
						case 2:
								return 0x94 + column;
						case 3:
								return 0xd4 + column;
				}
		case LCD16x2B:
				return 0x80 + (row * 40) + column;
		case LCD16x2:
		case LCD20x2:
		default:
				return 0x80 + (row * 0x40) + column;
	}
}
static void cls(void) {
    writeCommand(0x01); // cls, and set cursor to 0
    delay_ms(2);     	// This command takes 1.64 ms
    locate(0, 0);
}
static void init(void)
{
	_e  = 1;
	_rs = 0;            	// command mode

  delay_ms(15);        // Wait 15ms to ensure powered up

												// send "Display Settings" 3 times (Only top nibble of 0x30 as we've got 4-bit bus)
  for (int i=0; i<3; i++)
	{
     writeByte(0x3);
     delay_ms(2);  // this command takes 1.64ms, so HAL_Delay for it
  }
	writeByte(0x2);     // 4-bit mode
	delay_us(40);    // most instructions take 40us

	writeCommand(0x28); // Function set 001 BW N F - -
	writeCommand(0x0C);
	writeCommand(0x6);  // Cursor Direction and Display Shift : 0000 01 CD S (CD 0-left, 1-right S(hift) 0-no, 1-yes
	cls();
}

static void character(u8 column, u8 row, u8 c) {
    int a = address(column, row);
    writeCommand(a);
    writeData(c);
}
static int _putc(char value) {
    if (value == '\n') {
        _column = 0;
        _row++;
        if (_row >= rows()) {
            _row = 0;
        }
    } else {
        character(_column, _row, value);
        _column++;
        if (_column >= columns()) {
            _column = 0;
            _row++;
            if (_row >= rows()) {
                _row = 0;
            }
        }
    }
    return value;
}


static void _puts(const char *str)
{
    for (; *str != '\0'; str++)
    {
        _putc(*str);
    }
}
void _Printf(const char *format,...)
{ 
    va_list arg;
    va_start(arg, format);
    int len = vsnprintf(NULL, 0, format, arg);
    if (len < STRING_STACK_LIMIT) 
		{
        char temp[STRING_STACK_LIMIT];
        vsprintf(temp, format, arg);
        _puts(temp);
		}
    va_end(arg);
}
//
TextLCD_FuncDef LCD1602=
{
	init,
	cls,
	locate,
	_Printf,
};

