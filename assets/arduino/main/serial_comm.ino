
//Now the command list must be defined https://github.com/CreativeRobotics/Commander/blob/master/examples/BasicCommands/masterCommands.ino
const commandList_t commands[] = {
  {"setLightRGB", setLightRGB,   "Set color of the LED"},
  {"pumpRunFor",   pumpRunFor,   "Start pump and run for X minutes"},
  {"pumpStart",   pumpStart,   "Start pump"},
  {"pumpStop",    pumpStop,   "Stop pump"},
  {"setBrightness",         setBrightness,   "Set brightness"},
  {"stopAll",         stopAll,   "Stop pump, lights and sensor readings"},
  {"LEDShow",         LEDShow,   "Start a ledshow of n seconds"},
  {"sysInfo",         sysInfo,   "Prints some information on the system"},

};

// pumpStart
// pumpStop
// pumpRunFor 20
// setBrightness 15
// setLightRGB 23 10 115
// stopAll
// LEDShow 5 0.2
// sysInfo

// sendInfo

//Initialisation function
void initialiseCommander() {
  cmd.begin(&Serial, commands, sizeof(commands)); //start Commander on Serial
}


void LEDShow(Commander &Cmdr) {
  int val1 = 0;
  int val2 = 0;
  int val3 = 0;
  float freq = 0;
  float dur = 0;
  float values[3] = {0, 0};
  for (int n = 0; n < 2; n++) {
    //try and unpack an int, if it fails there are no more left so exit the loop
    if (Cmdr.getFloat(values[n])) {

    } else break;
  }
  dur = 1000 * values[0];
  freq = 1000 * values[1];

  sendInfo(String("Lightshow started. Duration " + String(dur/1000) + " freq " + String((freq/1000))));
  for (int i = 0; i < dur / freq; i++) {
    val1 = random();
    val2 = random();
    val3 = random();

    leds[0] = CRGB(val2, val3, val1);
    FastLED.show();
    delay(freq);
  }
  leds[0] = CRGB::Black;
  FastLED.show();
  sendInfo(String("Lightshow stopped"));

}

void sendInfo(String info)
{ // for help on dtostrf http://forum.arduino.cc/index.php?topic=85523.0
  //{"type": "I", "pumpON":1,"RGB": "20 255 30","brightness":80}
  String line = "";
  line = String("{\"type\":") + String("\"I\",") + "\"message\":" + "\"" + info + "\"" + "}";
  Serial.println(line);
  delay(100);
  //return 0;
}

//this will be an emergency stop
void stopAll(Commander &Cmdr)
{

  removeOverride();
  sendInfo(String("Override manually removed"));

  return 0;
}

bool pumpStop(Commander &Cmdr) {
  if (!pumpOverride)
  { // Here code to swith the pump off
    pumpON = false;
    digitalWrite(PUMP_PIN,HIGH);

    sendInfo(String("Pump stopped"));
  }
  else
  {
    sendInfo(String("Punp cannot stop, pump stop override active"));
  }
  return 0;
}

bool pumpStart(Commander &Cmdr) {
  // Here code to swith the pump on
  digitalWrite(PUMP_PIN,LOW);
  pumpON = true;
  sendInfo(String("Pump start"));
  //setRGBLED(0,0,255);
  return 0;
}

bool removeOverride() {
  pumpOverride = false;

}


void setRGBLED(R,G,B){
  RGBLED[0] = R;
  RGBLED[1] = G;
  RGBLED[2] = B;
  leds[0] = CRGB(RGBLED[0], RGBLED[1], RGBLED[2]);
  FastLED.show();
  sendInfo(String("LED set to RGB: ") + RGBLED[0] + " " + RGBLED[1] + " " + RGBLED[2]); 
}

bool pumpRunFor(Commander &Cmdr) {
  long myInt;
  pumpOverride = true;
  if (Cmdr.getInt(myInt)) {
    pumpON = true;
    digitalWrite(PUMP_PIN,LOW);
    timer.in(myInt * 1000 , pumpStop);
    timer.in(myInt * 1000 - 1, removeOverride);
    sendInfo(String("Pump started for: ") + myInt + String(" seconds"));
    //setRGBLED(0,0,255);
 
  }
  else
  { sendInfo(String("Command pumpRunFor failed: no duration supplied"));
  }

  return 0;
}


