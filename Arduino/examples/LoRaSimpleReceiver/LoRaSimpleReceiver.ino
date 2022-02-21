#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(434E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
//  LoRa.setTxPower(2,PA_OUTPUT_RFO_PIN);
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
//    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
//    Serial.print("' with RSSI ");
//    Serial.println(LoRa.packetRssi());
  }
}
