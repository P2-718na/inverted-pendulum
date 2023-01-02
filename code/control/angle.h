#pragma once
#include <AS5600.h>
#include <AverageAngle.h>
#include <RunningAverage.h>

#define START_POSITION word(0/0.087)
#define END_POSITION   word(360/0.087)
// word(356/0.087)

class Angle {
  private:
    AS5600 encoder;
    RunningAverage wRA = RunningAverage(6);
    AverageAngle AA = AverageAngle(1); // 1 = radians

    int zeroAngle;
    float angle;
    float oldAngle;
    
    int translatedAngle();
    float convertRawAngleToDegrees(word newAngle);
    float convertRawAngleToRadians(word newAngle);
    

  public:
    Angle();

    void loop(int dt);

    float a();
    float w();

};