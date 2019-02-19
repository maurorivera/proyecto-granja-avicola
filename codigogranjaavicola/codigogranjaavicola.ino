#include <SoftwareSerial.h>
#include <DHT.h>
SoftwareSerial SIM900(7, 8);
SoftwareSerial wifi(3, 2);
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define DEBUG true
boolean decision = false;
String temperatura;
String humedad ;
String amoniaco;



void setup() {
  Serial.begin(9600);  //Configura velocidad del puerto serie del Arduino
  SIM900.begin(19200);
  wifi.begin(9600);
  dht.begin();



  //Configura velocidad del puerto serie para el SIM900
}

void loop() {
  temperatura = calcularTemperatura();
  amoniaco = calcularAmoniaco();
  humedad = calcularHumedad();
  Serial.println(decision);
  enviarwifi();
  if (decision == false) {
    enviarsim();
  }
  else {
    decision = false;
  }
    if (temperatura.toInt() > 31 || amoniaco.toInt() > 600 || humedad.toInt() > 75) {
      enviarmsm();
    }
  delay(60000);
}


void enviarsim() {
  String peticion = "GET http://tepremiapp.com/granja/datos.php?temperatura=" + temperatura + "&humedad=" + humedad + "&amoniaco=" + amoniaco + "\r\nHost:www.tepremiapp.com";
  int longitud = peticion.length();
  sendDatasim("AT\r\n", 5000, DEBUG);
  sendDatasim("AT+CGATT=1\r\n", 5000, DEBUG);
  sendDatasim("AT+CSTT=\"internet.comcel.com.co\",\"comcel\",\"comcel\"\r\n", 5000, DEBUG);
  sendDatasim("AT+CIICR\r\n", 5000, DEBUG);
  sendDatasim("AT+CIFSR\r\n", 1000, DEBUG);
  sendDatasim("AT+CIPSTART=\"TCP\",\"www.tepremiapp.com\",\"80\"\r\n", 5000, DEBUG);
  sendDatasim("AT+CIPSEND=" + String(longitud) + "\r\n", 1000, DEBUG);
  String respuesta = sendDatasim(peticion, 10000, DEBUG);
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
  String peticion = "GET http://tepremiapp.com/granja/datos.php?temperatura=" + temperatura + "&humedad=" + humedad + "&amoniaco=" + amoniaco + "\r\nHost:www.tepremiapp.com";
  int longitud = peticion.length();
  sendDatawifi("AT+RST\r\n", 10000, DEBUG);
  sendDatawifi("AT+CWJAP=\"Movistar_22162734\",\"00929330332\"\r\n", 10000, DEBUG);
  sendDatawifi("AT+CIPSTART=\"TCP\",\"107.180.50.184\",80\r\n", 5000, DEBUG);
  sendDatawifi("AT+CIPSEND=" + String(longitud) + "\r\n", 1000, DEBUG);
  String respuesta1 = sendDatawifi(peticion, 3000, DEBUG);
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

void enviarmsm()
{
  sendDatasim("AT+CMGF=1\r\n", 1000, DEBUG);
  sendDatasim("AT+CMGS=\"3104706624\"\r\n", 1000, DEBUG);
  sendDatasim("ALERTA!!\nSr Mesa: \nSe recomienda tomar medidas en su granja\nTemperatura: " + temperatura + "C\nHumedad:" + humedad + "%\nAmoniaco:" + amoniaco + "ppm", 100, DEBUG);
  SIM900.println((char)26);
}

String calcularTemperatura() {
  int t = dht.readTemperature();
  return String(t);
}
String calcularHumedad() {
  int h = dht.readHumidity();
  return String(h);
}

String calcularAmoniaco() {
  int pinsensormq135 = analogRead(0);
  return String(pinsensormq135);

}
