#include <SoftwareSerial.h>
SoftwareSerial wifi(3, 2);
#define DEBUG true

void setup() {
  wifi.begin(9600);
  Serial.begin(9600);
  sendData("AT+RST\r\n", 10000, DEBUG);
  sendData("AT+CIOBAUD=9600\r\n", 10000, DEBUG);
  sendData("AT+CWJAP=\"Movistar_22160435\",\"00929329818\"\r\n", 10000, DEBUG);
}

void loop() {
  enviardato();
 // delay(30000);
}


void enviardato() {
  sendData("AT+CIPSTART=\"TCP\",\"107.180.50.184\",80\r\n", 5000, DEBUG);
  sendData("AT+CIPSEND=101\r\n", 1000, DEBUG);
  String respuesta=sendData("GET http://tepremiapp.com/sensor/datos.php?temperatura=5&humedad=6&amoniaco=7\r\nHost:www.tepremiapp.com", 5000, DEBUG);
  int dato = respuesta.indexOf("IPD");
  if (dato > 0) {
    Serial.println("Se almacenaron datos por wifi");
    return true;
  }
  return false;
}


String sendData(String command, const int timeout, boolean debug) {
  String cadena = "";
  wifi.print(command); // enviar el carácter leído al modulo
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (wifi.available())
    {

      // Muestra los Datos de salida del modulo por el seguimiento de la serie
      char c = wifi.read(); //Lee el proximo caracter
      cadena += c;
    }
  }

  if (debug)
  {
    Serial.print(cadena);
  }

  return cadena;
}
