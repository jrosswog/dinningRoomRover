#include <AccelStepper.h>

#include "Rover.h"

Rover roverController = Rover();

void setup() {
  Serial.begin(9600);
  Serial.print("Staring Dinning Room Rover!!");
  roverController.begin();
  // roverController.moveForward(1000.0);
  // roverController.turn(90.0);
  roverController.addTask(FORWARD, 100.0);
  roverController.addTask(TURN, 90.0);
}

void loop() {
  //if(roverController.instructionsComplete()){
  //  Serial.print("Nothing to do\n");
  //}else{
  //  Serial.print("Still working!\n");
  //}
  roverController.execute();
}
