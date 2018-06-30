#include "SPI.h"
#include "delay.h"
#include "max7219.h"

#define SELECT_MAX7219()      GPIOA->BRR=BIT4
#define DESELECT_MAX7219()    GPIOA->BSRR=BIT4 

uint8_t _num_devices=1;

void WriteMAX7219(UCHAR RegAddress, UCHAR RegData)
{
	//SELECT_TMC429();
	UCHAR	Write[2];
	Write[0]=RegAddress;
	Write[1]=RegData;
	HAL_SPI_Transmit(&hspi1,Write,2,1000);
	//DESELECT_TMC429();
}
//*********************************************************************
int32_t set_num_devices(uint8_t num_devices)
{
    int32_t rtn_val = -1;
    
    if(num_devices > 0)
    {
        _num_devices = num_devices;
        rtn_val = _num_devices;
    }
    
    return(rtn_val);
}


//*********************************************************************
void set_display_test(void)
{
    uint8_t idx = 0;
    
    SELECT_MAX7219(); 
    for(idx = 0; idx < _num_devices; idx++)
    {
			WriteMAX7219(MAX7219_DISPLAY_TEST,1);
    }
		DESELECT_MAX7219(); 
}


//*********************************************************************
void clear_display_test(void)
{
    uint8_t idx = 0;
    
    SELECT_MAX7219(); 
    for(idx = 0; idx < _num_devices; idx++)
    {
			WriteMAX7219(MAX7219_DISPLAY_TEST,0);
    }
    DESELECT_MAX7219(); 
}

//*********************************************************************
int32_t init_device(max7219_configuration_t config)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(config.device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(config.device_number == 0)
    {
        //device numbering starts with index 1
        rtn_val = -2;
    }
    else
    {
        //write DECODE_MODE register of device
        SELECT_MAX7219(); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(config.device_number == idx)
            {
							WriteMAX7219(MAX7219_DECODE_MODE,config.decode_mode);
            }
            else
            {
							WriteMAX7219(MAX7219_NO_OP,0);
            }
        }
       DESELECT_MAX7219(); 
        
        delay_us(1);
        
        //write INTENSITY register of device
        SELECT_MAX7219(); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(config.device_number == idx)
            {
							WriteMAX7219(MAX7219_INTENSITY,config.intensity);
            }
            else
            {
							WriteMAX7219(MAX7219_NO_OP,0);
            }
        }
        DESELECT_MAX7219(); 
        
        delay_us(1);
        
        //write SCAN_LIMT register of device
        SELECT_MAX7219(); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(config.device_number == idx)
            {
							WriteMAX7219(MAX7219_SCAN_LIMIT,config.scan_limit);
            }
            else
            {
							WriteMAX7219(MAX7219_NO_OP,0);
            }
        }
       DESELECT_MAX7219(); 
        
        delay_us(1);
        
        rtn_val = 0;
    }
    
    return(rtn_val);
}

//*********************************************************************
void init_display(max7219_configuration_t config)
{
    uint8_t idx = 0;
    
    //write DECODE_MODE register of all devices
    SELECT_MAX7219(); 
    for(idx = 0; idx < _num_devices; idx++)
    {
			WriteMAX7219(MAX7219_DECODE_MODE,config.decode_mode);
    }
   DESELECT_MAX7219(); 
    
    delay_us(1);
    
    //write INTENSITY register of all devices
    SELECT_MAX7219(); 
    for(idx = 0; idx < _num_devices; idx++)
    {
			WriteMAX7219(MAX7219_INTENSITY,config.intensity);
    }
   DESELECT_MAX7219(); 
    
    delay_us(1);
    
    //write SCAN_LIMT register of all devices
    SELECT_MAX7219(); 
    for(idx = 0; idx < _num_devices; idx++)
    {
			WriteMAX7219(MAX7219_SCAN_LIMIT,config.scan_limit);			
    }
   DESELECT_MAX7219(); 
    
    delay_us(1);
}


//*********************************************************************
int32_t enable_device(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        //device numbering starts with index 1
        rtn_val = -2;
    }
    else
    {
        SELECT_MAX7219(); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(device_number == idx)
            {
							WriteMAX7219(MAX7219_SHUTDOWN,1);			
            }
            else
            {
							WriteMAX7219(MAX7219_NO_OP,0);	
            }
        }
       DESELECT_MAX7219(); 
        
        rtn_val = 0;
    }
    
    return(rtn_val);
}


//*********************************************************************
void enable_display(void)
{
    uint8_t idx = 0;
    
    SELECT_MAX7219(); 
    for(idx = 0; idx < _num_devices; idx++)
    {
			WriteMAX7219(MAX7219_SHUTDOWN,1);
    }
   DESELECT_MAX7219(); 
}
    

//*********************************************************************    
int32_t disable_device(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        //device numbering starts with index 1
        rtn_val = -2;
    }
    else
    {
        SELECT_MAX7219(); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(device_number == idx)
            {
							WriteMAX7219(MAX7219_SHUTDOWN,0);
            }
            else
            {
							WriteMAX7219(MAX7219_NO_OP,0);
            }
        }
       DESELECT_MAX7219(); 
        
        rtn_val = 0;
    }
    
    return(rtn_val);
}


