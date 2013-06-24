#define BUFFSIZ 120
char ReadSerial[BUFFSIZ];
char *parseptr;
char buffer_start[25];
byte counter_sms=0;
int setupIBRU = 0;
int serialNrIBRU = 0;
byte unlock_setup=0;
byte counter_setup=0;
int gut = 0;
char *search = ",";
char *brkb, *pEnd;




void read_serial(){
  while(Serial.available() > 0){

    if(unlock_setup==0){
      ReadSerial[0]=Serial.read();
      //Serial.print(buffer_setup[0]);
      if(ReadSerial[0]=='+'){
        unlock_setup=1; 
      }
    }
    else{
      ReadSerial[counter_setup]=Serial.read();
      //Serial.print(buffer_setup[counter_setup]);

      if(ReadSerial[counter_setup]=='*'){
        unlock_setup=0;
		        

        if (strncmp(ReadSerial, "wpco",4) == 0){
          parseptr = ReadSerial+5;
          parseptr = strtok_r(ReadSerial, search, &brkb);
          parseptr = strtok_r(NULL, search, &brkb);
		  osdOn = 1;
          osd_waypointDirection = atoi(parseptr);
        }

        if (strncmp(ReadSerial, "wpnu",4) == 0){
          parseptr = ReadSerial+5;
          parseptr = strtok_r(ReadSerial, search, &brkb);
          parseptr = strtok_r(NULL, search, &brkb);
		  osdOn = 1;
          wp_number = atoi(parseptr);
        }


        if (strncmp(ReadSerial, "head",4) == 0){
          parseptr = ReadSerial+5;
          parseptr = strtok_r(ReadSerial, search, &brkb);
          parseptr = strtok_r(NULL, search, &brkb);
		  osdOn = 1;
          osd_heading = atoi(parseptr);
        }

        if (strncmp(ReadSerial, "wpdi",4) == 0){
          parseptr = ReadSerial+5;
          wp_dist = atoi(parseptr);
		  osdOn = 1;
          parseptr = strchr(parseptr, ',') + 1;
          wp_einheit = atoi(parseptr);
          wp_no = 0;
        }


        if (strncmp(ReadSerial, "alt",3) == 0){
          parseptr = ReadSerial+4;
          parseptr = strtok_r(ReadSerial, search, &brkb);
          parseptr = strtok_r(NULL, search, &brkb);
		  osdOn = 1;
          osd_alt = atoi(parseptr);
        }

        if (strncmp(ReadSerial, "mode",4) == 0){
          parseptr = ReadSerial+5;
          parseptr = strtok_r(ReadSerial, search, &brkb);
          parseptr = strtok_r(NULL, search, &brkb);
		  osdOn = 1;
          osd_mode = atoi(parseptr);
        }


        if (strncmp(ReadSerial, "sfix",4) == 0){
          parseptr = ReadSerial+5;
          parseptr = strtok_r(ReadSerial, search, &brkb);
          parseptr = strtok_r(NULL, search, &brkb);
		  osdOn = 1;
          osd_fix_type = atoi(parseptr);
        }

        if (strncmp(ReadSerial, "debu",4) == 0){
          parseptr = ReadSerial+5;
          parseptr = strtok_r(ReadSerial, search, &brkb);
          parseptr = strtok_r(NULL, search, &brkb);
		  osdOn = 1;
          Serial.println(atoi(parseptr));
        }


        if (strncmp(ReadSerial, "nowp",4) == 0){
			osdOn = 1;
			wp_no = 1;
        }


		if (strncmp(ReadSerial, "clear",5) == 0){
          osd.clear();
		  osdOn = 0;
        }
		
		updatePanel();
		

        for(int a=0; a<=counter_setup; a++){
          ReadSerial[a]=0;
        } 
        counter_setup=0;
      }
      else{
        counter_setup++;
      }
    }
  }


}
























