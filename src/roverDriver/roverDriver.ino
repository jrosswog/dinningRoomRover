#include <AccelStepper.h>

#include "Rover.h"

Rover roverController = Rover();

void setup() {
  Serial.begin(9600);
  Serial.print("Staring Dinning Room Rover!!");
  Serial.print("\n");
  roverController.begin();
  
  // roverController.addTask(FORWARD, 240.0);
  // roverController.addTask(TURN, 90.0);
  roverController.addTask(FORWARD,223.52);
  roverController.addTask(TURN,90.0);
  roverController.addTask(FORWARD,217.1);
  roverController.addTask(TURN,90.0);
  roverController.addTask(FORWARD,223.52);
  roverController.addTask(TURN,90.0);
  roverController.addTask(FORWARD,217.1);
  roverController.addTask(TURN,90.0);
  
}

void loop() {
  //if(roverController.instructionsComplete()){
  //  Serial.print("Nothing to do\n");
  //}else{
  //  Serial.print("Still working!\n");
  //}
  roverController.execute();
}
