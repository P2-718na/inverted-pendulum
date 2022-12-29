#include "angle.h"
#include <Wire.h>

float Angle::convertRawAngleToDegrees(word newAngle) {
  /* Raw data reports 0 - 4095 segments, which is 0.08791 of a degree */
  return newAngle * AS5600_RAW_TO_DEGREES;
}

float Angle::convertRawAngleToRadians(word newAngle) {
  /* Raw data reports 0 - 4095 segments, which is 0.001534 of a radian */
  return newAngle * .001534;
}

int Angle::translatedAngle() {
  return - (encoder.rawAngle() - zeroAngle) % 4095;
}

Angle::Angle() {
  Wire.begin();

  /*if(encoder.detectMagnet() == 0 ){
    while(1){
        if(encoder.detectMagnet() == 1 ){
            Serial.print("Current Magnitude: ");
            Serial.println(encoder.getMagnitude());
            break;
        }
        else{
            Serial.println("Can not detect magnet");
        }
        delay(1000);
    }
  }*/

  zeroAngle = encoder.rawAngle() - 2064;
  //encoder.setOffset(180.f);
}


float Angle::a() {  
  AA.reset();
  for (unsigned int i = 0; i < 4; ++i) {
    const float angle = convertRawAngleToRadians(translatedAngle());
    AA.add(angle);
  }
  return AA.getAverage();
}

float Angle::w() {  
  return wRA.getAverage();
}

// dt is in us
void Angle::loop(int dt) {
  angle = a(); //radians

  float dTheta = angle - oldAngle;
  float speed = dTheta / dt * 1E6f;
  wRA.addValue(speed);

  oldAngle = angle;
}