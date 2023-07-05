/*
 IE-0624 Laboratorio de Microcontroaldores
 Proyecto 
 Monitoreo automatizado de plantas
 Script para obtener los datos r, g y b del sensor APDS9960
*/

#include <Arduino_APDS9960.h>

void setup() {

  Serial.begin(9600);
  while (!Serial) {};

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
  }

  // print the header
  Serial.println("Red,Green,Blue");
}

void loop() {
  int r, g, b, c, p;
  float sum;

  // wait for proximity and color sensor data
  while (!APDS.colorAvailable() || !APDS.proximityAvailable()) {}

  // read the color and proximity data
  APDS.readColor(r, g, b, c);
  sum = r + g + b;
  p = APDS.readProximity();

  // if object is close and well enough illumated
  if (p == 0 && c > 10 && sum > 0) {

    float redRatio = r / sum;
    float greenRatio = g / sum;
    float blueRatio = b / sum;

    // print the data in CSV format
    Serial.print(redRatio, 3);
    Serial.print(',');
    Serial.print(greenRatio, 3);
    Serial.print(',');
    Serial.print(blueRatio, 3);
    Serial.println();
  }
}