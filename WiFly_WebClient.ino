#include <SPI.h>
#include <WiFly.h>
#include <SoftwareSerial.h>

#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__
//* WiFi Credentials
char passphrase[] = "pass";
char ssid[] = "id";
//*/
#endif

long int interval = 3600000;
long int time = 0 ;
SoftwareSerial dylos = SoftwareSerial(2, 3); // RX, TX

/*Sensor ID. Sequential.
Currently,
Community Center = 1
Boulevard & Carol St = 2 (James' place)
Railyard = 3
*/
int sensorID = 1;

//Client client(server, 80);
WiFlyClient client("hawaii.sagz.in", 80);

void setup() {
  digitalWrite(7, HIGH);
  pinMode(7,OUTPUT);
  
  Serial.begin(9600);
  dylos.begin(9600);
  WiFly.begin();
  
  while (!WiFly.join(ssid, passphrase)) {
    Serial.println("Association failed.");
  }  

  Serial.println("connecting...");
  
}

void loop() {
  
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  time = millis();
  if (time % 10000 == 0) Serial.println(time);
  
  if (!client.connected()){
    if (dylos.available() ){  //|| Serial.available())
      Serial.println("reconnecting.");
      httpRequest();
    }
    else
       if(time > interval){
         Serial.println("RESET!");
         digitalWrite(7, LOW); //Pulling the RESET pin LOW triggers the reset.
       }}
      
      /*
      if(millis() % interval < 1000)
        //WiFly.softwareReboot(true);
        if (!WiFly.join(ssid, passphrase, true))
          Serial.println("Association failed."); */
  else client.stop();//Serial.println("Dylos data not yet available");

}

// this method makes an HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect()) {
    Serial.println("connecting..");
    //delay(1000);
    // send the HTTP GET request:
    
    String data = "GET /sensor.php?sensorID=";
    data.concat(sensorID);
    data.concat("&data=");
    String dylosValues = "";
    
    while(dylos.available()) dylosValues.concat(((char)dylos.read()));
    //Serial.println(dylosValues.length());
    dylosValues.trim();
    /*if(dylosValues.indexOf('\n') != -1){
      dylosValues.replace("\n",",");
      //dylosValues.replace(",","%2C");
      Serial.println("New line character processed!");
    }*/
    
    data.concat(dylosValues);
    //while(Serial.available()) data.concat(((char)Serial.read()));
    //data.trim();
    data.concat(" HTTP/1.1");
    
    //if(dylosValues.length() >= 3){
    Serial.println(data);
    client.println(data);
    client.println("Host: hawaii.sagz.in");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    //client.println(Serial.read());
    //}
    // note the time that the connection was made:
    //lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection:
    //Serial.println("connection failed");
    //if (!WiFly.join(ssid, passphrase, true)) Serial.println("Association failed."); 
    //Serial.println("disconnecting.");
    client.stop();
  }
}

