#include "Stepper.h"
#include "Servo.h"

/* Constants */

const int stepsPerRevolution = 200; // change depending on stepper
const int stepperMotorSpeed = 100; 
const int stepsToRotate = 700; 
const int maxDistance = 80; // set threshold of distance sensor
const long waitTime = 3000; // time to wait before closing the drawer

/* Pins */
const int switchPin = 2;

Stepper drawerMotor(stepsPerRevolution, 3, 4, 5, 6); // initialize the stepper library on pins 3-6:

// ultrasonic sensor pins
const int trigPin = 7; 
const int echoPin = 8;

// skull servo and LED pins
const int servoVPin = 9; // operates the up-down rotation of the skull
const int servoHPin = 10; // operates the left-right rotation of the skull
const int ledPin = 11; // skull eyes

/* Variables */ 

bool isDrawerOpen = 0; // 1 means the drawer is open
bool isLightOn = 0; 

long lastMoveTime = 0;
long lastBlinkTime = 0;
long lastOpenTime = 0;

int distance_cm; // distance detected by the sensor
int animationSequence = 0; // movement sequence to perform in playSkullAnimation()
int animationStep = 0; // step in movement sequence in playSkullAnimation()

//// unused, delete later
bool isMeasuringDistance = 0;
int skullMotionInterval = 1000;
int blinkInterval = 500;
long sensorDuration; 
int threshold;
int last_distance;
////

Servo servoMotorV;
Servo servoMotorH;

