#include<ArduinoJson.h>
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {41, 42, 43, 44}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {45, 46, 47, 48}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String line0 ="";
String line1 = "";
String line2 ="";
String line3 ="";   
char buf[21] ;
char customKey;
long timeVar =0;
void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);
 
  }

int color1Cnt = 0;
int color2Cnt = 5;
int color3Cnt = 10;
int FullCopCount = 14;
int krichiCnt = 32;
int BinSize = 0;


void loop() {
  customKey = customKeypad.getKey();
  

  if(millis()-timeVar >1000 or customKey)
  {
    timeVar = millis();
    displayCount();
  }
  delay(1);
}


void displayCount()
{
  char tempBuffer[21];
  sprintf(tempBuffer,"TB1=%-5dTB2=%-5d",color1Cnt,color2Cnt);
  String L1Count = tempBuffer;
  sprintf(tempBuffer,"RB1=%-5dRB2=%-5d",FullCopCount,krichiCnt);
  String L2Count = tempBuffer;
  sprintf(tempBuffer,"TB3=%-5dBS=%-5d",color3Cnt,BinSize);
  String L3Count = tempBuffer;
  sprintf(tempBuffer,"%-20c",customKey);
  String L4Count = tempBuffer;
  displayMenu(L1Count,L2Count,L3Count,L4Count);
  BinSize +=1;
}

void displayMenu(String line0,String line1,String line2,String line3){
   DynamicJsonDocument doc(200);
   doc["L0"] = line0;
   doc["L1"] = line1;
   doc["L2"] = line2;
   doc["L3"] = line3;
   String message ;
   serializeJsonPretty(doc, message);
   Serial1.println(message);
   Serial.println(message);
 
}
