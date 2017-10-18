/*
versie 2.0
- Code toegevoegd om de kleuren meter te laten werken
- code om de gemeten kleur en de voorgestelde kleur te vergelijken
- namen van kleuren meter code veranderd
- comments toegevoegd aan kleuren meter code

versie 1.1
- overal comments toegevoegd
- benodigde setup code voor 2 lcd schermen toegevoegd
- benodigde setup code toegevoegd voor 2 knoppen (reset en kleurscan activeren)
- timer toegevoegd
- framework voor punten optellingen toegevoegd, de lcd code kan worden toegevoegd
- tijd meting gemaakt voor team 1 en 2
- code  loopt tot dat de status wordt geupdate of er een team heeft gewonnen, dan kan het spel opnieuw beginnen
- score vergelijking toegevoegd

versie 1.0 
- random RGB waarde generator toegevoegd
- pseudocode gemaakt
- functies voor ledring aan doen en uitdoen
- functie gemaakt die checkt of de resetknop is ingedrukt
- reset functie gemaakt voor de code
- basis frame gemaakt voor de code




pseudocode zonder timer
// als een van de knoppen wordt ingedrukt start de game
// de ledstrip geeft een kleur R G of B
// wacht op een knop die ingedrukt wordt, team 1 of 2. 
// zodra de knop is ingedrukt gaat de kleuren scanner scannen.
// als de kleur van de ledbalk hetzelfde is als de kleur die de kleurensensor geeft timer stoppen en score team 1 optellen en laten zien op lcd 1
// zelfde geldt voor team 2 aleen zij klikken op de knop voor hun team en hun timer gaat lopen en de score wordt bij dat team opgeteld
// het eerste team die tot bijv 6 punten heeft wint, pixel ring kleurt regenboog
// wacht tot de reset knop wordt ingedrukt programma begint opnieuw op klik

pseudocode met timer
// V klik op een knop om te starten
// V led wordt een kleur
// X timer gaat lopen team 1 gaat zoeken 
// X team 1 scant object als de kleur klopt stopt de timer
// X timer gaat lopen voor team 2, zij gaan zoeken
// X team 2 scant object als de kleur klopt stopt de timer
// X tijden worden vergeleken, snelste tijd wint, score wordt 1 opgeteld op de display van dat team.
// X begint opnieuw
// X bij score 16 vav n een team gaat ledje regenboog branden en reset het spel
*/


//#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS       12   // aantal pixels op de ring
#define ledPin          2    // pin dat de pixelring aan zit
#define resetKnop       8    // de pin van de reset knop
#define scanKnop        9    // de pin van de knop om de kleur meting te starten
#define S0              4
#define S1              5
#define S2              6
#define S3              7
#define sensorOut 8

// pixel ring instellen
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);
// 2 lcd schermen instellen
//LiquidCrystal_I2C lcd1(0x3E, 16, 2);    // scherm met jumper gesoldeerd
//LiquidCrystal_I2C lcd2(0x3F, 16, 2);    // scherm standaard

int aan = 0;                    // nummer van de pixel op de ring die aan gaat, wordt door de code ingesteld
int snelheid = 100;             // delay tussen pixels veranderen laag is snel en hoog is langzaam veranderen.
int kleur = 0;                  // selectie voor R G of B kleur
int R = 0;                      // waarden voor rood 
int G = 0;                      // waarden voor groen
int B = 0;                      // waarden voor blauw
int iemandGewonnen = false;     // zodra iemand heeft gewonnen wordt deze true, de code begint dan opnieuw


bool resetIngedrukt = false;          // deze twee variabelen worden gebruikt om bij te houden of er een klik is geweest bij een knop
bool resetKlik = false;               // 
bool scanIngedrukt = false;           // deze twee variabelen worden gebruikt om bij te houden of er een klik is geweest bij een knop
bool scanKlik = false;                // 
bool team1Geweest = false;            // houd bij welk team er aan de beurt is
bool team2Geweest = false;            // houd bij of de scores al kunne worden vergeleken

unsigned long tijdBegin;              // gebruikt om de tijd bij te houden
unsigned long tijdEind;               // gebruikt om de tijd bij te houden
unsigned long tijdTeam1 = 0;                    // de tijd voor het 1e team
unsigned long tijdTeam2 = 0;                    // de tijd voor het 2e team

void setup() 
{
  Serial.println("setup");
  pixels.begin();                       // pixelring activeren
  pixels.setBrightness(100);            // brightness van de pixels
  pixels.show();                        // alle pixels uit
  Serial.begin(9600);                   // star serial communicatie op 9600hz
  pinMode (resetKnop, INPUT_PULLUP);    // de pin voor de resetknop instellen met een pullup (geen weerstand nodig)
  pinMode (scanKnop, INPUT_PULLUP);     // de pin voor de knop die het kleur scannen activeert instellen met een pullup (geen weerstand nodig)
  //lcd1.backlight();                     // lcd licht aan doen
  //lcd2.backlight();                     //
  //lcd1.init();                          // lcd aandoen
  //lcd2.init();                          //
  //lcd1.clear();                         // lcd leeg maken
  //lcd2.clear();                         //
}

