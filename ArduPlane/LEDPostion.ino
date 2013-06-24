
/*

#define ledChannel CH_6

void setupLED(){
	ledPosition = 0;
	servo_write(ledChannel, 1000);
}


void checkLED(){

	if (ledPosition || hal.rcin->read(ledChannel) > 1600)
		servo_write(ledChannel, 1580);

}




void setLED(){
	if (g.fpv){		
		switch (g_gps->status()){
		case (2):
			digitalWrite(44, HIGH);
			break;
		default:
			if (superslow_loopCounter3 < 2){
				digitalWrite(44, LOW);
			}else{
				superslow_loopCounter3 = 0;
				digitalWrite(44, HIGH);
			}
			break;
		}
	}
}

*/