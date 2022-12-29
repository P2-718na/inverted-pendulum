#include "position.h"

Position pos;

void setup() {
  Serial.begin(9600);
}


double time;
double oldTime;

void loop() {
  time = micros();
  const int dt = time - oldTime;

  pos.loop(dt);

  Serial.print(pos.x());
  Serial.print(" ");
  Serial.println(pos.x_abs());

  oldTime = time;
}
