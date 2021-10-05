#include <TimerOne.h>
#include <TM74HC595Display.h> //Modificar para atender minhas necessidades
#include <Thermistor.h>

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


//Variaveis do display
int SCLK = 7;
int RCLK = 6;
int DIO = 5;


TM74HC595Display disp(SCLK, RCLK, DIO);
unsigned char LED_0F[30];


//Variaveis de tempo
long timer1 = 0, timer2 = 0;


//Variaveis controle PI
float setPoint = 37.5, Kp = 5000, Ki = 0.26;
float P = 0, I = 0, oldP = 0, saida = 0;


void setup() {
  
  LED_0F[0] = 0xC0; //0
  LED_0F[1] = 0xF9; //1
  LED_0F[2] = 0xA4; //2
  LED_0F[3] = 0xB0; //3
  LED_0F[4] = 0x99; //4
  LED_0F[5] = 0x92; //5
  LED_0F[6] = 0x82; //6
  LED_0F[7] = 0xF8; //7
  LED_0F[8] = 0x80; //8
  LED_0F[9] = 0x90; //9
  LED_0F[10] = 0x88; //A
  LED_0F[11] = 0x83; //b
  LED_0F[12] = 0xC6; //C
  LED_0F[13] = 0xA1; //d
  LED_0F[14] = 0x86; //E
  LED_0F[15] = 0x8E; //F
  LED_0F[16] = 0xC2; //G
  LED_0F[17] = 0x89; //H
  LED_0F[18] = 0xF9; //I
  LED_0F[19] = 0xF1; //J
  LED_0F[20] = 0xC7; //L
  LED_0F[21] = 0xA9; //n
  LED_0F[22] = 0xC0; //O
  LED_0F[23] = 0x8C; //P
  LED_0F[24] = 0x98; //q
  LED_0F[25] = 0x92; //S
  LED_0F[26] = 0xC1; //U
  LED_0F[27] = 0x91; //Y
  LED_0F[28] = 0xFE; //hight -
  LED_0F[29] = 0x7F; //.

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
