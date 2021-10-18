#include <Arduino.h>
#include "heltec.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6
void setup() {
    //WIFI Kit series V1 not support Vext control
    Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

}

void loop()
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        // received a packet
        Serial.print("Received packet '");
        // read packet
        String msg = "Received: ";
        while (LoRa.available())
        {
            char received = ((char)LoRa.read());
            Serial.print(received);
            msg = msg + received;
        }
        Heltec.display->clear();
        Heltec.display->drawString(0, 0, msg);
        Heltec.display->display();
        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }
}