void setup() {
  Serial.begin(9600);
  drawerMotor.setSpeed(stepperMotorSpeed);

  pinMode(switchPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  servoMotorV.attach(servoVPin);
  servoMotorH.attach(servoHPin);
}

void loop() {
  /*
  The loop has four steps: 
  1. Calculate the distance from the sensor. 
  2. Set the speed of skull motors and LED transitions based on the distance.
  3. Write to the skull motors and LEDs.
  4. Operate the drawer based on button press.
  */

  getDistance();
  // setAnimationTime(); 
  chooseSkullAnimation();
  playSkullAnimation();
  moveDrawer();
}

void getDistance() {
  /*
  Calculate the distance to whatever is in
  front of the ultrasonic sensor. 
  */
  
  /* 
  Trigger pin low-high-low to send a pulse and
  calculate the average distance across 10 pulses.
  */
  // int totalTime = 0;
  int sumDistance = 0;
  for (int i = 0; i < 10; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // totalTime += pulseIn(echoPin, HIGH);
    int dist = (pulseIn(echoPin, HIGH) * 0.0343) / 2;
    if (dist > maxDistance) dist = maxDistance; // remove large values for calculation purposes
    sumDistance += dist;
  }

  distance_cm = sumDistance / 10;
  // sensorDuration = totalTime / 10; 

  // Read pulse
  // sensorDuration = pulseIn(echoPin, HIGH);
  // last_distance = distance_cm;
  // distance_cm = (sensorDuration * 0.0343) / 2;

  // if (isMeasuringDistance) {
  //   if (abs(distance_cm - last_distance) > 30 && last_distance != 0) distance_cm = last_distance; 
  //   if (distance_cm > maxDistance) distance_cm = maxDistance;
  // } else {
  //   isMeasuringDistance = 1;
  // }

  // Serial.println(distance_cm);
}

void setAnimationTime() {
  /*
  Set different animation speeds of the skull depending 
  on distance or whether the drawer is currently open. 
  */

  if (!isDrawerOpen) {
    if (distance_cm > 70) {
      skullMotionInterval = 2000;
      blinkInterval = 500;
    } else if (distance_cm >= 30) {
      skullMotionInterval = 1000;
      blinkInterval = 250;
    } else if (distance_cm < 30) {
      skullMotionInterval = 500;
      blinkInterval = 125;
    }
  }
  else {
    skullMotionInterval = 500;
    blinkInterval = 125;
  }
}

void chooseSkullAnimation() {
  /*
  Set a movement for the skull based on whether the 
  drawer is open, and how far away the sensor reads.
  */
  if (isDrawerOpen) {
    animationSequence = 2;
  } else {
    if (distance_cm > 70) animationSequence = 0;
    else if (distance_cm <= 40) animationSequence = 1;
  }
}

void playSkullAnimation() {
  /*
  The skull servos cycle through positions, and the LED 
  blinks on and off, based on the durations determined in
  setAnimationTime() and sequence chosen in chooseSkullAnimation().
  */

  /* Keep track of which animation is playing. */
  // Serial.print(distance_cm);
  // Serial.print(" cm, step ");
  // Serial.print(animationStep);
  // Serial.print(" of animation ");
  // Serial.println(animationSequence);

  if (animationSequence == 0) {
    /*
    Rotate the skull left and right. Note that the H servo
    at 60 degrees sets the head facing center.
    */
    if (millis() - lastMoveTime > 1000) {
      if (animationStep == 0) { servoMotorV.write(0); }
      else if (animationStep == 1) { servoMotorH.write(30); } 
      else if (animationStep == 2) {servoMotorH.write(90); }
      lastMoveTime = millis();
      animationStep++;
      if (animationStep > 2) { animationStep = 0; }
    }
    if (millis() - lastBlinkTime > 500) {
      if (isLightOn) { digitalWrite(ledPin, HIGH); } 
      else { digitalWrite(ledPin, LOW); }
      isLightOn = !isLightOn; // reverses the truth value
      lastBlinkTime = millis();
    }
  }

  else if (animationSequence == 1) {
    /*
    Fix the skull at the center, and raise its jaw up and down.
    */
    if (millis() - lastMoveTime > 500) {
      if (animationStep == 0) { servoMotorH.write(60); }
      else if (animationStep == 1) { servoMotorV.write(0); }
      else if (animationStep == 2) { servoMotorV.write(15); }
      lastMoveTime = millis();
      animationStep++;
      if (animationStep > 2) { animationStep = 0; }
    }
    if (millis() - lastBlinkTime > 250) {
      if (isLightOn) { digitalWrite(ledPin, HIGH); } 
      else { digitalWrite(ledPin, LOW); }
      isLightOn = !isLightOn; // reverses the truth value
      lastBlinkTime = millis();
    }
  }

  else if (animationSequence == 2) {
    /*
    Fix the skull at center and move its jaw only slightly
    up and down, while its eyes stay fixed on. 
    */
    if (millis() - lastMoveTime > 1000) {
      if (animationStep == 0) { servoMotorH.write(60); }
      else if (animationStep == 1) { servoMotorV.write(20); }
      else if (animationStep == 2) { servoMotorV.write(15); }
      lastMoveTime = millis();
      animationStep++;
      if (animationStep > 2) { animationStep = 0; }
    }
    digitalWrite(ledPin, HIGH);
    if (!isLightOn) isLightOn = 1;
  }

  /*
  An alternate method of controlling the skull, using intervals 
  determined by distance to speed up or slow down motion. Use a better
  distance sensor if you intend to use this code.  
  */
  // if (millis() - lastMoveTime > skullMotionInterval) {
  //   //Serial.println(animationStep);
  //   if (animationStep == 0) { servoMotorH.write(0); } 
  //   else if (animationStep == 1) { servoMotorV.write(0); }
  //   else if (animationStep == 2) { servoMotorH.write(60); }
  //   else if (animationStep == 3) { servoMotorV.write(15); }

  //   lastMoveTime = millis();
  //   animationStep++;
  //   if (animationStep > 3) { animationStep = 0; }
  // }

  // if (millis() - lastBlinkTime > blinkInterval) {
  //   if (isLightOn) { digitalWrite(ledPin, HIGH); } 
  //   else { digitalWrite(ledPin, LOW); }
  //   isLightOn = !isLightOn; // reverses the truth value
  //   lastBlinkTime = millis();
  // }
}

void moveDrawer() {
  /*
  Rotate the stepper motor to open or close the drawer, based on 
  whether the button is pressed when the drawer is closed, or whether 
  a specified amount of time has passed. 
  */

  if (!isDrawerOpen) {
    if (digitalRead(switchPin) == HIGH) {
      isDrawerOpen = 1;
      drawerMotor.step(stepsToRotate);
      lastOpenTime = millis();
    } 
  } else {
    if (millis() - lastOpenTime > waitTime) {
      isDrawerOpen = 0;
      drawerMotor.step(-stepsToRotate);      
    } 
  }
}
 

