
#include "ISL29125.h"
#include "mcu_softi2c.h"
#include "delay.h"

// ISL29125 I2C address
#define ISL29125_I2C_ADDR      0x88  // I2C address

// ISL29125 registers
// ------------------
#define ISL29125_REG_WHOAMI    0x00  // Read : device ID - should return 0x7D.
                                     // Write 0x46 : reset all registers to default.
#define ISL29125_REG_CFG1      0x01  // Operating mode, sensing range and start ADC.
#define ISL29125_REG_CFG2      0x02  // Active IR compensation.
#define ISL29125_REG_CFG3      0x03  // Interrupt threshold assignment, IRQ persist control, IRQ on conversion done.
#define ISL29125_REG_ITL_LO    0x04  // Interrupt Treshold Low value, low byte 
#define ISL29125_REG_ITL_HI    0x05  // Interrupt Treshold Low value, high byte 
#define ISL29125_REG_ITH_LO    0x06  // Interrupt Treshold High value, low byte 
#define ISL29125_REG_ITH_HI    0x07  // Interrupt Treshold High value, high byte 
#define ISL29125_REG_STATUS    0x08  // Status register : Interrupt, conversion done, brownout, R/G/B conversion busy.
#define ISL29125_REG_DATA_GLO  0x09  // Green data low byte.
#define ISL29125_REG_DATA_GHI  0x0A  // Green data high byte.
#define ISL29125_REG_DATA_RLO  0x0B  // Red data low byte.
#define ISL29125_REG_DATA_RHI  0x0C  // RED data high byte.
#define ISL29125_REG_DATA_BLO  0x0D  // Blue data low byte.
#define ISL29125_REG_DATA_BHI  0x0E  // Blue data high byte.

// ISL29125 register fields
// ------------------------
// Device register 0x00
#define ISL29125_RESET         0x46  // Software reset command.
#define ISL29125_WHOAMI        0x7D  // Device ID


// Config register 0x01
// RGB conversion operating modes (bits 0..2) - see header file for values
#define ISL29125_MODE_MASK     0xF8
// RGB data sensing range (bit 3)
// 0 : full scale range = 375 lux
// 1 : full scale range = 10000 lux
#define ISL29125_RNG_MASK      0xF7 
// ADC resolution (bit 4)
// 0 : 16-bit resolution
// 1 : 12-bit resolution
#define ISL29125_BITS_MASK     0xEF
// INT pin mode (bit 5)
// 0 : INT pin is SYNC input - ADC starts at rising edge of INT.
// 1 : INT pin is IRQ output.
#define ISL29125_SYNC_MASK     0xDF
#define ISL29125_SYNC_SHIFT       5


// Config register 0x02 - Active IR compensation (bit 0..5 = fine adjust, bit 7 = coarse adjust)
#define ISL29125_IR_CMP_MASK   0xBF
#define ISL29125_IRC_MAX       0xBF  // max. out IR compensation value (as per the datasheet specs)


// Config register 0x03 - IRQ
// IRQ assignment (bits 0,1) - see header file for values
#define ISL29125_INTSEL_MASK   0xFC
// IRQ persist control (bits 2,3) - activate IRQ after n consecutive transients(register value) : 1(0), 2(1), 4(2) or 8(3)
#define ISL29125_PRST_MASK     0xF3
// IRQ when conversion is done (bit 4)
// Enable conversion done IRQ
// Clear this bit to disable conversion done IRQ
#define ISL29125_CONVEN_MASK   0xEF
#define ISL29125_CONVEN_SHIFT     4


// Status register 0x08
// Interrupt status (1 when triggered)
#define ISL29125_RGBTHF_SHIFT  0
#define ISL29125_RGBTHF        (1 << ISL29125_RGBTHF_SHIFT)
// Conversion status (1 when done)
#define ISL29125_CONVENF_SHIFT 1
#define ISL29125_CONVENF       (1 << ISL29125_CONVENF_SHIFT)
// Brownout status (1 when brownout occured)
#define ISL29125_BOUTF_SHIFT   2
#define ISL29125_BOUTF         (1 << ISL29125_BOUTF_SHIFT)
// RGB conversion progress - see header file for values
#define ISL29125_RGBCF_MASK    0x30
#define ISL29125_RGBCF_SHIFT   4

