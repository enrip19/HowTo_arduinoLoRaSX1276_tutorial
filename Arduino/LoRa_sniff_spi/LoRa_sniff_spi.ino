#include <SPI.h>              // include libraries
#include <LoRa.h>

#define SNIFF_EN
#define PRINT_EN

#ifdef SNIFF_EN
// #define SNIFF_SETUP
//#define SNIFF_TX
 #define SNIFF_RX
#endif

const long frequency = 433E6;  // LoRa Frequency
const int txPower = 17; //min 2dBm max 20dBm
const int SF = 7; //7 faster, 12 more secure
const int BW = 250E3;
const int syncWord = 0x34;

const int numPackets = 1;
const int numBytes = 10;
String dataTx;

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin

const int trigPin = 7;

bool go = 1;
uint8_t count = 0;
String input;

#ifdef SNIFF_EN
const uint8_t btn_pin = PC13 ; 

const int trigg_sniff = D0; //pin to trigger sniffer like logic analyzer

#endif

void setup() {
  #ifdef PRINT_EN
  Serial.begin(9600);                   // initialize serial
  while (!Serial);
  #endif

  #ifdef SNIFF_TX
  pinMode(trigg_sniff, OUTPUT);
  #endif

  #ifdef SNIFF_EN
  pinMode(btn_pin, INPUT);
  while(digitalRead(btn_pin)==HIGH)
  {
//    #ifdef PRINT_EN  
//    Serial.print(digitalRead(btn_pin));
//    Serial.println("waiting for button push to start de program");
//    #endif
  }
  #endif

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {

    #ifdef PRINT_EN    
    Serial.println("LoRa init failed. Check your connections.");
    #endif

    while (true);                       // if failed, do nothing
  }
  LoRa.setSPIFrequency(1E6);
  LoRa.setTxPower(txPower);
  LoRa.writeRegister(0x0b, 0x3F);
  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(BW);
  LoRa.setSyncWord(syncWord);
  
  #ifndef SNIFF_SETUP
  #ifndef SNIFF_TX
  LoRa.onReceive(onReceive);
  #endif
  #ifndef SNIFF_RX
  LoRa.onTxDone(onTxDone);
  #endif
  #endif

  #ifdef PRINT_EN
  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Gateway");
  Serial.println("Only receive messages from nodes");
  Serial.println("Tx: invertIQ enable");
  Serial.println("Rx: invertIQ disable");
  Serial.println();
  #endif

  #ifndef SNIFF_SETUP
  for(int i=0; i<numBytes; i++){
      if(i == numBytes-1){
        dataTx = dataTx + "a";
      }
      else{
        dataTx = dataTx + "f";
      }
  }
  #ifndef SNIFF_TX
  LoRa_rxMode();
  #endif
  #endif
}

void loop() {
  #ifndef SNIFF_SETUP
  digitalWrite(trigg_sniff, HIGH);
  #ifndef SNIFF_RX
  if (go==1){//runEvery(5000)) { // repeat every 5000 millis
    go = 0;
    LoRa_sendMessage(dataTx); // send a message

    Serial.println("Send Message!");
  }
  if(Serial.available()){
    input = Serial.readStringUntil('\n');
    if(input=="a"){
      Serial.println(count);
      Serial.println("count reset");
      count = 0;  
    }
  }
  #endif
  #endif
}

#ifndef SNIFF_SETUP
#ifndef SNIFF_TX
void LoRa_rxMode(){
  //LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}
#endif

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  ///LoRa.enableInvertIQ();                // active invert I and Q signals
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

#ifndef SNIFF_TX
void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  Serial.println(message);
//  count++;
//  if(count == numPackets){
//    Serial.print("All messages received");
//    count = 0;
//    go = 1;  
//  }
 
  //Serial.print("Gateway Receive: ");
  //Serial.println(message);
}
#endif

void onTxDone() {
  Serial.println("TxDone");
  digitalWrite(trigg_sniff, LOW);
  #ifndef SNIFF_TX
  LoRa_rxMode();
  #else
  LoRa.idle();
  #endif
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

#endif
