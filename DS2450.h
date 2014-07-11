/*
 *   DS2450.h
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

#ifndef DS2450_h
#define DS2450_h

#include <Arduino.h>
#include <OneWire.h>

#define DS2450_WRITE_COMMAND 0x55
#define DS2450_BEGIN_VOLTAGE_CONVERSION 0x3c
#define DS2450_REGISTER_4CHANNEL 0x0f
#define DS2450_READOUT_CONTROL 0xaa
#define DS2450_MEMORY_READ_COMMAND 0xaa
#define DS2450_8_BIT_RESOLUTION 0x08
#define DS2450_POR_OFF_NO_ALARMS_5V_RANGE 0x01
#define DS2450_VCC_CONTROL_BYTE_ADDR_LO 0x1c
#define DS2450_VCC_CONTROL_BYTE_ADDR_HI 0x00
#define DS2450_VCC_POWERED 0x40
#define DS2450_CONTROL_STATUS_DATA_ADDR_LO 0x08
#define DS2450_CONTROL_STATUS_DATA_ADDR_HI 0x00
#define DS2450_CONVERSION_COMPLETE 0xff
#define DS2450_AD_CHANNELS_ADDR_LO 0x00
#define DS2450_AD_CHANNELS_ADDR_HI 0x00
#define DS2450_TEMP_CONVERSION_TIMEOUT 50

class DS2450 {
    public:
        DS2450(OneWire *ow, uint8_t *address);
        boolean begin();
        void update();
        float getVoltage(int channel);
        boolean isError();
        unsigned long getTimestamp();
    private:
        OneWire *_ow;
        uint8_t *_address;
        float _voltage[4];
        uint32_t _timestamp;
        boolean _error;
};

#endif
