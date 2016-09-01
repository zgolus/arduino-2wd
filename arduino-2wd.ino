#include <NewPing.h>
#include <AFMotor.h>
#include <SoftwareSerial.h>

#define TRIGGER_PIN  A0
#define ECHO_PIN     A1
#define MAX_DISTANCE 100

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
SoftwareSerial BtSerial(A5, A4);

bool inManagedMode = true;
char command = 'u';

void setup() {
  Serial.begin(9600);
  BtSerial.begin(9600);
  fullSpeed();
  halt();
}

void loop() {
  // Serial.println(inManagedMode);
  if (BtSerial.available()) {
    command = BtSerial.read();
    Serial.println(command);

    if (command == 'U') {
      fullSpeed();
      forward();
    }
    if (command == 'D') {
      fullSpeed();
      backward();
    }
    if (command == 'L') {
      slowDown();
      turnLeft();
    }
    if (command == 'R') {
      slowDown();
      turnRight();
    }
    if (command == 'u' || command == 'd') {
      halt();
    }
    if (command == 'l' || command == 'r') {
      halt();
    }
    if (command == 'm') {
      switchMode();
      halt();
    }
    delay(10);
  } else if (!inManagedMode) {
    delay(50);
    unsigned int distance = sonar.ping_cm();
    if (distance > 0 && distance < 30) {
      Serial.println("obstacle!");
      Serial.print(distance);
      turnRight();
    } else {
      forward();
    }
  }
}

void slowDown() {
  Serial.println("Slow down");
  motor1.setSpeed(150);
  motor2.setSpeed(150);
}

void fullSpeed() {
  Serial.println("Full speed");
  motor1.setSpeed(250);
  motor2.setSpeed(250);
}

void halt() {
  Serial.println("Halt");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void forward() {
  Serial.println("Forward");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void backward() {
  Serial.println("Backward");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void switchMode() {
  if (inManagedMode) {
    inManagedMode = false;
    fullSpeed();
  } else {
    inManagedMode = true;
    // halt();
  }
  Serial.println(inManagedMode);
}
