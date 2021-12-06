#include "displayConfig.h"
#include "thermistorConfig.h"
#include "mediaMovel.h"
#include "zeroCrossing.h"
#include "controlPI.h"
#include "timerOneConfig.h"
#include "pinsConfig.h"
#include "timeCompare.h"
#include "millisOverflow.h"


void setup() {

  LED_OF_config();
  timerConfigurate();
  pinsConfig();
  
}

void loop() {

  //Atualiza temperatura
  if(timeCompare()){
    
    rcbTemp = temp.getTemp();
  }

  //Chama media movel
  somatorio = mediaMovel();

  temperatura = int(somatorio*100);
  
  //disp.digit4showZero(temperatura);
    disp.send(LED_0F[0], 0b0001); 
    controlePI();

}
