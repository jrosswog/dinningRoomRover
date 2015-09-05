#include <AccelStepper.h>

#include "Rover.h"

Rover roverController = Rover();

void setup() {
  roverController.begin();

  Serial.begin(9600);
}

void loop() {
  roverController.execute();
}