ISL29125_init(Gpio sda, Gpio scl, Gpio irqsync, u8 InterruptIn)
{
    uint8_t cmd[2]; 				// cmd[0] = register address, cmd[1] = data
	SoftI2c.scl_pin=&scl;
	SoftI2c.sda_pin=&sda;
	
    _ismode = 0;
    // Set the ISL29125 in a known state : perform Software Reset
    cmd[0] = ISL29125_REG_WHOAMI;
    cmd[1] = ISL29125_RESET;
    writeRegs(cmd, 2);
    // Init the ISL29125 : Enable RGB operating mode, sensing range = 10000 lux, 16 bit resolution
    // Following registers remain at the default reset values :
    // Register 0x03 - Interrupt source (none), persist control (1) , INT when conversion (0).
    // Register 0x04, 0x05 - Low threshold interrupt : 0x0000
    // Register 0x06, 0x07 - High threshold interrupt : 0xFFFF
    cmd[0] = ISL29125_REG_CFG1;
    cmd[1] = (uint8_t)(ISL29125_RGB | ~ISL29125_RNG_MASK);
    // When irqsync is nonzero and no fptr is declared, use Sync mode (only start ADC on rising edge at sync output)
    if((irqsync != NC) && (fptr == NULL))
    {
        _ismode = 2;
        cmd[1] |= ~ISL29125_SYNC_MASK;
        _syncpin = new DigitalOut(irqsync);             // Create DigitalOut pin
        _syncpin->write(0);
    }
    writeRegs(cmd, 2);
    // Max. out IR compensation value (as per the datasheet specs)
    cmd[0] = ISL29125_REG_CFG2;
    cmd[1] = ISL29125_IRC_MAX;
    writeRegs(cmd, 2);
    // When both irqsync and fptr are nonzero, we use InterruptIn: irqsync pin is interrupt input and Attach local ISR (calls user-ISR).
    if((irqsync != NC) && (fptr != NULL))
    {
        _ismode = 1;
        _irqpin = new InterruptIn(irqsync);             // Create InterruptIn pin
        _irqpin->fall(this, &ISL29125__alsISR);        // Attach falling interrupt to local ISR
        _fptr.attach(fptr);                             // Attach function pointer to user function
    }
}

uint8_t ISL29125_Status(void)
{
    return (readReg( ISL29125_REG_STATUS));
}

uint8_t ISL29125_WhoAmI(void)
{
    return (readReg( ISL29125_REG_WHOAMI));
}

bool ISL29125_Read(uint8_t color, uint16_t * data) {
    uint8_t i, addr = 0, reg_cnt = 2, res[6];
    if(Status() & ISL29125_CONVENF)             // Only return data when a conversion is finished.
    {
        switch (color)
        {
            case ISL29125_R:
                addr = ISL29125_REG_DATA_RLO;
                break;
            case ISL29125_G:
                addr = ISL29125_REG_DATA_GLO;
                break;
            case ISL29125_B:
                addr = ISL29125_REG_DATA_BLO;
                break;
            case ISL29125_RGB:
                addr = ISL29125_REG_DATA_GLO;
                reg_cnt = 6;
                break;
            default:
                return(0);
        }
        readRegs(addr, res, reg_cnt);
        for(i=0 ; i<reg_cnt-1 ; i+=2)
            *(data+(i/2)) = (res[i+1] << 8) | (res[i]);
        return (1);
    }
    else return(0);
}

uint16_t ISL29125_Threshold(uint8_t reg, uint16_t thres) {
    if(reg == ISL29125_LTH_R || reg == ISL29125_HTH_R)
    {
        uint8_t res[2];
        readRegs(reg*2, res, 2);
        return(res[1] << 8 | res[0]);
    }
    else
    {
        uint8_t data[3];
        data[0] = reg;
        data[1] = thres & 0xff;
        data[2] = (thres >> 8) & 0xff;
        writeRegs(data, 3);
    }
    return(thres);
}

uint8_t ISL29125_RGBmode(uint8_t RGBmode) {
    if(RGBmode == 0xff)
    {
        return(readReg( ISL29125_REG_CFG1) & ~ISL29125_MODE_MASK);
    }
    else
    {
        if((RGBmode != ISL29125_G) && (RGBmode != ISL29125_R) && (RGBmode != ISL29125_B) &&
           (RGBmode != ISL29125_RG) && (RGBmode != ISL29125_BG) && (RGBmode != ISL29125_RGB) &&
           (RGBmode != ISL29125_STBY) && (RGBmode != ISL29125_OFF)) return(0xff);
        uint8_t data[2];
        data[0] = ISL29125_REG_CFG1;
        data[1] = (readReg( ISL29125_REG_CFG1) & ISL29125_MODE_MASK) | RGBmode;
        writeRegs(data, 2);
    }
    return(RGBmode);
}

uint8_t ISL29125_Range(uint8_t range) {
    if(range == 0xff)
    {
        return(readReg( ISL29125_REG_CFG1) & ~ISL29125_RNG_MASK);
    }
    else
    {
        uint8_t data[2];
        if((range != ISL29125_375LX) && (range != ISL29125_10KLX)) return(0xff);
        data[0] = ISL29125_REG_CFG1;
        data[1] = (readReg( ISL29125_REG_CFG1) & ISL29125_RNG_MASK) | range;
        writeRegs(data, 2);
    }
    return(range);
}

