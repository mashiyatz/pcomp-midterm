#include "Stepper.h"
#include "Servo.h"

const int stepsPerRevolution = 512; // change depending on stepper
const int speed = 40; 
const int switchPin = 2;
const int trigPin = 7;
const int echoPin = 8;
const int servoVerticalPin = 9;
const int servoHorizontalPin = 10;

int drawerState = 0; // 1 is open
int rotationAxis = -1; // 
long duration;
long lastMoveTime = 0;
int distance_cm;

// initialize the stepper library on pins 3-6:
Stepper myStepper(stepsPerRevolution,3,4,5,6); 
Servo servoMotorV;
Servo servoMotorH;

void setup() {
  // initialize the serial port:
  Serial.begin(9600);
  myStepper.setSpeed(speed);
  pinMode(switchPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servoMotorV.attach(servoVerticalPin);
  servoMotorH.attach(servoHorizontalPin);

  digitalWrite(switchPin, LOW);

}
 
void loop() {
  // trigger pin low-high-low to send a pulse 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // read pulse
  duration = pulseIn(echoPin, HIGH);
  distance_cm = (duration * 0.0343) / 2;

  if (drawerState == 0) {
    if (digitalRead(switchPin) == HIGH) {
      drawerState = 1;
      myStepper.step(stepsPerRevolution);
    }
  } else if (drawerState == 1) {
    if (distance_cm > 30) {
      drawerState = 0;
      myStepper.step(-stepsPerRevolution);
    }
  }

  // Serial.print(distance_cm);
  // Serial.print(" cm ");
  // Serial.print("State: ");
  // Serial.println(drawerState);

  // create coinFlip to help manage motions 
  if (millis() - lastMoveTime > 20) {
    if (rotationAxis == -1) {
    servoMotorH.write(int(random(0, 90))); 
    } else if (rotationAxis == 1) {
    servoMotorV.write(int(random(120, 160)));
    }
    rotationAxis *= -1;
  }
  

  // delay(250); // delay makes it hard to time interaction

//  separate IR sensor for skull?
//  phototransistor when drawer opens?

}
