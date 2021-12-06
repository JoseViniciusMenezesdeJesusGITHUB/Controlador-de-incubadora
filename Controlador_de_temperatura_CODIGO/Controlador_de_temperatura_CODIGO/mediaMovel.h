//Variaveis da media movel
#define ciclos 3000
float somar = 0;
int n = 0;
float somatorio = 0;
float oldSomatorio = 0;

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
