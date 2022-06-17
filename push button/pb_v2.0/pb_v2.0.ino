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


String Data1[] = {{"10 "}, {"120"}, {"140"}, {"160"}, {"180"}, {"200"}};
String Data2[] = {{"ON "}, {"OFF"}};
String Data3[] = {{"SINGLE COLOUR MODE"}, {"MULTI COLOUR MODE "}};



const int leftButtonPin = 6;   
const int rightButtonPin = 7;  
const int upButtonPin = 8;     
const int downButtonPin = 9;   
const int enterButtonPin = 10; 



int directionPush = 0;
int directionUpDown1Push = 0; 
int directionUpDown2Push = 0; 
int directionUpDown3Push = 0; 


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
bool serialPrint1Flag = LOW;
bool serialPrint2Flag = LOW;
bool serialPrint3Flag = LOW;


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

  
  if(serialPrint1Flag == HIGH)
  {
   lcd.setCursor(9, 1);
   lcd.print(Data1[directionUpDown1Push]);
   serialPrint1Flag = LOW;
  }

  if(serialPrint2Flag == HIGH)
  {
   lcd.setCursor(0, 2);
   lcd.print(Data2[directionUpDown2Push]);
   serialPrint2Flag = LOW;
  }
  if(serialPrint3Flag == HIGH)
  {
   lcd.setCursor(0, 2);
   lcd.print(Data3[directionUpDown3Push]);
   serialPrint3Flag = LOW;
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
      directionUpDown1Push = 0;
      directionUpDown2Push = 0;
      directionUpDown3Push = 0;
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
      directionUpDown1Push = 0;
      directionUpDown2Push = 0;
      directionUpDown3Push = 0;
      }
  
        if (directionPush > 3)                           // If the user tries to scroll above the last menu option,
        {                                                // the program will loop back to the first menu option.
        directionPush = 0;
        }
    }
    // Up BUTTON PRESS Bin Size
    if (currentButtonStateUp != buttonStateUp && directionPush == 1)       // Up button scrolls the menu options to the Up.  
    { 
    buttonStateUp = currentButtonStateUp;  
                 
      if (buttonStateUp == HIGH)                        // Once the button is released, the push is registered and the code below runs.
      {                                                 
      directionUpDown1Push--;                                   // Both the up and down press counts will be reset to zero when the Up button is pushed.
      serialPrint1Flag = HIGH;
      }
          
        if (directionUpDown1Push < 0)                           // If the user tries to scroll below the first menu option,
        {                                                // the program will loop back to the last menu option.
        directionUpDown1Push = 5;
        }
    }

   // Down BUTTON PRESS Bin size
    if (currentButtonStateDown != buttonStateDown && directionPush == 1)     // Down button scrolls the menu options to the Down.    
    { 
    buttonStateDown = currentButtonStateDown;  
                 
      if (buttonStateDown == LOW)                         
      {                                                 
      directionUpDown1Push++;// Both the up and down press counts will be reset to zero when the Down button is pushed.   
      serialPrint1Flag = HIGH;
      }
  
        if (directionUpDown1Push > 5)                           // If the user tries to scroll above the last menu option,
        {                                                // the program will loop back to the first menu option.
        directionUpDown1Push = 0;
        }
    }
  
    // Up BUTTON PRESS Bobbin mix
    if (currentButtonStateUp != buttonStateUp && directionPush == 2)       // Up button scrolls the menu options to the Up.  
    { 
    buttonStateUp = currentButtonStateUp;  
                 
      if (buttonStateUp == HIGH)                        // Once the button is released, the push is registered and the code below runs.
      {                                                 
      directionUpDown2Push--;                                   // Both the up and down press counts will be reset to zero when the Up button is pushed.
      serialPrint2Flag = HIGH;
      }
          
        if (directionUpDown2Push < 0)                           // If the user tries to scroll below the first menu option,
        {                                                // the program will loop back to the last menu option.
        directionUpDown2Push = 1;
        }
    }

   // Down BUTTON PRESS Bobbin mix
    if (currentButtonStateDown != buttonStateDown && directionPush == 2)     // Down button scrolls the menu options to the Down.    
    { 
    buttonStateDown = currentButtonStateDown;  
                 
      if (buttonStateDown == LOW)                         
      {                                                 
      directionUpDown2Push++;// Both the up and down press counts will be reset to zero when the Down button is pushed.   
      serialPrint2Flag = HIGH;
      }
  
        if (directionUpDown2Push > 1)                           // If the user tries to scroll above the last menu option,
        {                                                // the program will loop back to the first menu option.
        directionUpDown2Push = 0;
        }
    }



    // Up BUTTON PRESS Sensor option
    if (currentButtonStateUp != buttonStateUp && directionPush == 3)       // Up button scrolls the menu options to the Up.  
    { 
    buttonStateUp = currentButtonStateUp;  
                 
      if (buttonStateUp == HIGH)                        // Once the button is released, the push is registered and the code below runs.
      {                                                 
      directionUpDown3Push--;                                   // Both the up and down press counts will be reset to zero when the Up button is pushed.
      serialPrint3Flag = HIGH;
      }
          
        if (directionUpDown3Push < 0)                           // If the user tries to scroll below the first menu option,
        {                                                // the program will loop back to the last menu option.
        directionUpDown3Push = 1;
        }
    }

   // Down BUTTON PRESS Sensor option
    if (currentButtonStateDown != buttonStateDown && directionPush == 3)     // Down button scrolls the menu options to the Down.    
    { 
    buttonStateDown = currentButtonStateDown;  
                 
      if (buttonStateDown == LOW)                         
      {                                                 
      directionUpDown3Push++;// Both the up and down press counts will be reset to zero when the Down button is pushed.   
      serialPrint3Flag = HIGH;
      }
  
        if (directionUpDown3Push > 1)                           // If the user tries to scroll above the last menu option,
        {                                                // the program will loop back to the first menu option.
        directionUpDown3Push = 0;
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
