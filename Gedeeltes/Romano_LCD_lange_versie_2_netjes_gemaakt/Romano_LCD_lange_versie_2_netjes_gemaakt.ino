#include <LiquidCrystal_I2C.h>
// om te kunnen tesen
#define Button 3               // een knop zit aan pin 3 

LiquidCrystal_I2C lcd1(0x3E, 16,2 );  // Set the lcd1 I2C address
LiquidCrystal_I2C lcd2(0x3E, 16,2 );  // Set the lcd2 I2C address


int teamGewonnen = 0;           // het nummer van het team dat heeft gewonnen. wordt gebruikt om te kijken bij wie de score wordt opgeteld
int scoreTeam1 = 0;
int scoreTeam2 = 0;
String scoreTeam1LCD[] = {"O","X","XX","XXX","XXXX","XXXXX","XXXXXX","XXXXXXX","XXXXXXXX"};
String scoreTeam2LCD[] = {"O","X","XX","XXX","XXXX","XXXXX","XXXXXX","XXXXXXX","XXXXXXXX"};

// om te kunnen tesen
bool ingedrukt = false;     // of de knop van de encoder een keer is ingedrukt
bool klik = false;          // status geklikt van encoder knop

void setup() 
{
  pinMode (Button, INPUT_PULLUP);
  Serial.begin(9600);
  lcd1.backlight();                     // lcd1 licht aan doen
  lcd2.backlight();                     //
  lcd1.init();                          // lcd1 aandoen
  lcd2.init();                          //
  lcd1.clear();                         // lcd1 leeg maken
  lcd2.clear();                         //
}

void loop() 
{
  if (teamGewonnen == 1)                // team 1 heeft gewonnen
  {
    lcd1.clear();
    lcd1.setCursor(6,0);
    lcd1.print("Team1");
    lcd1.setCursor(0,1); 
    lcd1.print(scoreTeam1LCD[scoreTeam1]);    // de lcd print een gedeelte van de array met scores omdat we de score van het team elke keer dat ze winnen 1 omhoog doen wordt ook elke keer een volgend gedeelte van de array geprint
    teamGewonnen = 0;
    scoreTeam1++;                       // de score van team 1 optellen
    delay(1000);
    if (scoreTeam1 == 8)                // als de maximale score behaald is reset het spel en wordt de kleur van het winnende team knipperend en ronddraaiend op de pixel ring getoond
    {
      //iemandGewonnen = true;
      scoreTeam1 = 0;                                 // score resetten
      scoreTeam2 = 0;                                 // score resetten
      // nog ff code voor pixel ring toevoegen
      Serial.println("SPEL OVER!!!!!!!!!!!!!!!");
      return;                                         // terug naar het begin van de code
    }
  }
  if (teamGewonnen == 2)                // team 2 heeft gewonnen
  {
    lcd2.clear();
    lcd2.setCursor(6,0);
    lcd2.print("Team2");
    lcd2.setCursor(0,1); 
    lcd2.print(scoreTeam1LCD[scoreTeam1]);    // de lcd print een gedeelte van de array met scores omdat we de score van het team elke keer dat ze winnen 1 omhoog doen wordt ook elke keer een volgend gedeelte van de array geprint
    teamGewonnen = 0;
    scoreTeam1++;                       // de score van team 1 optellen
    delay(1000);
    if (scoreTeam1 == 8)                // als de maximale score behaald is reset het spel en wordt de kleur van het winnende team knipperend en ronddraaiend op de pixel ring getoond
    {
      //iemandGewonnen = true;
      scoreTeam1 = 0;                                 // score resetten
      scoreTeam2 = 0;                                 // score resetten
      // nog ff code voor pixel ring toevoegen
      Serial.println("SPEL OVER!!!!!!!!!!!!!!!");
      return;                                         // terug naar het begin van de code
    }
  }
}

// om te kunnen tesen

void knopCheck()
{
  ingedrukt =! digitalRead(Button);
  if (ingedrukt == true && klik == false)
  {
    ingedrukt = false;
    klik = true;
  }
  if (ingedrukt == true && klik == true)
  {
    ingedrukt = false;
    klik = false;
  }
}

