#include "Arduino.h"
#include "Rover.h"
#include <AccelStepper.h>

Rover::Rover(){
  
}

void Rover::begin(){
  _rightMotor = AccelStepper(AccelStepper::HALF4WIRE, _rightMotorPins[0], _rightMotorPins[2], _rightMotorPins[1], _rightMotorPins[3]);
  _leftMotor = AccelStepper(AccelStepper::HALF4WIRE, _leftMotorPins[0], _leftMotorPins[2], _leftMotorPins[1], _leftMotorPins[3]); 
  _rightMotor.setMaxSpeed(_maxSpeed);
  _rightMotor.setAcceleration(_acceleration);
  _rightMotor.setSpeed(_maxSpeed);
  _leftMotor.setMaxSpeed(_maxSpeed);
  _leftMotor.setAcceleration(_acceleration);
  _leftMotor.setSpeed(_maxSpeed);
}

// Converts CM to steps
int Rover::cmToSteps(float distanceInCm){
  int steps = (int)((distanceInCm / _wheelCircumInCm) * _stepsPerRev);
  return steps;
}

// Instructs the motor to move a given distance
void Rover::moveMotorCm(AccelStepper& motor, float distanceCm){
  int steps = cmToSteps(distanceCm);
  motor.setCurrentPosition(0);
  motor.move(steps);
}

// Instructs the rover to move forward
void Rover::moveForward(float distanceInCm){
  moveMotorCm(_rightMotor, distanceInCm);
  moveMotorCm(_leftMotor, -1*distanceInCm);
}

void Rover::turn(float degrees){
  float fracOfCircle = abs(degrees/360.0);
  float turnRadiusInCm = _wheelBaseInCm / 2.0;
  float turnCircumInCm = 2.0 * pi * turnRadiusInCm;
  float turnDistanceInCm = fracOfCircle * turnCircumInCm;
  //Turn right (move left motor) if angle is positive, else turn left
  if(degrees > 0.0){
    moveMotorCm(_leftMotor, -1 * turnDistanceInCm);
  }else{
    moveMotorCm(_rightMotor, turnDistanceInCm);
  }
}

bool Rover::instructionsComplete(){ 
  bool complete = false; 
  if(_rightMotor.distanceToGo()==0 || _leftMotor.distanceToGo()==0){
    complete = true;
  }
  return complete;
}

void Rover::execute(){
  _rightMotor.run();
  _leftMotor.run();
}

void Rover::setStepsPerRev(int steps){
  _stepsPerRev = steps;
}

void Rover::setMaxSpeed(float speed){
  _maxSpeed = speed;
}

void Rover::setRightMotorPins(int pins[4]){
  memcpy(&_rightMotorPins, &pins, sizeof _rightMotorPins);
}

void Rover::setLeftMotorPins(int pins[4]){
  memcpy(&_leftMotorPins, &pins, sizeof _leftMotorPins);
}



