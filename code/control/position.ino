#include "position.h"

int Position::readAngleAverage() {
  AA.reset();
  for (unsigned int i = 0; i < 6; ++i) {
    const float angle = (analogRead(A0) >> 1) / 511.f * M_PI;
    AA.add(angle);
  }
  return AA.getAverage() * 511.f / M_PI;
}

// dt is in microseconds
void Position::loop(int dt) {
  int value = readAngleAverage();

  // Update position...
  // Sensor counts in opposite rotation...
  pos -= value - oldValue;
  if (value < THRESHOLD_LOWER && oldValue > THRESHOLD_UPPER) {
    pos -= 512;
  } else if (value > THRESHOLD_UPPER && oldValue < THRESHOLD_LOWER) {
    pos += 512;
  }

  int dTheta = pos - oldPos;
  // Care for order of operations, dTheta and dT are ints, so int division can be a problem.
  float speed = dTheta * 1E6 / dt;
  speedRA.addValue(speed);

  oldPos = pos;
  oldValue = value;
}

Position::Position(char encoderPin, int ultrasonicPin) {
  // Setup encoder pin
  pinMode(encoderPin, INPUT);

  // Setup ultrasonic sensor
  us.attach(ultrasonicPin, ultrasonicPin, 0, 100);

  // Setup initial position
  pos = (x_abs() + x_abs() + x_abs() + x_abs()) / 4. / EFFECTIVE_RADIUS - readAngleAverage();
  oldValue = readAngleAverage();

  // speed running average
  speedRA.clear();
}

float Position::x_abs() {
  // us.read() returns distance in cm
  return us.getDistanceCM() / 100.f;
}

float Position::x() {
  return pos * EFFECTIVE_RADIUS;
}

float Position::v() {
  return speedRA.getAverage() * EFFECTIVE_RADIUS;
}
