
void readControlSwitches(){

	/*
	if (g.fpv == 0 && hal.rcin->read(CH_6) > 1800 && g.RTL_altitude_cm > 10000){
		g.RTL_altitude_cm = 10000;
		prev_WP = current_loc;
        do_RTL();
		//set_mode(RTL);
	}
	*/

	//Wenn die Homelocation erreicht wurde soll er runter auf 100m über Boden kreisen
	if (wp_distance < 500 && control_mode == RTL && current_loc.alt <= 300000UL && ausl == 2){
		g.RTL_altitude_cm = 10000;
		g.loiter_radius = 150;
		g.waypoint_radius = 150;
		do_RTL();
		ausl = 3;

		gcs_send_text_P(SEVERITY_HIGH, PSTR("runter auf 100m über boden kreisen"));
	}

		
	if (hal.rcin->read(CH_6) > 1800 && ausl < 1 && externActivate == 0){ //fuer Notfall falls Flieger nicht von ballon getrennt wird Manuel auslösen
		externActivate = 1;
	}
		

	//read control switch muss aus der slowloop schleife herausgenommen werden da es sonst nur alle
	// 5 sekunden gebraucht wird
	/*
	if (control_mode == RTL &&
		get_distance(&current_loc, &home) < 100 && //distanz zwischen current und home in meter
		abs(current_loc.alt - read_alt_to_hold()) <= 2500 &&  //curent altitude in cm    und home altitude in cm, zusätzlich 25 meter spazig
		MotorAbstellen == 0){
			hal.console->println();
			hal.console->println("RTL wurde erreicht Fallschirm wird ausgeloest");
			hal.console->println();

			//set_mode(MANUAL); //während des Fallschirmabstiegs braucht es keine Stabilisierung
			servo_write(CH_5, 1100); //fallschirm wird ausgelöst
			MotorAbstellen = 1; //damit in attiude.ino motor abgestellt wird (muss noch einprogrammiert werden und muss nicht mehr auf den Radio CH5 schauen sondern auf ausl parameter)			
	}
	
	if (hal.rcin->read(CH_5) < 1200 && MotorAbstellen == 1){
		MotorAbstellen = 0;
	}
	*/



	/*
    update_loiter();
	if (wp_distance <= (uint32_t)max(g.waypoint_radius,0) || 
        nav_controller->reached_loiter_target()) {
			//rtl erreicht home aber home x meter über desire latitude oder exakte höhe?
	}
	*/

}
	