bool setLightsONTime(Commander &Cmdr) {
  String myString = "";
  int itms = Cmdr.countItems();


  if (Cmdr.getString(myString)) {

    sendInfo(String("LED start set fron: ") + myString);

  } else Cmdr.println("Operation failed");

  //Cmdr.chain();
  //Cmdr.printDiagnostics();
  return 0;
}

bool setBrightness(Commander &Cmdr) {
  // Here code to swith the pump on
  long myInt;
  //The server has brigtness range 0-255, the hardware is library dependent. We do not scale but keep the same ranger in the GUI Sx:100=Hx:MAX_BRIGHTNESS
  if (Cmdr.getInt(myInt)) {
    Brightness = myInt;
    FastLED.setBrightness(Brightness);
    FastLED.show();
    sendInfo(String("Brightness set to: ") + myInt + String(" (") + 100*myInt/255 + String("%)"));
  }
  return 0;
}

bool sysInfo(Commander &Cmdr){
  String line = "";
  String commands ="pumpStart, pumpStop, pumpRunFor 2, setBrightness 15, setLightRGB 23 10 115, stopAll, LEDShow 5 0.2 sysInfo";
  line = String("{\"version\":") + VERSION + ",\"baud rate\":" + "\"" + BAUDRATE + "\"" ",\"commands\":" + "\"" + commands + "\"}";
  
  Serial.println(line);
  delay(100);


}

//setLightRGB 255 255 255
bool setLightRGB(Commander &Cmdr) {
  //create an array to store any values we find
  int values[4] = {0, 0, 0};
  for (int n = 0; n < 3; n++) {
    //try and unpack an int, if it fails there are no more left so exit the loop
    if (Cmdr.getInt(values[n])) {

    } else break;
  }
  //print it out
  String pRGB = "";
  for (int n = 0; n < 3; n++) {
    pRGB = pRGB + String(values[n]) + String(" ");
    RGBLED[n] = values[n];
  }

  leds[0] = CRGB(RGBLED[0], RGBLED[1], RGBLED[2]);
  FastLED.show();
  sendInfo(String("LED set to RGB: ") + pRGB);

  return 0;
}



void TelemetryTX()
{ // for help on dtostrf http://forum.arduino.cc/index.php?topic=85523.0

  String line = "";
  String telemMarker = "T";
  //Need to calculate parameters here because the main loop has a different frequency
  //TxLoopTime = millis() - TxLoopTime;


  line = telemMarker + SEPARATOR +
         String(pumpON) +  SEPARATOR +
         String(RGBLED[0]) + " " + String(RGBLED[1]) + " " + String(RGBLED[2]) +  SEPARATOR +
         String(Brightness) +  SEPARATOR +
         String((float(random(1000, 9999)) / 100)) +  SEPARATOR +

         String(info) +  SEPARATOR;
  Serial.println(line);


  /*line = "T" + SEPARATOR
         + yaw + SEPARATOR
         + pitch + SEPARATOR
         + roll + SEPARATOR
         + heading + SEPARATOR
         + Info
    //+ SEPARATOR
    //+ LastEvent;*/
  //Serial.println(line);

}

void TelemetryTXJSON() //statusReport
{ // for help on dtostrf http://forum.arduino.cc/index.php?topic=85523.0
  //{"type": "I", "pumpON":1,"RGB": "20 255 30","brightness":80}
  delay(100);
  String line = "";
  line = String("{\"type\":") + String("\"T\",") + "\"pumpON\":" + pumpON + ",\"RGB\":" + "\"" + String(RGBLED[0]) + " " + String(RGBLED[1]) + " " + String(RGBLED[2]) + "\"" + ",\"brightness\":" + Brightness + "}";
  Serial.println(line);
  //return 0;
}


//Initialisation function
void initTimer() {
  timer.every(1  * 1000, TelemetryTXJSON);

}
