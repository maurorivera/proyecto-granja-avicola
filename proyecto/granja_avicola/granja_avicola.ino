#include <SoftwareSerial.h>
#include <DHT.h>
SoftwareSerial BT1(4, 3); // RX | TX

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
  BT1.println("AT+RST");
  BT1.setTimeout(2000);
  BT1.println("AT");
  delay(3000);
  if (BT1.find(("OK")))
    Serial.println("Respuesta AT correcto");
  else
    Serial.println("Error en ESP8266");
  BT1.println("AT+CWJAP=\"Cesar\",\"Cesar123\"");
  BT1.setTimeout(10000);
  if (BT1.find("OK"))
    Serial.println("WIFI conectado");
  else
    Serial.println("Error al conectarse en la red");
}

void loop()
{
  int amoniaco = sensormq135();
  int temperatura = calcularTemperatura();
  int humedad = CalcularHumedad();
  enviarDato(temperatura,humedad,amoniaco);
  //delay(900000);
}

void enviarDato(int temperatura,int humedad, int amoniaco)
{
  String temp=String(temperatura);
  String hume=String (humedad);
  String amon = String(amoniaco);
  String peticion = "GET http://tepremiapp.com/sensor/datos.php?temperatura="+temp+"&humedad="+hume+"&amoniaco="+amon;
  int longitud = peticion.length() + 23;
  Serial.println(longitud);

  conexionServer();
  BT1.println("AT+CIPSEND=" + String(longitud));
  BT1.setTimeout(10000);
  //if (BT1.find(">")) {
  Serial.println("enviando peticion http");
  BT1.println(peticion);
  // BT1.println("GET http://tepremiapp.com/sensor/datos.php?valor=999");
  BT1.setTimeout(500);
  BT1.println("Host:www.tepremiapp.com");
  Serial.println("se envio la peticion");
  if (BT1.find("+IPD,")) {
    Serial.println("Datos almacenados");
  }
  delay(1000);
}

void conexionServer() {
  BT1.println("AT+CIPSTART=\"TCP\",\"107.180.50.184\",80");
  delay(10000);
  if (BT1.find("OK"))
    Serial.println("Se establecio comunicacion con el servidor");
  else
    Serial.println("Error al conectarse al servidor");
}


int sensormq135() {
  pinsensormq135 = analogRead(0);
  //Serial.print(pinsensormq135, DEC);
  Serial.print(" ppm");
  delay(250);

  //gas propano y butano
  if (pinsensormq135 >= 400)
  {
    // Serial.print(" propano butano");
    delay(50);
  }
  return pinsensormq135;
}

int calcularTemperatura() {
  Serial.print("\n");
  float t = dht.readTemperature();
  return t;
}

int CalcularHumedad() {
  Serial.print("\n");
  float h = dht.readHumidity();
  return h;
}
