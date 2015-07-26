#include "Ardunio.h"
#include "Rover.h"
#include <AccelStepper.h>

Rover::Rover(){
  
}

Rover::begin(){
  rightMotor = AccelStepper(AccelStepper::HALF4WIRE,rightMotorPins[0], rightMotorPins[2], rightMotorPins[1], rightMotorPins[3]);
  leftMotor = AccelStepper(AccelStepper::HALF4WIRE,leftMotorPins[0], leftMotorPins[2], leftMotorPins[1], leftMotorPins[3]); 
  rightMotor.setMaxSpeed(maxSpeed);
  leftMoter.setMaxSpeed(maxSpeed);
}

// Instructs the rover to move forward
void Rover:moveForward(float distanceInCm){
  int steps = (int)((distanceInCm / wheelCircumInCm) * stepsPerRev);
  rightMotor.setCurrentPosition(0);
  rightMotor.moveTo(steps);
  rightMotor.setCurrentPosition(0);
  rightMotor.moveTo(steps);
}



