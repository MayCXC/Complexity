#include <SoftwareSerial.h>

// The readers connect to four virtual serial connections on pins 4-11
SoftwareSerial sa(4,5), sb(6,7), sc(8,9), sd(10,11); // SoftwareSerial sensor(RX,TX);

// The virtual serial inputs, win conditions, and correct tag IDs are stored in three parallel arrays
#define N_SS 4 // The number of sensors
#define N_ID 14 // The byte length of each tag ID
SoftwareSerial soft[N_SS] = { sa, sb, sc, sd };
boolean correct[N_SS] = { false, false, false, false };
byte correctIDs[N_SS][N_ID] = {
  {2,49,68,48,48,50,55,65,66,55,70,69,69,3}, // A
  {2,49,68,48,48,50,55,55,69,51,69,55,65,3}, // B
  {2,49,68,48,48,50,55,54,65,50,67,55,67,3}, // C
  {2,49,68,48,48,50,55,56,53,51,70,56,48,3}, // D
};

// Begin the hardware serial and software serial connections, and lock the magnet.
void setup()
{
  Serial.begin(9600);
  for(int i=0; i<N_SS; i++) {
    soft[i].begin(9600);
  }
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
}

// Main loop, repeats every 200ms
void loop()
{  
  for(int s=0; s<N_SS; s++) { // Cycle soft[s] through sa, sb, sc, and sd
    soft[s].listen(); // Only one software serial port can listen at a time
    delay(50); // Brief pause to fill the buffer

    if(soft[s].available()) { // Check if the sensor has sent any data
      byte input[N_ID] = {0}; // One empty tag ID

      // Fill the empty tag ID with bytes from the sensor
      int i=0;
      while(i<N_ID && soft[s].available()) {
        input[i] = soft[s].read();
        i++;
      }

      // Print the number of bytes read
      Serial.print("Reader #");
      Serial.print(s);
      Serial.print(" read ");
      Serial.print(i);
      Serial.print(" bytes: ");
      for(int j=0; j<i; j++) {
        Serial.print(input[j]);
        Serial.print(" ");
      }
      Serial.println();

      // For reliability, each sensor condition is correct after reading the correct ID, and until reading an incorrect ID
      if(i==N_ID) { // Check if the whole ID was read
        correct[s] = memcmp(input,correctIDs[s],N_ID) == 0; // Compare the ID to the correct ID

        // Print whether or not the sensor read the correct tag
        Serial.print("Reader #");
        Serial.print(s);
        if(correct[s]) {
          Serial.print(" read correct tag.");
        }
        else {
          Serial.print(" read incorrect tag.");
        }
        Serial.println();

      }
    }
  }

  // Check if all four sensors have read the correct tag, and release the magnet if they have.
  boolean won = true;
  for(int i=0; i<N_SS; i++) {
    won = won && correct[i];
  }
  if(won) {
    digitalWrite(2,LOW);
  }
}
