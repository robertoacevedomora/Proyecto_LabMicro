/*
 IE-0624 Laboratorio de Microcontroaldores
 Proyecto 
 Monitoreo automatizado de plantas
 Sensor LPS22HB
 Sensor APDS9960
 TensorFlowLite
*/

// Arduino_TensorFlowLite - Version: 0.alpha.precompiled
#include <TensorFlowLite.h>
#include <Arduino_LPS22HB.h>

#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
//#include <tensorflow/lite/version.h>
#include <Arduino_APDS9960.h>
#include "model_banano.h"

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize];

// array to map gesture index to a name
const char* CLASSES[] = {
  "Amarillo",
  "Cafe",
  "Verde",
};

#define NUM_CLASSES (sizeof(CLASSES) / sizeof(CLASSES[0]))

void setup() {
  Serial.begin(9600);
  while (!Serial) {};

  Serial.println("Object classification using RGB color sensor");
  Serial.println("--------------------------------------------");
  Serial.println("Arduino Nano 33 BLE Sense running TensorFlow Lite Micro");
  Serial.println("");

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
  }

  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }

  // get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  static tflite::MicroInterpreter static_interpreter(
      tflModel, tflOpsResolver, tensorArena, tensorArenaSize);
  tflInterpreter = &static_interpreter; 
  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}



void loop() {
  int r, g, b, p, c;
  float sum;

  // check if both color and proximity data is available to sample
  while (!APDS.colorAvailable() || !APDS.proximityAvailable()) {}

  // read the color and proximity sensor
  APDS.readColor(r, g, b, c);
  p = APDS.readProximity();
  sum = r + g + b;

  // check if there's an object close and well illuminated enough
  if (p == 0 && c > 10 && sum > 0) {

    float redRatio = r / sum;
    float greenRatio = g / sum;
    float blueRatio = b / sum;

    // input sensor data to model
    tflInputTensor->data.f[0] = redRatio;
    tflInputTensor->data.f[1] = greenRatio;
    tflInputTensor->data.f[2] = blueRatio;

    // Run inferencing
    TfLiteStatus invokeStatus = tflInterpreter->Invoke();
    if (invokeStatus != kTfLiteOk) {
      Serial.println("Invoke failed!");
      while (1);
      return;
    }

    // Output results
    for (int i = 0; i < NUM_CLASSES; i++) {
      Serial.print(CLASSES[i]);
      Serial.print(" ");
      Serial.print(int(tflOutputTensor->data.f[i] * 100));
      Serial.print("%\n");
      Serial.print("\t");
    }

    //Lee sensor temperatura y presion atmosferica
     float pressure = BARO.readPressure();

  // print the sensor value
  Serial.print("\t");
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  float temperature = BARO.readTemperature();

  // print the sensor value
  Serial.print("\t");
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" C");
  

    // Wait for the object to be moved away
    while (!APDS.proximityAvailable() || (APDS.readProximity() == 0)) {}
  }

}