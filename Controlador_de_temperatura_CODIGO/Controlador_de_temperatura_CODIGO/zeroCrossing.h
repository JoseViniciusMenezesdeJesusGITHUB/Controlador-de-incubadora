//Variaveis do Zero crossing
int i = 0;
int zeroCross = 0;
int zeroc = 2;
int brightness = 8200;
int  triac = 3;
int motor = 4;
int flag = 0;

//INTERRUPÇÃO POR TEMPO PARA ACIONAR O TRIAC
void dim_check(){
  if(zeroCross == 1){
    if(i >= brightness){
      digitalWrite(triac, HIGH);
      i = 0;
      zeroCross = 0;  
    }

    else{
      i = (i + 100);
    }
  }
}
