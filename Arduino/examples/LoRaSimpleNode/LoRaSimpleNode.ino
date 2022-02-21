/*
  LoRa Simple Gateway/Node Exemple

  This code uses InvertIQ function to create a simple Gateway/Node logic.

  Gateway - Sends messages with enableInvertIQ()
          - Receives messages with disableInvertIQ()

  Node    - Sends messages with disableInvertIQ()
          - Receives messages with enableInvertIQ()

  With this arrangement a Gateway never receive messages from another Gateway
  and a Node never receive message from another Node.
  Only Gateway to Node and vice versa.

  This code receives messages and sends a message every second.

  InvertIQ function basically invert the LoRa I and Q signals.

  See the Semtech datasheet, http://www.semtech.com/images/datasheet/sx1276.pdf
  for more on InvertIQ register 0x33.

  created 05 August 2018
  by Luiz H. Cassettari
*/

#include <SPI.h>              // include libraries
#include <LoRa.h>

const long frequency = 433E6;  // LoRa Frequency
const int txPower = 17; //min 2dBm max 20dBm
const int SF = 7; //7 faster, 12 more secure
const int BW = 250E3;
const int syncWord = 0x34;

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin
int i = 0;
int string;
void setup() {
  Serial.begin(9600);                   // initialize serial
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
//  LoRa.setTxPower(0,PA_OUTPUT_RFO_PIN);

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Node");
  Serial.println("Only receive messages from gateways");
  Serial.println("Tx: invertIQ disable");
  Serial.println("Rx: invertIQ enable");
  Serial.println();
  
  LoRa.setSPIFrequency(1E6);
  LoRa.setTxPower(txPower);
  LoRa.writeRegister(0x0b, 0x3F);
  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(BW);
  LoRa.setSyncWord(syncWord);
//  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
//  LoRa_rxMode();

while(!Serial.available());
string = Serial.read();
}
void loop() {
  if(Serial.available() > 0){
    string = Serial.read();
    if(string == 0x66){ //cal apretar la f al teclat x a que envii algo
      uint8_t message1 = 0x0A+i;
      uint8_t message2 = 0x47+i;
      uint8_t message3 = 0x98+i;
      uint8_t message4 = 0x77+i;
      
      
      LoRa_sendMessage(message1,message2,message3);//,message4); // send a message
      
//      Serial.write(message1);
//      Serial.write(message2);
//      Serial.write(message3);
//      Serial.write(message4);
      i++;
    }
    else{};
  }
}

void LoRa_rxMode() {
  //  LoRa.enableInvertIQ();                // active invert I and Q signals
  //  LoRa.receive();                       // set receive mode
}

void LoRa_txMode() {
  LoRa.idle();                          // set standby mode
  //  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(uint8_t message1, uint8_t message2, uint8_t message3){//, uint8_t message4) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  for(int k=0; k<10;k++){
    LoRa.write(message1+k);
    Serial.write(message1+k);
  }
//  LoRa.write(message1);                  // add payload
//  LoRa.write(message2);
//  LoRa.write(message3);
  //LoRa.write(message4);
  LoRa.endPacket(true);                 // finish packet and send it
}

//void onReceive(int packetSize) {
//  String message = "";
//
//  while (LoRa.available()) {
//    message += (char)LoRa.read();
//  }
//
//  Serial.print("Node Receive: ");
//  Serial.println(message);
//}

void onTxDone() {
//  Serial.println("TxDone");
//  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
