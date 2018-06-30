/**
 *****************************************************************************
 * File Name    : EEPROM.h
 *
 * Title        : I2C EEPROM 24LCXXX Claass Header File
 * Revision     : 1.0
 * Notes        :
 * Target Board : mbed NXP LPC1768, mbed LPC1114FN28  etc
 *
 * Revision History:
 * When         Who         Description of change
 * -----------  ----------- -----------------------
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

#ifndef EEPROM_H_
#define EEPROM_H_

#ifndef MBED_H
#include "mbed.h"
#endif  //end of MBED_H

#define     DEVICE_ADDR         0x50
#define     BLOCK_SIZE          0x100

struct device_t{
        int id;
        int maxAddress;
        int pageSize;
        int blockNumbers;
        bool operator<(const device_t & other)const{
            return (id < other.id);
        }
        bool operator>(const device_t & other)const{
            return (id > other.id);
        }
//        bool operator==(const device_t & other)const{
//            return (id == other.id);
//        }
//        bool operator!=(const device_t & other)const{
//            return (id != other.id);
//
//        bool operator<=(const device_t & other)const{
//            return (id <= other.id);
//        }
//        bool operator>=(const device_t & other)const{
//            return (id >= other.id);
//        }
};

const device_t C24LC01   = {0x001, 0x007F, 0x08, 0x01};
const device_t C24LC02   = {0x002, 0x00FF, 0x08, 0x01};
const device_t C24LC04   = {0x004, 0x01FF, 0x10, 0x02};
const device_t C24LC08   = {0x008, 0x03FF, 0x10, 0x04};
const device_t C24LC16   = {0x010, 0x07FF, 0x10, 0x08};
const device_t C24LC32   = {0x020, 0x0FFF, 0x20, 0x01};
const device_t C24LC64   = {0x040, 0x1FFF, 0x20, 0x01};
const device_t C24LC128  = {0x080, 0x3FFF, 0x40, 0x01};
const device_t C24LC256  = {0x100, 0x7FFF, 0x40, 0x01};
const device_t C24LC512  = {0x200, 0xFFFF, 0x40, 0x01};

namespace wodji {

class EEPROM {

    public:

        /** Constructor
         *
         * @param i2c               - pointer to i2c object
         * @param _device           - eeprom model:
         *                                  C24LC01     - 1k    1 block     8 bytes page    max address 0x007F
         *                                  C24LC02     - 2k    1 block     8 bytes page    max address 0x00FF
         *                                  C24LC04     - 4k    2 blocks    16 bytes page   max address 0x01FF
         *                                  C24LC08     - 8k    4 blocks    16 bytes page   max address 0x03FF
         *                                  C24LC16     - 16k   8 blocks    16 bytes page   max address 0x07FF
         *                                  C24LC32     - 32k   1 block     32 bytes page   max address 0x0FFF
         *                                  C24LC64     - 64k   1 block     32 bytes page   max address 0x1FFF
         *                                  C24LC128    - 128k  1 block     64 bytes page   max address 0x3FFF
         *                                  C24LC256    - 256k  1 block     64 bytes page   max address 0x7FFF
         *                                  C24LC512    - 512k  1 block     64 bytes page   max address 0xFFFF
         * @param deviceAddress     - address of eeprom, default 0x50
         */
        EEPROM(I2C *i2c, const device_t _device = C24LC32,
                const int deviceAddress = DEVICE_ADDR);

        /** Constructor
         *
         * @param sda               - SDA pin of i2c object definitiion
         * @param scl               - SCL pin of i2c object definitiion
         * @param _device           - eeprom model:
         *                                  C24LC01     - 1k    1 block     8 bytes page    max address 0x007F
         *                                  C24LC02     - 2k    1 block     8 bytes page    max address 0x00FF
         *                                  C24LC04     - 4k    2 blocks    16 bytes page   max address 0x01FF
         *                                  C24LC08     - 8k    4 blocks    16 bytes page   max address 0x03FF
         *                                  C24LC16     - 16k   8 blocks    16 bytes page   max address 0x07FF
         *                                  C24LC32     - 32k   1 block     32 bytes page   max address 0x0FFF
         *                                  C24LC64     - 64k   1 block     32 bytes page   max address 0x1FFF
         *                                  C24LC128    - 128k  1 block     64 bytes page   max address 0x3FFF
         *                                  C24LC256    - 256k  1 block     64 bytes page   max address 0x7FFF
         *                                  C24LC512    - 512k  1 block     64 bytes page   max address 0xFFFF
         * @param deviceAddress     - address of eeprom, default 0x50
         */
        EEPROM(PinName sda, PinName scl, const device_t _device = C24LC32,
                const int deviceaAddress = DEVICE_ADDR);

        /** Writing to eeprom memory
         *
         * @param memoryAddress     - address of memory
         * @param data char         - pointer of data
         * @param dataLength        - lengt of data bytes to be sent default 1
         * @return                  - 0 sucsesfull, 1 i2c error, -1 wrong addres, page or block size
         */
        int write(int memoryAddress, const char *data, int dataLength = 1);

        /** Reading from eeprom memory
         *
         * @param memoryAddress     - address of memory
         * @param data char         - pointer of data
         * @param dataLength        - lengt of data bytes to be sent default 1
         * @return                  - 0 sucsesfull, 1 i2c error, -1 wrong addres, page or block size
         */
        int read(int memoryAddress, char *data, int dataLength = 1);

        /** Write any type of data
         *
         * @param memoryAddress     - address of memory
         * @param data eny type     - reference to data type
         * @return                  - 0 sucsesfull, 1 i2c error, -1 wrong addres, page or block size
         */
        template<typename T>
        int write(int memoryAddress, const T &data) {
            const char* dataPointer = (const char*) &data;
            return write(memoryAddress, dataPointer, sizeof(data));
        }

        /** Reading any type of data
         *
         * @param memoryAddress     - address of memory
         * @param data eny type     - reference to data type
         * @return                  - 0 sucsesfull, 1 i2c error, -1 wrong addres, page or block size
         */
        template<typename T>
        int read(int memoryAddress, T &data) {
            char* dataPointer = (char*) (void*) &data;
            return read(memoryAddress, dataPointer, sizeof(data));
        }

    private:

        /** read execution
         *
         * @param deviceAddress     - address of device
         * @param memoryAddress     - address of memory
         * @param addressLength     - length of memory address bytes
         * @param data char         - pointer of data
         * @param dataLength        - lengt of data bytes to be sent
         * @return                  - 0 sucsesfull, 1 i2c error, -1 wrong addres, page or block size
         */
        int readEXE(int deviceAddress, char* memoryAddress, int addressLength, char* data, int dataLength);

        /** write execution
         *
         * @param deviceAddress     - address of device
         * @param memoryAddress     - address of memory
         * @param addressLength     - length of memory address bytes
         * @param data char         - pointer of data
         * @param dataLength        - lengt of data bytes to be sent
         * @return                  - 0 sucsesfull, 1 i2c error, -1 wrong addres, page or block size
         */
        int writeEXE(int deviceAddress, char* memoryAddress, int addressLength, const char* data, int dataLength);

        /** Writing max page size of data
         *
         * @param memoryAddress     - address of memory
         * @param data char         - pointer of data
         * @param dataLength        - lengt of data bytes to be sent
         * @return                  - 0 sucsesfull, 1 i2c error, -1 wrong addres, page or block size
         */
        int writePage(int memoryAddress, const char *data, int dataLength);

        bool            _status;
        int             _deviceAddress;
        I2C*            _i2c;
        const device_t  _device;
};

} //end of namspace wodji

using namespace wodji;

#endif /* EEPROM_H_ */