void loop() 
{
  resetKnopCheck();                                   // check of de reset knop een keer is ingedrukt
  
  while (resetKlik == true)                           // controleren of de reset knop is ingedrukt, zo ja dan begint het spel (opnieuw)
  {
    while (iemandGewonnen == false)
    {
      resetKlik = false;                                // klik false maken zodat je aan het einde van het spelletje weer het spel kan resetten
      randomRGB();                                      // random kleur R, G of B ophalen
      ledsClear();                                      // pixelring leeg halen
      ledsTonen();                                      // pixelring updaten met de kleur
      delay(1000);                                       
      timerStart();                                     // start timer
      while (scanKlik == false)
      {
        scanKnopCheck();                                // check of de scanknop is ingedrukt. Je wacht hier tot een team een object heeft gevonden
        Serial.println("Wachten op scannen");
      }
      while (scanKlik == true && team1Geweest == false)        // team 1 aan de beurt
      {
        scanKnopCheck();
        Serial.println("aan het scannen... team 1");
        delay(1000);
        // gedeelte vervangen met check op de kleur van de kleuren scanner hetzelfde is als de rgb van de pixel ring
        Serial.println("de kleur klopt! ");
        //
        delay(1000);
        timerStopTeam1();
        team1Geweest = true;
      }
      while (scanKlik == true && team1Geweest == true)         // team 2 aan de beurt
      {
        scanKnopCheck();
        Serial.println("aan het scannen... team 2");
        delay(1000);
        // gedeelte vervangen met check op de kleur van de kleuren scanner hetzelfde is als de rgb van de pixel ring
        Serial.println("de kleur klopt! ");
        //
        delay(1000);
        timerStopTeam2();
        team1Geweest = false;
        team2Geweest = true;
      }
      if (team2Geweest == true)
      {
        scoreVergelijken();                                      // punten calculeren en optellen
        team2Geweest = false;
      }
    }
  }
}

// code om ter kijken of de reset knop een keer is ingedrukt
void scanKnopCheck()
{
  scanIngedrukt =! digitalRead(scanKnop);
  if (scanIngedrukt == true && scanKlik == false)
  {
    scanIngedrukt = false;
    scanKlik = true;
  }
  if (scanIngedrukt == true && scanKlik == true)
  {
    scanIngedrukt = false;
    scanKlik = false;
  }
}

// code om ter kijken of de kleur scan knop een keer is ingedrukt
void resetKnopCheck()
{
  resetIngedrukt =! digitalRead(resetKnop);               // waarde van de pin lezen
  if (resetIngedrukt == true && resetKlik == false)       // 
  {
    resetIngedrukt = false;
    resetKlik = true;
  }
  if (resetIngedrukt == true && resetKlik == true)
  {
    resetIngedrukt = false;
    resetKlik = false;
  }
}

// hier komt random de kleur rood, groen of blauw uit.
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

// ledjes van de ring uitdoen in een ronddraai patroon
void ledsClear()
{
  for (aan = 0; aan <= NUMPIXELS; aan ++)
  {
    pixels.setPixelColor(aan, 0, 0, 0);
    pixels.show();
    delay(30);                                  // de delay voor het updaten van het volgende ledje op de ring
  }
}

// ledjes van de ring aandoen in een ronddraai patroon
void ledsTonen()
{
  for (aan = 0; aan <= NUMPIXELS; aan ++)
  {
    pixels.setPixelColor(aan, R, G, B);
    pixels.show();
    delay(snelheid);                            // de delay voor het updaten van het volgende ledje op de ring
  }
}

// de timer starten
void timerStart()
{
  tijdBegin = 0;
  tijdBegin = micros();
  Serial.print("Start de tijd! ");
}

// de timer stoppen en de tijd meegeven voor team 1
void timerStopTeam1()
{
  tijdEind = micros();                    // huidige tijd ophalen
  tijdTeam1 = tijdEind - tijdBegin;       // tijd meten van deze ronde
  tijdTeam1 = tijdTeam1 / 1000;         // naar seconden omzetten
  Serial.println("je tijd is: ");
  Serial.print(tijdTeam1);
}

// de timer stoppen en de tijd meegeven voor team 2
void timerStopTeam2()
{
  tijdEind = micros();                    // huidige tijd ophalen
  tijdTeam2 = tijdEind - tijdBegin;       // tijd meten van deze ronde
  tijdTeam2 = tijdTeam2 / 1000;         // naar seconden omzetten
  Serial.println("je tijd is: ");
  Serial.println(tijdTeam2);
}

// scores vergelijken team 1 en 2 daarna bij het snelste team de punten optellen
void scoreVergelijken()
{
  if (tijdTeam1 < tijdTeam2)
  {
    Serial.println("Team 1 wint!");
    // score optellen team 1 code romano
    // pixelring feest in kleur team 1
    // zodra iemand 16 punten heeft wordt variabele iemandGewonnen op true gezet
  }
  if (tijdTeam1 > tijdTeam2)
  {
    Serial.println("Team 2 wint!");
    // score optellen team 2 code romano
    // pixelring feest in kleur team 2
    // zodra iemand 16 punten heeft wordt variabele iemandGewonnen op true gezet
  }
}





