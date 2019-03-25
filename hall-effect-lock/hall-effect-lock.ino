#define N_IN 4 // four picture frames
int inputs[N_IN] = { 3,4,5,6 }; // input pins

boolean won = HIGH; // relay starts locked

void setup() {
  Serial.begin(1200); // lower baud is more reliable
  for(int i=0; i<N_IN; i++)
    pinMode(inputs[i], INPUT_PULLUP); // inputs are high until sensor is active

  pinMode(2, OUTPUT); // relay powers magnetic lock
}

void loop() {
  digitalWrite(2,won); // set the relay state
  
  int on = 0; // count how many pictures are on the correct frame
  for(int i=0; i<N_IN; i++)
    on += digitalRead(inputs[i]) == LOW;
  Serial.println(on);

  
  if(on == N_IN) won = LOW; // all pictures are on the correct frame, unlock the door
  if(on == 0) won = HIGH; // all pictures are off the correct frame, lock the door
}
