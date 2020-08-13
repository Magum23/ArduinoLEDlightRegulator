#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//buttons
#define btn1 2

//rotary encoders
#define encA 3
#define encB 4

//leds
#define led1 5  //warm white
#define led2 6  //cold white

//constants
#define longPress 350 //minimum time needed for button - long press(millisecond )
#define menu_size 1

//display
#define lcdAddr 0x27
LiquidCrystal_I2C lcd(lcdAddr, 16, 2);

//variables
int btn1State;
int counter = 0;
int lastPosA;
int lastPosB;
int currPosA;
int currPosB;
bool menu_default = 1;
bool menu1 = 0;
bool menu2 = 0;
int cold_white = 255; //menu1
int warm_white = 255; //menu2

void setup() {
  pinMode(btn1, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(encA, INPUT);
  pinMode(encB, INPUT);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  analogWrite(led1,warm_white);
  analogWrite(led2,cold_white);
}

void loop() {
  //rotary encoder
  
  currPosA = digitalRead(encA);
  currPosB = digitalRead(encB);
  if ((currPosA != lastPosA)){
    if ((currPosA == 0) and (currPosB == 0)){
      if (menu_default == 1){
        counter--;
        if (counter < 0){
          counter = menu_size;
          }
        Serial.println(counter);
      }
     else if (menu1 == 1){
      warm_white -=15;
      if (warm_white < 0){
        warm_white = 0;
        }
      lcd.setCursor(0,1);
      lcd.print(warm_white);
      analogWrite(led1,warm_white);
      }
     else if (menu2 == 1){
      cold_white -=15;
      if (cold_white < 0){
        cold_white = 0;
        }
      lcd.setCursor(0,1);
      lcd.print(cold_white);
      analogWrite(led2,cold_white);
      }
    }
    if ((currPosA == 1) and (currPosB == 0)){
      if (menu_default == 1){
        counter++;
        if (counter > menu_size){
          counter = 0;
          }
        Serial.println(counter);
      }
      else if (menu1 == 1){
      warm_white +=15;
      if (warm_white > 255){
        warm_white = 255;
        }
      lcd.setCursor(0,1);
      lcd.print(warm_white);
      analogWrite(led1,warm_white);
      }
     else if (menu2 == 1){
      cold_white +=15;
      if (cold_white > 255){
        cold_white = 255;
        }
      lcd.setCursor(0,1);
      lcd.print(cold_white);
      analogWrite(led2,cold_white);
      }
    }
  lastPosA = currPosA;
  lastPosB = currPosB;
  }
  switch(counter){
    case 0:
      lcd.setCursor(0,0);
      lcd.print("Warm white");
      if (buttonPress(btn1) == 1){
        if (menu_default == 1){
          menu1 = 1;
          menu_default = 0;
          lcd.setCursor(0,1);
          lcd.print(warm_white);         
          }
        else{
          menu1 = 0;
          menu_default = 1;
          lcd.setCursor(0,1);
          lcd.print("                ");
          }
        }
      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("Cold white");
      if (buttonPress(btn1) == 1){
        if (menu_default == 1){
          menu2 = 1;
          menu_default = 0;
          lcd.setCursor(0,1);
          lcd.print(cold_white);         
          }
        else{
          menu2 = 0;
          menu_default = 1;
          lcd.setCursor(0,1);
          lcd.print("                ");
          }
        }
      break;
      
    }
}
//return 1 for short press, return 2 for long press
//function parameter - input button pin
int buttonPress(int btn){
  int lastMillis = millis();
  int millisState;
  bool btnState = 0;
  bool pressed = 0;
  int val = 0;
  if (digitalRead(btn) == LOW){
    btnState = 1;
    pressed = 1;
    }
  while (btnState == 1){
    btnState = 0;
    if (digitalRead(btn) == LOW){
      btnState = 1;
      }
    }
  millisState = millis() - lastMillis;
  if (pressed == 1){
    if (millisState < longPress) {
      val = 1;
      delay(50);
      }
    else{
      val = 2;
      delay(50);
      }
    }
  return val;
  }
