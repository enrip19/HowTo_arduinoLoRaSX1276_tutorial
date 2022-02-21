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

const long frequency = 434E6;  // LoRa Frequency

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin

bool btn = 0;
int byteCount = 0;
int i = 0;
int count = 0;
int string;
void setup() {
  Serial.begin(9600);                   // initialize serial
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Node");
  Serial.println("Only receive messages from gateways");
  Serial.println("Tx: invertIQ disable");
  Serial.println("Rx: invertIQ enable");
  Serial.println();

  LoRa.onReceive(onReceive);
//  LoRa.onTxDone(onTxDone);

//  LoRa_rxMode();
//
//  while(!Serial.available());
//  string = Serial.read();
  pinMode(3,INPUT);
}
void loop() {
  btn = digitalRead(3);
//  Serial.println(btn);
  if(btn==0){
    LoRa_rxMode();
  }
  else{
    LoRa.idle();
  }
//  if(Serial.available() > 0){
//    string = Serial.read();
//    if(string == 0x66){ //cal apretar la f al teclat x a que envii algo
//      uint8_t message1 = 0x0A+i;
//      uint8_t message2 = 0x47+i;
//      uint8_t message3 = 0x98+i;
//      uint8_t message4 = 0x77+i;
//      
//      
//      LoRa_sendMessage(message1,message2,message3);//,message4); // send a message
//      
//      Serial.write(message1);
//      Serial.write(message2);
//      Serial.write(message3);
//      Serial.write(message4);
//      i++;
//    }
//    else{};
//  }
}

void LoRa_rxMode() {
  //  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode() {
  LoRa.idle();                          // set standby mode
  //  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(uint8_t message1){//, uint8_t message2, uint8_t message3){//, uint8_t message4) {
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

void onReceive(int packetSize) {
  uint8_t message;

  while (LoRa.available()) {
    Serial.write(LoRa.read());
//    message += LoRa.read();
////    Serial.write(count);
//    count++;
  }
  LoRa.idle();
//  Serial.write(message);
  
//  if(count==byteCount){
////    Serial.write(0x0D);
////    Serial.write(0x0A);
//    count = 0;
//    LoRa_sendMessage(0x0A);
//  }
//  LoRa.idle();
}

void onTxDone() {
//  Serial.println("TxDone");
  LoRa_rxMode();
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
