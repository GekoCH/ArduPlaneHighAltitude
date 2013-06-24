// -*- formatted with the "Artistic Style" configuration file for the ArduPilot Coding Conventions -*-
/*
    AP_SHT2x.cpp - Arduino Library for Sensirion SHT2x I2C temperature and humidity sensor
    Code by Eric Larmanou

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
*/

extern "C" {
// AVR LibC Includes
#include <math.h>
#include "WConstants.h"
}

#include <Wire.h>
#include "AP_SHT2x.h"

#define error_result_temperature 0xFFFC;
#define error_result_humidity 0xFFFE;

//#define sht2x_address (unsigned char) 0x40  // sensor I2C 7 bit address
#define polynomial 0x131  //CRC P(x)=x^8+x^5+x^4+1 = 100110001

// Constructors ////////////////////////////////////////////////////////////////
AP_SHT2x_Class::AP_SHT2x_Class() {}

// Private Methods //////////////////////////////////////////////////////////////

//==============================================================================
unsigned char AP_SHT2x_Class::sht2x_check_crc(unsigned char data[])
//==============================================================================
{
    //const unsigned int polynomial = 0x131;  //CRC P(x)=x^8+x^5+x^4+1 = 100110001
    unsigned char crc = 0;
    signed char byteCtr;

    //calculates 8-Bit checksum with given polynomial
    for (byteCtr = 2; byteCtr > 0; --byteCtr) {
        crc ^= (data[byteCtr]);
        for (signed char bit = 8; bit > 0; --bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc = (crc << 1);
            }
        }
    }
    if (crc == data[0]) {
        return true;
    } else {
        return false;
    }
}

// Public Methods //////////////////////////////////////////////////////////////

//===========================================================================
unsigned char AP_SHT2x_Class::sht2x_measure_start(SHT2x_Measure_Type_T sht2x_measure_type)
//===========================================================================
{
    //-- write I2C sensor address and command --
    Wire.beginTransmission(sht2x_address);
    switch (sht2x_measure_type) {
    case HUMI:
        Wire.send(TRIG_RH_MEASUREMENT_POLL);
        break;
    case TEMP:
        Wire.send(TRIG_T_MEASUREMENT_POLL);
        break;
    }

    return Wire.endTransmission();
	//Wire.endTransmission returns:
	//0:success
	//1:data too long to fit in transmit buffer
    //2:received NACK on transmit of address
    //3:received NACK on transmit of data
    //4:other error
}

//===========================================================================
unsigned char AP_SHT2x_Class::sht2x_measure_poll(SHT2x_Measure_Type_T sht2x_measure_type)
//===========================================================================
{
    //const unsigned int error_result = 0xFFFC;
    unsigned char data[3];  //data array for checksum verification
    unsigned char i = 3;    //
    
    switch (sht2x_measure_type) {
    case TEMP:
        temperature = error_result_temperature;
        break;
    case HUMI:
        humidity = error_result_humidity;
        break;
    }
    
    //-- poll for measurement ready.
    Wire.requestFrom(sht2x_address, (unsigned char)3);  // request 3 bytes from device
    delayMicroseconds(10000);  //delay 10ms

    //-- read two data bytes and one checksum byte
	while (Wire.available()) {
        if (i != 0) {
            data[--i] = Wire.receive();  // receive one byte
        } else {
            return 0x05; //too many bytes
        }
    }

    if (i != 0) return 0x08 + i; //we didn't get enough bytes

    //-- verify checksum --
    if (sht2x_check_crc(data) == false) return 0x06;
    
    //verify appropriate answer on LSB (bit1=1: humidity, bit1=0: temperature)
    if ((data[1] & 0x02) != sht2x_measure_type) return 0x07;

    switch (sht2x_measure_type) {
    case TEMP:
        temperature = *((unsigned int *) (&data[1]));
        break;
    case HUMI:
        humidity = *((unsigned int *) (&data[1]));
        break;
    }

    return 0;
}

//==============================================================================
float AP_SHT2x_Class::sht2x_calc_humidity(unsigned int u16sRH)
//==============================================================================
{
    float humidityRH;              // variable for result

    u16sRH &= ~0x0003;          // clear bits 1 & 0 (status bits)
    //-- calculate relative humidity [%RH] --
    //result between -6.0 and 118.99
    humidityRH = -6.0 + 125.0 / 65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    return humidityRH;
}

//==============================================================================
float AP_SHT2x_Class::sht2x_calc_temperature(unsigned int u16sT)
//==============================================================================
{
    float temperatureC;            // variable for result

    u16sT &= ~0x0003;           // clear bits 1 & 0 (status bits)

    //-- calculate temperature [°C] --
    //result between -46.85 and 128.86
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
    return temperatureC;
}

