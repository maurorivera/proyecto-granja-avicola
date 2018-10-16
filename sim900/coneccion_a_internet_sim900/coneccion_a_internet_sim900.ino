#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

char direccion[] = "GET /sensor/datos.php?temperatura=1&humedad=2&amoniaco=3 HTTP/1.1\r\nHost: www.tepremiapp.com\r\nConnection: close\r\n\r\n";
char aux_str[50];

void setup()
{

  SIM900.begin(19200);  //Configura velocidad del puerto serie para el SIM900
  Serial.begin(19200);  //Configura velocidad del puerto serie del Arduino

  SIM900.println("AT");
  delay(3000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT correcto");
  else
    Serial.println("Error en COMANDO AT");

  SIM900.println("AT+CPIN=\"0000\"");  //Comando AT para introducir el PIN de la tarjeta
  delay(5000);  //Tiempo para que encuentre una RED
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT correcto");
  else
    Serial.println("Error en COMANDO AT+CPIN");

  SIM900.println("AT+CGATT=1");
  delay(3000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CGATT correcto");
  else
    Serial.println("Error en COMANDO AT+CGATT");

  SIM900.println("AT+CSTT=\"internet.comcel.com.co\",\"comcel\",\"comcel\"");
  delay(5000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CSTT correcto");
  else
    Serial.println("Error en COMANDO AT+CSTT");

  SIM900.println("AT+CIICR");
  delay(5000);
  if (SIM900.find(("OK")))
    Serial.println("Respuesta AT+CIICR correcto");
  else
    Serial.println("Error en COMANDO AT+CIICR");


  SIM900.println("AT+CIFSR");//ip
  delay(3000);

  SIM900.println("AT+CIPSTART=\"TCP\",\"www.tepremiapp.com\",\"80\"");
  delay(10000);
  if (SIM900.find(("CONNECT OK")))
    Serial.println("Respuesta AT+CIPSTART correcto");
  else
    Serial.println("Error en COMANDO AT+CIPSTART");


  sprintf(aux_str, "AT+CIPSEND=%d", strlen(direccion));
  Serial.println(strlen(direccion));


  SIM900.println(aux_str);
  delay(5000);
  if (SIM900.find((">")))
    Serial.println("Respuesta AT+CIPSEND correcto");
  else
    Serial.println("Error en COMANDO AT+CIPSEND");

  SIM900.println(direccion);
  if (SIM900.find(("HTTP/1.1 200 OK")))
    Serial.println("ENVIADO CORRECTAMENTE");
  else
    Serial.println("NO SE ENVIO");
   
  
}

void loop()
{ String B = "." ;
  if (SIM900.available())
  { char c = SIM900.read() ;
    Serial.print(c);
  }
  if (Serial.available())
  { char c = Serial.read();
    SIM900.print(c);
  }

}



void PeticionHttp()
{
  SIM900.println("AT+CIPSTART=\"TCP\",\"www.tepremiapp.com\",\"80\"");
  delay (10000);
  // Envíamos datos a través del TCP
  //SIM900.println("AT+CIPSEND="+strlen(direccion));
  // sprintf(aux_str, "AT+CIPSEND=%d", strlen(direccion));
  SIM900.println("AT+CIFSR");
  delay (10000);
  SIM900.println("AT+CIPSEND=114");
  delay (10000);
  SIM900.println(direccion);

}
