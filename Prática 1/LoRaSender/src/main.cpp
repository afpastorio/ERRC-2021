#include <Arduino.h>
#include "heltec.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SDS011.h>

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6
#define DHTPIN 3 //Pino responsavel por ler os dados do DHT11
#define DHTTYPE DHT11 //Utilizamos o DHT11

DHT dht(DHTPIN, DHTTYPE); //Construtor do objeto dht
SDS011 my_sds; //construtor do objeto SDS011

String rssi = "RSSI --";
String packSize = "--";
String packet ;
float p10, p25;
int err,ip10,ip25;

#ifdef ESP32 //Conexão HardwareSerial dos SDS
HardwareSerial port(2);
#endif

void setup() {
  Serial.begin(9600);
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  dht.begin();
  my_sds.begin(&port);
  pinMode(25, OUTPUT);

  
}

void loop() {
  float h = dht.readHumidity(); //lendo a umidade
  float t = dht.readTemperature(); //lendo a temperatura
  err = my_sds.read(&p25, &p10);
  if (!err) {
    Serial.println("Ok");
    LoRa.beginPacket();
    LoRa.print("Umi: ");
    LoRa.print(h);
    LoRa.println("%");
    LoRa.print("Temp: ");
    LoRa.print(t);
    LoRa.println("ºC");
    LoRa.print("PM25: ");
    LoRa.println(p25);
    LoRa.print("PM10: ");
    LoRa.println(p10);
    LoRa.endPacket(); //retorno= 1:sucesso | 0: falha
  }
  else{
    Serial.println("Erro!");
  }
  
}