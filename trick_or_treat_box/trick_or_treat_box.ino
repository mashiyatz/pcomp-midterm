#include "Stepper.h"
#include "Servo.h"

const int stepsPerRevolution = 200; // change depending on stepper
const int stepperMotorSpeed = 100; 
const int switchPin = 2;
// initialize the stepper library on pins 3-6:
Stepper drawerMotor(stepsPerRevolution, 3, 4, 5, 6); 
const int trigPin = 7;
const int echoPin = 8;
const int servoVPin = 9;
const int servoHPin = 10;
const int ledPin = 11;

bool isDrawerOpen = 0; // 1 means the drawer is open
bool isLightOn = 0; 
long sensorDuration;
long lastMoveTime = 0;
long lastBlinkTime = 0;
int distance_cm;
int skullMotionFrequency = 1;
int blinkFrequency = 2;
int animationStep = 0;


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
  setAnimationFrequency();
  playSkullAnimation();
  moveDrawer();
}

void getDistance() {
  /*
  Calculate the distance to whatever is in
  front of the ultrasonic sensor. 
  */
  
  // Trigger pin low-high-low to send a pulse 
  int totalTime = 0;
  for (int i = 0; i < 10; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    totalTime += pulseIn(echoPin, HIGH);
  }
  
  sensorDuration = totalTime / 10; // see about using threshold value

  // Read pulse
  // sensorDuration = pulseIn(echoPin, HIGH);
  distance_cm = (sensorDuration * 0.0343) / 2;
  Serial.println(distance_cm);
}

void setAnimationFrequency() {
  /*
  Set different animation speeds of the skull depending 
  on distance or whether the drawer is currently open. 

  Frequency is defined as the number of times the write
  function of the servo or LED should be called in a second,
  or 1000 ms. 
  */

  if (!isDrawerOpen) {
    if (distance_cm > 100) {
      skullMotionFrequency = 0.5;
      blinkFrequency = 2;
    } else if (distance_cm >= 50) {
      skullMotionFrequency = 1;
      blinkFrequency = 4;
    } else if (distance_cm < 50) {
      skullMotionFrequency = 2;
      blinkFrequency = 8;
    }
  }
  else {
    skullMotionFrequency = 2;
    blinkFrequency = 8;
  }
}

void playSkullAnimation() {
  /*
  The skull servos cycle through positions, and the LED 
  blinks on and off, based on the frequencies determined in
  setAnimationFrequency().
  */

  if (millis() - lastMoveTime > long(1000 / skullMotionFrequency)) {
    //Serial.println(animationStep);
    if (animationStep == 0) { servoMotorH.write(0); } 
    else if (animationStep == 1) { servoMotorV.write(0); }
    else if (animationStep == 2) { servoMotorH.write(60); }
    else if (animationStep == 3) { servoMotorV.write(60); }

    lastMoveTime = millis();
    animationStep++;
    if (animationStep > 3) { animationStep = 0; }
  }

  if (millis() - lastBlinkTime > long(1000 / blinkFrequency)) {
    if (isLightOn) { digitalWrite(ledPin, HIGH); } 
    else { digitalWrite(ledPin, LOW); }
    isLightOn = !isLightOn; // reverses the truth value
    lastBlinkTime = millis();
  }
}

void moveDrawer() {
  /*
  Rotate the stepper motor to open or close the drawer, based on 
  whether the button is pressed when the drawer is closed, or whether 
  someone is still in front of the sensor when the drawer is open. 
  */

  // if (!isDrawerOpen) {
  //   if (digitalRead(switchPin) == HIGH) {
  //     isDrawerOpen = 1;
  //     drawerMotor.step(100);
  //   } 
  // } else {
  //   if (distance_cm > 30) {
  //     isDrawerOpen = 0;
  //     drawerMotor.step(-100);
  //   } 
  // }

  // testing
  drawerMotor.step(250);
  delay(500);
  drawerMotor.step(-250);
  delay(500);
}
 
