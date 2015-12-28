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
long Rover::cmToSteps(float distanceInCm){
  Serial.print("Converting CM to Steps CM: ");
  Serial.print(distanceInCm);
  Serial.print(" steps: ");
  
  long steps = (long)((distanceInCm / _wheelCircumInCm) * _stepsPerRev);
  Serial.print(steps);
  Serial.print("\n");
  return steps;
}

// Instructs the motor to move a given distance
void Rover::moveMotorCm(AccelStepper& motor, float distanceCm){
  long steps = cmToSteps(distanceCm);
  motor.setCurrentPosition(0);
  Serial.print("Moving motor steps: ");
  Serial.print(steps);
  Serial.print("\n");
  motor.move(steps);
}

// Instructs the rover to move forward
void Rover::moveForward(float distanceInCm){
  moveMotorCm(_rightMotor, distanceInCm);
  moveMotorCm(_leftMotor, -1 * distanceInCm);
}

void Rover::turn(float degrees){
  float fracOfCircle = abs(degrees/360.0);
  float turnRadiusInCm = _wheelBaseInCm;
  float turnCircumInCm = 2.0 * pi * turnRadiusInCm;
  float turnDistanceInCm = fracOfCircle * turnCircumInCm;
  //Turn right (move left motor) if angle is positive, else turn left
  
  if(degrees > 0.0){
    moveMotorCm(_leftMotor, -1 * turnDistanceInCm);
    // moveMotorCm(_rightMotor, -1 * turnDistanceInCm);
  }else{
    moveMotorCm(_rightMotor, turnDistanceInCm);
    // moveMotorCm(_leftMotor, turnDistanceInCm);
  }
}

bool Rover::currentTaskComplete(){ 
  bool complete = false; 
  if(_rightMotor.distanceToGo()==0 && _leftMotor.distanceToGo()==0){
    complete = true;
  }
  return complete;
}

bool Rover::instructionsComplete(){
  bool complete = false;
  if(isTaskQueueEmpty() && currentTaskComplete()){
      complete = true;
  }
  return complete;
}

void Rover::executeTask(instruction task){
    switch(task.command){
      case FORWARD :
        Serial.print("Executing FORWARD command ");
        Serial.print(task.distance);
        Serial.print("\n");
        moveForward(task.distance);
        break;
      case TURN :
        Serial.print("Executing TURN command ");
        Serial.print(task.distance);
        Serial.print("\n"); 
        turn(task.distance);
        break;
    }
}

void Rover::execute(){
  if(currentTaskComplete() && !isTaskQueueEmpty()){
    executeTask(getNextTask());
  }
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

int Rover::getNextTaskIndex(int currentIndex){
  return (currentIndex + 1) % _taskQueueSize;
}

int Rover::addInstruction(instruction task){
  float distInCm = 0.0;
  int newHeadIndex = getNextTaskIndex(_taskQueueHeadIndex);
  if (newHeadIndex == _taskQueueTailIndex){
    // Queue is full!!!
    Serial.print("Task Queue Full!\n");
    return -1;
  }
  _taskQueueHeadIndex = newHeadIndex;
 
  switch(task.command){
      case FORWARD :
        distInCm = task.distance;
        if(distInCm > _maxCmToMove){
          Serial.print("Adding Forward task at idex ");
          Serial.print(_taskQueueHeadIndex);
          Serial.print("\n");
          Serial.print(_maxCmToMove);
          Serial.print("\n");
          _taskQueue[_taskQueueHeadIndex] = {.command = task.command, .distance = _maxCmToMove};
          addInstruction({.command = task.command, .distance = distInCm - _maxCmToMove});
        }else if(distInCm > 0.0){
          Serial.print("Adding Forward task at idex ");
          Serial.print(_taskQueueHeadIndex);
          Serial.print("\n");
          Serial.print(distInCm);
          Serial.print("\n");
          _taskQueue[_taskQueueHeadIndex] = task;
        }
        break;
      case TURN :
      Serial.print("Adding TURN task at idex ");
          Serial.print(_taskQueueHeadIndex);
          Serial.print("\n");
          Serial.print(task.distance);
          Serial.print("\n");
        _taskQueue[_taskQueueHeadIndex] = task;
        break;
  }
  return 0;
}

int Rover::addTask(command_enum command, float distance){
  return addInstruction({.command = command, .distance = distance});
}

bool Rover::isTaskQueueEmpty(){
  return (_taskQueueHeadIndex == _taskQueueTailIndex);
}

instruction Rover::getNextTask(){
  instruction task;
  if(isTaskQueueEmpty()){
    task.command = NOOP;
    task.distance = 0.0;
  }else{
    _taskQueueTailIndex = getNextTaskIndex(_taskQueueTailIndex);
    task = _taskQueue[_taskQueueTailIndex];
  }
  return task;
}



