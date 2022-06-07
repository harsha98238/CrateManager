#include<ArduinoJson.h>
#include <Watchdog.h>
#include <LiquidCrystal.h>



//const int rs = 12, en = 11, d0 = 9, d1 = 8, d2 = 7, d3 = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en,d0, d1, d2, d3, d4, d5, d6, d7);//d0, d1, d2, d3,


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//watchog  
Watchdog watchdog;

//watchdog

///Serial Communication///
String serialMessage = "";

long int color1Cnt;
long int color2Cnt;
long int color3Cnt;
long int FullCopCount;
long int krichiCnt;
  


/*Lcd refresh*/
unsigned long PreviousRefreshTimer = 0;
unsigned long RefreshTimer;
const unsigned long RefreshInterval = 1000;   // Change  every 10sec

/*motor keypad*/
bool MotorFlag = false;
long BinSize =0;


void setup()
{
  Serial.begin(115200);  //// For USB Debugging
  Serial.setTimeout(20);
  ///Serial Communication///
  Serial2.begin(115200);  ///Main to Secoundary Controller
  Serial2.setTimeout(20);
  ///Serial Communication///


  lcd.begin(20, 4);
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("                    ");

//  lcd.setCursor(0,0);
//  lcd.print("TB1=");
//  lcd.setCursor(10,0);
//  lcd.print("TB2=");
//  lcd.setCursor(0,1);
//  lcd.print("RB1=");
//  lcd.setCursor(10,1);
//  lcd.print("RB2=");
//  lcd.setCursor(0,2);
//  lcd.print("TB3=");
//  lcd.setCursor(10,2);
//  lcd.print("BS =");
//  
//  
//
////  delay(10000);
//
//  lcd.setCursor(4,0);
//  lcd.print(color1Cnt);
//  lcd.setCursor(14,0);
//  lcd.print(color2Cnt);
//  lcd.setCursor(4,2);
//  lcd.print(color3Cnt);
//  lcd.setCursor(14,1);
//  lcd.print(krichiCnt);
//  lcd.setCursor(4,1);
//  lcd.print(FullCopCount);

  
 // watchdog reset
  watchdog.enable(Watchdog::TIMEOUT_4S);
  
 // watchdog reset

}

///Serial Communication///
DynamicJsonDocument dataVariable(200);
String line0 ="";
String line1 ="";
String line2 ="";
String line3 ="";

///Serial Communication///
unsigned long delay1 =0;
void loop()
{
///Serial Communication///

  if ( millis() -delay1 > 1000)
  {
    delay1 = millis();
    BinSize +=1;
  }
  if(Serial2.available())
  {
    serialMessage = Serial2.readString();
    Serial.println(serialMessage); 
    DeserializationError error = deserializeJson(dataVariable, serialMessage);
    if (dataVariable["L0"]!= "")
      line0 =  (const char*)dataVariable["L0"];
    if (dataVariable["L1"]!= "")
      line1 =  (const char*)dataVariable["L1"];
    if (dataVariable["L2"]!= "")
      line2 =  (const char*)dataVariable["L2"];
    if (dataVariable["L3"]!= "")
      line3 =  (const char*)dataVariable["L3"];
  } 
///Serial Communication///
RefreshTimer = millis();

if (((RefreshTimer - PreviousRefreshTimer)  >=  RefreshInterval) ) 
 {
  PreviousRefreshTimer = RefreshTimer;
  
//  lcd.begin(20, 4);
//  delay(100);
//  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line0);
  lcd.setCursor(0,1);
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(line3);
 }

 // watchdog reset
 watchdog.reset();
 // watchdog reset
//

}
