#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); //CONSTRUYO UN OBJETO DE LA CLASE DHT QUE REPRESENTA EL SENSOR
/*
INICIALIZA EL DISPOSITIVO
PUERTO SRIAL VELOCIDAD
DIRECCIONES DE PINES DE ENTRADA Y SALIDA
CREAR OBJETOS QUE SE REPRESENTAN SENSORES, DISPOSITIVOS WIFI Y OBJETOS DE LIBRERIAS EXTERNAS
*/

const char* ssid = "SANTY";
const char* pass = "Julio930";
const char* host = "dweet.io";
const char* thing  = "uceva00";
const char* thing_content = "temperatura";
const char* thing_content2 = "&humedad";
const int port = 80; // puerto por defecto del servicio web

void setup() {
  dht.begin();
  Serial.begin(115200); //configurar 115200 bits por segundo velocidad
  WiFi.begin(ssid, pass); // para vincular esp a la red asignada
  Serial.print("Se está conectando a la red WiFi denominada ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
  Serial.print("conectado exitosamente!!");
  Serial.print("direccion IP: ");
  Serial.println(WiFi.localIP());

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  if(isnan(hum) || isnan(temp)){ //evalua si es numero o no
    Serial.println("ERROR");
    return;
  }

  WiFiClient cliente; //objeto que realiza peticiones a un socket servidor

  if(!cliente.connect(host, port)){
    Serial.println("conexion fallida");
    delay(1000);
    return;
  }

  //si llega a esta linea es porque si hubo conexion con el servidor
  //ahora tenemros un flujo de transmision y otro de recepcion.
  String request = "/dweet/for/";
  request += thing;
  request += "?";
  request += thing_content;
  request += "=";
  request += temp;
  request += thing_content2;
  request += "=";
  request += hum;
  //String request = String("GET /dweet/for/uceva00?temperatura=45 HTTP/1.1\r\n") + "Host: " + "host" + "\r\n" + "connection: keep alive\r\n\r\n"; //parametro de cabcera

  //enviamos la peticion en protocolo http al servidor

  Serial.print("Requesting URL: ");
  Serial.println(request);
  
  // This will send the request to the server
  cliente.print(String("GET ") + request + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep alive\r\n\r\n");

  unsigned long inicio = millis();
  while(cliente.available() == 0){
    if(millis() - inicio > 1300){
      Serial.println("tiempo de espera del servidor agotado");
      cliente.stop();
      return;
    }
  }

  //si se llega a este punto es porque se recibieron daros del servidor (cliente.available() !=0)

  while (cliente.available()) {
    String linea= cliente.readStringUntil('\r'); //lea una linea hasta que encuentre el caracter enter \r
    Serial.println(linea);
      }
  
    Serial.println("fin de conexion al servidor");
    cliente.stop();
    delay(1300); //para evitar de que se hagan muchas peticiones al servidor y nos metan un blacklist
  /*delay(500);
  digitalWrite(2,LOW);
  delay(500);
  digitalWrite(2,HIGH);*/
}

