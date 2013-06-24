/*
      10.01.2013
*/

#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#define isPAL 1


#include <FastSerial.h>
#include <AP_Common.h>
#include <AP_Math.h>
#include <math.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
// Get the common arduino functions
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif
#include <GCS_MAVLink.h>

#ifdef membug
#include <MemoryFree.h>
#endif

// Configurations
#include "OSD_Config.h"
#include "ArduCam_Max7456.h"
#include "OSD_Vars.h"
#include "OSD_Func.h"

#define  zeileEins 3
#define  zeileZwei 4

int osdOn = 1;

#define MinimOSD

#define TELEMETRY_SPEED  57600  // How fast our MAVLink telemetry is coming to Serial port
#define BOOTTIME         2000   // Time in milliseconds that we show boot loading bar and wait user input

// Objects and Serial definitions
FastSerialPort0(Serial);
OSD osd; //OSD object 


void setup(){
  pinMode(MAX7456_SELECT,  OUTPUT);
  Serial.begin(TELEMETRY_SPEED);
  // setup mavlink port
  mavlink_comm_0_port = &Serial;

  // Prepare OSD for displaying 
  unplugSlaves();
  osd.init();

  //startPanels();
  //delay(1000);

  apm_mav_type = 1; //arduplane

  updatePanel();
}


void loop(){



  read_serial();
}






void updatePanel(){
	if (osdOn){
  osd.clear();


  //richtung und distanz zum nächsten wegpunkt
  osd.setPanel(1, zeileEins);
  osd.openPanel();
  if (!wp_no){
    osd.printf("%1.0f", (double)wp_number);

    osd_wind_arrow_rotate_int = round((osd_waypointDirection - osd_heading)/360.0 * 16.0) + 1; //Convert to int 1-16 
    if(osd_wind_arrow_rotate_int < 0 ) osd_wind_arrow_rotate_int += 16; //normalize
    showArrow((uint8_t)osd_wind_arrow_rotate_int, 0);
    // +wpdi,901*
    if (wp_dist > 0 && wp_einheit == 0) //wenn die Distanz in meter uebermittelt wird
      osd.printf("%1.0fm", (double)((float)wp_dist));
    if (wp_dist > 0 && wp_einheit == 1) //wenn km uebermittelt werden
      osd.printf("%1.0fkm", (double)((float)wp_dist));
  }
  else{
    osd.printf("       ");
  }
  osd.closePanel();
  
  
  //GPS fix ja oder nein
  osd.setPanel(14, zeileEins);
  osd.openPanel();
  if(osd_fix_type == 0 || osd_fix_type == 1)
    osd.printf("%c",0x12);
  osd.closePanel();

  //modus des Autopiloten anzeigen
  panFlightMode(1, zeileZwei); 

  //höhe über meer
  osd.setPanel(24, zeileEins);
  osd.openPanel();
  osd.printf("%c%2.0f",0x85, (double)osd_alt);
  //osd.printf("%3.0f", (double)((float)osd_alt));
  osd.closePanel();


	}
}




void unplugSlaves(){
  //Unplug list of SPI
#ifdef ArduCAM328
  digitalWrite(10,  HIGH); // unplug USB HOST: ArduCam Only
#endif
  digitalWrite(MAX7456_SELECT,  HIGH); // unplug OSD
}

/*
void OnMavlinkTimer()
 {
 setHeadingPatern();  // generate the heading patern
 
 //  osd_battery_pic_A = setBatteryPic(osd_battery_remaining_A);     // battery A remmaning picture
 //osd_battery_pic_B = setBatteryPic(osd_battery_remaining_B);     // battery B remmaning picture
 
 setHomeVars(osd);   // calculate and set Distance from home and Direction to home
 
 writePanels();       // writing enabled panels (check OSD_Panels Tab)
 }
 */



















