#ifndef AP_SHT2x_h
#define AP_SHT2x_h

#include "../AP_Math/AP_Math.h"


class AP_SHT2x_Class
{
private:
    static const unsigned char sht2x_address = 0x40;   // sensor I2C 7 bit address

    //==============================================================================
    unsigned char sht2x_check_crc(unsigned char data[]);
    //==============================================================================
    // calculates checksum for n bytes of data and compares it with expected
    // checksum
    // input:  data[]       first byte is expected checksum, 2nd and 3rd bytes are data bytes
    // return: error:       false = checksum does not match
    //                      true  = checksum matches

public:
    //---------- Defines -----------------------------------------------------------
    // sensor command
    typedef enum {
        TRIG_T_MEASUREMENT_POLL  = 0xF3, // command trig. temp meas. no hold master
        TRIG_RH_MEASUREMENT_POLL = 0xF5, // command trig. humidity meas. no hold master
    } etSHT2xCommand;

    // measurement signal selection
    typedef enum {
        HUMI                     = 0x02,
        TEMP                     = 0x00
    } SHT2x_Measure_Type_T;

    unsigned int humidity;
    unsigned int temperature;

    //------------------------------------------------------------------------------

    AP_SHT2x_Class();  // Constructor

    //==============================================================================
    unsigned char sht2x_measure_start(SHT2x_Measure_Type_T sht2x_measure_type);
    //==============================================================================
    // start measurement for humidity or temperature.
    // input:  sht2x_measure_type
    // return: nothing
    // note:

    //==============================================================================
    unsigned char sht2x_measure_poll(SHT2x_Measure_Type_T sht2x_measure_type);
    //==============================================================================
    // measures humidity or temperature. This function polls every 10ms until
    // measurement is ready.
    // input:  sht2x_measure_type
    // output: *pMeasurand:  humidity / temperature as raw value
    // return: error

    //==============================================================================
    float sht2x_calc_humidity(unsigned int u16sRH);
    //==============================================================================
    // calculates the relative humidity
    // input:  sRH: humidity raw value (16bit scaled)
    // return: pHumidity relative humidity [%RH]

    //==============================================================================
    float sht2x_calc_temperature(unsigned int u16sT);
    //==============================================================================
    // calculates temperature
    // input:  sT: temperature raw value (16bit scaled)
    // return: temperature [°C]
};

#endif
