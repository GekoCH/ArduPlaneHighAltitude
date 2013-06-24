


void ownSonar(){
	/*
	//#define Sonar_Pin 10
	#define sonar_always 1

	static AP_AnalogSource_Arduino sonarReadPin(10);
	uint16_t sonar_Value = sonarReadPin.read_average();

	uint16_t thisPitch = map(sonar_Value, 10, 120, 100, 1500);



	if (sonar_Value < 100 || sonar_always == 1){
	analogWrite(44, thisPitch);
	}

	*/

}
void bluetoothModul(){
	if (g.fpv){
		if (hal.rcin->read(CH_5) < 1700 && (g_gps->ground_speed * 0.01) <= 2){   //Bluetooth modul kann damit eigestellt werden aber nur wenn Speed < 2m/s
			digitalWrite(13, HIGH); //anschalten
		}else{
			digitalWrite(13, LOW); //ausschalten
		}
	}
}

void intRTL(){
	if (g.fpv){
		if (control_mode == RTL){
			if (switchRTL == 0){
				switchRTL = 1;
				if ((2 * abs(current_loc.alt - home.alt)) < get_distance_cm(&current_loc, &home)){ //wenn das FLugzeug weniger Steil als in einem 45 grad winkel zu uns steht wird +100 meter 
					if (home.alt < current_loc.alt && (current_loc.alt - home.alt) < 100000){ //neue RTL altitude ist plus 100 meter von aktueller Höhe	
						g.RTL_altitude_cm = (current_loc.alt - home.alt) + 10000; //falls do_rtl erst nach diesem Command passiert ist die RTL_altitude angepasst
						next_WP.alt = read_alt_to_hold(); //falls RTL schon von do_rtl gesetzt wurde funktioniert dies
					}
				}else{
					g.RTL_altitude_cm = 10000; //100 meter
					next_WP.alt = read_alt_to_hold();				
				}
			}
		}else{
			switchRTL = 0;
		}

	}
}


void sendOSD(){
	if (g.fpv){




		if (hal.rcin->read(CH_4) < 1320) //ganz links: OSD ausschalten
			OSDactivate = 0;
		else if (hal.rcin->read(CH_4) > 1700){ //ganz rechts: OSD einschalten
			OSDactivate = 1;
			wpNum = 1;
		}

		//change_command(1); //neustarten der Mission und fliegt direkt zum ersten WP und nicht zuerst zu HOME

/* Muss noch fuer 2.73 angepasst werden

		if (OSDactivate == 1){
			if (g.command_total > 0 && g_gps->status() > 1 && (control_mode == MANUAL || control_mode == AUTO || control_mode == STABILIZE)){
				//entweder mit einem Drehschalter
				//wpNum = map(hal.rcin->read(CH_4), 1300, 1800, 1, g.command_total);


				//mit dem linken stick nach rechts = +1 nach links = -1
				if (hal.rcin->read(CH_4) > 1620 && hal.rcin->read(CH_4) < 1700 && wpNum < g.command_total)
					wpNum += 1;
				if (hal.rcin->read(CH_4) < 1410 && hal.rcin->read(CH_4) > 1320 && wpNum > 1)
					wpNum -= 1;


				struct Location temp = get_cmd_with_index(wpNum);

				set_next_WP(&temp);

				//wp_totalDistance        = get_distance(&current_loc, &temp);
				//target_bearing_cd       = get_bearing_cd(&current_loc, &temp);

				Serial2.print("+wpdi,");
				if (wp_totalDistance > 999){
					Serial2.print(int(wp_totalDistance/1000));
					Serial2.print(",1");
				}else{
					Serial2.print(wp_totalDistance);
					Serial2.print(",0");
				}	
				Serial2.print("*");


				Serial2.print("+wpco,");
				Serial2.print(int(target_bearing_cd / 100));
				Serial2.print("*");

				Serial2.print("+wpnu,");
				Serial2.print(wpNum);
				Serial2.print("*");


				Serial2.print("+head,");
				Serial2.print((ahrs.yaw_sensor / 100) % 360);
				//Serial2.print(int(nav_bearing_cd / 100));
				Serial2.print("*");
			}else{
				Serial2.print("+nowp*");
			}


			Serial2.print("+debu,");
			Serial2.print(g.command_total);
			Serial2.print("*");

			if (g_gps->status() > 1){
				Serial2.print("+alt,");
				Serial2.print(int(g_gps->altitude/100)); //in meterÜberMeer
				Serial2.print("*");
			}

			Serial2.print("+mode,");
			Serial2.print(control_mode);
			Serial2.print("*");


			Serial2.print("+sfix,");
			Serial2.print(g_gps->status());
			Serial2.print("*");

		}
		else{
			Serial2.print("+clear*");
		}
		*/
	}
}