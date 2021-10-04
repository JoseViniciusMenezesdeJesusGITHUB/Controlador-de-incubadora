#include <TimerOne.h>
#include <Thermistor.h>
#include "displayConfig.h"

//Variaveis do Zero crossing
int i = 0;
int zeroCross = 0;
int zeroc = 2;
int brightness = 8200;
int  triac = 3;
int motor = 4;
int flag = 0;

//Variaveis do Termistor
Thermistor temp(2);
float rcbTemp = 0;
int temperatura;


//Variaveis da media movel
#define ciclos 3000
float somar = 0;
int n = 0;
float somatorio = 0;
float oldSomatorio = 0;


//Variaveis de tempo
long timer1 = 0, timer2 = 0;


//Variaveis controle PI
float setPoint = 37.5, Kp = 5000, Ki = 0.26;
float P = 0, I = 0, oldP = 0, saida = 0;


void setup() {

  LED_OF_config();
  
  pinMode(triac, OUTPUT);
  attachInterrupt(0, cruzamentoZero, RISING);
  pinMode(motor, OUTPUT);
  
  Timer1.initialize(100);
  Timer1.attachInterrupt(dim_check, 100);
}

void loop() {

  //Atualiza temperatura
  if((millis() - timer1)>= 10){

    rcbTemp = temp.getTemp();
    timer1 = millis();

  }
  //Tratamento para overflow do millis

    /* 
      Caso millis  tenha um overflow  timer1  tera  um  valor 
    maior que millis, pois millis zerará e com isso temos que
    associar o valor novo de  millis  a timer1 para recomeçar 
    a contagem de tempo sem problemas. 
    */

//  else if(millis() < timer1){
//    timer1 = 0;
//    timer2 = 0

//  }
//  if((millis() - timer2) >= 2000){
//    digitalWrite(motor, HIGH);
//    if((millis() - timer2) >= 5000)
    digitalWrite(motor, LOW);
//    timer2 = millis();
//  }

  //Chama media movel
  somatorio = mediaMovel();

  temperatura = int(somatorio*100);
  
    //disp.digit4showZero(temperatura);
    disp.send(LED_0F[0], 0b0001); 
    controlePI();

}

//Funções e procedimentos 


//CRUZAMENTO DE ZERO
void cruzamentoZero(){
  zeroCross = 1;
  i = 0;
  digitalWrite(triac, LOW);
}


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


//PARTE RESPONSAVEL POR ELIMINAR O RUÍDO
float mediaMovel(){
  
  if(n <= ciclos){
    somar += rcbTemp;
    n = n+1;
  }
  else{ 
    somar = 0;
    n = 0;
  }
  if(n == ciclos){ 
    oldSomatorio = somar/n;
    return (somar/n);
  }
  if(n < ciclos) return oldSomatorio;
}

void controlePI(){
  P = ((setPoint+0.05) - somatorio);
  I = I + oldP;

  if(I > 8200) I = 8200;
  if(I < 0) I = 0;

  saida = (Kp*P) + (Ki*I);
  if(saida > 8200) saida = 8200;
  if(saida < 0)  saida = 0;
  //Serial.println(I);
  brightness = 8200-saida;  

  oldP = P;
}
