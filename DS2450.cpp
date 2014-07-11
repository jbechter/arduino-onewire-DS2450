/*
 *   DS2450.cpp
 *
 *   by Joe Bechter
 *
 *   (C) 2012, bechter.com
 *
 *   All files, software, schematics and designs are provided as-is with no warranty.
 *   All files, software, schematics and designs are for experimental/hobby use.
 *   Under no circumstances should any part be used for critical systems where safety,
 *   life or property depends upon it. You are responsible for all use.
 *   You are free to use, modify, derive or otherwise extend for your own non-commercial purposes provided
 *       1. No part of this software or design may be used to cause injury or death to humans or animals.
 *       2. Use is non-commercial.
 *       3. Credit is given to the author (i.e. portions © bechter.com), and provide a link to the original source.
 *
 */

#include "DS2450.h"

DS2450::DS2450(OneWire *ow, uint8_t *address) {
    _ow = ow;
    _address = address;
};

boolean DS2450::begin() {
    _ow->reset();
    _ow->select(_address);
    _ow->write(DS2450_WRITE_COMMAND, 0);
    _ow->write(DS2450_VCC_CONTROL_BYTE_ADDR_LO, 0);
    _ow->write(DS2450_VCC_CONTROL_BYTE_ADDR_HI, 0);
    _ow->write(DS2450_VCC_POWERED, 0);
    _ow->read(); // crc lsb
    _ow->read(); // crc msb
    _ow->read(); // verify

    _ow->reset();
    _ow->select(_address);
    _ow->write(DS2450_WRITE_COMMAND, 0);
    _ow->write(DS2450_CONTROL_STATUS_DATA_ADDR_LO, 0);
    _ow->write(DS2450_CONTROL_STATUS_DATA_ADDR_HI, 0);
    for (int i = 0; i < 4; i++) {
        _ow->write(DS2450_8_BIT_RESOLUTION, 0);
        _ow->read(); // crc
        _ow->read();
        _ow->read(); // verify data
        _ow->write(DS2450_POR_OFF_NO_ALARMS_5V_RANGE, 0);
        _ow->read(); // crc
        _ow->read();
        _ow->read(); // verify data
        _voltage[i] = 0.0;
    }
    _timestamp = 0;
    _error = true;
    return true;
}

void DS2450::update() {
    uint8_t data[13];

    _timestamp = millis();
    _error = true;

    _ow->reset();
    _ow->select(_address);
    data[0] = DS2450_BEGIN_VOLTAGE_CONVERSION;
    data[1] = DS2450_REGISTER_4CHANNEL;
    data[2] = DS2450_READOUT_CONTROL;
    _ow->write(data[0], 0);
    _ow->write(data[1], 0);
    _ow->write(data[2], 0);
    data[3] = _ow->read(); // crc
    data[4] = _ow->read();
    while(1) {
        if ((long)millis() - (long)_timestamp <= DS2450_TEMP_CONVERSION_TIMEOUT) {
            if (_ow->read() == DS2450_CONVERSION_COMPLETE) {
                break;
            }
        } else {
            // conversion taking too long
            return;
        }
    }
    delay(1);
    _ow->reset();
    _ow->select(_address);
    data[0] = DS2450_MEMORY_READ_COMMAND;
    data[1] = DS2450_AD_CHANNELS_ADDR_LO;
    data[2] = DS2450_AD_CHANNELS_ADDR_HI;
    _ow->write(data[0]);
    _ow->write(data[1]);
    _ow->write(data[2]);
    for (int i = 0; i < 10; i++) {
        data[i + 3] = _ow->read();
    }
    _voltage[0] = (float)data[4] / 50.0;
    _voltage[1] = (float)data[6] / 50.0;
    _voltage[2] = (float)data[8] / 50.0;
    _voltage[3] = (float)data[10] / 50.0;
    uint16_t crc = _ow->crc16(data, 11);
    uint8_t *crcBytes = (uint8_t *)&crc;
    uint8_t crcLo = ~data[11];
    uint8_t crcHi = ~data[12];
    _error = !(crcBytes[0] == crcLo && crcBytes[1] == crcHi);
}

float DS2450::getVoltage(int channel) {
    if (channel >= 0 && channel < 4) {
        return _voltage[channel];
    } else {
        return 0.0;
    }
}

boolean DS2450::isError() {
    return _error;
}

unsigned long DS2450::getTimestamp() {
    return _timestamp;
}


