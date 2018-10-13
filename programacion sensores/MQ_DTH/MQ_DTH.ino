// Incluimos librería sensor dth11
#include <DHT.h>

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

void setup() {
  // Inicializamos comunicación serie
  Serial.begin(9600);
  // Comenzamos el sensor DHT
  dht.begin();

}

void loop() {
  delay(10000);
//Serial.print(sensormq135());

Serial.print( temperatura());
Serial.print( humedad());
}


int temperatura() {
  Serial.print("\n");
  float t = dht.readTemperature();
  return t;
}

int humedad() {
  Serial.print("\n");
  float h = dht.readHumidity();
  return h;
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
