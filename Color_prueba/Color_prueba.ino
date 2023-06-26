/*
  APDS9960 - Color Sensor

  This example reads Color data from the on-board APDS9960 sensor of the
  Nano 33 BLE Sense and prints the color RGB (red, green, blue) values
  to the Serial Monitor once a second.

  The circuit:
  - Arduino Nano 33 BLE Sense

  This example code is in the public domain.
*/

#include <Arduino_APDS9960.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
  }
}

void loop() {

  // check if a color reading is available
  while (! APDS.colorAvailable()) {
    delay(5);
  }
  int r, g, b;

  // read the color
  APDS.readColor(r, g, b);

  if (r > g & r > b)
  {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
  }
  else if (g > r & g > b)
  {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDB, HIGH);
  }
  else if (b > g & b > r)
  {
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
  }
  else
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
  }


  // print the values
  Serial.print("Red light = ");
  Serial.println(r);
  Serial.print("Green light = ");
  Serial.println(g);
  Serial.print("Blue light = ");
  Serial.println(b);
  Serial.println();

  // wait a bit before reading again
  delay(500);
}