/*
  IMU Classifier

  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU, once enough samples are read, it then uses a
  TensorFlow Lite (Micro) model to try to classify the movement as a known gesture.

  Note: The direct use of C/C++ pointers, namespaces, and dynamic memory is generally
        discouraged in Arduino examples, and in the future the TensorFlowLite library
        might change to make the sketch simpler.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.

  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
//#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
//#include "tensorflow/lite/version.h"

#include "model3.h" //this is the NN model
#include "hand.h" //this is the image


// global variables used for TensorFlow Lite (Micro)
// tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

tflite::MicroErrorReporter micro_error_reporter;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 200 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

// array to map gesture index to a name
const char* GESTURES[] = {
  "rock",
  "paper",
  "scissor"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  //Serial.begin(9600);
  //while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  // get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    //Serial.println("Model schema mismatch!");
    while (1);
  }

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, error_reporter);

  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);

  std::memcpy(tflInputTensor, image_data, sizeof(image_data));
  //std::copy(image_data.begin(), image_data.end(), tflInputTensor);
  //TfLiteTensorCopyFromBuffer(tflInputTensor, image_data, 64*16);
}

void loop() {

  //TODO: LOAD THE DATA

  //while (samplesRead < numSamples) {
    
  //TODO: NORMALIZE DATA

      //samplesRead++;

      //if (samplesRead == numSamples) {
        // Run inferencing
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        TfLiteStatus invokeStatus = tflInterpreter->Invoke();
        if (invokeStatus != kTfLiteOk) {
          digitalWrite(LED_BUILTIN, HIGH);
          //Serial.println("Invoke failed!");
          while (1);
          return;
        }
        digitalWrite(LED_BUILTIN, LOW);

        // Loop through the output tensor values from the model
        for (int i = 0; i < NUM_GESTURES; i++) {
          //Serial.print(GESTURES[i]);
          //Serial.print(": ");
          //Serial.println(tflOutputTensor->data.f[i], 6);
          digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
          delay(1000);                      // wait for a second
          digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
          delay(1000);
        }
        digitalWrite(LED_BUILTIN, HIGH);
        //Serial.println();
      //}
    }
//  }
//}
