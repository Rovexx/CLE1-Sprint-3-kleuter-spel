/*

Spelletje voor kleuters, vind de kleur

Klik op een knop om te starten.
Ledring wordt een kleur rood, groen of blauw.
Timer gaat lopen team 1 gaat zoeken naar een object die dezelfde kleur heeft als de kleur die de led ring aangeeft.
team 1 scant object als de kleur klopt stopt de timer en wordt de tijd opgeslagen.
Er wordt weer een random kleur getoond.
Timer gaat lopen voor team 2, zij gaan zoeken
Team 2 scant object als de kleur klopt stopt de timer en wordt de tijd opgeslagen.
Tijden worden vergeleken, snelste tijd wint, score wordt 1 opgeteld op de display van dat team.
de volgende rond begint het proces herhaalt zich.
Bij score 8 van een team gaat ledje regenboog branden en reset het spel.

versie 3.1
- beschrijving toegevoegd
- oude niet gebruikte tekst weg gehaald
- lijst gemaakt met aansluitingen in het circuit
- regenboog effect toegevoegd als een team wint
- lcd code afgemaakt
- lcd code netjes neergezet en opgedeeld in functies

versie 3.0
- de lcd code van Romano is toegevoegd
- als team 1 wint gaat hun score omhoog, zo ook voor team 2
- als een team de socre van 8 behaald heeft 

versie 2.1
- start marker verplaats hopelijk fixt dit de bug dat het programma maar 1 keer runt
- lcd basics van code toegevoegd (library en setup)

versie 2.0
- Code toegevoegd om de kleuren meter te laten werken
- code om de gemeten kleur en de voorgestelde kleur te vergelijken
- namen van kleuren meter code veranderd
- comments toegevoegd aan kleuren meter code
- er wordt een winnaar bepaald

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



pseudocode met timer (V is gedaan X is moet nog gemaakt worden)
V klik op een knop om te starten
V led wordt een kleur
V timer gaat lopen team 1 gaat zoeken 
V team 1 scant object als de kleur klopt stopt de timer
V timer gaat lopen voor team 2, zij gaan zoeken
V team 2 scant object als de kleur klopt stopt de timer
V tijden worden vergeleken, snelste tijd wint, score wordt 1 opgeteld op de display van dat team.
V begint opnieuw
V bij score 8 van een team gaat ledje regenboog branden en reset het spel
*/


#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS       12    // aantal pixels op de ring
#define ledPin          2     // pin dat de pixelring aan zit
#define resetKnop       12    // de pin van de reset knop
#define scanKnop        13    // de pin van de knop om de kleur meting te starten
#define S0              4     // S0 zit op pin 4
#define S1              5     // S1 zit op pin 5
#define S2              6     // S2 zit op pin 6
#define S3              7     // S3 zit op pin 7
#define sensorOut       8     // sensorOut zit op pin 8

// pixel ring instellen
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);
// 2 lcd schermen instellen
LiquidCrystal_I2C lcd1(0x3E, 16, 2);    // scherm met jumper gesoldeerd
LiquidCrystal_I2C lcd2(0x3F, 16, 2);    // scherm standaard

int aan = 0;                    // nummer van de pixel op de ring die aan gaat, wordt door de code ingesteld
int snelheid = 100;             // delay tussen pixels veranderen laag is snel en hoog is langzaam veranderen.
int kleur = 0;                  // selectie voor R G of B kleur
int R = 0;                      // waarden voor rood 
int G = 0;                      // waarden voor groen
int B = 0;                      // waarden voor blauw
int Red = 0;                    // waarden voor rood gebruikt door kleuren sensor
int Green = 0;                  // waarden voor groen gebruikt door kleuren sensor
int Blue = 0;                   // waarden voor blauw gebruikt door kleuren sensor
int frequency = 0;
int teamGewonnen = 0;           // het nummer van het team dat heeft gewonnen. wordt gebruikt om te kijken bij wie de score wordt opgeteld
int scoreTeam1 = 0;             // team 1 score
int scoreTeam2 = 0;             // team 2 score

String scoreTeamLCD[] = {"O","X","XX","XXX","XXXX","XXXXX","XXXXXX","XXXXXXX","XXXXXXXX"};   // array met aantal punten

bool iemandGewonnen = false;          // zodra iemand heeft gewonnen wordt deze true, de code begint dan opnieuw
bool resetIngedrukt = false;          // deze twee variabelen worden gebruikt om bij te houden of er een klik is geweest bij een knop
bool resetKlik = false;               // voor de start van het programma wordt dit gebruikt om te kijken wanneer het spel moet beginnen
bool scanIngedrukt = false;           // deze twee variabelen worden gebruikt om bij te houden of er een klik is geweest bij een knop
bool scanKlik = false;                // aan/uit waarde voor de knop die kleur scannen activeerd
bool team1Geweest = false;            // houd bij welk team er aan de beurt is
bool team2Geweest = false;            // houd bij of de scores al kunne worden vergeleken
bool kleurJuist = false;              // zodra de juiste kleur is gescant wordt deze true en dan gaat de code verder

