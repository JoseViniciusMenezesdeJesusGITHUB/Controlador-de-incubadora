//Variaveis controle PI
float setPoint = 37.5, Kp = 5000, Ki = 0.26;
float P = 0, I = 0, oldP = 0, saida = 0;

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
