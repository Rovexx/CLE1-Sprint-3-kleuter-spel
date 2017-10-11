
/*
versie 1.0 
- random RGB waarde generator toegevoegd
- pseudocode gemaakt
- functies voor ledring aan doen en uitdoen
- funcite gemaakt die checkt of de resetknop is ingedrukt
- reset functie gemaakt voor de code
- basis frame gemaakt voor de code

versie 1.1




pseudocode zonder timer
// als een van de knoppen wordt ingedrukt start de game
// de ledstrip geeft een kleur R G of B
// wacht op een knop die ingedrukt wordt, team 1 of 2. 
// zodra de knop is ingedrukt gaat de kleuren scanner scannen.
// als de kleur van de ledbalk hetzelfde is als de kleur die de kleurensensor geeft timer stoppen en score team 1 optellen en laten zien op lcd 1
// zelfde geldt voor team 2 aleen zij klikken op de knop voor hun team en hun timer gaat lopen en de score wordt bij dat team opgeteld
// het eerste team die tot bijv 6 punten heeft wint, pixel ring kleurt regenboog
// wacht tot de reset knop wordt ingedrukt programma begint opnieuw op klik

// klik op een knop om te starten
// led wordt een kleur
// timer gaat lopen team 1 gaat zoeken 
// team 1 scant object als de kleur klopt stopt de timer
// timer gaat lopen voor team 2, zij gaan zoeken
// team 2 scant object als de kleur klopt stopt de timer
// tijden worden vergeleken, snelste tijd wint, score wordt 1 opgeteld op de display van dat team.
// begint opnieuw
// bij score 16 gaat ledje regenboog branden en reset het spel
*/



#include <Adafruit_NeoPixel.h>

#define PIN            6    // pin dat de pixelring aan zit
#define NUMPIXELS      12   // aantal pixels op de ring
#define resetKnop      8    // de resetknop zit op pin 8

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd1(0x3E, 16, 2);
LiquidCrystal_I2C lcd2(0x3F, 16, 2);

int aan = 0;                  // nummer van de pixel op de ring die aan gaat
int snelheid = 100;            // delay tussen pixels veranderen laag is snel en hoog is langzaam veranderen.
int kleur = 0;   // selectie voor R G of B kleur

int R = 0;              // waarden voor rood 
int G = 0;              // waarden voor groen
int B = 0;              // waarden voor blauw

bool ingedrukt = false;     // of de knop van de encoder een keer is ingedrukt
bool resetKlik = false;          // status geklikt van encoder knop
bool 
void setup() 
{
  pixels.begin();                       // pixelring activeren
  pixels.setBrightness(100);            // brightness van de pixels
  pixels.show();                        // alle pixels uit
  Serial.begin(9600);                   // star serial communicatie op 9600hz
  pinMode (resetKnop, INPUT_PULLUP);    // de pin voor de resetknop instellen met een pullup (geen weerstand nodig)
  lcd1.backlight();                     // lcd licht aan doen
  lcd2.backlight();                     
  lcd1.init();                          // lcd aandoen
  lcd2.init();
  lcd1.clear();                         // lcd leeg maken
  lcd2.clear();
}

void loop() 
{
  resetKnopCheck();
  
  while (resetKlik == true)                                                                // controleren of de reset knop is ingedrukt, zo ja dan begint het spel (opnieuw)
  {
    randomRGB();
    ledsClear();
    ledsTonen();
    delay(1000);
    resetKlik = false;
    //timer starten
    if (metingknopKlik == true)
    {
      // kleur meting
      // als de kleur klop timer stoppen
      // tijd onthouden
      // nieuwe kleur
      // timer starten
      // kleur 
    }
  }
}


void resetKnopCheck()
{
  ingedrukt =! digitalRead(resetKnop);
  if (ingedrukt == true && resetKlik == false)
  {
    ingedrukt = false;
    resetKlik = true;
  }
  if (ingedrukt == true && resetKlik == true)
  {
    ingedrukt = false;
    resetKlik = false;
  }
}

void randomRGB()
{
R = 0;
G = 0;
B = 0;
kleur = random(0, 3);  
switch (kleur) 
  {
  case 0:
  R = 255;
  break;
  case 1:
  G = 255;
  break;
  case 2:
  B = 255;
  break;
  }
}

void ledsClear()
{
  for (aan = 0; aan <= NUMPIXELS; aan ++)
  {
    pixels.setPixelColor(aan, 0, 0, 0);
    pixels.show();
    delay(30);
  }
}

void ledsTonen()
{
  for (aan = 0; aan <= NUMPIXELS; aan ++)
  {
    pixels.setPixelColor(aan, R, G, B);
    pixels.show();
    delay(snelheid);
  }
}