unsigned long tijdBegin;              // gebruikt om de tijd bij te houden
unsigned long tijdEind;               // gebruikt om de tijd bij te houden
unsigned long tijdTeam1 = 0;          // de tijd voor het 1e team
unsigned long tijdTeam2 = 0;          // de tijd voor het 2e team

void setup() 
{
  Serial.println("setup");
  pixels.begin();                       // pixelring activeren
  pixels.setBrightness(100);            // brightness van de pixels
  pixels.show();                        // alle pixels uit
  Serial.begin(9600);                   // star serial communicatie op 9600hz
  pinMode (resetKnop, INPUT_PULLUP);    // de pin voor de resetknop instellen met een pullup (geen weerstand nodig)
  pinMode (scanKnop, INPUT_PULLUP);     // de pin voor de knop die het kleur scannen activeert instellen met een pullup (geen weerstand nodig)
  pinMode(S0, OUTPUT);                  // pin mode goed zetten voor de kleuren sensor
  pinMode(S1, OUTPUT);                  //
  pinMode(S2, OUTPUT);                  //
  pinMode(S3, OUTPUT);                  //
  pinMode(sensorOut, INPUT);            //          
  digitalWrite(S0,HIGH);                // frequentie schalen naar 100% zetten
  digitalWrite(S1,HIGH);                //
  lcd1.backlight();                     // lcd licht aan doen
  lcd2.backlight();                     //
  lcd1.init();                          // lcd aandoen
  lcd2.init();                          //
  lcd1.clear();                         // lcd leeg maken
  lcd2.clear();                         //
}

