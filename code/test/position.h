#pragma once
#include <RunningAverage.h>
#include <AverageAngle.h>
#include <EasyUltrasonic.h>

#define EFFECTIVE_RADIUS 0.000033162447
// Sensor will read angle between 0 and 511.
#define THRESHOLD_LOWER 170
#define THRESHOLD_UPPER 341

class Position {
  private:
    char encoderPin;
    EasyUltrasonic us;
    RunningAverage speedRA = RunningAverage(6);
    AverageAngle AA = AverageAngle(1); // 1 = radians

    // Counter
    long pos;
    long oldPos;

    // Current sensor rotation
    int value;
    int oldValue;

    int readAngleAverage();

  public:
    Position(char encoderPin = A0, int ultrasonicPin = 12);

    float x_abs();
    float x();
    float v();

    void loop(int dt);
};
