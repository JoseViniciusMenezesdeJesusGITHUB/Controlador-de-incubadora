//Variaveis de tempo
#define dt 10
long timer1 = 0;

int timeCompare(){
  if((millis() - timer1)>= 10){
    timer1 = millis();
    return true;
  }
  else return false;
}
