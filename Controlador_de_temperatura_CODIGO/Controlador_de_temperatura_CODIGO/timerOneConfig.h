#include <TimerOne.h>
//CRUZAMENTO DE ZERO
void cruzamentoZero(){
  zeroCross = 1;
  i = 0;
  digitalWrite(triac, LOW);
}

void timerConfigurate(){
  attachInterrupt(0, cruzamentoZero, RISING);
  Timer1.initialize(100);
  Timer1.attachInterrupt(dim_check, 100);
}
