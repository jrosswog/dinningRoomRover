#include <Stepper.h>
#include <AccelStepper.h>
int motor1Pins[] = {7,6,5,4};
int motor2Pins[] = {11,10,9,8};
int ledPin = 3;
int delayFreq = 1000;
int distance = 10000;
int turnFactor = 2;
// number of steps per revolution of the stepper motor
int stepsPerRev = 64 * 64;
bool rightTurn = false;
float maxSpeed = 600.0;
float acceleration = 100.0;

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
// Stepper stepper(stepsPerRev, motorPins[0], motorPins[1], motorPins[2], motorPins[3]);

// The 28BYJ-48 needs to be operated in half-step mode, ie an 8 pulse sequence - the standard Arduino Stepper library can't do this.
// The 28BYJ-48 coils need to be energised in the pin sequence 1,3,2,4, rather than the standard 1,2,3,4 sequence
// http://www.element14.com/community/people/neilk/blog/2015/01/24/arduino-controlling-stepper-motor-28byj-48-with-accelstepper-library
AccelStepper motor1(AccelStepper::HALF4WIRE,motor1Pins[0], motor1Pins[2], motor1Pins[1], motor1Pins[3]);
AccelStepper motor2(AccelStepper::HALF4WIRE,motor2Pins[0], motor2Pins[2], motor2Pins[1], motor2Pins[3]); 

void updateMotors(){
  AccelStepper* m1 = &motor1;
  AccelStepper* m2 = &motor2;
  if(rightTurn){
    m1 = &motor2;
    m2 = &motor1;
  }
  m1->setMaxSpeed(maxSpeed);
  m1->setAcceleration(acceleration);
  motor1.stop();
  motor1.setCurrentPosition(0);
  motor1.moveTo(distance);
  m2->setMaxSpeed(maxSpeed / turnFactor);
  m2->setAcceleration(acceleration / turnFactor);
  motor2.stop();
  motor2.setCurrentPosition(0);
  motor2.moveTo(-1*distance);
}

void setup() {
  // led control
  pinMode(ledPin, OUTPUT);
  // motor control
  // for(int i=0; i<4; i++){
  //   pinMode(motorPins[i], OUTPUT);
  // }
  
  updateMotors();

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(motor1.distanceToGo()==0 || motor2.distanceToGo()==0){    
    if(rightTurn){
      rightTurn = false;
      digitalWrite(ledPin, LOW);
    }else{
      rightTurn = true;
      digitalWrite(ledPin, HIGH);
    }
    updateMotors();
  }
  motor1.run();
  motor2.run();
  
  // delay(delayFreq);
}
