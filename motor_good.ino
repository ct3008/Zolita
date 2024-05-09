// Connect the port of the stepper motor driver
#include <Stepper.h>

const int stepsPerRevolution = 1024;

// int outPorts[] = {36, 27, 26, 25};
Stepper myStepper(stepsPerRevolution, 32, 26, 27, 25);
//1, 3, 2, 4

void setup(){
  myStepper.setSpeed(30);
  Serial.begin(9600);
}

void loop(){
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);

  Serial.println("counter");
  myStepper.step(-stepsPerRevolution);
}