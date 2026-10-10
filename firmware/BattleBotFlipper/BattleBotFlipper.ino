
/*
 * Battle Bot Flipper | V2
 * Arduino Mega 2560 | FlySky RC Receiver
 * Differential drive, reversible flipper, PWM motor control
 * RC signal monitoring, motor speed ramping and automatic disarming
 * Physical kill switch operates independently of Arduino
 */

#include <Arduino.h>

// 1. RC receiver pin definitions
const byte STEER_IN = 2;
const byte THROTTLE_IN = 3;
const byte FLIP_IN = 4;

// 2. Motor driver pin definitions
const byte L_PWM = 10;
const byte L_DIR = 11;

const byte R_PWM = 8;
const byte R_DIR = 9;

const byte F_PWM = 12;
const byte F_DIR = 13;

// 3. RC signal settings
const unsigned long RC_TIMEOUT_US = 25000;
const unsigned long RC_MIN_US = 900;
const unsigned long RC_MAX_US = 2100;

const int RC_DEADBAND = 35;
const int INVALID_RC = 9999;

// 4. Motor speed settings
const int MAX_SPEED = 255;
const int DRIVE_RAMP_STEP = 15;
const int FLIP_RAMP_STEP = 20;

// 5. Controller variables
const unsigned long ARM_TIME_MS = 1000;

bool armed = false;
bool armingStarted = false;

unsigned long armStartTime = 0;

int leftSpeed = 0;
int rightSpeed = 0;
int flipSpeed = 0;

// 6. RC receiver signal reading
int readRC(byte pin) {
  unsigned long pulse = pulseIn(pin, HIGH, RC_TIMEOUT_US);

  if (pulse < RC_MIN_US || pulse > RC_MAX_US) {
    return INVALID_RC;
  }

  int value = map((long)pulse, 1000, 2000, -MAX_SPEED, MAX_SPEED);
  value = constrain(value, -MAX_SPEED, MAX_SPEED);

  if (abs(value) < RC_DEADBAND) {
    value = 0;
  }

  return value;
}

// 7. Motor direction and PWM control
void motor(byte pwmPin, byte dirPin, int speedValue) {
  speedValue = constrain(speedValue, -MAX_SPEED, MAX_SPEED);

  if (speedValue == 0) {
    analogWrite(pwmPin, 0);
    return;
  }

  digitalWrite(dirPin, speedValue > 0 ? HIGH : LOW);
  analogWrite(pwmPin, abs(speedValue));
}

// 8. Motor acceleration and deceleration
int rampSpeed(int current, int target, int stepSize) {
  if ((current > 0 && target < 0) ||
      (current < 0 && target > 0)) {
    target = 0;
  }

  if (current < target) {
    current = min(current + stepSize, target);
  }
  else if (current > target) {
    current = max(current - stepSize, target);
  }

  return current;
}

// 9. Stop all motors
void stopAll() {
  leftSpeed = 0;
  rightSpeed = 0;
  flipSpeed = 0;

  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
  analogWrite(F_PWM, 0);
}

// 10. Controller disarming
void disarmController() {
  armed = false;
  armingStarted = false;
  stopAll();
}

// 11. Arduino setup
void setup() {
  digitalWrite(L_PWM, LOW);
  digitalWrite(R_PWM, LOW);
  digitalWrite(F_PWM, LOW);

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  digitalWrite(F_DIR, LOW);

  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(F_PWM, OUTPUT);

  pinMode(L_DIR, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(F_DIR, OUTPUT);

  pinMode(STEER_IN, INPUT);
  pinMode(THROTTLE_IN, INPUT);
  pinMode(FLIP_IN, INPUT);

  Serial.begin(115200);
  stopAll();

  Serial.println("Battle Bot Controller");
  Serial.println("System Disarmed");
}

// 12. Main control loop
void loop() {
  int steering = readRC(STEER_IN);
  int throttle = readRC(THROTTLE_IN);
  int flipper = readRC(FLIP_IN);

  bool signalValid =
    steering != INVALID_RC &&
    throttle != INVALID_RC &&
    flipper != INVALID_RC;

  if (!signalValid) {
    if (armed) {
      Serial.println("RC Signal Error");
    }

    disarmController();
    return;
  }

  if (!armed) {
    stopAll();

    bool neutral =
      steering == 0 &&
      throttle == 0 &&
      flipper == 0;

    if (!neutral) {
      armingStarted = false;
      return;
    }

    if (!armingStarted) {
      armStartTime = millis();
      armingStarted = true;
      return;
    }

    if (millis() - armStartTime < ARM_TIME_MS) {
      return;
    }

    armed = true;
    armingStarted = false;
    Serial.println("Controller Armed");
  }

  int leftTarget = constrain(
    throttle + steering, -MAX_SPEED, MAX_SPEED
  );

  int rightTarget = constrain(
    throttle - steering, -MAX_SPEED, MAX_SPEED
  );

  leftSpeed = rampSpeed(
    leftSpeed, leftTarget, DRIVE_RAMP_STEP
  );

  rightSpeed = rampSpeed(
    rightSpeed, rightTarget, DRIVE_RAMP_STEP
  );

  motor(L_PWM, L_DIR, leftSpeed);
  motor(R_PWM, R_DIR, rightSpeed);

  flipSpeed = rampSpeed(
    flipSpeed, flipper, FLIP_RAMP_STEP
  );

  motor(F_PWM, F_DIR, flipSpeed);

  delay(5);
}
