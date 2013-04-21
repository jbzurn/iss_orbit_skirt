//ISS Orbit Skirt
//iss_orbit_skirt_rev00.ino
//Project by Leslie Birch, Sonia Petruse and J. Brooks Zurn.
//Space Apps 2013
//This project helps to show the orbit of the International Space Station (ISS) around our planet 
//using a black skirt displaying an image of the earth. Using Lilypad Arduino and stitchable LED's, 
//the flight will slowly light up around the skirt over a period of 91 min. until it reaches the
//approximate time when it is over our city (indicated by it's presence over the Earth).

//NOTE: First version. Cycles completely through all the Lilypad Arduino LEDs, holds 
//the 'current' location lit, but is holding the 'current' LED too long (needs to hold 
//it for about 5 min, and is currently only holding it for about 1 min).


//credit for original starting point for code was the blinking bike safety patch
//http://www.instructables.com/id/LilyPad-Arduino-Blinking-Bike-Safety-Patch/step5/Program-the-Board/
//our contributions: correct for 91 minute orbit, adjust number of LEDs, remove double-sided blinking,
//add a delay before turning off previous LED to add a sort of "Sparkler Trail Effect" or comet's tail.
//add options 

///////////////CHANGE THESE PARAMETERS TO MATCH YOUR CIRCUIT/////////////////////
//int location[] = { 3, 5, 6, 9, 10, 11, 1, A5}; // 
int location[] = {2,3,4,5,6,7,8,9,10,11,12,13,A0,A1,A2,A3,A4}; // array of all the LED pins on the Lilypad Arduino. 
                                                                // Enter them in the order they will be lit. The
                                                                // program will select them in sequence.
int home_location = 2;  // this is your 'home' pin location. 

//int num_locations = 8; // the number of pins (i.e. the length of the array), must enter this manually. 
int num_locations = 17;

///NOTE: this doesn't work yet, so don't change it!!
int minutes_since_last_pass = 4; // USE THIS TO SET THE CURRENT STARTING LOCATION OF THE STATION!!!
                                 // Find out what time the station last passed over your latitude,
                                 // and enter the number of minutes here. 
                                 // This is so you don't have to wait for the station to pass overhead
                                 // before resetting the Arduino, you can start it in the middle of a 
                                 // cycle. Program will convert minutes into the current location on the skirt.
                                 
int orbit_duration = 91; // Number of MINUTES the ISS takes to circle the Earth.


//////////////////////OPTIONAL DEMO/TROUBLESHOOTING ADJUSTMENT PARAMETERS ////////////////////////////////
int speed_up_orbit = 1;  // This is for demo/troubleshooting, it can be used to speed up the complete sequence.
                         // For example, if you'd like the lights to complete their 'orbit' (circle around)
                         // twice as fast, change this to 2. For 10x faster, change to 10. 

////////////////PROGRAM CODE, DO NOT CHANGE /////////////////////////////////////

  //int orbit_adjusted = orbit_duration/speed_up_orbit;  //adjust the orbit time by the current scaling factor
  //int time_orbit_adjusted = orbit_adjusted*60*1000;    //convert minutes into milliseconds: (min)*(60sec/min)*(1000ms/sec)
  
  //int time_for_this_segment = time_orbit_adjusted/num_locations; //divide up the time each segment will get. This makes
                                                            //it automatically adjust in case the total # of LED changes.
  //int segment_hold_this_LED = .9*time_for_this_segment;   //divide the segment time into 'hold' and 'chase' (circling lights)
  //int segment_chase = .1*time_for_this_segment;

  //int chases = 3;   //number of times for chase sequence to circle around
  //int time_chases = (segment_chase/2)/chases; //time each LED is turned on/off during chase

void setup()
{
  int i;
  
  
  //special case: LED is the home LED, so it should blink.
  //int segment_home_LED = segment_hold_this_LED/1000;
  
  //init chase divisions
  //int segment_chase_hold = (segment_chase/num_locations)/3; //cycle the chase around 3x
    
  for (i = 0; i < num_locations; i++){ // the array elements are numbered from 0 to num_pins - 1
  pinMode(location[i], OUTPUT); // set each pin as an output
  }

  //int timer = 100; // The higher the number, the slower the timing.
  //int pause = 100; // a pause to keep the previous LED lit, to simulate "Sparkler Trail Effect" POV. This
                 // helps the discrete LEDs to look more continuous.

}

///////program starts here /////////
void loop()
{
  ///////////////////init vals
  int i;
  int j;
  int k;
  int this_shift=0; //shift temp variable so we can start 'chase' sequence in current location
  int temp_location;
  
  int orbit_adjusted;
  orbit_adjusted = orbit_duration/speed_up_orbit;  //adjust the orbit time by the current scaling factor
  int time_orbit_adjusted;
  time_orbit_adjusted = orbit_adjusted*60*1000;    //convert minutes into milliseconds: (min)*(60sec/min)*(1000ms/sec)
  
  int time_for_this_segment;
  //time_for_this_segment = time_orbit_adjusted/num_locations; //divide up the time each segment will get. This makes
  //time_for_this_segment = 10000;
  time_for_this_segment = (orbit_duration*60*1000);
  
  //it automatically adjust in case the total # of LED changes.
  int segment_hold_this_LED;
  segment_hold_this_LED = .9*time_for_this_segment;   //divide the segment time into 'hold' and 'chase' (circling lights)
  int segment_chase;
  segment_chase = .1*time_for_this_segment;

  int chases = 3;   //number of times for chase sequence to circle around
  int time_chases = (segment_chase/2)/chases; //time each LED is turned on/off during chase

  /////////////////////
  
  for (i = 0; i < num_locations; i++) {
    // turn current location LED on!
    //if(minutes_since_last_pass>0)
    //{  temp_location = (round(100*minutes_since_last_pass/orbit_duration))*num_locations;
    //   i = temp_location;
    //   minutes_since_last_pass = 0;       
    //}
    
    digitalWrite(location[i], HIGH); 
    delay(segment_hold_this_LED);
    
    //now start chase sequence
    for (k = 0; k < 3; k++){  //repeat chase 3x
      for (j = 0; j < num_locations; j++){
        this_shift = j + i;
        if(this_shift >= num_locations) { //force this_shift to be a circular array
          this_shift = (this_shift - num_locations);
        }
        digitalWrite(location[this_shift], HIGH); 
        delay(time_chases);
 
        if (this_shift == 0){ //turn the last LED off from the previous go 'round
          digitalWrite(location[num_locations-1], LOW); //
          delay(time_chases);
        } else{
          digitalWrite(location[this_shift-1], LOW); // turn off each previous LED
          delay(time_chases);
        }
        
        //digitalWrite(pins[(i+num_pins/2)-1], LOW);
        
        //delay(time_chases); // to create a virtual "sparkler's trail effect", an apparent visual streak even though nothing has moved
        digitalWrite(location[this_shift], LOW);
      }/// end chase sequence 
    }/// end 3 repeats of chase sequence
  }

}
