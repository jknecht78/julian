#include <LedControl.h> 
#include <Ultrasonic.h>

// MAX7219 LED MATRIX
const int DIN = 12;
const int CLK = 11;
const int CS = 10;
const int n_modules = 4;
const int n_rows_per_module = 8;
const int n_total_rows = n_modules * n_rows_per_module;
LedControl lc=LedControl(DIN, CLK, CS, n_modules); 

// HC-SR04 Ultrasonic Sensor
const int trigPin = 3;
const int echoPin = 2;
Ultrasonic ultrasonic(trigPin, echoPin);

int distance = 0;
int last_distance = 0;
const int upper_limit = 100;
const int lower_limit = 20;

void setup() {

  // Initialize Serial
  Serial.begin(9600);

  // Initialize HC-SR04 Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize MAX7219 LED MATRIX
  for(int index=0; index<4; index++) {
    lc.shutdown(index,false); // Wake up displays    
    lc.clearDisplay(index); // Clear Displays    
  }

  lc.setIntensity(0, 1);
  lc.setIntensity(1, 5);
  lc.setIntensity(2, 10);
  lc.setIntensity(3, 15);

} 

void loop() {

  distance = ultrasonic.read(CM); 
  int tolerance = 9;
  if ( (distance < last_distance - tolerance) or (distance > last_distance + tolerance) )  {
    clear_display();
  }

  int row_to_iluminate = distance;
  if (distance > upper_limit) {
    distance = upper_limit;
  } 
  if (distance < lower_limit) {
    distance = lower_limit;
  }     

  row_to_iluminate = map(row_to_iluminate, lower_limit, upper_limit, 32, 0);
  
  int module = 0;
  int module_row = 8;
  for (int row=0; row < row_to_iluminate; row++) { 
    lc.setColumn(module, module_row, B11111111);
    module_row--;
    if (module_row < 0) {
      module_row = 8;
      module++;
    }  
  }
  
  last_distance = distance;

}

void clear_display() {
  for(int index=0; index<4; index++) {
    lc.clearDisplay(index); // Clear Displays
  }
}
