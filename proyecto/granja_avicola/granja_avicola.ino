#include <SoftwareSerial.h>
#include <DHT.h>
SoftwareSerial BT1(4, 3); // RX | TX

boolean retorno = false;

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
  conectarwifi();
  delay(10000);
  conectardatosSIM();
  delay(10000);
}


void loop()
{
  int amoniaco = sensormq135();
  int temperatura = calcularTemperatura();
  int humedad = CalcularHumedad();
  enviarDatoWIFI(temperatura, humedad, amoniaco);
  //if (enviarDatoWIFI(temperatura, humedad, amoniaco)) {
  //Serial.println("SE GUARDO POR WIFI");
  //retorno = false;
  //}
  //else {
  //while (retorno == false) {
  enviardatosSIM();
  //}
  //Serial.println("SE GUARDO POR SIM");
  //retorno = false;
  //}
  delay(60000);
}

void conectarwifi() {
  BT1.println("AT+RST");
  BT1.setTimeout(2000);
  BT1.println("AT");
  delay(3000);
  if (BT1.find(("OK")))
    Serial.println("Respuesta AT correcto");
  else
    Serial.println("Error en ESP8266");
  BT1.println("AT+CWJAP=\"Movistar_22160435\",\"00929329818\"");
  BT1.setTimeout(10000);
  if (BT1.find("OK"))
    Serial.println("WIFI conectado");
  else
    Serial.println("Error al conectarse en la red");
}

////////////////////////////////////////////////////////////////////////
void conectardatosSIM() {

  SIM900.println("AT");
  delay(3000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT correcto");
  else
    Serial.println("Error en COMANDO AT");

  SIM900.println("AT+CPIN=\"0000\"");  //Comando AT para introducir el PIN de la tarjeta
  delay(1000);  //Tiempo para que encuentre una RED
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT correcto");
  else
    Serial.println("Error en COMANDO AT+CPIN");
}

void enviardatosSIM() {
  SIM900.println("AT+CGATT=1");
  delay(1000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CGATT correcto");
  else
    Serial.println("Error en COMANDO AT+CGATT");

  SIM900.println("AT+CSTT=\"internet.comcel.com.co\",\"comcel\",\"comcel\"");
  delay(3000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CSTT correcto");
  else
    Serial.println("Error en COMANDO AT+CSTT");

  SIM900.println("AT+CIICR");
  delay(3000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CIICR correcto");
  else
    Serial.println("Error en COMANDO AT+CIICR");


  SIM900.println("AT+CIFSR");//ip
  delay(3000);



  SIM900.println("AT+CIPSTART=\"TCP\",\"www.tepremiapp.com\",\"80\"");
  delay(5000);
  if (SIM900.find(("CONNECT OK")))
    Serial.println("Respuesta AT+CIPSTART correcto");
  else
    Serial.println("Error en COMANDO AT+CIPSTART");
  char direccion[] = "GET /sensor/datos.php?temperatura=1&humedad=2&amoniaco=3 HTTP/1.1\r\nHost: www.tepremiapp.com\r\nConnection: close\r\n\r\n";
  char aux_str[50];

  sprintf(aux_str, "AT+CIPSEND=%d", strlen(direccion));
  Serial.println(strlen(direccion));


  SIM900.println(aux_str);
  delay(5000);
  if (SIM900.find((">")))
    Serial.println("Respuesta AT+CIPSEND correcto");
  else
    Serial.println("Error en COMANDO AT+CIPSEND");

  SIM900.println(direccion);
  if (SIM900.find(("HTTP/1.1 200 OK"))) {
    Serial.println("ENVIADO CORRECTAMENTE");
    retorno = true;
  }

  else
    Serial.println("NO SE ENVIO");

  SIM900.println("AT+CIPCLOSE");
  delay(10000);
  if (SIM900.find(("CLOSE OK")))
    Serial.println("Respuesta AT+CIPCLOSE correcto");
  else
    Serial.println("Error en COMANDO AT+CIPCLOSE");

  SIM900.println("AT+CIPSHUT");
  delay(10000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CIPSHUT correcto");
  else
    Serial.println("Error en COMANDO AT+CIPSHUT");

  SIM900.println("AT+CGATT=0");
  delay(1000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CGATT correcto");
  else
    Serial.println("Error en COMANDO AT+CGATT");
}

////////////////////////////////////////////////////////////////////////




boolean enviarDatoWIFI(int temperatura, int humedad, int amoniaco)
{
  String temp = String(temperatura);
  Serial.println(temp);
  String hume = String (humedad);
  Serial.println(hume);
  String amon = String(amoniaco);
  Serial.println(amon);
  String peticion = "GET http://tepremiapp.com/sensor/datos.php?temperatura=" + temp + "&humedad=" + hume + "&amoniaco=" + amon;
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
   delay(3000);
  if (BT1.find("+IPD,")) {
    Serial.println("Datos almacenados");
    retorno = true;
  }
  else {
    Serial.println("No se almacenaron datos");
  }
  delay(1000);
  return retorno;
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