//*********************************************************************    
void disable_display(void)
{
    uint8_t idx = 0;
    
    SELECT_MAX7219(); 
    for(idx = 0; idx < _num_devices; idx++)
    {
			WriteMAX7219(MAX7219_SHUTDOWN,0);
    }
   DESELECT_MAX7219(); 
}

//********************************************************************* 
int32_t write_digit(uint8_t device_number, uint8_t digit, uint8_t data)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(digit > MAX7219_DIGIT_7)
    {
        rtn_val = -3;
    }
    else if(digit < MAX7219_DIGIT_0)
    {
        rtn_val = -4;
    }
    else
    {
        if(device_number > _num_devices)
        {
            rtn_val = -1;
        }
        else if(device_number == 0)
        {
            rtn_val = -2;
        }
        else
        {
            SELECT_MAX7219(); 
            for(idx = _num_devices; idx > 0; idx--)
            {
                if(idx == device_number)
                {
									WriteMAX7219(digit,data);
                }
                else
                {
									WriteMAX7219(MAX7219_NO_OP,0);
                }
            }
           DESELECT_MAX7219(); 
            
            rtn_val = 0;
        }
    }
    
    return(rtn_val);
}
    
//*********************************************************************     
int32_t clear_digit(uint8_t device_number, uint8_t digit)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(digit > MAX7219_DIGIT_7)
    {
        rtn_val = -3;
    }
    else if(digit < MAX7219_DIGIT_0)
    {
        rtn_val = -4;
    }
    else
    {
        if(device_number > _num_devices)
        {
            rtn_val = -1;
        }
        else if(device_number == 0)
        {
            rtn_val = -2;
        }
        else
        {
            SELECT_MAX7219(); 
            for(idx = _num_devices; idx > 0; idx--)
            {
                if(idx == device_number)
                {
									WriteMAX7219(digit,0);
                }
                else
                {
									WriteMAX7219(MAX7219_NO_OP,0);
                }
            }
           DESELECT_MAX7219(); 
            
            rtn_val = 0;
        }
    }
    
    return(rtn_val);
}


//********************************************************************* 
int32_t device_all_on(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        rtn_val = -2;
    }
    else
    {
        rtn_val = 0;
        
        //writes every digit of given device to 0xFF
        for(idx = 0; idx < 8; idx++)
        {
            if(rtn_val == 0)
            {
							rtn_val = write_digit(device_number, (idx + 1), 0xFF);
            }
        }
    }
    
    return(rtn_val);
}


//********************************************************************* 
int32_t device_all_off(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        rtn_val = -2;
    }
    else
    {
        rtn_val = 0;
        
        //writes every digit of given device to 0
        for(idx = 0; idx < 8; idx++)
        {
            if(rtn_val == 0)
            {
               rtn_val = write_digit(device_number, (idx + 1), 0);

            }
        }
    }
    
    return(rtn_val);
}


void display_all_on(void)
{
    uint8_t idx, idy;
    
    //writes every digit of every device to 0xFF
    for(idx = 0; idx < _num_devices; idx++)
    {
        for(idy = 0; idy < MAX7219_DIGIT_7; idy++)
        {
						//NO-Decode
            write_digit((idx + 1), (idy + 1), 0xFF);
        }
    }
}
    
    
void display_all_off(void)
{
    uint8_t idx, idy;
    
    //writes every digit of every device to 0
    for(idx = 0; idx < _num_devices; idx++)
    {
        for(idy = 0; idy < MAX7219_DIGIT_7; idy++)
        {
          write_digit((idx + 1), (idy + 1), 0);
        }
    }
}

MAX7219_FuncDef	max7219=
{
	set_num_devices,
	
	init_device,
	enable_device,
	disable_device,
	
	init_display,
	enable_display,
	disable_display,
	
	set_display_test,
	clear_display_test,
	
	write_digit,
	clear_digit,
	
	device_all_on,
	device_all_off,
	
	display_all_on,
	display_all_off,
	
};

max7219_configuration_t cfg = {
.device_number = 1,
.decode_mode   = 0xFF,//0x00-No-Decode
.intensity     = MAX7219_INTENSITY_8,
.scan_limit    = MAX7219_SCAN_5
};
#define MAX7219_Test	1

#ifdef MAX7219_Test 
//OK
void MAX7219_FunctionTest(void)
{
	max7219.Set_N_O_MAX7219(1);
	max7219.Init_MAX7219(cfg);
  max7219.Enable_MAX7219(1);
//  max7219.DisplayTest();	
//	HAL_Delay(1000);
//	max7219.ClearDisplayTest();
//	max7219.Write_MAX7219_Off(1);
	HAL_Delay(20);
	
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_0, 0x0f);
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_1, 0x0f);
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_2, 0x0f);
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_3, 0x0f);
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_4, 0x0f);
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_5, 0x0f);
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_6, 0x0f);
	max7219.Write_MAX7219_Digi(1, MAX7219_DIGIT_7, 0x0f);
}

#endif
