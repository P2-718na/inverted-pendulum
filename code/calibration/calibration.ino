#include "position.h"
#include "motor.h"

Position pos;
Motor mot;

void setup() {
   Serial.begin(460800);
}

int voltage = -128;
int state = 0;

long start_time;
long time_prev;

void loop() {

  if (0==state) { // retour chariot
    Serial.print("[");
    Serial.print(voltage/255.*12.);
    Serial.print(",");

    if (voltage < 0) {
      mot.drive(30);

      while (pos.x() < .25) {pos.loop(0);};
    }
    else {
      mot.drive(-30);

      while (pos.x() > .05) {pos.loop(0);};
    }

    mot.drive(0);
    delay(300);
    
    mot.drive(voltage);

    state = 1;

    start_time = micros();
  }
  else if (1==state) {
    long t  = micros() - start_time;
    long dt = t - time_prev;
    time_prev = t;
    
    pos.loop(dt);

    float cart_x = pos.x();

    if (voltage < 0) {
      if (cart_x > .07 && t<2000000) {
        Serial.print(cart_x*1000);
        Serial.print(", ");
        Serial.print(dt);
        Serial.print(",");
      } else {
        mot.drive(0);
        state = 2;
      }
    }
    else {
      if (cart_x < .27 && t<2000000) {
        Serial.print(cart_x*1000);
        Serial.print(", ");
        Serial.print(dt);
        Serial.print(",");
      } else {
        mot.drive(0);
        state = 2;
      }
    }
  }
  else if (2==state) {
    if (voltage+20<128) {
      Serial.println("],");
      voltage += 20;
      state = 0;
    }
  }
}