void loop() 
{
  Serial.println("het spel begint");
  resetKnopCheck();                                   // check of de reset knop een keer is ingedrukt
  while (resetKlik == true)                           // controleren of de reset knop is ingedrukt, zo ja dan begint het spel (opnieuw)
  {
    iemandGewonnen = false;
    while (iemandGewonnen == false)
    {
      resetKlik = false;                                // klik false maken zodat je aan het einde van het spelletje weer het spel kan resetten
      randomRGB();                                      // random kleur R, G of B ophalen
      ledsClear();                                      // pixelring leeg halen
      ledsTonen();                                      // pixelring updaten met de kleur                                       
      timerStart();                                     // start timer
      while (scanKlik == false)                               // gedeelte tijdens het zoeken naar een kleur
      {
        scanKnopCheck();                                // check of de scanknop is ingedrukt. Je wacht hier tot een team een object heeft gevonden
        Serial.println("Wachten op scannen");
      }
      while (scanKlik == true && team1Geweest == false && kleurJuist == false)        // team 1 aan de beurt
      {
        Serial.println("aan het scannen... team 1");
        kleurMeten();                                   // kleuren meten en vergelijken
        kleurVergelijken();
        delay(2000);
        
        if (kleurJuist == true)                               // als de kleur juist is gaat het spel door, team1Geweest zorgt daar voor
        {
          timerStopTeam1();                             // de timer stopt
          team1Geweest = true;                          // team 1 is geweest
          scanKnopCheck();                              // de knop die het kleurenmeten begint checken
          kleurJuist = false;
          scanKlik = false;
        }
        
      }
      randomRGB();                                      // random kleur R, G of B ophalen
      ledsClear();                                      // pixelring leeg halen
      ledsTonen();                                      // pixelring updaten met de kleur                                       
      timerStart();                                     // start timer
      while (scanKlik == true && team1Geweest == true && kleurJuist == false)         // team 2 aan de beurt
      {
        Serial.println("aan het scannen... team 2");
        // kleuren meten en vergelijken
        kleurMeten();
        kleurVergelijken();
        delay(1000);
        //
        if (kleurJuist == true)         // als de kleur juist is mag team 2 gaan, team1Geweest zorgt daar voor
        {
          timerStopTeam2();             // de timer stopt
          team1Geweest = false;
          team2Geweest = true;
          scanKnopCheck();                                        // de knop die het kleurenmeten begint checken
          kleurJuist = false;
          scanKlik = false;
        }
      }
      if (team2Geweest == true)                                                       // de twee teams hebben hun kleuren gescant nu gaat de winnaar bepaald worden
      {
        team2Geweest = false;                           // weer op false zetten zodat dit stukje niet blijft loopen
        delay(3000);
        tijdVergelijken();                              // tijd van beide teams vergelijken en bepalen welk team gewonnen heeft
        scorePrinten();                                 // scores op de juiste lcd printen, als de score van een team 8 is reset de score       
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
  resetIngedrukt =! digitalRead(resetKnop);               // waarde van de pin lezen en omkeren omdat wij een pullup weerstand gebruiken wat de logica van de knop omdraait
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
  Serial.println("R = " + R);
  Serial.println("G = " + G);
  Serial.println("B = " + B);
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
  Serial.println("Start de tijd! ");
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
void tijdVergelijken()
{
  if (tijdTeam1 < tijdTeam2)
  {
    Serial.println("Team 1 wint!");
    teamGewonnen = 1;
  }
  if (tijdTeam1 > tijdTeam2)
  {
    Serial.println("Team 2 wint!");
    teamGewonnen = 2;
  }
}

// scores op de juiste lcd printen, als de score van een team 8 is reset de score
void scorePrinten()
{
  if (teamGewonnen == 1)                                                        // team 1 heeft gewonnen
  {
    lcd1.clear();
    lcd1.setCursor(5,0);
    lcd1.print("Team 1");
    lcd1.setCursor(0,1); 
    lcd1.print(scoreTeamLCD[scoreTeam1]);      // de lcd print een gedeelte van de array met scores omdat we de score van het team elke keer dat ze winnen 1 omhoog doen wordt ook elke keer een volgend gedeelte van de array geprint
    teamGewonnen = 0;
    scoreTeam1++;                               // de score van team 1 optellen
    delay(1000);
    if (scoreTeam1 == 8)                        // als de maximale score behaald is reset het spel en wordt de kleur van het winnende team knipperend en ronddraaiend op de pixel ring getoond
    {
      iemandGewonnen = true;
      scoreTeam1 = 0;                                 // score resetten
      scoreTeam2 = 0;                                 // score resetten
      // code voor pixelring als iemand heeft gewonnen
      regenboogEffect();
      Serial.println("SPEL OVER!!!!!!!!!!!!!!!");
      return;                                         // terug naar het begin van de code
    }
  }
  if (teamGewonnen == 2)                                                      // team 2 heeft gewonnen
  {
    lcd2.clear();
    lcd2.setCursor(5,0);
    lcd2.print("Team 2");
    lcd2.setCursor(0,1); 
    lcd2.print(scoreTeamLCD[scoreTeam1]);    // de lcd print een gedeelte van de array met scores omdat we de score van het team elke keer dat ze winnen 1 omhoog doen wordt ook elke keer een volgend gedeelte van de array geprint
    teamGewonnen = 0;
    scoreTeam1++;                       // de score van team 1 optellen
    delay(1000);
    if (scoreTeam1 == 8)                // als de maximale score behaald is reset het spel en wordt de kleur van het winnende team knipperend en ronddraaiend op de pixel ring getoond
    {
      iemandGewonnen = true;
      scoreTeam1 = 0;                                 // score resetten
      scoreTeam2 = 0;                                 // score resetten
      // code voor pixelring als iemand heeft gewonnen
      regenboogEffect();
      Serial.println("SPEL OVER!!!!!!!!!!!!!!!");
      return;                                         // terug naar het begin van de code
    }
  }
}

// kleur meten
void kleurMeten()
{
  // Rode photodiodes aanzetten om gelezen te worden
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // output frequentie lezen
  Red = pulseIn(sensorOut, LOW);
  // Rode output frequentie printen
  Serial.print("Red= ");
  Serial.print(Red);
  Serial.print("  ");

  // Groene photodiodes aanzetten om gelezen te worden
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // output frequentie lezen
  Green = pulseIn(sensorOut, LOW);
  // Groene output frequentie printen
  Serial.print("Green= ");
  Serial.print(Green);
  Serial.print("  ");

  // Blauwe photodiodes aanzetten om gelezen te worden
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // output frequentie lezen
  Blue = pulseIn(sensorOut, LOW);
  // Blauwe output frequentie printen
  Serial.print("Blue= ");
  Serial.print(Blue);
  Serial.println("  ");
}

// kleuren vergelijken van het object en de sensor
void kleurVergelijken()
{
  if (Red < Green && Red < Blue)       // er is rood gedetecteerd
  {
    Serial.println("RED detected!");
    if (R == 255)
    {
      kleurJuist = true;
      Serial.println(kleurJuist);
    }
    else 
      {
        kleurJuist = false;
      }
  }
  if (Green < Red && Green < Blue)     // er is groen gedetecteerd
  {
    Serial.println("GREEN detected!");
    if (G == 255)
    {
      kleurJuist = true;
      Serial.println(kleurJuist);
    }
    else 
    {
      kleurJuist = false;
    }
  }
  if (Blue < Red && Blue < Green)      // er is blauw gedetecteerd
  {
    Serial.println("BLUE detected!");
    if (B == 255)
    {
      kleurJuist = true;
      Serial.println(kleurJuist);
    }
    else 
    {
      kleurJuist = false;
    }
  }
}

// regenboog effect op pixelring
void regenboogEffect()
{
for (int timer = 0; timer <= 10; timer++)             // 10 keer een kleur van de regenboog tonen
  {
    for (int led = 0; led < NUMPIXELS; led++)
    {
      // gedeelte voor regenboog kleur
      if (B == 0 && R == 255 && G < 255)
      {
        G ++;
      }
      if (B == 0 && G == 255 && R > 0)
      {
        R --;
      }
      if (G == 255 && R == 0 && B < 255)
      {
        B ++;
      }
      if (B == 255 && R == 0 && G > 0)
      {
        G --;
      }
      if (B == 255 && G == 0 && R < 255)
      {
        R ++;
      }
      if (R == 255 && G == 0 && B > 0)
      {
        B --;
      }
      // --------------------------------
      pixels.setPixelColor(led, R, G, B);
      pixels.show();
      delay(20);            // snelheid effect
    }
  }
}

