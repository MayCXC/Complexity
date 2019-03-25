#define N_IN 8 // the number of hall effect sensors
int inputs[N_IN] = { 4,5,6,7, 10,11,12,13 }; // the digital input pins they occupy

boolean won = LOW; // fire starts off unlit

void setup() {
  Serial.begin(1200); // lower baud rate is more reliable
  for(int i=0; i<N_IN; i++)
    pinMode(inputs[i], INPUT_PULLUP); // inputs will be high until sensor is active

  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2,won);

  int on = 0; // count how many sensors are active
  for(int i=0; i<N_IN; i++)
    on += digitalRead(inputs[i]) == LOW; // input is low when sensor is active
  Serial.println(on);

  if(on == N_IN) won = HIGH; // all sensors are active, turn on the fire
  if(on == 0) won = LOW; // all sensors are inactive, turn off the fire
}
