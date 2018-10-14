#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); 
void setup()
   {
      //digitalWrite(9, HIGH); // Descomentar para activar la alimentación de la tarjeta por Software
      //delay(1000); 
      //digitalWrite(9, LOW);
      delay (5000);  //Nos damos un tiempo para encender el GPRS y la alimentación de la tarjeta
      SIM900.begin(19200);  //Configura velocidad del puerto serie para el SIM900
      Serial.begin(19200);  //Configura velocidad del puerto serie del Arduino
      Serial.println("OK");
      delay (1000);
      SIM900.println("AT+CPIN=\"0000\"");  //Comando AT para introducir el PIN de la tarjeta
      delay(25000);  //Tiempo para que encuentre una RED
   }


void llamar()
   {
      Serial.println("Realizando llamada...");
      SIM900.println("ATD3218334774;");  //Comando AT para realizar una llamada
      delay(30000);  // Espera 30 segundos mientras realiza la llamada
      SIM900.println("ATH");  // Cuelga la llamada
      delay(1000);
      Serial.println("Llamada finalizada");
   }

   void mensaje_sms()
   {
      Serial.println("Enviando SMS...");
      SIM900.print("AT+CMGF=1\r");  //Configura el modo texto para enviar o recibir mensajes
      delay(1000);
      SIM900.println("AT+CMGS=\"3218334774\"");  //Numero al que vamos a enviar el mensaje
      delay(1000);
      SIM900.println("SMS enviado desde un Arduino. perro hp.");  // Texto del SMS
      delay(100);
      SIM900.println((char)26); //Comando de finalización ^Z
      delay(100);
      SIM900.println();
      delay(5000);  // Esperamos un tiempo para que envíe el SMS
      Serial.println("SMS enviado");
   }

   void loop()
   {
    mensaje_sms();
     // llamar();  //Realiza la llamada
      while (1);
   }
