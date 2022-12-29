#include "position.h"
#include "angle.h"
#include "motor.h"
#include <timer.h>

Position pos;
Angle* ang;
Motor mot;

Timer controlTimer;
Timer driftTimer;

double c[] = {30.789042, 20.549323, 67.716816, 12.689277};
double A = .251;
double B = .0473;
double C = .00804;
bool controllable = false;
void control() {
  const float x = pos.x();
  const float v = pos.v();
  float a = ang->a();
  const float w = ang->w();
  if (a > 3.14) {
    a -= 6.282;
  }

  controllable = (a > -.28 && a < .28) && (x > .03 && x < .3);
  if (controllable) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }
  // motor (0.25100000000000011, 0.047271014362306074, -0.0080361899323083148)

  double f = c[0]*(x-.175) + c[1]*v + c[2]*a + c[3]*w;
  // .02 = deltaT, 0.2593640128 = mass
  // QUesto .6 dovrebbe essere la massa ma ok i guess va bene così
  double u = ((1 - A)*v + .02 * f/ .62 + copysignf(C, v)) / B;

  mot.drive(u*255./12.);

  //Serial.println();
  //#define DEBUG
  #ifdef DEBUG
    Serial.print(x);
    Serial.print(" ");
    Serial.print(v);
    Serial.print(" ");
    Serial.print(a);
    Serial.print(" ");
    Serial.print(w);
    Serial.println();
  #endif
}

void zeroPosition() {
  pos.oldPos = pos.pos - pos.oldPos;
  pos.pos = pos.x_abs() / EFFECTIVE_RADIUS;
  pos.oldPos = pos.pos - pos.oldPos;
}

void setup() {
  Serial.begin(460800);

  // Must be initialised AFTER Serial.begin();
  ang = new Angle();

  // Move motor with buttons
  pinMode(4, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  // Setup timer
  controlTimer.setInterval(20);
  controlTimer.setCallback(control);
  controlTimer.start();

  driftTimer.setInterval(1000);
  driftTimer.setCallback(zeroPosition);
  driftTimer.start();
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
  driftTimer.update();

  if (!controllable) {
    if (digitalRead(3) == LOW) {
      digitalWrite(4, HIGH);
      mot.drive(-32);
    } else if (digitalRead(2) == LOW) {
      digitalWrite(4, HIGH);
      mot.drive(32);
    } else {
      mot.drive(0);
    }
  }

  oldTime = time;
}
