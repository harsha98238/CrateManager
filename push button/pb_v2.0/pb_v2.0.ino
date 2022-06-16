#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//String menuOption[] = {{"Count Page   "}, {"Bin size"}, {"Bobbin Mix   "}, {"Sensor option   "}};

String line1[] = {{"TB1="},{""},{"BOBBIN MIX OPTION"},{"SENSOR OPTION"}};
String line2[] = {{"RB1="},{"BIN SIZE="},{""},{""}};
String line3[] = {{"TB3="},{""},{""},{""}};

String Coloumn1[] = {{"TB2="},{""},{""},{""}};
String Coloumn2[] = {{"RB2="},{""},{""},{""}};
String Coloumn3[] = {{"BS ="},{""},{""},{""}};



const int leftButtonPin = 6;   
const int rightButtonPin = 7;  
const int upButtonPin = 8;     
const int downButtonPin = 9;   
const int enterButtonPin = 10; 

int setting1Counter = 0;
int setting2Counter = 0;
int setting3Counter = 0;
int setting4Counter = 0;

int directionPush = 0;
int upPressCount = 0; 
int downPressCount = 0;

boolean buttonStateLeft = HIGH;            // Button states for the "Left" command
boolean lastButtonStateLeft = HIGH;                
boolean currentButtonStateLeft = HIGH;
boolean buttonStateRight = HIGH;           // Button states for the "Right" command
boolean lastButtonStateRight = HIGH;                
boolean currentButtonStateRight = HIGH;
boolean buttonStateUp = HIGH;              // Button states for the "Up" command
boolean lastButtonStateUp = HIGH;                
boolean currentButtonStateUp = HIGH;
boolean buttonStateDown = HIGH;            // Button states for the "Down" command
boolean lastButtonStateDown = HIGH;                
boolean currentButtonStateDown = HIGH; 
boolean buttonStateEnter = HIGH;           // Button states for the "Enter" command
boolean lastButtonStateEnter = HIGH;                
boolean currentButtonStateEnter = HIGH;

unsigned long lastDebounceTime = 0;       // This variable takes a "snapshot" of time when any button is pushed.
unsigned long debounceDelay = 50;         // Delay time in milliseconds; the amount of time the button is pressed must be higher than the delay to register a push.

bool serialPrintFlag = HIGH;
void setup()
{
  Serial.begin(115200);
  lcd.begin(20, 4);
  pinMode(leftButtonPin, INPUT_PULLUP);    // SETS THE leftButtonPin AS AN INPUT
  pinMode(rightButtonPin, INPUT_PULLUP);   // SETS THE rightButtonPin AS AN INPUT
  pinMode(upButtonPin, INPUT_PULLUP);      // SETS THE upButtonPin AS AN INPUT
  pinMode(downButtonPin, INPUT_PULLUP);    // SETS THE downButtonPin AS AN INPUT
  pinMode(enterButtonPin, INPUT_PULLUP);   // SETS THE enterButtonPin AS AN INPUT
 
}


void loop()
{

  if(serialPrintFlag == HIGH)
  {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1[directionPush]);
  lcd.setCursor(0, 1);
  lcd.print(line2[directionPush]);
  lcd.setCursor(0, 2);
  lcd.print(line3[directionPush]);
  
  lcd.setCursor(10, 0);
  lcd.print(Coloumn1[directionPush]);
  lcd.setCursor(10, 1);
  lcd.print(Coloumn2[directionPush]);
  lcd.setCursor(10, 2);
  lcd.print(Coloumn3[directionPush]);
    
  serialPrintFlag = LOW;
  }

  currentButtonStateLeft = digitalRead(leftButtonPin);          
  currentButtonStateRight = digitalRead(rightButtonPin); 
  currentButtonStateUp = digitalRead(upButtonPin);          
  currentButtonStateDown = digitalRead(downButtonPin);    
  currentButtonStateEnter = digitalRead(enterButtonPin);

  if (currentButtonStateLeft != lastButtonStateLeft || currentButtonStateRight != lastButtonStateRight || 
  currentButtonStateUp != lastButtonStateUp || currentButtonStateDown != lastButtonStateDown)        
  {
  lastDebounceTime = millis();      // lastDebounceTime is set equal to the running millis() function.
  }  
  
  if ((millis() - lastDebounceTime) > debounceDelay)   
  {                                             
    // LEFT BUTTON PRESS
    if (currentButtonStateLeft != buttonStateLeft)       // Left button scrolls the menu options to the left.  
    { 
    buttonStateLeft = currentButtonStateLeft;  
                 
      if (buttonStateLeft == HIGH)                        // Once the button is released, the push is registered and the code below runs.
      {                                                 
      directionPush--;                                   // Both the up and down press counts will be reset to zero when the left button is pushed.
      serialPrintFlag = HIGH;
      upPressCount = 0;
      downPressCount = 0;
      }
          
        if (directionPush < 0)                           // If the user tries to scroll below the first menu option,
        {                                                // the program will loop back to the last menu option.
        directionPush = 3;
        }
    }

   // RIGHT BUTTON PRESS
    if (currentButtonStateRight != buttonStateRight)     // Right button scrolls the menu options to the right.    
    { 
    buttonStateRight = currentButtonStateRight;  
                 
      if (buttonStateRight == LOW)                         
      {                                                 
      directionPush++;// Both the up and down press counts will be reset to zero when the right button is pushed.   
      serialPrintFlag = HIGH;
      upPressCount = 0;
      downPressCount = 0;
      }
  
        if (directionPush > 3)                           // If the user tries to scroll above the last menu option,
        {                                                // the program will loop back to the first menu option.
        directionPush = 0;
        }
    }

  }

  // After a button is pushed and the count recorded, all the states reset back to LOW for the data to be processed correctly.
  lastButtonStateLeft = currentButtonStateLeft;       // resets the left button state to LOW
  lastButtonStateRight = currentButtonStateRight;     // resets the right button state to LOW
  lastButtonStateUp = currentButtonStateUp;           // resets the up button state to LOW
  lastButtonStateDown = currentButtonStateDown;       // resets the down button state to LOW
  lastButtonStateEnter = currentButtonStateEnter;     // resets the enter button state to LOW
}
