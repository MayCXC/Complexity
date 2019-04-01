#include <Adafruit_NeoPixel.h>

#define smolder 16 // the red value of unlit candles
#define caught 96 // the limit a candle passes above to stay lit
#define breath 8 // the limit a sensor drops below to light its sensor
#define wind 1.0/64 // the amout candles cool, warm, and flicker per loop

#define N_IN 6 // the number of piezo inputs
int inputs[N_IN] = { A5,A4,A3,A2,A1,A0 };
int outputs[N_IN] = { 2,3,4,5,6,7 };

Adafruit_NeoPixel leds[N_IN]; // the candle LEDs

int flame[N_IN]; // the brightness of each candle

bool anySensorHot() { // returns true iff any sensor is hot
  bool hot = false;
  for(int n=0; n<N_IN; n++)
    hot = hot || analogRead(inputs[n]) < breath;
  return hot;
}

void setup() {
  Serial.begin(9600);

  pinMode(8, OUTPUT); // the door relay

  for(int n=0; n<N_IN; n++) { // light each candle
    pinMode(inputs[n], INPUT);
    pinMode(outputs[n], OUTPUT);
    flame[n] = smolder;
    leds[n] = Adafruit_NeoPixel(1, outputs[n], NEO_GRB + NEO_KHZ800);
    leds[n].begin();
  }

  while(anySensorHot()) // wait for sensors to warm up before starting the puzzle
    delay(0);
}

int flicker(int amplitude, double theta, double phase) { // wave function used to simulate a flickering fire
  return (int)(amplitude*(1+sin(2*PI*(theta+phase))));
}

/* set a candle's brightness, returns true iff a candle passes above the "caught" limit, dependant on:
 * n, the input/output index
 * next, the sensor input
 * delta, the change in input
 * amplitude, the ficker intensity
 * theta, the current time
 */
bool light(int n, int next, int delta, double amplitude, double theta) {
    int last = flame[n]; // store the previous flame intensity
  
    if(next < breath || breath < delta) // brighten the candle if the sensor input is bottomed out or decreasing
      flame[n] += min(delta, 255-flame[n]);
    else if(smolder < flame[n] && flame[n] < caught) // dim the candle if it is below the "caught" limit
      flame[n] -= wind;
    else if(caught <= flame[n] && flame[n] < 255) // brighten the candle if it is above the "caught" limit
      flame[n] += wind;

    leds[n].setPixelColor( // display the red heat and yellow flicker of the candle on its LED
      0,
      flame[n],
      flicker( amplitude, theta, (double)n/N_IN ),
      0
    );

    leds[n].show();

    return last < caught && caught <= flame[n]; // the candle passed the "caught" limit
}

double seconds() { // return the current time in seconds as a double
  return millis()/1000.0;
}

int last[N_IN] = {0}; // store the last input of each sensor

int order[N_IN] = {-1}; // the order in which the candles were blown out
int win[N_IN] = { 2,4,1,0,3,5 }; // the win condition peizo sequence
double recent = 0.0;
void progress(int p, int theta) { // add a candle to the order
  recent = theta;
  for(int n=0; n<N_IN; n++)
    if(order[n]<0) {
      order[n] = p;
      return;
    }
}

void loop() {
  double theta = seconds();

  for(int n=0; n<N_IN; n++) { // read its sensor input, display its LED, and update the order for each candle
    int next = analogRead(inputs[n]);
    int delta = last[n]-next;

    if(light(n, next, delta, flame[n]*pow(wind,.25), theta))
      progress(n, theta);

    last[n] = next;
  }

  bool won = true; // unlock the door if the current order matches the win order
  for(int n=0; n<N_IN; n++)
    won = won && order[n] == win[n];

  digitalWrite(8, !won);

  // reset the puzzle if all candles were activated in the wrong order, or if players are taking too long
  if( !won && ( order[N_IN-1]>=0 || recent > 0.0 && theta-recent > breath ) ) { 
    recent = 0.0;
    
    for(int n=0; n<N_IN; n++)
      order[n] = -1;

    while(anySensorHot() || seconds() < breath/2) // flicker the candles and wait for the sensors to warm up
      for(int n=0; n<N_IN; n++)
        light(n, 0, 0, 2*smolder, 2*seconds());

    for(int n=0; n<N_IN; n++) { // relight each candle
      flame[n] = smolder;
      last[n] = flame[n];
    }
  }
}

