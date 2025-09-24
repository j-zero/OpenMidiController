#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h> //Click here to get the library: http://librarymanager/All#SparkFun_Qwiic_Alphanumeric_Display by SparkFun
#include "LED.h"
#include "HotButton.h"

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

HT16K33 display;

HotButton BTN_1(PIN_BTN_1, true, LOW);
HotButton BTN_2(PIN_BTN_2); 
HotButton BTN_3(PIN_BTN_3); 
HotButton BTN_4(PIN_BTN_4); 

long EXP_1_CURRENT_VALUE = 0;
long EXP_1_OLD_VALUE = 0;


void setup()
{

  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);

  pinMode(PIN_BTN_1, INPUT_PULLUP);
  pinMode(PIN_BTN_2, INPUT_PULLUP);
  pinMode(PIN_BTN_3, INPUT_PULLUP);
  pinMode(PIN_BTN_4, INPUT_PULLUP);


  Wire.begin(); //Join I2C bus

  Serial.begin(115200);
  while (!Serial);
  

  if (display.begin() == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
  Serial.println("Display acknowledged.");

  display.print("Midi");


}

void loop()
{
  HandleButtons();
  HandleExpressionPedal();
  //Serial.println(digitalRead(PIN_BTN_1));

}

void HandleExpressionPedal()
{
  #define POT_READ_COUNT 10
  #define POT_READ_CHANGE_OFFSET 5

  uint8_t MIDI_EXP_1_VALUE = 0;

  // Expression
  pinMode(PIN_EXP_1_RING, OUTPUT);
  pinMode(PIN_EXP_1_SLEEVE, OUTPUT);
  pinMode(PIN_EXP_1_TIP, INPUT);
  
  digitalWrite(PIN_EXP_1_RING, HIGH);
  digitalWrite(PIN_EXP_1_SLEEVE, LOW);

  EXP_1_CURRENT_VALUE = 0;
  for(int i = 0; i < POT_READ_COUNT; i++)
    EXP_1_CURRENT_VALUE += analogRead(PIN_EXP_1_TIP);
  EXP_1_CURRENT_VALUE /= POT_READ_COUNT;
  

  MIDI_EXP_1_VALUE = map(EXP_1_CURRENT_VALUE, 30, 940, 0, 127);


  if((EXP_1_CURRENT_VALUE < (EXP_1_OLD_VALUE - POT_READ_CHANGE_OFFSET)) || (EXP_1_CURRENT_VALUE >= (EXP_1_OLD_VALUE + POT_READ_CHANGE_OFFSET))){
    EXP_1_OLD_VALUE = EXP_1_CURRENT_VALUE;
    

    Serial.print(MIDI_EXP_1_VALUE);
    display.print(MIDI_EXP_1_VALUE);
    Serial.println("");
  }
  

  //delay(100);
}

void HandleButtons()
{
  BTN_1.update();
  BTN_2.update();
  BTN_3.update();
  BTN_4.update();

  if (BTN_1.event(SHORT)){
    Serial.println("BTN_1 Short!");
  }

  if (BTN_1.event(LONG)){
    Serial.println("BTN_1 Long!");
  }

  if (BTN_2.event(SHORT)){
    Serial.println("BTN_2 Short!");
  }

  if (BTN_2.event(LONG)){
    Serial.println("BTN_2 Long!");
  }

  if (BTN_3.event(SHORT)){
    Serial.println("BTN_3 Short!");
  }

  if (BTN_3.event(LONG)){
    Serial.println("BTN_3 Long!");
  }

  if (BTN_4.event(SHORT)){
    Serial.println("BTN_4 Short!");
  }

  if (BTN_4.event(LONG)){
    Serial.println("BTN_4 Long!");
  }

}