uint8_t ISL29125_Resolution(uint8_t resol) {
    if(resol == 0xff)
    {
        return(readReg( ISL29125_REG_CFG1) & ~ISL29125_BITS_MASK);
    }
    else
    {
        uint8_t data[2];
        if((resol != ISL29125_16BIT) && (resol != ISL29125_12BIT)) return(0xff);
        data[0] = ISL29125_REG_CFG1;
        data[1] = (readReg( ISL29125_REG_CFG1) & ISL29125_BITS_MASK) | resol;
        writeRegs(data, 2);
    }
    return(resol);
}

uint8_t ISL29125_Persist(uint8_t persist) {
    if(persist == 0xff)
    {
        return(readReg( ISL29125_REG_CFG3) & ~ISL29125_PRST_MASK);
    }
    else
    {
        uint8_t data[2];
        if((persist != ISL29125_PERS1) && (persist != ISL29125_PERS2) && (persist != ISL29125_PERS4) && (persist != ISL29125_PERS8)) return(0xff);
        data[0] = ISL29125_REG_CFG3;
        data[1] = (readReg( ISL29125_REG_CFG3) & ISL29125_PRST_MASK) | persist;
        writeRegs(data, 2);
    }
    return(persist);
}

uint8_t ISL29125_IRQonCnvDone(uint8_t irqen) {
    uint8_t tmprgb;
    uint8_t data[2];
    if(irqen == 0xff)
    {
        return((readReg( ISL29125_REG_CFG3) & ~ISL29125_CONVEN_MASK) >> ISL29125_CONVEN_SHIFT);
    }
    else
    {
        if((irqen != true) && (irqen != false)) return(0xff);
        tmprgb = RGBmode();     // Save current ADC operating mode
        RGBmode(ISL29125_OFF);  // Stop ADC conversion (changing IRQonCnvDone while ADC is running results in i2c failure)
        data[0] = ISL29125_REG_CFG3;
        data[1] = (readReg( ISL29125_REG_CFG3) & ISL29125_CONVEN_MASK) | (irqen << ISL29125_CONVEN_SHIFT);
        writeRegs(data, 2);
        RGBmode(tmprgb);        // Restore ADC operating mode
    }
    return(irqen);
}

uint8_t ISL29125_IRQonColor(uint8_t RGBmode) {
    if(RGBmode == 0xff)
    {
        return(readReg( ISL29125_REG_CFG3) & ~ISL29125_INTSEL_MASK);
    }
    else
    {
        uint8_t data[2];
        if((RGBmode != ISL29125_G) && (RGBmode != ISL29125_R) && (RGBmode != ISL29125_B) && (RGBmode != ISL29125_OFF)) return(0xff);
        data[0] = ISL29125_REG_CFG3;
        data[1] = (readReg( ISL29125_REG_CFG3) & ISL29125_INTSEL_MASK) | RGBmode;
        writeRegs(data, 2);
    }
    return(RGBmode);
}

uint8_t ISL29125_IRcomp(uint8_t ircomp) {
    if(ircomp == 0xff)
    {
        return(readReg( ISL29125_REG_CFG2) & ISL29125_IR_CMP_MASK);
    }
    else
    {
        uint8_t data[2];
        if(((ircomp > 63) && (ircomp < 128)) || (ircomp > 191)) return(0xff); // Range must be between 0..63 or 128..191
        data[0] = ISL29125_REG_CFG2;
        data[1] = ircomp & ISL29125_IR_CMP_MASK;
        writeRegs(data, 2);
    }
    return(ircomp);
}

bool ISL29125_Run(void) {
    if(_ismode == 2)   // Only allow write to sync pin when irqsync pin was declared without the ISR pointer.
    {
        _syncpin->write(1);
        return(1);
    }
    return(0);
}

void ISL29125__alsISR(void)
{
    Status();
    _fptr.call();
}

void ISL29125_i2cfail(void)
{
    printf("I2C fail\r\n");
    while(1);
}

void ISL29125_readRegs(uint8_t addr, uint8_t * data, uint8_t len) {
    char t[1] = {addr};
    if(SoftI2c.read(ISL29125_I2C_ADDR, (char *)data, len)) i2cfail();
}

//
uint8_t ISL29125_readReg(uint8_t addr) 
{
    char t[1] = {addr};
    if(SoftI2c.write(ISL29125_I2C_ADDR, t, 1)) i2cfail();
    if(SoftI2c.read(ISL29125_I2C_ADDR, t, 1))  i2cfail();
    return t[0];
}

void ISL29125_writeRegs(uint8_t * data, uint8_t len) 
{
    if(SoftI2c.write(ISL29125_I2C_ADDR, (char *)data, len)) i2cfail();
}
