#ifndef Rover_h
#define Rover_h

#include "Arduino.h"
#include <AccelStepper.h>

typedef enum {FORWARD, TURN, NOOP} command_enum;
typedef struct {
  command_enum command;
  float distance;
} instruction;

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
    void setStepsPerRev(int steps);

    // set the max speed
    void setMaxSpeed(float speed);

    // set the pins for the right motor
    void setRightMotorPins(int pins[4]);

    // set the pins for the left motor
    void setLeftMotorPins(int pins[4]);

    // adds an instruction to the the queue of tasks
    // return 0 for success and non zero if the task was not add
    int addInstruction(instruction task);

    // adds a task to the the queue of tasks
    // return 0 for success and non zero if the task was not add
    int addTask(command_enum command, float distance);
    
  private:
    const float pi = 3.14159;
    long _stepsPerRev = 64 * 64;
    float _maxSpeed = 600.0;
    // float _maxSpeed = 100.0;
    float _acceleration = 100.0;
    int _rightMotorPins[4] = {7,6,5,4};
    int _leftMotorPins[4] = {11,10,9,8};
    float _wheelBaseInCm = 11.0;
    float _wheelCircumInCm = 18.0;
    float _maxCmToMove = (pow(2.0,30.0)/(float)_stepsPerRev) * _wheelCircumInCm;

    //task queue - Queue of actions for the rover
    static const int _taskQueueSize = 50;
    instruction _taskQueue[_taskQueueSize];
    int _taskQueueHeadIndex = 0;
    int _taskQueueTailIndex = 0;
    
    // The 28BYJ-48 needs to be operated in half-step mode, ie an 8 pulse sequence - the standard Arduino Stepper library can't do this.
    // The 28BYJ-48 coils need to be energised in the pin sequence 1,3,2,4, rather than the standard 1,2,3,4 sequence
    // http://www.element14.com/community/people/neilk/blog/2015/01/24/arduino-controlling-stepper-motor-28byj-48-with-accelstepper-library
    AccelStepper _rightMotor = AccelStepper(AccelStepper::HALF4WIRE,_rightMotorPins[0], _rightMotorPins[2], _rightMotorPins[1], _rightMotorPins[3]);
    AccelStepper _leftMotor = AccelStepper(AccelStepper::HALF4WIRE,_leftMotorPins[0], _leftMotorPins[2], _leftMotorPins[1], _leftMotorPins[3]);
    long cmToSteps(float distanceInCm);
    void moveMotorCm(AccelStepper& motor, float distanceCm);
    bool isTaskQueueEmpty();
    instruction getNextTask();
    //Computes the next index in the circular buffer
    int getNextTaskIndex(int currentIndex);
    // return true if the task the rover is working on is complete. False otherwise
    bool currentTaskComplete();
    // execute the given instruction
    void executeTask(instruction task);
};

#endif
