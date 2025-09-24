#define DEBUG


#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h> //Click here to get the library: http://librarymanager/All#SparkFun_Qwiic_Alphanumeric_Display by SparkFun
#include "LED.h"
#include "HotButton.h"

#define DISP_MAX_BRIGHTNESS 6

#define EXP_MIN_VALUE 73
#define EXP_MAX_VALUE 512

#define PIN_BTN_1 10
#define PIN_BTN_2 16
#define PIN_BTN_3 14
#define PIN_BTN_4 15

#define PIN_EXP_1_TIP A2
#define PIN_EXP_1_RING A3
#define PIN_EXP_1_SLEEVE 9

#define PIN_EXP_2_TIP A0
#define PIN_EXP_2_RING A1
#define PIN_EXP_2_SLEEVE 8



#define POT_READ_COUNT 20
#define POT_READ_CHANGE_OFFSET 2


HT16K33 display;

HotButton BTN_1(PIN_BTN_1, true, LOW);
HotButton BTN_2(PIN_BTN_2, true, LOW);
HotButton BTN_3(PIN_BTN_3, true, LOW);
HotButton BTN_4(PIN_BTN_4, true, LOW);


long EXP_1_CURRENT_VALUE = 0;
long EXP_1_OLD_VALUE = 0;

long EXP_2_CURRENT_VALUE = 0;
long EXP_2_OLD_VALUE = 0;

long EXP_1_MIN_VALUE = 50;
long EXP_1_MAX_VALUE = 800;



void setup()
{

  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);

  pinMode(PIN_BTN_1, INPUT_PULLUP);
  pinMode(PIN_BTN_2, INPUT_PULLUP);
  pinMode(PIN_BTN_3, INPUT_PULLUP);
  pinMode(PIN_BTN_4, INPUT_PULLUP);

  pinMode(PIN_EXP_1_SLEEVE, INPUT_PULLUP);
  

  pinMode(PIN_EXP_2_TIP, INPUT);
  pinMode(PIN_EXP_2_RING, INPUT);
  pinMode(PIN_EXP_2_SLEEVE, INPUT);


  Wire.begin(); //Join I2C bus

  Serial.begin(115200);
  //while (!Serial);
  

  if (display.begin() == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
  Serial.println("Display acknowledged.");
  display.setBrightness(0);
  display.print("Moin");

  for(int i=0; i<=255; i++){
    LEDRed(i);
    display.setBrightness(map(i, 0, 255, 0, DISP_MAX_BRIGHTNESS));
    delay(5);
  }  

}

void loop()
{
  HandleButtons();
  HandleExpressionPedal1();

  if(digitalRead(PIN_EXP_1_SLEEVE)) // Something is connected
    HandleExpressionPedal();


  
  

}

uint8_t HandleExpressionPedal1() {

  if(digitalRead(PIN_EXP_2_SLEEVE)) // Something is connected
    return;

  EXP_2_CURRENT_VALUE = 0;
  for(int i = 0; i < POT_READ_COUNT; i++)
    EXP_2_CURRENT_VALUE += analogRead(PIN_EXP_2_TIP);
  EXP_2_CURRENT_VALUE /= POT_READ_COUNT;
  
  if(EXP_2_CURRENT_VALUE > EXP_MAX_VALUE)
    EXP_2_CURRENT_VALUE = EXP_MAX_VALUE;
  if(EXP_2_CURRENT_VALUE < EXP_MIN_VALUE)
    EXP_2_CURRENT_VALUE = EXP_MIN_VALUE;

  if((EXP_2_CURRENT_VALUE < (EXP_2_OLD_VALUE - POT_READ_CHANGE_OFFSET)) || (EXP_2_CURRENT_VALUE >= (EXP_2_OLD_VALUE + POT_READ_CHANGE_OFFSET))){
    EXP_2_OLD_VALUE = EXP_2_CURRENT_VALUE;
    
      long a0 = map(EXP_2_OLD_VALUE, EXP_MIN_VALUE,EXP_MAX_VALUE,0,1023);
      display.print(a0);
      Serial.println(a0);
  }
}


void HandleExpressionPedal()
{

  uint8_t MIDI_EXP_1_VALUE = 0;

  // Expression
  pinMode(PIN_EXP_1_RING, OUTPUT);
  pinMode(PIN_EXP_1_TIP, INPUT);

  digitalWrite(PIN_EXP_1_RING, HIGH);

  EXP_1_CURRENT_VALUE = 0;
  for(int i = 0; i < POT_READ_COUNT; i++)
    EXP_1_CURRENT_VALUE += analogRead(PIN_EXP_1_TIP);
  EXP_1_CURRENT_VALUE /= POT_READ_COUNT;

  if(EXP_1_CURRENT_VALUE < EXP_1_MIN_VALUE)
    EXP_1_MIN_VALUE = EXP_1_CURRENT_VALUE;

  if(EXP_1_CURRENT_VALUE > EXP_1_MAX_VALUE)
    EXP_1_MAX_VALUE = EXP_1_CURRENT_VALUE;

  MIDI_EXP_1_VALUE = map(EXP_1_CURRENT_VALUE, EXP_1_MIN_VALUE, EXP_1_MAX_VALUE, 0, 128);
  if(MIDI_EXP_1_VALUE > 127) MIDI_EXP_1_VALUE = 127;


  if((EXP_1_CURRENT_VALUE < (EXP_1_OLD_VALUE - POT_READ_CHANGE_OFFSET)) || (EXP_1_CURRENT_VALUE >= (EXP_1_OLD_VALUE + POT_READ_CHANGE_OFFSET))){
    EXP_1_OLD_VALUE = EXP_1_CURRENT_VALUE;
    
#ifdef DEBUG
    Serial.print(EXP_1_CURRENT_VALUE);
    Serial.print(", ");
    Serial.print(analogRead(PIN_EXP_1_RING));
    Serial.println("");
#endif

    display.print(MIDI_EXP_1_VALUE);
  }
  

  //delay(100);
}

void HandleButtons()
{

  #define LONG_PRESS_TIME 500

  BTN_1.update();
  BTN_2.update();
  BTN_3.update();
  BTN_4.update();

  if (BTN_1.event(SHORT)){
    display.print("B1 S");
  }
  if (BTN_1.pressedFor(LONG_PRESS_TIME)){
    display.print("B1 L");
  }
  if (BTN_1.isDoubleClick()){
    display.print("B1 D");
  }

  if (BTN_2.event(SHORT)){
    display.print("B2 S");
  }
  if (BTN_2.pressedFor(LONG_PRESS_TIME)){
    display.print("B2 L");
  }
  if (BTN_2.isDoubleClick()){
    display.print("B2 D");
  }

  if (BTN_3.event(SHORT)){
    display.print("B3 S");
  }
  if (BTN_3.pressedFor(LONG_PRESS_TIME)){
    display.print("B3 L");
  }
  if (BTN_3.isDoubleClick()){
    display.print("B3 D");
  }

  if (BTN_4.event(SHORT)){
    display.print("B4 S");
  }
  if (BTN_4.pressedFor(LONG_PRESS_TIME)){
    display.print("B4 L");
  }
  if (BTN_4.isDoubleClick()){
    display.print("B4 D");
  }


}



