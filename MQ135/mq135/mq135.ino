

int pinsensormq135 = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  pinsensormq135 = analogRead(0); 
  float mq135_voltaje = pinsensormq135 * (5.0 / 1023.0);
  float mq135_resistencia = 1000*((5-mq135_voltaje)/mq135_voltaje);
  double amoniaco = 161.7*pow(mq135_resistencia/5463, -2.26);
        
  Serial.println(pinsensormq135, DEC);
  Serial.println(amoniaco);
  delay(250); 

}
