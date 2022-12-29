#include "position.h"
#include "angle.h"
#include "motor.h"
#include <timer.h>

Position pos;
Angle* ang;
Motor mot;

Timer controlTimer;

void control() {
  

  //#define DEBUG
  #ifdef DEBUG
    Serial.print(pos.x());
    Serial.print(" ");
    Serial.print(pos.v());
    Serial.print(" ");
    Serial.print(ang->a());
    Serial.print(" ");
    Serial.print(ang->w());
    Serial.println();
  #endif
}

void setup() {
  Serial.begin(460800);

  // Must be initialised AFTER Serial.begin();
  ang = new Angle();

  // Move motor with buttons
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  // Setup timer
  controlTimer.setInterval(20); 
  controlTimer.setCallback(control);
  controlTimer.start();
}

double time;
double oldTime;

void loop() {
  time = micros();
  const int dt = time - oldTime;

  //Serial.print(" .");

  pos.loop(dt);
  ang->loop(dt);
  controlTimer.update();

  if (digitalRead(3) == LOW) {
    mot.drive(-32);
  } else if (digitalRead(2) == LOW) {
    mot.drive(32);
  } else {
    mot.drive(0);
  }

  oldTime = time;
}
