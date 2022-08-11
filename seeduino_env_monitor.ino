//#include <Arduino.h>
#include "sgp30.h"

// #define LOOP_TIME_INTERVAL_MS  2000
#define LOOP_TIME_INTERVAL_MS  5000
// #define LOOP_TIME_INTERVAL_MS  600000
//#define LOOP_TIME_INTERVAL_MS  900000

void setup() {
  Serial.begin(115200);
  Serial.println(F("Env monitor startup"));
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  //temp_sensor_main();
  //delay(500);
  gas_sensor_setup();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  gas_sensor_main();
  delay(500);
  // F macro moves strings to PROGMEM instead of RAM
  Serial.print(F("temperature = "));
  Serial.println(temp_sensor_main());
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(LOOP_TIME_INTERVAL_MS);
  
}
