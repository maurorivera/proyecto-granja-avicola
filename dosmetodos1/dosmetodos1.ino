#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
SoftwareSerial wifi(3, 2);
#define DEBUG true
boolean decision = false;


void setup() {
  Serial.begin(9600);  //Configura velocidad del puerto serie del Arduino
  SIM900.begin(19200);
  wifi.begin(9600);
  //Configura velocidad del puerto serie para el SIM900
}

void loop() {
  Serial.println(decision);
  enviarwifi();
  if (decision == false) {
    enviarsim();
  }
  else{
    decision=false;
  }
 Serial.println("2: "+decision);
}


void enviarsim() {
  sendDatasim("AT\r\n", 5000, DEBUG);
  sendDatasim("AT+CGATT=1\r\n", 5000, DEBUG);
  sendDatasim("AT+CSTT=\"internet.comcel.com.co\",\"comcel\",\"comcel\"\r\n", 5000, DEBUG);
  sendDatasim("AT+CIICR\r\n", 5000, DEBUG);
  sendDatasim("AT+CIFSR\r\n", 5000, DEBUG);
  sendDatasim("AT+CIPSTART=\"TCP\",\"www.tepremiapp.com\",\"80\"\r\n", 5000, DEBUG);
  sendDatasim("AT+CIPSEND=101\r\n", 5000, DEBUG);
  String respuesta = sendDatasim("GET http://tepremiapp.com/sensor/datos.php?temperatura=1&humedad=2&amoniaco=3\r\nHost:www.tepremiapp.com", 10000, DEBUG);
  int dato = respuesta.indexOf("true");
  if (dato > 0) {
    Serial.println("Se almacenaron datos por sim");
  }
}

String sendDatasim(String command, const int timeout, boolean debug) {
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

//////////////////////////////////////////////////////////////////////////////////////////////
void enviarwifi() {
  sendDatawifi("AT+RST\r\n", 10000, DEBUG);
  sendDatawifi("AT+CWJAP=\"Movistar_22160435\",\"00929329818\"\r\n", 10000, DEBUG);
  sendDatawifi("AT+CIPSTART=\"TCP\",\"107.180.50.184\",80\r\n", 5000, DEBUG);
  sendDatawifi("AT+CIPSEND=101\r\n", 1000, DEBUG);
  String respuesta1 = sendDatawifi("GET http://tepremiapp.com/sensor/datos.php?temperatura=5&humedad=6&amoniaco=7\r\nHost:www.tepremiapp.com", 3000, DEBUG);
  int dato1 = respuesta1.indexOf("true");
  if (dato1 > 0) {
    Serial.println("Se almacenaron datos por wifi");
    decision = true;
  }

}


String sendDatawifi(String command, const int timeout, boolean debug) {
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
