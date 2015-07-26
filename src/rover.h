#ifndef Rover_h
#define Rover_h

#include "Arduino.h"
class Rover{
  public:
    Rover();

    void begin();
    
    // Instructs the rover to move forward
    void moveForward(float distanceInCm);

    // Instructs the rover to turn
    // degrees is [-360,360] positive turn right, negative turns left
    void turn(float degrees);

    // Returns true if the rover is done completing the last command
    // false otherwise
    bool instructionsComplete();

    // executes one part of command. This method should be called
    // as often as possible
    void execute();

    // sets the step per revolution
    void setStepsPerRev(int step);

    // set the max speed
    void setMaxSpeed();

    // set the pins for the right motor
    void setRightMotorPins(int[] pins);

    // set the pins for the left motor
    void setLeftMotorPins(int[] pins);
    
  private:
    int stepsPerRev = 64 * 64;
    float maxSpeed = 600.0;
    float acceleration = 100.0;
    int motorRightPins[] = {7,6,5,4};
    int motorLeftPins[] = {11,10,9,8};
    float wheelCircumInCm = 18.0;
    float wheelBaseInCm = 11.43;
    AccelStepper rightMotor(AccelStepper::HALF4WIRE,rightMotorPins[0], rightMotorPins[2], rightMotorPins[1], rightMotorPins[3]);
    AccelStepper leftMotor(AccelStepper::HALF4WIRE,leftMotorPins[0], leftMotorPins[2], leftMotorPins[1], leftMotorPins[3]);
};

#endif
