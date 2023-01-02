#pragma once

class Motor{

  int LPWM_Output;
  int RPWM_Output;

  public:
    Motor(int LPWM_Output = 6, int RPWM_Output = 5);

    void drive(int u);
};