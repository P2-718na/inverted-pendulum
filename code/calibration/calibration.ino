#include "../test/position.h"

void set_speed(int u) {
    if (u < 0)
   {
     // reverse rotation
     int reversePWM = -u;
     analogWrite(LPWM_Output, 0);
     analogWrite(RPWM_Output, reversePWM);
   }
    else
   {
     // forward rotation
     int forwardPWM = u;
     analogWrite(LPWM_Output, forwardPWM);
     analogWrite(RPWM_Output, 0);
   }
}


double cart_x_prev = 0;
long start_time, time_prev;
EasyUltrasonic ultrasonic;
void setup() {
   pinMode(RPWM_Output, OUTPUT);
   pinMode(LPWM_Output, OUTPUT);
   Serial.begin(115200);
   ultrasonic.attach(4, 3);
}

int voltage = -255;
int state = 0;



    long pos = 0;
    long th = 0;
    long oldTh = 0;
void loop() {
  if (0==state) { // retour chariot
    Serial.print("[");
    Serial.print(voltage/255.*12.);
    Serial.print(",");
    state = 1;

    set_speed(voltage<0 ? 30 : -30);

      double cart_x = ultrasonic.getDistanceCM()/100.;
    while (!(fabs(cart_x)<.03 || fabs(cart_x)>.26) ) {
      cart_x = ultrasonic.getDistanceCM()/100.;
      //Serial.println(cart_x);
    };
    set_speed(0);
    delay(300);
    
    set_speed(voltage);

    cart_x_prev = 0;
    start_time = micros();
    time_prev = 0;
  }
  if (1==state) {
    long t  = micros() - start_time;
    long dt = t - time_prev;
    time_prev = t;
    double cart_x = ultrasonic.getDistanceCM()/100.;
    cart_x_prev = cart_x;

    th = analogRead(0);
    if (th < 300 && oldTh > 800) {
      pos += 1024 -oldTh + th;  
    }
    else if (th > 800 && oldTh < 300) {
      pos -= 1024 - th + oldTh;  
    } else {
      pos += th - oldTh;
    }
    
    oldTh = th;
   

    if (((fabs(cart_x)<.25 && voltage < 0) ||(fabs(cart_x)>.1 && voltage > 0))  && t<2000000 ) {
      //Serial.print(pos / 1024. * 6.2831*5.);
Serial.print(cart_x*1000);
      Serial.print(", ");
      Serial.print(dt);
      Serial.print(",");
    } else {
      set_speed(0);
      state = 2;
    }
  }
  if (2==state) {
    if (voltage+20<256) {
      Serial.println("],");
      voltage += 20;
      state = 0;
    }
  }
}
