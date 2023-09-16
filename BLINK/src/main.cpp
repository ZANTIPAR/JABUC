#include <Arduino.h>

#define LED 2
 
void setup() {
  Serial.begin(115200); //Inicializamos el puerto Serial a 115200 bits / segundo
  pinMode(LED,OUTPUT); //Colocar un pin como entrada o como salida
}
 
void loop() {
  delay(500);  //Realizar retardos de tiempo en milisegundos
  digitalWrite(LED,HIGH);  //Escribir en un pin, ya sea un alto (HIGH) o bajo (LOW)

  Serial.println("Led encendido");

  delay(500);
  digitalWrite(LED,LOW);

  Serial.println("Led apagado");
}