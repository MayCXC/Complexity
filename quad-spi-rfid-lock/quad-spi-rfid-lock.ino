/* Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      2             -         -          -                -
 * SPI SS 2    SDA(SS)      3             -         -          -                -
 * SPI SS 3    SDA(SS)      4             -         -          -                -
 * SPI SS 4    SDA(SS)      5             -         -          -                -
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define N_IN 4 // the number of readers
#define N_UID 4 // the length of each uid
#define RST_PIN 9 // the reset pin

int inputs[N_IN] = { 2,3,4,5 }; // the sda pin of each reader

boolean won = HIGH; // the relay state

byte keys[N_IN][N_UID] = { // the correct tag uid for each reader
  {182, 244, 141,  64},
  {  6, 168, 144,  64},
  {102, 168, 144,  64},
  {  6,  36, 146,  64}
};

MFRC522 rfid[N_IN]; // initialize a seperate reader on each input

void setup() { // begin communications
  Serial.begin(9600);
  SPI.begin();

  pinMode(7, OUTPUT); // relay pin
}

void loop() {
  digitalWrite(7, won); // set relay state

  int matches = 0; // count the number of uid matches we find
  
  for(int i=0; i<N_IN; i++) { // compare input from each reader
    digitalWrite(inputs[i], HIGH); // fixes unreliable input, see https://github.com/miguelbalboa/rfid/issues/290
    delay(50);

    rfid[i].PCD_Init(inputs[i], RST_PIN); // enable the reader
    delay(50);

    if ( rfid[i].PICC_IsNewCardPresent() && rfid[i].PICC_ReadCardSerial() ) { // we found a tag
      // rfid[i].PICC_DumpToSerial(&(rfid[i].uid));
      boolean match = true; // true if every byte matches
      for(int j=0; j<rfid[i].uid.size; j++)
        match = match && rfid[i].uid.uidByte[j] == keys[i][j];

      if(match) // increment the number of total matches
        matches++;

      rfid[i].PICC_HaltA(); // disable the reader
      rfid[i].PCD_StopCrypto1(); // might save some volts
    }
  }

  if(matches == N_IN) won = HIGH; // all tags in their correct position, open the relay
  if(matches == 0) won = LOW; // no tags in their correct position, close the relay
}
