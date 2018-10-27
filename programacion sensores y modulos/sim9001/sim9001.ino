#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
#define DEBUG true

void setup() {
  SIM900.begin(19200);  //Configura velocidad del puerto serie para el SIM900
  Serial.begin(19200);  //Configura velocidad del puerto serie del Arduino
}

void loop()
{
  enviar();

  enviarmsm();

}
void enviar() {
  sendData("AT\r\n", 5000, DEBUG);
  sendData("AT+CGATT=1\r\n", 5000, DEBUG);
  sendData("AT+CSTT=\"internet.comcel.com.co\",\"comcel\",\"comcel\"\r\n", 5000, DEBUG);
  sendData("AT+CIICR\r\n", 5000, DEBUG);
  sendData("AT+CIFSR\r\n", 5000, DEBUG);
  sendData("AT+CIPSTART=\"TCP\",\"www.tepremiapp.com\",\"80\"\r\n", 5000, DEBUG);
  sendData("AT+CIPSEND=101\r\n", 5000, DEBUG);
  sendData("GET http://tepremiapp.com/sensor/datos.php?temperatura=1&humedad=2&amoniaco=3\r\nHost:www.tepremiapp.com", 10000, DEBUG);
}


String sendData(String command, const int timeout, boolean debug) {
  String cadena = "";
  SIM900.print(command); // enviar el carácter leído al modulo
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (SIM900.available())
    {

      // Muestra los Datos de salida del modulo por el seguimiento de la serie
      char c = SIM900.read(); //Lee el proximo caracter
      cadena += c;
    }
  }

  if (debug)
  {
    Serial.print(cadena);
  }

  return cadena;
}



void enviarmsm()
{
  sendData("AT+CMGF=1\r\n", 1000, DEBUG);
  sendData("AT+CMGS=\"3104706624\"\r\n", 1000, DEBUG);
  sendData("SMS enviado desde un Arduino", 100, DEBUG);
  SIM900.println((char)26);
}
