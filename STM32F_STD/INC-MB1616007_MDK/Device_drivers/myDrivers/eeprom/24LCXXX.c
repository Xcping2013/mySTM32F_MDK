
#include "24lcxxx.h"

#define DEBUG_24CXX			 1
#define DEBUG_24CXX_RTT	 0

#if DEBUG_24CXX_RTT
	#include <rtthread.h>
	#define DEBUG rt_kprintf
#eif	DEBUG_24CXX
	#define printf
#else
	#define DEBUG(...)
#endif

AT24LCXXX_init(IIC_GPIO pins)
{
	SoftI2c.pin_init(pins);
}
uint8_t AT24LCXXX_reads(at24lcxx_t dev, uint16_t deviceAddress, uint8_t addressLength, uint16_t reg_address, uint8_t* data, uint16_t num_to_read);


int AT24LCXXX_read(at24lcxx_t dev, int memoryAddress, char* data, int dataLength)
{
    int errnum = -1;
    if((memoryAddress + dataLength) > dev.eeprom.maxAddress) return errnum;
    int loc_deviceAddress = _deviceAddress;

    if (_device < C24LC04){
        char loc_memoryAddress = (char)memoryAddress;
        errnum = readEXE(loc_deviceAddress, &loc_memoryAddress, 1, data, dataLength);
    }
    else if (_device > C24LC02 && _device < C24LC32){
        int totalBlocksSize =  BLOCK_SIZE;
        char loc_memoryAddres;
        int startBlock, blocksToRead, remindLength, partLength = 0;
        int loc_dataLength[_device.blockNumbers];

        for (int i = 0; i < _device.blockNumbers; i++){
            if (memoryAddress < totalBlocksSize){
                startBlock = i;
                loc_memoryAddres = memoryAddress - (BLOCK_SIZE * startBlock);
                break;
            }
            else totalBlocksSize =+ BLOCK_SIZE;
        }

        loc_dataLength[0] = BLOCK_SIZE - loc_memoryAddres;
        if (dataLength <= loc_dataLength[0]){
            loc_dataLength[0] = dataLength;
            blocksToRead = 1;
        }
        else {
            remindLength = dataLength - loc_dataLength[0];
            for (int i = 1; i < _device.blockNumbers; i++) {
                if (remindLength <= BLOCK_SIZE) {
                    loc_dataLength[i] = remindLength;
                    blocksToRead++;
                    break;
                }
                loc_dataLength[i] = BLOCK_SIZE;
                remindLength -= BLOCK_SIZE;
            }
        }

        for (int i = 0; i < blocksToRead; i++){
            loc_deviceAddress &= 0xF0;
            loc_deviceAddress |= (startBlock << 1);
            errnum = readEXE(loc_deviceAddress, &loc_memoryAddres, 1, &data[partLength], loc_dataLength[i]);
            if (errnum != 0) return errnum;
            partLength += loc_dataLength[i];
            startBlock++;
            loc_memoryAddres = 0x00;
        }
    }
    else {
        char loc_memoryAddress[2];
        loc_memoryAddress[0] = (char)(memoryAddress >> 8);        // Read Address High byte set
        loc_memoryAddress[1] = (char)memoryAddress;               // Read Address Low  byte set
        errnum = readEXE(loc_deviceAddress, loc_memoryAddress, 2, data, dataLength);
    }
	return errnum;
}

int AT24LCXXX_write(int memoryAddress, const char* data, int dataLength){
	int errnum = -1;
	if((memoryAddress + dataLength) > _device.maxAddress) return errnum;

	int offset = 0;
	while(offset < dataLength) {
		int loc_dataLength = _device.pageSize - ((memoryAddress + offset) % _device.pageSize);
		if((offset + loc_dataLength) > dataLength) loc_dataLength = dataLength - offset;
		errnum = writePage((memoryAddress + offset), (data + offset), loc_dataLength);
		if(errnum != 0) return errnum;
		offset += loc_dataLength;
	}
	return errnum;
}

int AT24LCXXX_writePage(int memoryAddress, const char* data, int dataLength){
    int errnum = -1;
    if(((memoryAddress % _device.pageSize) + dataLength) > _device.pageSize) return errnum;
    int loc_deviceAddress = _deviceAddress;

    if (_device < C24LC04) {
        char loc_memoryAddress = (char)memoryAddress;
        errnum = writeEXE(loc_deviceAddress, &loc_memoryAddress, sizeof(loc_memoryAddress), data, dataLength);
    }
    else if (_device > C24LC02 && _device < C24LC32){
        int totalBlocksSize =  BLOCK_SIZE;
        char loc_memoryAddres;
        int startBlock, blocksToWrite, remindLength, partLength = 0;
        int loc_dataLength[2];

        for (int i = 0; i < _device.blockNumbers; i++){
            if (memoryAddress < totalBlocksSize){
                startBlock = i;
                loc_memoryAddres = memoryAddress - (BLOCK_SIZE * startBlock);
                break;
            }
            else totalBlocksSize =+ BLOCK_SIZE;
        }

        loc_dataLength[0] = BLOCK_SIZE - loc_memoryAddres;
        if (dataLength <= loc_dataLength[0]){
            loc_dataLength[0] = dataLength;
            blocksToWrite = 1;
        }
        else {
            loc_dataLength[1] = dataLength - loc_dataLength[0];
            blocksToWrite = 2;
        }

        for (int i = 0; i < blocksToWrite; i++){
            loc_deviceAddress &= 0xF0;
            loc_deviceAddress |= (startBlock << 1);
            errnum = writeEXE(loc_deviceAddress, &loc_memoryAddres, 1, &data[partLength], loc_dataLength[i]);
            if (errnum != 0) return errnum;
            partLength += loc_dataLength[i];
            startBlock++;
            loc_memoryAddres = 0x00;
        }
    }
    else {
        char loc_memoryAddress[2];
        loc_memoryAddress[0] = (char)(memoryAddress >> 8);                    // Write Address High byte set
        loc_memoryAddress[1] = (char)memoryAddress;                           // Write Address Low  byte set
        errnum = writeEXE(loc_deviceAddress, loc_memoryAddress, sizeof(loc_memoryAddress), data, dataLength);
    }
    return errnum;
}

