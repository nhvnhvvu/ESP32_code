#include "PinSensor.h"

PinSensor::PinSensor()
{
	Wire.begin();
}

PinSensor::PinSensor(uint8_t intr, func f)
{
	attachInterrupt(intr, f, FALLING);
	Wire.begin();
}

double PinSensor::getVoltage()
{
	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_VCELL_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1704X_ADDR, (uint8_t)2);
	return ((Wire.read() << 4) + (Wire.read() >> 4)) * 0.00125;
}

double PinSensor::getSOC()
{
	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_SOC_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1704X_ADDR, (uint8_t)2);
	return Wire.read() + (double)Wire.read() / 256;
}

uint8_t PinSensor::getCompensation()
{
	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_RCOMP_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1704X_ADDR, (uint8_t)1);
	return Wire.read();
}

uint8_t PinSensor::getAlertThreshold()
{
	return (~getStatus() & 0x1F) + 1;
}

uint8_t PinSensor::getStatus()
{
	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_ATHRD_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1704X_ADDR, (uint8_t)1);
	return Wire.read();
}

uint8_t PinSensor::setAlertThreshold(uint8_t thrd)
{
	if (thrd > 32)
		thrd = 32;
	else if (thrd < 1)
		thrd = 1;
	thrd = (~thrd + 1) & 0x1F;

	uint8_t comp, sleepBit;
	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_CONFIG_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1704X_ADDR, (uint8_t)2);
	comp = Wire.read();
	sleepBit = Wire.read() & 0x80;

	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_CONFIG_ADDR);
	Wire.write(comp);
	Wire.write(sleepBit | thrd);
	return Wire.endTransmission();
}

uint8_t PinSensor::clearAlertInterrupt()
{
	uint8_t comp = getCompensation();
	uint8_t status = getStatus();

	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_CONFIG_ADDR);
	Wire.write(comp);
	Wire.write(0xDF & status);
	return Wire.endTransmission();
}

uint8_t PinSensor::sleep()
{
	uint8_t comp = getCompensation();
	uint8_t thrd = getAlertThreshold();

	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_CONFIG_ADDR);
	Wire.write(comp);
	Wire.write(0x80 | thrd);
	return Wire.endTransmission();
}

uint8_t PinSensor::wake()
{
	uint8_t comp = getCompensation();
	uint8_t thrd = getAlertThreshold();

	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_CONFIG_ADDR);
	Wire.write(comp);
	Wire.write(0x7F & thrd);
	return Wire.endTransmission();
}

boolean PinSensor::sleeping()
{
	return (getStatus() & 0x80) == 0x80;
}

uint8_t PinSensor::quickStart()
{
	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_MODE_ADDR);
	Wire.write(0x40);
	Wire.write(0x00);
	return Wire.endTransmission();
}

uint8_t PinSensor::reset()
{
	Wire.beginTransmission(MAX1704X_ADDR);
	Wire.write(MAX1704X_COMMAND_ADDR);
	Wire.write(0x54);
	Wire.write(0x00);
	return Wire.endTransmission();
}