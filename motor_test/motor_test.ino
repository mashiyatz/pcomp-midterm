#include "Stepper.h"

const int stepsPerRevolution = 200; // change depending on stepper
const int stepperMotorSpeed = 50; 

const int switchPin = 2;
Stepper drawerMotor(stepsPerRevolution, 3, 4, 5, 6); 

void setup() {
  // put your setup code here, to run once:
  drawerMotor.setSpeed(stepperMotorSpeed);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(switchPin) == HIGH) {
    drawerMotor.step(1);
  } else {
    drawerMotor.step(-1);
  }
}
