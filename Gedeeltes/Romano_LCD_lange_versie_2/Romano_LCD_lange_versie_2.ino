#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3E, 16,2 );  // Set the LCD I2C address
LiquidCrystal_I2C lcd2(0x3E, 16,2 );  // Set the LCD I2C address

bool klik1 = true;
bool klik2 = false;
int scoreTeam1 = 0;
int scoreTeam2 = 0;
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(klik1 == true){
    scoreTeam1++;
    lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.clear();
      lcd.setCursor(0,1); 
      if(scoreTeam1 == 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      lcd.print("x");
      }else if(scoreTeam1 == 2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      lcd.print("xx");
      }else if(scoreTeam1 == 3){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      lcd.print("xxx");
      }else if(scoreTeam1 == 4){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      lcd.print("xxxx");
      }else if(scoreTeam1 == 5){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      lcd.print("xxxxx");
      }else if(scoreTeam1 == 6){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      lcd.print("xxxxxx");
      }else if(scoreTeam1 == 7){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      lcd.print("xxxxxxx");
      }else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team1 Wint!");
      lcd.setCursor(0,1);
      lcd.print("xxxxxxxx");
  } 
      delay(690);
    if(klik2 == true){
    scoreTeam2++;
      lcd.setCursor(0,0);
      lcd.print("Team1");
      lcd.setCursor(0,1);
      delay(1000);  
      if(scoreTeam2 == 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2");
      lcd.setCursor(0,1);
      lcd.print("x");
      }else if(scoreTeam2 == 2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2");
      lcd.setCursor(0,1);
      lcd.print("xx");
      }else if(scoreTeam2 == 3){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2");
      lcd.setCursor(0,1);
      lcd.print("xxx");
      }else if(scoreTeam2 == 4){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2");
      lcd.setCursor(0,1);
      lcd.print("xxxx");
      }else if(scoreTeam2 == 5){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2");
      lcd.setCursor(0,1);
      lcd.print("xxxxx");
      }else if(scoreTeam2 == 6){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2");
      lcd.setCursor(0,1);
      lcd.print("xxxxxx");
      }else if(scoreTeam2 == 7){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2");
      lcd.setCursor(0,1);
      lcd.print("xxxxxxx");
      }else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Team2 Wint!");
      lcd.setCursor(0,1);
      lcd.print("xxxxxxxx");
      } 
      delay(690);
  }
  }
  }
