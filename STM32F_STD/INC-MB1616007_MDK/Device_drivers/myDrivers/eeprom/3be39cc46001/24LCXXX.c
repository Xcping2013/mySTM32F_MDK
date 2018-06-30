/**
 *****************************************************************************
 * File Name    : EEPROM.cpp
 *
 * Title        : I2C EEPROM 24LCXXX Claass Source File
 * Revision     : 1.0
 * Notes        :
 * Target Board : mbed NXP LPC1768, mbed LPC1114FN28  etc
 *
 * Revision History:
 * When         Who         Description of change
 * -----------  ----------- ----------------------
 *
 *****************************************************************************
 *
 * Copyright (C) 2016 wodji
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **/

#include <EEPROM.h>


EEPROM::EEPROM(I2C* i2c, const device_t device, const int deviceAddress) :
		_deviceAddress(deviceAddress << 1), _i2c(i2c), _device(device){
}

EEPROM::EEPROM(PinName sda, PinName scl, const device_t device,
		const int deviceAddress) : _deviceAddress(deviceAddress << 1), _device(device){
	    _i2c = new I2C(sda, scl);
}

int EEPROM::read(int memoryAddress, char* data, int dataLength){
    int errnum = -1;
    if((memoryAddress + dataLength) > _device.maxAddress) return errnum;
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

int EEPROM::write(int memoryAddress, const char* data, int dataLength){
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

int EEPROM::writePage(int memoryAddress, const char* data, int dataLength){
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

int EEPROM::readEXE(int deviceAddress, char* address, int addressLength, char* data, int dataLength){
    int errnum;
    errnum = _i2c->write(deviceAddress, address, addressLength, true);
    if(errnum != 0)return errnum;
    return _i2c->read(deviceAddress, data, dataLength, false);
}

int EEPROM::writeEXE(int deviceAddress, char* memoryAddress, int addressLength, const char* data, int dataLength){
    int errnum;
    char loc_data[addressLength + dataLength];
    for(int i = 0; i < addressLength; i++) loc_data[i] = memoryAddress[i];
    for(int i = 0; i < dataLength; i++) loc_data[i + addressLength] = data[i];
    errnum = _i2c->write(deviceAddress, loc_data, sizeof(loc_data), false);
    wait_ms(5);
    return errnum;
}
