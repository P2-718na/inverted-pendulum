#include "motor.h"

Motor::Motor(int LPWM_Output_, int RPWM_Output_) {
  RPWM_Output = RPWM_Output_;
  LPWM_Output = LPWM_Output_;

  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
}

void Motor::drive(int u){  
   if (u < 0) {
    // reverse rotation
    analogWrite(LPWM_Output, -u);
    analogWrite(RPWM_Output, 0);
    
    return;
   }

   if (u > 0) {
     // forward rotation
     analogWrite(LPWM_Output, 0);
     analogWrite(RPWM_Output, u);

     return;
   }

  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 0);
}