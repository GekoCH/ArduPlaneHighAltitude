// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

// filter altitude from the barometer with a low pass filter
static LowPassFilterInt32 altitude_filter;


static void init_barometer(void)
{
    gcs_send_text_P(SEVERITY_LOW, PSTR("Calibrating barometer"));    
    barometer.calibrate();

    // filter at 100ms sampling, with 0.7Hz cutoff frequency
    altitude_filter.set_cutoff_frequency(0.1, 0.7);

    gcs_send_text_P(SEVERITY_LOW, PSTR("barometer calibration complete"));
}

// read the barometer and return the updated altitude in centimeters
// above the calibration altitude
static int32_t read_barometer(void)
{
    barometer.read();
    return altitude_filter.apply(barometer.get_altitude() * 100.0);
}

// in M/S * 100
static void read_airspeed(void)
{
    airspeed.read();
    calc_airspeed_errors();
}

static void zero_airspeed(void)
{
    airspeed.calibrate();
    gcs_send_text_P(SEVERITY_LOW,PSTR("zero airspeed calibrated"));
}

static void read_battery(void)
{
    if(g.battery_monitoring == 0) {
        battery_voltage1 = 0;
        return;
    }

    if(g.battery_monitoring == 3 || g.battery_monitoring == 4) {
        // this copes with changing the pin at runtime
        batt_volt_pin->set_pin(g.battery_volt_pin);
        battery_voltage1 = BATTERY_VOLTAGE(batt_volt_pin);
    }
    if(g.battery_monitoring == 4) {
        // this copes with changing the pin at runtime
        batt_curr_pin->set_pin(g.battery_curr_pin);
        current_amps1    = CURRENT_AMPS(batt_curr_pin);
        current_total1   += current_amps1 * (float)delta_ms_medium_loop * 0.0002778;                                    // .0002778 is 1/3600 (conversion to hours)
    }

if (g.DRONE_BATTERY_EVENT == ENABLED){
    if (control_mode >= FLY_BY_WIRE_A && control_mode < LOITER){
        //if(battery_voltage1 < LOW_VOLTAGE)	low_battery_event();
//    if(g.battery_monitoring == 4 && current_total1 > g.pack_capacity) low_battery_event();
      if(g.battery_monitoring == 4 && ((100.0 * (g.pack_capacity - current_total1) / g.pack_capacity) <= g.minimBattery))	low_battery_event(); //das -1 ist damit er genua bei zb. 98 umkehrt und nicht erst bei 97
  }
}
}

// read the receiver RSSI as an 8 bit number for MAVLink
// RC_CHANNELS_SCALED message
void read_receiver_rssi(void)
{
    rssi_analog_source->set_pin(g.rssi_pin);
    float ret = rssi_analog_source->voltage_average() * 50;
    receiver_rssi = constrain_int16(ret, 0, 255);
}


/*
  return current_loc.alt adjusted for ALT_OFFSET
  This is useful during long flights to account for barometer changes
  from the GCS, or to adjust the flying height of a long mission
 */
static int32_t adjusted_altitude_cm(void)
{
    return current_loc.alt - (g.alt_offset*100);
}




void lm335Read(void){
	temp_pin1->set_pin(TEMP_PIN_1);  //TEMP_PIN_1
    tempLM335K = temp_pin1->voltage_average() * 100 - g.LM335offset;

	if (tempLM335K < 0 || tempLM335K > 400)
		tempLM335K = 0;

	/*
	hal.console->println();
	hal.console->print("LM335: ");
	hal.console->print((tempLM335K));
	hal.console->println();
	*/
	//hal.uartC->print("LM335: ");
	//hal.uartC->println((tempLM335K-273.15));
}



void readDallas(){
	int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

  OneWireReset(TEMP_PIN_DALLAS);
  OneWireOutByte(TEMP_PIN_DALLAS, 0xcc);
  OneWireOutByte(TEMP_PIN_DALLAS, 0x44); // perform temperature conversion, strong pullup for one sec

  OneWireReset(TEMP_PIN_DALLAS);
  OneWireOutByte(TEMP_PIN_DALLAS, 0xcc);
  OneWireOutByte(TEMP_PIN_DALLAS, 0xbe);

  LowByte = OneWireInByte(TEMP_PIN_DALLAS);
  HighByte = OneWireInByte(TEMP_PIN_DALLAS);
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit

  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
    
  temperatureDallas = Whole + 100;
  
  hal.uartC->print("DS1820: ");
  hal.uartC->println(Whole);    

  if (SignBit){
    temperatureDallas *= -1; 
  }
}


void OneWireReset(int Pin) // reset.  Should improve to act as a presence pulse
{
  digitalWrite(Pin, LOW);
  pinMode(Pin, OUTPUT); // bring low for 500 us
  hal.scheduler->delay_microseconds(500);
  pinMode(Pin, INPUT);
  hal.scheduler->delay_microseconds(500);
}


void OneWireOutByte(int Pin, uint8_t d) // output byte d (least sig bit first).
{
  uint8_t n;

  for(n=8; n!=0; n--)
  {
    if ((d & 0x01) == 1)  // test least sig bit
    {
      digitalWrite(Pin, LOW);
      pinMode(Pin, OUTPUT);
      hal.scheduler->delay_microseconds(5);
      pinMode(Pin, INPUT);
      hal.scheduler->delay_microseconds(60);
    }
    else
    {
      digitalWrite(Pin, LOW);
      pinMode(Pin, OUTPUT);
      hal.scheduler->delay_microseconds(60);
      pinMode(Pin, INPUT);
    }

    d=d>>1; // now the next bit is in the least sig bit position.
  }

}

uint8_t OneWireInByte(int Pin) // read byte, least sig byte first
{
  uint8_t d, n, b;

  for (n=0; n<8; n++)
  {
    digitalWrite(Pin, LOW);
    pinMode(Pin, OUTPUT);
    hal.scheduler->delay_microseconds(5);
    pinMode(Pin, INPUT);
    hal.scheduler->delay_microseconds(5);
    b = digitalRead(Pin);
    hal.scheduler->delay_microseconds(50);
    d = (d >> 1) | (b<<7); // shift d to right and insert b in most sig bit position
  }
  return(d);
}


