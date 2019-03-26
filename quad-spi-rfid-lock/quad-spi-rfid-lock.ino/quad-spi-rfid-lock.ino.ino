/*
*          MFRC522 - Library
*          SPI - Library
*
*----------------------------------------------------------------------------- empty_skull 
* Pin layout should be as follows:
* Signal     Pin             
*            Arduino Uno     MFRC522 board
* ------------------------------------------------------------
* Reset      8               RST
* SPI SS     xx              SDA
* SPI MOSI   11              MOSI  
* SPI MISO   12              MISO
* SPI SCK    13              SCK
*
*  
*/

// DEFINES
// Provides debugging info to the serial window
#define DEBUG

// LIBRARIES
#include <SPI.h>
#include <MFRC522.h>

// GLOBALS to set
// The number of RFID readers
const byte numReaders = 4; 

// Each reader has a unique Slave Select Pin on Arduino
const byte ssPins[]= {2,3,4,5};

// Shared reset pin
const byte resetPin = 8;


// Initialize an arrayof MRFC522 instance representing each reader 
MFRC522 mfrc522[numReaders];


// SOLUTION ======>>>> set of tag IDs to SOLVE the puzzle
const String correctIDs[] = {"19824213964", "8613913864", "1504213864", "3824313964"};  // For four, the arrary reads with items separated by columns



/*
Reader #0 on Pin #2 detected tag: 19824213964
Reader #1 on Pin #3 detected tag: 8613913864
Reader #2 on Pin #4 detected tag: 1504213864
Reader #3 on Pin #5 detected tag: 3824313964






*/



// the below is the string that holds the current detected tags
String currentIDs[numReaders];

// this pin controls the relay - driven to LOW releases the lock
const byte lockPin = A0;

/****************************************
*                  Setup Section Below
*/

void setup() {
  // put your setup code here, to run once:
  
  #ifdef DEBUG
  Serial.begin(9600);   // Initialize serial channel with the PC
  Serial.println("Serial communication started");
  #endif
  
  // Set the lock pin as output to secure the lock
  
  // For Testing - commenting out the lockpin, instead use onboard LED
  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin,HIGH);
  
  
  // Just for testing - add the onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  // End Test LED code Here
  
  // Initialize the SPI bus
  SPI.begin();
  //SPI.setClockDivider(SPI_CLOCK_DIV128);
  
  for (uint8_t i=0; i<numReaders; i++){
    
    // Note the SPI pins on the reader must always be connecte to certain
    // Arduino pins for MOSI (11), MISO (12), and SCK (13)
    // Slave Select and Reset can be assigned to any
    // For this project, running all Reset through (8) and SS through (2, 3, 4, 5)
    
    mfrc522[i].PCD_Init(ssPins[i], resetPin);
    mfrc522[i].PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_max);  // Unsure if this does anything
    
    // Info for Debug 
    
    Serial.print("Readers #");
    Serial.print(i);
    Serial.print(" initialized on pin ");
    Serial.print(String(ssPins[i]));
    Serial.print(". Antenna strength: ");
    Serial.print(mfrc522[i].PCD_GetAntennaGain());
    Serial.print(". Version: ");
    mfrc522[i].PCD_DumpVersionToSerial();
    
    
    // Slight delay before activating the next reader
    delay(1000);
    
  }  //end of initialize
  Serial.println("--- End of the Setup ---");
}  //end of setup

void loop() {
  // put your main code here, to run repeatedly:
  
  // Assume the puzzle is solved and that no changes in tags since last reading
  boolean puzzleSolved = true;
  boolean changedValue = false;
  
  
  // Loop through the readers to see if puzzle is ACTUALLY Solved
  
  for (uint8_t i=0; i<numReaders; i++) {
  // Initialize the sensor
  mfrc522[i].PCD_Init();
  
  String readRFID = "";    
  
  // If the sensor detects a tag and it is able to read it (&&) is logical AND
  if(mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()){
    // Extracdt the ID info from the Tag
    for (int x=0; x<4; x++){
      readRFID = readRFID + mfrc522[i].uid.uidByte[x];  
    }  // end of for
    // Serial.println(readRFID); 
  }   // end of Reader Loop
    
    
    // If the current reading is different from the last known reading, then....
  if(readRFID != currentIDs[i]){
    changedValue  = true; // flag change
    currentIDs[i] = readRFID; //replace the stored value with the newly read vaule
  }
  // If the stored value fails to match the solution (for this reader)
  if(currentIDs[i] != correctIDs[i]) {
    puzzleSolved = false; // if a stored doesn't match correct, changed the solved flag
  }
  
  //Halt PICC
  mfrc522[i].PICC_HaltA();
  //Stop encryption on PCD
  mfrc522[i].PCD_StopCrypto1();

} //end of loop through sensors

// If the changedvalue flag has been set, at least one sensor has changed.

if(changedValue) {
  // report to screen the updated info for all sensors
  for (uint8_t i=0; i<numReaders; i++) {
    Serial.print("Reader #");
    Serial.print(String(i));
    Serial.print(" on Pin #");
    Serial.print(String(ssPins[i]));
    Serial.print(" detected tag: ");
    Serial.println(currentIDs[i]);
  }//end of each sensors report
  Serial.println("-----"); // 
} // end of all sensors



// If the puzzleSolved flag is set, all sensors matched
if(puzzleSolved){
  onSolve();         // subroutine called  
}

delay(100);

}  //end of loop


void onSolve(){
  
  #ifdef DEBUG
  // Print debugging message
  Serial.println("Puzzle Solved, yo!");
  
  // Release the lock
  #endif
  
  //For Testing - commenting out the lockpin,
  digitalWrite(lockPin,LOW);
  
  
  // Just for testing - add the onboard LED
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  // End Test LED code Here
  
  /*while(true){
  }
  */
  //* Current Code Unlocks, waits 100 seconds and re-locks.... can make longer, or can add a reset card.... or both
  delay(10000);
  digitalWrite(lockPin,HIGH);

} // end of onSolve subroutine

