//dieses Neue Tab muss umbedingt bei den naechsten Firmwareupdates mitkopiert werden

// 27.11.2012

void setupRelais(){
  //release_altitude = (long)eeprom_read_dword((uint32_t*)4080) * 100; //in cm
  servo_write(CH_6, 1000);
  
  //moegliche Landeorte  
  //Werrikon

  loc_pos_1.id 	= MAV_CMD_NAV_WAYPOINT;
  loc_pos_1.lat = 473624940;				// Lon * 10^7
  loc_pos_1.lng = 87003350;				// Lat * 10^7
  loc_pos_1.alt = 55000;                                // cm
  //Huegel Uster
  loc_pos_2.id 	= MAV_CMD_NAV_WAYPOINT;
  loc_pos_2.lat = 473653160;
  loc_pos_2.lng = 87039080;
  loc_pos_2.alt = 55000;
  //Parkplatz
  loc_pos_3.id 	= MAV_CMD_NAV_WAYPOINT;
  loc_pos_3.lat = 473645830;
  loc_pos_3.lng = 87072510;
  loc_pos_3.alt = 55000;
  //Payerne:
  loc_pos_pay.id 		= MAV_CMD_NAV_WAYPOINT;
  loc_pos_pay.lat 	= 468130430;				// Lon * 10^7
  loc_pos_pay.lng 	= 69437200;				// Lat * 10^7
  loc_pos_pay.alt 	= 49200;                // cm
  
  //nähre von Romont
  //loc_pos_pay.id 		= MAV_CMD_NAV_WAYPOINT;
  //loc_pos_pay.lat 	= 466616790;				// Lon * 10^7
  //loc_pos_pay.lng 	= 69630880;				// Lat * 10^7
  //loc_pos_pay.alt 	= 95000;                // cm
  

  safeAltitudeReached = 0;
}


void ReleaseRelaisActivate(){
  //Relais wird eingeschalten sobald die Hoehe erreicht wird
  if (((ausl < 1) && (current_loc.alt >= g.release_altitude) && (g_gps->status() == GPS::GPS_OK_FIX_3D)) || externActivate == 1){
    servo_write(CH_6, 1800); //Relais wird eingeschalten
    superslow_loopCounter2 = 0;
    ausl = 1;
    externActivate = 0;
  }
  
  //Anfang - ballon abkopeln nach Zeit fals er nicht platzen will (bei GPS Ausfall oder plafonieren)
  if ((ausl < 1 && safeAltitudeReached == 1 && (((millis()-safeAltitudeReachedMillis) / 1000) > (g.release_altitude - (minHeigth * 100)) / 100 / avSteigAusloese))){ 
    externActivate = 1;
	safeAltitudeReached = 2;
	cliSerial->printf_P(PSTR("\n\n---> vom ballon trennen wegen zeit ueberschreitung \n\n"));
  }
  //ende

  //Anfang - wenn Ballon blatzt bevor Mechanismus Ballone ablösen konnte
  if (ausl < 1 && safeAltitudeReached == 0 && current_loc.alt >= (minHeigth * 100)){ 
    safeAltitudeReached = 1;
	safeAltitudeReachedMillis = millis(); 
	oldAlt = current_loc.alt;
  }

  if (ausl < 1 && safeAltitudeReached == 1 && current_loc.alt > oldAlt){
    oldAlt = current_loc.alt;
  }

  if ((ausl < 1 && safeAltitudeReached == 1 && current_loc.alt <= (oldAlt - (200 * 100)))){ //200 meter später reagiert System
    externActivate = 1;
	superslow_loopCounter2 = (time_after_reached - 1) * 3; 
	safeAltitudeReached = 2;
	cliSerial->printf_P(PSTR("\n\n---> vom Ballon trennen Ballon ist bereits geplatzt\n"));
	//cliSerial->println_P(oldAlt);
	cliSerial->printf_P(PSTR("\n\n"));
  }
  //ende


  //Relais wird ausgeschalten nach 5sek (=15)
  if((superslow_loopCounter2 >= (time_after_reached * 3)) && ausl == 1){
    servo_write(CH_6, 1000);
    gcs_send_text_P(SEVERITY_HIGH, PSTR("relay wurde aktiviert"));
	cliSerial->printf_P(PSTR("\n\n---> vom Ballon getrennt \n\n"));
    ausl = 2;
    superslow_loopCounter2 = 0;

    //Wenn ausgesucht wird welcher Waypoint als nächster ist wird HOME neu gesetzt je nach dem ist das
    //dann nicht payerne (Payerne: 46.8130430, 6.9437200)
	/*
	loc_pos_home.id 	= loc_pos_pay.id;
	loc_pos_home.lat 	= loc_pos_pay.lat;
	loc_pos_home.lng 	= loc_pos_pay.lng;
	loc_pos_home.alt 	= loc_pos_pay.alt;
	*/

	home = loc_pos_pay;
	

    //einprogramieren der Waypoints
    //g.command_total.set_and_save(0); //alle wegpunkte löschen
    //g.command_total.set_and_save(3); //wieviele Weppunkte werden gesetzt
    //set_cmd_with_index(home, 0);
    //set_cmd_with_index(loc_pos_1, 1);
    //set_cmd_with_index(loc_pos_2, 2);
    //set_cmd_with_index(loc_pos_3, 3);

    //set_mode(AUTO); //in Missionsmodus versetzen
    //change_command(1); //neustarten der Mission


    //next_WP 	= home;
    //wp_totalDistance 	= get_distance(&current_loc, &next_WP); //in meter

    //nur brauchen wenn der nächst anzufliegende WP nicht home sein soll
    //z.B. wenn man einen Punkt vor dem landefelde anfliegen soll und dann erst Home gealden wird
    //oder wenn man in den Bergen ist damit man pber die Spitzte kommt und nicht schon vorher tiefer fliegt 
    //next_WP.lat 		= home.lat + 1000;	// so we don't have bad calcs
    //next_WP.lng 		= home.lng + 1000;	// so we don't have bad calcs
    //next_WP.alt 		= 40000; //in cm


    //so fliegen wir nach Hause:
    set_mode(RTL);
  }
}



