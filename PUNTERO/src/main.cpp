#include <Arduino.h>

int numero = 10;

void setup ()
{
   Serial.begin(115200);
}

void Duplicar (int *punteronumero){
   *punteronumero = *punteronumero * 2;
   Serial.print("EL DOBLE ES: ");
   Serial.println(*punteronumero);

   delay(2000);

}

void loop() {
     Serial.print("en loop: numero= ");
     Serial.println(numero);
     Duplicar( &numero);

}