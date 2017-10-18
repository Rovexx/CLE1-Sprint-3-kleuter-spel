#include <Adafruit_NeoPixel.h>
 
#define PIN 9
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8


int controleB = 0;
int controleR = 0;
int controleG = 0;

int frequency = 0;

const int buttonPin = 2;

int buttonState = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

int buttonState2 = 0;
const int buttonPin2 = 10;

void setup() {
  strip.begin();
  strip.setBrightness(30); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Setting frequency-scaling to 100%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);
  
  Serial.begin(9600);

  
}

 void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
void loop() {
  
  buttonState2 = digitalRead(buttonPin2);

  if (buttonState2 == LOW){

  int kleur = random(0,3);
    switch(kleur){
      case 0:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
      controleR = 1;
      controleG = 0;
      controleB = 0;
      
      break;
      case 1:
  colorWipe(strip.Color(0, 255, 0), 50); // Green
      controleG = 1;
      controleR = 0;
      controleB = 0;
      
      break;
      case 2:
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
      controleB = 1;
      controleG = 0;
      controleR = 0;
      
      break;
    }
  }

      buttonState = digitalRead(buttonPin);

 // Serial.println(buttonState);

  if (buttonState == LOW) { //het is omgekeerd maar het werkt. Just let it be.
   
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
int  Red = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("Red= ");//printing name
  Serial.print(Red);//printing RED color frequency
  Serial.print("  ");
  delay(10);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
int  Green = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("Green= ");//printing name
  Serial.print(Green);//printing RED color frequency
  Serial.print("  ");
  delay(10);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
int  Blue = pulseIn(sensorOut, LOW) * 3;
  // Printing the value on the serial monitor
  Serial.print("Blue= ");//printing name
  Serial.print(Blue);//printing RED color frequency
  Serial.println("  ");
 

  if(Red < Green && Red < Blue && controleR == 1){
     Serial.println(" - RED detected!");
     Serial.println("Gewonnen!");
     delay(2000);
     colorWipe(strip.Color(0,0,0), 50);
     controleR = 0;
  }
  if(Green < Red && Green < Blue && controleG == 1){
    Serial.println(" - GREEN detected!");
    Serial.println("Gewonnen!");
     delay(2000);
     colorWipe(strip.Color(0,0,0), 50);
     controleG = 0;
  }
  if(Blue < Red && Blue < Green && controleB == 1){
    Serial.println(" - BLUE detected!");
    Serial.println("Gewonnen!");
     delay(2000);
     colorWipe(strip.Color(0,0,0), 50);
     controleB = 0;
  }

  delay(1000);
}
}
