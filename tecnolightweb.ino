#include <SoftwareSerial.h>
#define DEBUG true
#define dbg Serial  // USB local debug

// DIGITAL PORT 11 and 12 on the UNO..
SoftwareSerial esp(2, 3); // RX,TX

//parametros para conexion a la red
String ssid = "telecable";
String pass = "GuzmanAngulo2015";
String ip="200.56.204.17"; //ip del servidor al que enviare la peticion

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  esp.print(command); // send the read character to the esp8266
  long int time = millis();
  while ( (time + timeout) > millis()) {
    while (esp.available()) {
      // The esp has data so display its output to the serial window
      char c = esp.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }
  return response;
}


void setup() {
  //pinMode(2, INPUT);
  //pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT); // For debugging purposes w/o USB
  digitalWrite(13, HIGH);
  esp.begin(9600);
  dbg.begin(9600);
  dbg.println("Iniciando configuracion WIFI\n");
  
  sendData("AT+RST\r\n", 2000, DEBUG); // reset module
  sendData("AT\r\n", 2000, DEBUG); // reset module
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG); // configure as access point
  sendData("AT+CWJAP=\"" + ssid + "\",\"" + pass + "\"\r\n", 6000, DEBUG); // configure as access point
  sendData("AT+CIPMUX=0\r\n", 2000, DEBUG); // reset module
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
  
  dbg.println("configuracion WIFI terminada\n\n");
}

int i=0;
void loop() {
  i++;
  //creando la peticon mediante comandos AT
//  inicio la peticion
  dbg.println("Conexion: "+i+"\r\n");
  String req="AT+CIPSTART=\"TCP\",\""+ip+"\",80\r\n"; //conectar con la ip
  sendData(req, 1000, DEBUG);
  String httpreq="GET http://juliomxmx.sytes.net/tecnolightweb/prueba.php\r\n";
  String cmd = "AT+CIPSEND=";
  cmd += httpreq.length();
  cmd += "\r\n";
  sendData(cmd, 1000, DEBUG);
  sendData(httpreq, 1000, DEBUG);
  sendData("AT+CIPCLOSE\r\n", 1300, DEBUG);//cierro la conexion y espero un tiempo
}

