#ifndef LiFuelGauge_h
#define LiFuelGauge_h

#include <Arduino.h>
#include <Wire.h>

const uint8_t MAX1704X_ADDR = 0x36;
const uint8_t MAX1704X_VCELL_ADDR = 0x02;
const uint8_t MAX1704X_SOC_ADDR = 0x04;
const uint8_t MAX1704X_MODE_ADDR = 0x06;
const uint8_t MAX1704X_VERSION_ADDR = 0x08;
const uint8_t MAX1704X_CONFIG_ADDR = 0x0C;
const uint8_t MAX1704X_RCOMP_ADDR = 0x0C;
const uint8_t MAX1704X_ATHRD_ADDR = 0x0D;
const uint8_t MAX1704X_COMMAND_ADDR = 0xFE;

typedef void (*func)();

class PinSensor
{
public:
    PinSensor();
    PinSensor(uint8_t intr, func f);
    double getVoltage(); // 0-5V range w/ 1.25mV resolution for the MAX17043
    double getSOC();     // resolution 1/256%

    uint8_t getCompensation(); // return the MSByte of the CONFIG register
    uint8_t getAlertThreshold();
    uint8_t getStatus();                     // returns the LSByte of the CONFIG register
    uint8_t setAlertThreshold(uint8_t thrd); // the acceptable range is 1-32%. Default threshold is 4%
    uint8_t clearAlertInterrupt();           // clears the alert bit on the CONFIG register
    uint8_t sleep();                         // puts the MAX17043 to sleep
    uint8_t wake();
    boolean sleeping();
    uint8_t quickStart(); // restart fuel-gauge calculations
    uint8_t reset();      // reset completely

};

#endif