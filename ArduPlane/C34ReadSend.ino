
void setupC34(){
	hal.uartC->begin(2400, 128, SERIAL_BUFSIZE);
}


/*
void calcCRwithGPS(){
	if (g_gps->status() >= GPS::GPS_OK_FIX_3D && (millis() - counterCRgpsMillis)  >= (secondsCRGPS * 1000)) {		
		crFromGPS = (current_loc.alt - lastAlt) / 100.0 / secondsCRGPS; //mittel über secondsCRGPS sekunden
		lastAlt = current_loc.alt;
		counterCRgpsMillis = millis();
	}
}
*/

union Number{
  long numbI;
  uint8_t barray[5];
} 
II;  

union floatNumber{
  float numbF;
  uint8_t barray[5];
}
FF;  


void readC34(){

  while(hal.uartC->available()){
  
    int tempit = int(hal.uartC->read());
	
	
    if (tempit == 0){ //wenn es eine O ist dann könnte dies der anfang sein
      unlock = 1;
      hal.uartC->write(tempit);
      break;
    }

    //wenn schon unlock ist und ein 255 kommt dann wird ein anderer Datensatz eingespitzt
    if (tempit == 255 && unlock == 1 && millis() > (milC34 + 1000)){
      // integerUebermitteln(  
	  // floatUebermitteln(
		
		if (countC34send >= 5){ //diese Daten werden nur alle 5 Sekunden gesendet
			floatUebermitteln(70, control_mode); //Modus
			floatUebermitteln(71, battery_voltage1); //Batterie Spannung (in Volt)
			floatUebermitteln(72, battery_remaining);
			floatUebermitteln(73, g.release_altitude/100); //Ausloesehoehe
			floatUebermitteln(74, ((uint16_t)(constrain_int32(g.channel_throttle.norm_output() * 100, -100, 100) + 100)) / 2); //throttle in %
			floatUebermitteln(80, barometer.get_temperature()/10.0); //Barometer Temp (C)
			floatUebermitteln(81, tempLM335K - 273.15); //Batterien Temperatur (in C)

			countC34send = 0;
		}
		
		floatUebermitteln(75, ahrs.roll*180/PI); //Roll (°)
		floatUebermitteln(76, (ahrs.pitch - radians(g.pitch_trim_cd*0.01))*180/PI); //Pitch (°)
		floatUebermitteln(77, ahrs.yaw*180/PI); //bearing (°)
		
		floatUebermitteln(78, (g_gps->ground_speed) / 100.0); // (m/s)
		floatUebermitteln(79, airspeed.get_airspeed()); // (m/s)
				
		floatUebermitteln(82, ((battery_current * 10.0) / 1000.0)); // aktuellen verbrauch falls motor wieder einsetzt (A)
		
		floatUebermitteln(83, get_distance(&current_loc, &home)); //in meter wie weit nach hause

		//floatUebermitteln(84, release_wire_voltage); //gibt die Spannung der Batterie an welche für das abkoppeln zuständig ist
		floatUebermitteln(84, g_gps->velocity_down()); //cr vom flieger mithilfe des GPS berechnet

		floatUebermitteln(85, nav_pitch_cd); //cr vom flieger mithilfe des GPS berechnet


		
		////////////////////////////////////////////////////////////////
		////////////////////in AMS42 Config file einfuegen//////////////
		////////////////////////////////////////////////////////////////


		countC34send++;
		milC34 = millis();
		break;
    }
    else if (tempit != 255 && unlock == 1){
      hal.uartC->write(tempit);
      unlock = 0;
      break;
    }
    hal.uartC->write(tempit);
  }
}





void integerUebermitteln(int kanal, int wert){
  II.numbI = wert;
  hal.uartC->write(255);
  hal.uartC->write(kanal);    
  for(int i=0; i < 4; i++){
    hal.uartC->write(II.barray[3-i]);
  }
  //checksumme
  uint8_t WertA = 0;
  uint8_t WertB = 0;
  for(int i=0; i < 5; i++){
    if (i==0){
      WertA = WertA + uint8_t(kanal);
    }
    else{
      WertA = WertA + II.barray[4-i];
    }
    WertB = WertB + WertA;
  }
  WertB = ~WertB;
  hal.uartC->write(WertA);
  hal.uartC->write(WertB);
  //anfang des naechsten Datensatztes schreiben
  hal.uartC->write(uint8_t(0x00));
  unlock = 0;
}




void floatUebermitteln(int kanal, float wert){
  FF.numbF = wert;
  hal.uartC->write(255);
  hal.uartC->write(kanal);    
  for(int i=0; i < 4; i++){
    hal.uartC->write(FF.barray[3-i]);
  }

  //checksumme
  uint8_t WertA = 0;
  uint8_t WertB = 0;
  for(int i=0; i < 5; i++){
    if (i==0){
      WertA = WertA + uint8_t(kanal);
    }
    else{
      WertA = WertA + FF.barray[4-i];
    }
    WertB = WertB + WertA;
  }
  WertB = ~WertB;
  hal.uartC->write(WertA);
  hal.uartC->write(WertB);

  //anfang des naechsten Datensatztes schreiben
  hal.uartC->write(uint8_t(0x00));
  unlock = 0;
}





