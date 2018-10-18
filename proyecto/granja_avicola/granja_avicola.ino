#include <SoftwareSerial.h>
#include <DHT.h>
SoftwareSerial BT1(4, 3); // RX | TX

////////////////////////////////////sim900///////////
SoftwareSerial SIM900(7, 8);


/////////////////////////////////// configuracion inicial sensor temperatura y humedad ////////////////////////////////

// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// configuracion inicial sensor mq135 ////////////////////////////////
//Declaramos pin debe ser analogico
int pinsensormq135 = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(115200);
  dht.begin();
  BT1.begin(115200);
  SIM900.begin(19200);  //Configura velocidad del puerto serie para el SIM900
  Serial.begin(19200);  //Configura velocidad del puerto serie del Arduino
}


void loop()
{
  conectarwifi();
  delay(10000);
  conectardatosSIM();
  delay(10000);
  int amoniaco = sensormq135();
  int temperatura = calcularTemperatura();
  int humedad = CalcularHumedad();
  enviarDatowifi(temperatura, humedad, amoniaco);
  mensaje_sms(temperatura, humedad, amoniaco);
  enviardatosSIM();

  delay(60000);
}

void mensaje_sms(int temperatura, int humedad, int amoniaco)
{
  String temp = String(temperatura);
  String hume = String (humedad);
  String amon = String(amoniaco);
  Serial.println("Enviando SMS...");
  SIM900.print("AT+CMGF=1\r");  //Configura el modo texto para enviar o recibir mensajes
  delay(1000);
  SIM900.println("AT+CMGS=\"3218334774\"");  //Numero al que vamos a enviar el mensaje
  delay(1000);
  SIM900.println("Sr. Mesa: \nTemperatura: " + temp + "C \nHumedad: " + hume + "% \nAmoniaco :" + amon + "ppm"); // Texto del SMS
  delay(100);
  SIM900.println((char)26); //Comando de finalización ^Z
  delay(100);
  SIM900.println();
  delay(5000);  // Esperamos un tiempo para que envíe el SMS
  Serial.println("SMS enviado");
}

void conectarwifi() {
  BT1.println("AT+RST");
  BT1.setTimeout(2000);
  BT1.println("AT");
  delay(3000);
  BT1.println("AT+CWJAP=\"Movistar_22160435\",\"00929329818\"");
  BT1.setTimeout(10000);
}

void enviarDatowifi(int temperatura, int humedad, int amoniaco)
{
  String temp = String(temperatura);
  String hume = String (humedad);
  String amon = String(amoniaco);
  String peticion = "GET http://tepremiapp.com/sensor/datos.php?temperatura=" + temp + "&humedad=" + hume + "&amoniaco=" + amon;
  int longitud = peticion.length() + 23;
  Serial.println(longitud);
  BT1.println("AT+CIPSTART=\"TCP\",\"107.180.50.184\",80");
  delay(10000);
  BT1.println("AT+CIPSEND=" + String(longitud));
  BT1.setTimeout(10000);
  //if (BT1.find(">")) {
  Serial.println("enviando peticion http");
  BT1.println(peticion);
  // BT1.println("GET http://tepremiapp.com/sensor/datos.php?valor=999");
  BT1.setTimeout(500);
  BT1.println("Host:www.tepremiapp.com");
  Serial.println("se envio la peticion");
  BT1.setTimeout(10000);
  if (BT1.find("PD,") || BT1.find("+HPD,") || BT1.find("SEND OK")) {
    Serial.println("Datos almacenados");
  }
  delay(1000);
}



////////////////////////////////////////////////////////////////////////
void conectardatosSIM() {
  SIM900.println("AT+CPIN=\"0000\"");  //Comando AT para introducir el PIN de la tarjeta
  delay(1000);  //Tiempo para que encuentre una RED
}

void enviardatosSIM() {
  SIM900.println("AT+CGATT=1");
  delay(1000);
  SIM900.println("AT+CSTT=\"internet.comcel.com.co\",\"comcel\",\"comcel\"");
  delay(3000);
  SIM900.println("AT+CIICR");
  delay(3000);
  SIM900.println("AT+CIFSR");//ip
  delay(3000);
  SIM900.println("AT+CIPSTART=\"TCP\",\"www.tepremiapp.com\",\"80\"");
  delay(5000);
  char direccion[] = "GET /sensor/datos.php?temperatura=1&humedad=2&amoniaco=3 HTTP/1.1\r\nHost: www.tepremiapp.com\r\nConnection: close\r\n\r\n";
  char aux_str[50];
  sprintf(aux_str, "AT+CIPSEND=%d", strlen(direccion));
  SIM900.println(aux_str);
  delay(5000);
  SIM900.println(direccion);
  if (SIM900.find(("HTTP/1.1 200 OK"))) {
    Serial.println("ENVIADO CORRECTAMENTE");
  }
  else
  {
    Serial.println("NO SE ENVIO");
  }
  SIM900.println("AT+CIPCLOSE");
  delay(10000);
  SIM900.println("AT+CIPSHUT");
  delay(10000);
  SIM900.println("AT+CGATT=0");
  delay(1000);
}

////////////////////////////////////////////////////////////////////////






int sensormq135() {
  pinsensormq135 = analogRead(0);
  return pinsensormq135;
}

int calcularTemperatura() {
  float t = dht.readTemperature();
  return t;
}

int CalcularHumedad() {
  float h = dht.readHumidity();
  return h;
}
