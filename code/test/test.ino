#include "position.h"
#include "Angle.h"

Position pos;
Angle* ang;

void setup() {
  Serial.begin(9600);

  // Must be initialised AFTER Serial.begin();
  ang = new Angle();
}


double time;
double oldTime;

void loop() {
  
  time = micros();
  const int dt = time - oldTime;

  pos.loop(dt);
  ang->loop(dt);

  Serial.print(pos.x());
  Serial.print(" ");
  Serial.print(pos.v());
  Serial.print(" ");
  Serial.print(ang->a());
  Serial.print(" ");
  Serial.print(ang->w());
  Serial.println();

  oldTime = time;
}
