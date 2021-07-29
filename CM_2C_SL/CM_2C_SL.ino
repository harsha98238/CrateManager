#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {41, 42, 43, 44}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {45, 46, 47, 48}; //connect to the column pinouts of the keypad
 
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


//sensor input variables
//gpio to use to trigger delay
const int beltDetection    = 32;
const int fullCopDetection = 33;
const int yarn1            = 34;
const int color1           = 35;
const int obj1             = 36;
const int color2           = 37;
const int obj2             = 38;
const int crate1           = 39;
const int crate2           = 40;
//const int pas              = 41;

//output variable
const int vcMotor       = 2;
const int ccb           = 3;
const int s1            = 4;
const int s2            = 5;
const int s3            = 6;
const int copStorage1   = 7; //orientation box 1
const int copStorage2   = 8; //orientation box 2
const int crateEjector1 = 9; //bin change 1
const int crateEjector2 = 10; //bin change 2
const int binLock1      = 11; 
const int binLock2      = 12;
const int binUp1        = 14; //bin down 1
const int binUp2        = 15;  //bin down 2
const int alarmLightyellow = 16;

//ccb
bool loaderState = false;
unsigned long timerccb = 0;

/*shutter1*/
//1 yarn
unsigned long yarn2simulatorPreviousPulse = 0;

//to adjust yarn2 pulse delay
int yarn2pulsedelay = 16;
bool yarn2simulatorflag = false;
bool yarn1flag =  false;
int yarn2index = 0;
int yarn2indexcurrent = 0;
//int yarn2currentOutput = 0;
int yarn2simTrainarray[100] ;
unsigned long timeryarn1 = 0;
bool yarn1detected = false;


//color1
bool color1flag = false;
bool s2on = false;
bool color1identified = false;
//object1
bool Object1flag1 = false;
bool Object1Detected = false;
unsigned long Object1DetectTmr = 0;
bool Shutter2OnReady = false;
bool Shutter2Onflag1 = false;
bool Shutter2Onflag2 = false;
bool C1CountFlag2 = false;
bool C1CountFlag1 = false;
short s2OnDelay = 2; //was2
short s2OffDelay=6;//was 6
unsigned long color1Cnt;
unsigned long color1Cntflag = 0;

//copstorage1
bool flag1CopStorage1 = false;
bool flag2CopStorage1 = false;
unsigned long timer1CopStorage1 =  0;
unsigned long timer2CopStorage1 =  0;
bool copstorage1highflag = false;
bool copstorage1lowflag = false;
short copStorage1Ondelay=20; //20
short copStorage1Offdelay=20;//10
bool copstorage1Shutterflag1 = false;
bool copstorage1Shutterflag2 = false;
unsigned long copStorage1Count =0;
unsigned long copStorage1Countflag = 0;
short numCopStorage1 = 4;


//crate1error 
bool tray1check = false;
bool tray1checkflag = false;
unsigned long crate1alert = 0;
short crate1alertondelay=200;
bool crate1error = false;
unsigned long  binup1inerrortime = 0;
unsigned long  binup1inerrortime2 = 0;
bool binup1inerrorok1 = false;
bool binup1inerrorok2 = false;
bool crate1alertcheck = false;

//crate1ejection
bool flag1CrateEjection1 = false;
bool flag2CrateEjection1 = false;
unsigned long timer1CrateEjection1 = 0 ;
unsigned long timer2CrateEjection1 = 0 ;
bool flag3CrateEjection1 = false;
short crate1RemoveOndelay=40;
bool crate1Complete = false;

//crate1replacement
bool crate1ProcessFlag = false;
bool flag1binUp1 =  false;
unsigned long timer1binUp1 = 0 ;


/*shutter3*/
//yarn2
int yarn2currentOutput = 0;
bool yarn2flag =  false;
bool yarn2detected =false;
//color2
bool color2flag = false;
bool s3on = false;
bool color2identified = false;
//object2
bool Object2flag1 = false;
bool Object2Detected = false;
unsigned long Object2DetectTmr = 0;
bool Shutter3OnReady = false;
bool Shutter3Onflag1 = false;
bool Shutter3Onflag2 = false;
bool C2CountFlag2 = false;
bool C2CountFlag1 = false;
short s3OnDelay = 2; //was2
short s3OffDelay=6;//was 6
unsigned long color2Cnt;
unsigned long color2Cntflag = 0;

//copstorage2
bool flag1CopStorage2 = false;
bool flag2CopStorage2 = false;
unsigned long timer1CopStorage2 =  0;
unsigned long timer2CopStorage2 =  0;
bool copstorage2highflag = false;
bool copstorage2lowflag = false;
short copStorage2Ondelay=20;
short copStorage2Offdelay=20;
bool copstorage2Shutterflag1 = false;
bool copstorage2Shutterflag2 = false;
unsigned long copStorage2Count = 0 ;
unsigned long copStorage2Countflag = 0;
short numCopStorage2 = 4;


//crate2error 
bool tray2check = false;
bool tray2checkflag = false;
unsigned long crate2alert = 0;
short crate2alertondelay=200;
bool crate2error = false;
unsigned long  binup2inerrortime = 0;
unsigned long  binup2inerrortime2 = 0;
bool binup2inerrorok1 = false;
bool binup2inerrorok2 = false;
bool crate2alertcheck = false;

//crate2ejection
bool flag1CrateEjection2 = false;
bool flag2CrateEjection2 = false;
unsigned long timer1CrateEjection2 = 0 ;
unsigned long timer2CrateEjection2 = 0 ;
bool flag3CrateEjection2 = false;
short crate2RemoveOndelay=40;
bool crate2Complete = false;

//crate2replacement
bool crate2ProcessFlag = false;
bool flag1binUp2 =  false;
unsigned long timer1binUp2 = 0 ;

//krichicount
bool krichi1 = false;
bool krichi2 = false;
unsigned long krichi1Cnt;
unsigned long krichi2Cnt;


//errors2
bool error=false;
bool errorflag=false;
bool erroroccured=false;
bool erroroccured1=false;
bool erroroccuredoff=false;
unsigned long errorontmr=0;
unsigned long errorofftmr=0;
//obj1
bool objerrflag=false;
bool objerrflag1=false;
unsigned long objerrtimer=0;
unsigned long objerrtimerondelay=100;
//color1
bool colorerrflag=false;
bool colorerrflag1=false;
unsigned long colorerrtimer=0;
unsigned long colorerrtimerondelay=100;
//yarn1
bool yarnerrflag=false;
bool yarnerrflag1=false;
unsigned long yarnerrtimer=0;
unsigned long yarnerrtimerondelay=100;


//errors3
bool errors3=false;
bool errors3flag=false;
bool errors3occured=false;
bool errors3occured1=false;
bool errors3occuredoff=false;
unsigned long errors3ontmr=0;
unsigned long errors3offtmr=0;
//obj2
bool objerrflag2=false;
bool objerrflag12=false;
unsigned long objerrtimer2=0;
unsigned long objerrtimerondelay2=100;
//color2
bool colorerrflag2=false;
bool colorerrflag12=false;
unsigned long colorerrtimer2=0;
unsigned long colorerrtimerondelay2=100;


//colorcount reset
unsigned long c1Cntreset = 40;
//unsigned long c2Cntreset = 40;
bool krichiCntFlag = false;
unsigned long  krichiCnt;

//belt
bool toggle = false;
bool toggle1 = false;
unsigned long beltonDetection = 0;
unsigned long beltoffDetection= 0;
unsigned long timeblock=100;


//belt error
bool errorB=false;
//bool errorBflag=false;
bool errorBoccured=false;
//bool errorBoccured1=false;
//bool errorBoccuredoff=false;
//unsigned long errorBontmr=0;
//unsigned long errorBofftmr=0;



//full bobbin error
bool errorFC = false;
bool fberrflag=false;
bool fberrflag1=false;
unsigned long fberrtimer=0;
unsigned long fberrtimerondelay=100;

//full cop
bool togglefc=false;
unsigned long fullcopBeltSeperationTimerStart=0;
unsigned long fullcopBeltSeperationTimerEnd=0;
unsigned long timefc=0;
unsigned long fullcopinterval=0;
short s1OnDelay=1;
short s1OffDelay=4;
//unsigned long FCCnt = 0;
//change from unsigned long
bool FCCountFlag = false;


//EEPROM
unsigned long previousMillis = 0;
const unsigned long interval = 1UL*60UL*60UL*1000UL;   // Change  every 60 minute

//new copstorage
bool copstorage1loopflag = false;
bool copstorage2loopflag = false;

//tray empty
bool Tray1EmptyFlag = false;
bool Tray2EmptyFlag = false;


//2 ERROR
bool  BeltError = false;
bool  FullCopError  = false;
bool  Tray1Error  = false;
bool  Tray2Error  = false;

bool Color1ErrorOccuredFlag = false;
bool Object1ErrorOccuredFlag = false;
bool YarnErrorOccuredFlag = false;
bool Color2ErrorOccuredFlag = false;
bool Object2ErrorOccuredFlag = false;


bool printErrorStatus = false;
unsigned long timer, timerInterval = 0;


//errors  tray
bool Tray1LightError=false;
bool Tray2LightError=false;
bool Trayerrorflag=false;
bool Trayerroroccured=false;
bool Trayerroroccured1=false;
bool Trayerroroccuredoff=false;
unsigned long Trayerrorontmr=0;
unsigned long Trayerrorofftmr=0;


//keypad
bool ResetSwitch = false;
bool ErrorCycle = false; 


/*init*/
unsigned long InitCopStorageTimer = 0;
const int InitCopStorageOnDelay = 20 ;
bool InitCopStorageFlag = false;


unsigned long InitCopStorageTimer2 = 0;
const int InitCopStorageOffDelay = 20 ;
bool  InitCopStorageFlag2 = false;


unsigned long InitCrateEjectionTimer1 = 0;
bool InitCrateEjectionFlag1 = false;

bool InitCrateReplacementFlag = false;

unsigned long InitCrateEjectionTimer2 = 0;
const int InitCrateRemoveOnDelay = 40 ;
bool  InitCrateEjectionFlag2 = false;

bool InitCrateProcessFlag = false;
bool InitCrateComplete = false;
bool InitBinUpFlag1 = false;
unsigned long InitTimer1BinUp1 = 0;

//new
bool Initializationflag = true;



void setup() {
  
  Serial.begin(9600);//bits per second

  Serial.println("setup started");
  // initialize the LCD
  lcd.init(); 

  
  Serial.println("setup completed");
//  while(1);
  
  // Turn on the blacklight and print a message.
  lcd.backlight();
  pinMode(beltDetection, INPUT);
  pinMode(fullCopDetection, INPUT);
  pinMode(color1, INPUT);
  pinMode(color2, INPUT);
  pinMode(obj1, INPUT);
  pinMode(obj2, INPUT);
  pinMode(yarn1, INPUT);
  pinMode(crate1, INPUT);
  pinMode(crate2, INPUT);
//pinMode(pas, INPUT);

  pinMode(ccb,OUTPUT);
  pinMode(vcMotor,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(binLock2,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(copStorage1,OUTPUT);
  pinMode(copStorage2,OUTPUT);
  pinMode(crateEjector1,OUTPUT);
  pinMode(crateEjector2,OUTPUT);
  pinMode(binUp1,OUTPUT);
  pinMode(binUp2,OUTPUT);
  pinMode(binLock1,OUTPUT);
  
  //pinMode(alarmLightred,OUTPUT);
  pinMode(alarmLightyellow,OUTPUT);
 // pinMode(alarmLightgreen,OUTPUT);

//this is both horizontal and vertical
digitalWrite(vcMotor,HIGH);

lcd.setCursor(0,0);
lcd.print("C1=");
lcd.setCursor(8,0);
lcd.print("C2=");
lcd.setCursor(0,1);
lcd.print("KRICHI=");
//lcd.setCursor(0,2);
//lcd.print("ERROR=");

//EEPROM
//copStorage1Count = EEPROM.read(2);

int c1=EEPROM.read(0);  //c1
int d1=EEPROM.read(1);
int e1=((c1*255)+d1);
color1Cnt = e1;
lcd.setCursor(3,0);
lcd.print(color1Cnt);


int c2=EEPROM.read(2);  //c2
int d2=EEPROM.read(3);
int e2=((c2*255)+d2);
color2Cnt = e2;
lcd.setCursor(11,0);
lcd.print(color2Cnt);



int x1=EEPROM.read(4);  //k1
int y1=EEPROM.read(5);
int z1=((x1*255)+y1);
krichi1Cnt = z1;



int x2=EEPROM.read(6);  //k2
int y2=EEPROM.read(7);
int z2=((x2*255)+y2);
krichi2Cnt = z2;


krichiCnt  =  krichi1Cnt + krichi2Cnt;
lcd.setCursor(7,1);
lcd.print(krichiCnt);

//multiple error
timer = millis();

//Serial.println("setup completed");

}



void loop(){ 

//Serial.println("in loop");
/*int*/

  if (/*millis() - InitCopStorageTimer > InitCopStorageOnDelay*50 and*/ Initializationflag = true and InitCopStorageFlag == false )
{
  digitalWrite(copStorage1,HIGH); 
  digitalWrite(copStorage2,HIGH);
  InitCopStorageFlag = true;
   
  InitCopStorageTimer2 = millis();
  InitCopStorageFlag2 = true;

  Serial.println("cop storage  high at setup");
}
  if( millis() - InitCopStorageTimer2 > InitCopStorageOffDelay*50 and InitCopStorageFlag2 ==  true)
{
  digitalWrite(copStorage1,LOW);
  digitalWrite(copStorage2,LOW);
  InitCopStorageFlag2 = false;
  
  InitCrateEjectionTimer1 = millis();  
  InitCrateEjectionFlag1 = true;

  Serial.println("cop storage  low at setup");
}




/*************************************************CrateEjection********************************************/



  if  (millis() - InitCrateEjectionTimer1 > 2000 and InitCrateEjectionFlag1 == true)
{
  digitalWrite(crateEjector1,HIGH);
  digitalWrite(crateEjector2,HIGH);
  InitCrateEjectionFlag1 = false;
    
  InitCrateEjectionTimer2 = millis();
  InitCrateEjectionFlag2 = true;

  Serial.println("crate eject high at setup");

}

  if( millis() - InitCrateEjectionTimer2 > (InitCrateRemoveOnDelay * 50) and InitCrateEjectionFlag2 == true)
{
  digitalWrite(crateEjector1,LOW);
  digitalWrite(crateEjector2,LOW);
  InitCrateEjectionFlag2 = false;


  InitCrateReplacementFlag = true;

  InitCrateComplete = true;
}

/*****************************************************CrateReplacement************************************************************/
  if( InitCrateReplacementFlag == true and millis() - InitCrateEjectionTimer2 > 1700 and InitCrateProcessFlag == false)
{
  if( digitalRead(crate1) == false)
  {
    InitCrateProcessFlag =true;
  }
}

  if( InitCrateComplete == true  and InitCrateProcessFlag == false and InitBinUpFlag1 ==false)
{
  digitalWrite(binUp1,HIGH);
  digitalWrite(binUp2,HIGH);
  InitTimer1BinUp1 = millis();
  InitBinUpFlag1 = true;
}

  if(millis() - InitTimer1BinUp1 > 1000  and InitBinUpFlag1 == true)
{
   digitalWrite(binLock1,HIGH);
   digitalWrite(binLock2,HIGH);

}

  if(millis() - InitTimer1BinUp1 > 2400 and InitBinUpFlag1 == true)
{
  digitalWrite(binLock1,LOW);
  digitalWrite(binLock2,LOW);
 
}
  if(millis() - InitTimer1BinUp1 > 2000 and InitBinUpFlag1 == true)
{
  digitalWrite(binUp1,LOW);
  digitalWrite(binUp2,LOW);  
}
  if(millis() - InitTimer1BinUp1 > 4000 and InitBinUpFlag1 == true)
{
  InitCrateReplacementFlag = false;
  InitCrateComplete =false;
  InitCrateProcessFlag =false;
  InitBinUpFlag1 = false;
  Initializationflag = false;
  

}

unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) 
 {
  previousMillis += interval;

int a1=color1Cnt/255;
int b1=(color1Cnt-(a1*255));
EEPROM.update(0,a1);
EEPROM.update(1,b1);

int a2=color2Cnt/255;
int b2=(color2Cnt-(a2*255));
EEPROM.update(2,a2);
EEPROM.update(3,b2);

int v1=krichi1Cnt/255;
int w1=(krichi1Cnt-(v1*255));
EEPROM.update(4,v1);
EEPROM.update(5,w1);


int v2=krichi2Cnt/255;
int w2=(krichi2Cnt-(v2*255));
EEPROM.update(6,v2);
EEPROM.update(7,w1);


}


//multiple error
if (ErrorCycle == true)
{
  if((millis()-timer) > timerInterval)
  {
    printErrorStatus = printError();
    timer = millis();
    if(printErrorStatus == true){
     timerInterval = 1000;
    }
    else
    {
      ErrorCycle = false;
      timerInterval = 0;
    }
    
  }
}

  
    //Cop Collecting Box
  if (((millis() - timerccb) > 2000) /*and error == false*/)
  {
    loaderState = !loaderState;
    digitalWrite(ccb, loaderState);
    timerccb = millis();
    //Serial.println(loaderOn);
  }

/*********************belt movement detection*************/ 
if (digitalRead(beltDetection) == true and toggle == false )
  {
    beltonDetection = millis();
    toggle = true;
    toggle1 = false;
  }
  
if (digitalRead(beltDetection) == false and toggle1 == false )
  {
    toggle = false;
    toggle1 = true;
    beltoffDetection = millis();
  }
  
  
if ( (millis() - beltonDetection > (timeblock * 50)) or (millis() - beltoffDetection > (timeblock * 50)))
  {
//    lcd.setCursor(8,3);
//    lcd.print("VC ERROR");
    BeltError = true;
    Serial.println("Vertical belt error occured");
    errorB = true;
  }

/*error for belt*/

if (errorB == true and errorflag == false)
  {
    errorBoccured = true;
    errorflag = true;
    //digitalWrite(alarmLightred,HIGH);
  }
 
  
if (errorB == true and errorBoccured == true)
  {
    //digitalWrite(alarmLightyellow,HIGH);
    //errorBontmr = millis();
    errorBoccured = false;
    //errorBoccured1 = true;
    
  }

/*full bobin ejection*/

//**************Full bobbin Sensor error**************
if (digitalRead(fullCopDetection) == true and fberrflag == false)
  {
    
    fberrtimer = millis();
    fberrflag1 = true;
    fberrflag = true;
    
  }

if (digitalRead(fullCopDetection) == false )
  {
    fberrflag = false;
    fberrflag1 = false;
  }
   
if ((millis() - fberrtimer > (fberrtimerondelay * 50) and fberrflag1 == true) )
  { 
//    lcd.setCursor(0,3);
//    lcd.print("FULLCOP    ERROR");
    FullCopError  = true;
    Serial.println("Full bobbin error occured");
    errorFC = true;
    errorB = true;
  }




//Full Cop detection
  if (digitalRead(fullCopDetection) == true and togglefc == false)
  {
    fullcopBeltSeperationTimerStart= millis();
    Serial.println("full cop sens on");
    togglefc = true;
  }
  
  if (digitalRead(fullCopDetection) == false and togglefc == true )
  { 
    fullcopBeltSeperationTimerEnd = millis();
    Serial.println("full cop sens off");
    fullcopinterval = fullcopBeltSeperationTimerEnd - fullcopBeltSeperationTimerStart;
    if (fullcopinterval > 200)
      {
        Serial.println("start of Full cop"+String(fullcopBeltSeperationTimerStart));
        Serial.println("End of Full cop"+ String(fullcopBeltSeperationTimerEnd));
        Serial.println("belt cut time exceeded threshold: " + String(fullcopBeltSeperationTimerEnd - fullcopBeltSeperationTimerStart));
        Serial.println("setting timefc");
        timefc = millis();
      }
    togglefc = false;
    
  }
  if ((millis() - timefc > s1OnDelay*50) and (millis() - timefc < (s1OnDelay+s1OffDelay)*50) )
  { 
    Serial.println("S1high");
  digitalWrite(s1,HIGH);
//  FCCountFlag = true;
}
  else
  {
  digitalWrite(s1,LOW);
//    FCCountFlag = false;
  }

//if (FCCountFlag == true)
//{
//FCCountFlag = false;
//FCCnt++;
//Serial.print("FC count=");
//Serial.println(FCCnt);
//lcd.setCursor(10,1);
//lcd.print(FCCnt);
// 
//}  


/*shutter2 start*/
/*s2error */
if (error == true and errorflag == false)
  {
    
    erroroccured = true;
    errorflag = true;
  }
 
  
if (error == true and erroroccured == true)
  {
    errorontmr = millis();
    erroroccured = false;
    erroroccured1 = true;
    
  }

if (millis() - errorontmr > 1000 and erroroccured1 == true)
  {
    //digitalWrite(alarmLightyellow,HIGH);
    errorofftmr = millis();
    erroroccuredoff = true;
    erroroccured1 = false;
  }
if (millis() - errorofftmr > 1000 and erroroccuredoff == true)
  {
    //digitalWrite(alarmLightyellow,LOW);
    erroroccuredoff = false;
  errorflag = false;
  }

//object 1 error
if (digitalRead(obj1) == true and objerrflag == false)
  {
    objerrtimer = millis();
    objerrflag1 = true;
    objerrflag = true;
    //objerrtimer1 = millis();
    
  }

if (digitalRead(obj1) == false )
  {
    objerrflag = false;
    objerrflag1 = false;
  }
   
if ((millis() - objerrtimer > (objerrtimerondelay * 50) and objerrflag1 == true) )
  { 
//    lcd.setCursor(13,2);
//    lcd.print("Ob");
    Object1ErrorOccuredFlag = true;
    Serial.println("Object 1 sensor error occured");
    error = true;
   
   // errorCode = "Object Sensor error";
  }



//color1 error
  if (digitalRead(color1) == false and colorerrflag == false)
    {
      colorerrtimer = millis();
      colorerrflag1 = true;
      colorerrflag = true;
      //objerrtimer1 = millis();
    //Serial.println("Color error test error occured"); 
    }

  if (digitalRead(color1) == true )
    {
      colorerrflag = false;
      colorerrflag1 = false;
    }
   
  
  if ((millis() - colorerrtimer > (colorerrtimerondelay * 50) and colorerrflag1 == true) )
    { 
//      lcd.setCursor(12,2);
//      lcd.print("C");
      Color1ErrorOccuredFlag = true;
      Serial.println("color 1 sensor error occured");
      error = true;
      
     // errorCode = "Color Sensor error";
    }


  
//yarn1 error
  if (digitalRead(yarn1) == false and yarnerrflag == false)
  {
    yarnerrtimer = millis();
    yarnerrflag = true;
    yarnerrflag1 = true;
  }

  if (digitalRead(yarn1) == true )
    {
      yarnerrflag = false;
      yarnerrflag1 = false;
    }
    
  if ((millis() - yarnerrtimer > (yarnerrtimerondelay * 50) and yarnerrflag1 == true) )
  { 
//    lcd.setCursor(15,2);
//    lcd.print("Y");
    YarnErrorOccuredFlag = true;
    Serial.println("Yarn sensor error occured");
    error = true;
   
    //errorCode = "Yarn Sensor error";
  }

/*s2error ends*/
// ****************************Yarn1 identification********************************//
    if(digitalRead(yarn1) == false and yarn1flag == false  )
    {
    timeryarn1 = millis();
    yarn1flag = true;
    Serial.println("yarn1 identified");
    yarn2simulatorflag = true;
    }

    if( (millis() - yarn2simulatorPreviousPulse) >50 )
    {
      if( yarn2simulatorflag == true)
      {
       yarn2simTrainarray[yarn2index] = 1;
      }
      if( yarn2simulatorflag == false)
      {
       yarn2simTrainarray[yarn2index] = 0;
      }
      
      yarn2indexcurrent = yarn2index - yarn2pulsedelay;
      if (yarn2indexcurrent < 0)
      {
        yarn2indexcurrent = yarn2indexcurrent + 100 ;
      }

      yarn2currentOutput = yarn2simTrainarray[yarn2indexcurrent] ;
      if (yarn2indexcurrent < 99 and yarn2indexcurrent > 0 )
      {
      yarn2currentOutput = yarn2simTrainarray[yarn2indexcurrent] +
                            yarn2simTrainarray[yarn2indexcurrent - 1] +
                            yarn2simTrainarray[yarn2indexcurrent + 1];
      }
     
//      Serial.println("yarn2index " + String(yarn2index) +"current" + String(yarn2indexcurrent) );
      yarn2index = (yarn2index+1) % 100 ;
      yarn2simulatorPreviousPulse = millis();
      yarn2simulatorflag = false;
      
    }
// ****************************Color 1 identification********************************//

 if(digitalRead(color1) == false and  color1flag == false) //clr1false
 {
  color1flag = true;
  color1identified = true;
  Serial.println("colour1 identified");
  s2on = true;
}   
// ****************************Object 1 identification********************************//

  if(digitalRead(obj1) == true  and Object1flag1 == false)
  {
    Object1Detected = true;//only used inside loop
    Object1flag1 = true;
    Shutter2OnReady = false;
    Serial.println("Object1 identified");
    yarn1flag = false;
    color1flag = false;
    color1identified = false;
  }
//TO TRIGGER SHUTTER2 for color1COP
  if (Object1flag1 == true and digitalRead(obj1) == false and color1identified == true and yarn1flag == false)
      {
      Shutter2Onflag1 = true;
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false;
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      Serial.println("Color1 Empty Rested");
      }
  
  //Passing Shutter2 - cop must be 2nd or 3rd color    
     if (Object1flag1 == true and digitalRead(obj1) == false and color1identified == false and yarn1flag == false) 

     {
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false; 
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      Serial.println("Color2 empty Check");
    }
 //To find Yarn at Object1 sensor and use the information for shutter3 and shutter4     
        if   (Object1flag1 == true and digitalRead(obj1) == false and color1identified == false and yarn1flag == true)
    {
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false;
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      Serial.println("Color2 yarn Check");
    }
//Color 1 COP with Yarn (color 1 kirchi)
    if   (Object1flag1 == true and digitalRead(obj1) == false and color1identified == true and yarn1flag == true)
    {
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false;
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      krichi1 =  true;
      Serial.println("Color1 yarn Rested");
if (krichi1==true)
{
krichi1Cnt++;
Serial.print("krichi 1=");
Serial.println(krichi1Cnt);
//lcd.setCursor(3,1);
//lcd.print(krichi1Cnt);
krichiCntFlag=true;
krichi1==false;
}
}   
//Shutter2 ON flag
    if (Shutter2Onflag1 == true and Shutter2Onflag2 == false and Tray1EmptyFlag == false and Tray2EmptyFlag == false )
        {
          Shutter2OnReady = true;
          Shutter2Onflag2 = true;
          Shutter2Onflag1 = false;   
        }

     if (C1CountFlag1 == false)
     {
      Shutter2Onflag2 = false;
      //Serial.println("Shutter2 off ");
     }

// ****************************Color 1 Ejection********************************//
if( (millis()-Object1DetectTmr) > (s2OnDelay*1) and (millis()-Object1DetectTmr) < (s2OnDelay+s2OffDelay)*50 and Shutter2OnReady == true and Tray1EmptyFlag == false and Tray2EmptyFlag == false )//was*50
  
  {
  digitalWrite(s2,HIGH);
  //Serial.println("Shutter 2 On");
  C1CountFlag1 = true;
  }
else{
  digitalWrite(s2,LOW);
  C1CountFlag1 = false;
}



if (C1CountFlag1 == true and C1CountFlag2 == false)
{
  C1CountFlag2 = true;
  //color1Cnt = EEPROMcnt;
  color1Cnt++;
   color1Cntflag++;

  Serial.print("color 1 count=");
  Serial.println(color1Cnt);
  lcd.setCursor(3,0);
  lcd.print(color1Cnt);
  
}

if (C1CountFlag1 == false)
{
  C1CountFlag2 = false;
}

//if (color1Cnt >= 4 and C1CountFlag1 == false)//40 //color 1 count resetted
//{
//  color1Cnt = 0;
//}

if (color1Cntflag >=c1Cntreset and C1CountFlag1 == false)//40 //color 1 count resetted
{
  color1Cntflag = 0;
}




/************************************CopStorage 1*******************************/

//if (color1Cnt >= c1Cntreset  and flag1CopStorage1 == false)//40
//{
//  color1Cntflag = color1Cnt;
//  Serial.print("color1Cntflag=");
//  Serial.println(color1Cntflag);
//  flag1CopStorage1 = true;
//}

//if(color1Cntflag >= c1Cntreset and flag1CopStorage1 == false /*flag2CopStorage1 == false*/)//40

//if(color1Cntflag = c1Cntreset and flag1CopStorage1 == false /*flag2CopStorage1 == false*/)//40


if((color1Cnt>0) and ((color1Cnt %  c1Cntreset) ==  0 ) and flag1CopStorage1 == false and copstorage1loopflag == false /*and Tray1EmptyFlag == false and Tray2EmptyFlag == false/*flag2CopStorage1 == false*/) //40

{
  
 copstorage1loopflag = true;
  timer1CopStorage1 = millis(); 
  flag1CopStorage1 = true;
//  flag2CopStorage1 = true;
  copstorage1highflag = true;
  Serial.println("cop storage ready to high ");
  
}

if (millis() - timer1CopStorage1 > copStorage1Ondelay*50  and copstorage1highflag == true  and Initializationflag == false)//50
{
  digitalWrite(copStorage1,HIGH); 
  
  timer2CopStorage1 = millis();
  copstorage1Shutterflag1 = true;
  Serial.println("cop storage  high ");
  copstorage1highflag = false;
  copstorage1lowflag = true;
}
if( millis() - timer2CopStorage1 > copStorage1Offdelay*50 and copstorage1lowflag==true   and Initializationflag == false) //50
{
     digitalWrite(copStorage1,LOW);
     copstorage1Shutterflag1 = false;
     color1Cntflag = 0;
      flag2CopStorage1 = false;//false
      flag1CopStorage1 = false;//false
      copstorage1lowflag = false;
      
      //color1Cntflag = 0;
      //Serial.println("cop storage low ");
      //copStorage1Count++ ;
      
  }

if (  ((color1Cnt %  c1Cntreset) !=  0 ) and copstorage1loopflag == true and flag1CopStorage1 == false)
{
  
copstorage1loopflag = false;
  
}


if (copstorage1Shutterflag1 == true and copstorage1Shutterflag2 == false)
{
  copstorage1Shutterflag2 = true;
  copStorage1Count++ ;
  Serial.print("copStorage1Count=");
  Serial.println(copStorage1Count);
}



if (copstorage1Shutterflag1 == false)
{
  copstorage1Shutterflag2 = false;
}




if (copStorage1Count >= 4 and copstorage1Shutterflag1 == false)
{
  copStorage1Count = 0;
}

   /**************************************************crate error check********************************************/

if (digitalRead(crate1) == true and tray1check == false  and Initializationflag == false)
  {
    Tray1EmptyFlag = true;
//    Tray1Error = true;
//    Tray1LightError = true;
    crate1alert = millis();
    tray1check = true;
    tray1checkflag = true;
   
    Serial.println("TRAY1  EMPTY");
  }

  if (digitalRead(crate1) == false )
  {
    Tray1EmptyFlag = false;
    Tray1Error = false;
    Tray1LightError = false;
    tray1check = false;
    tray1checkflag = false;
  }
if (millis() - crate1alert > crate1alertondelay*50 and tray1checkflag == true)//50
{
  crate1alertcheck = true;
//  lcd.setCursor(0,2);
//  lcd.print("TRAY1  EMPTY");
  Serial.println("tray alert");
}
if (digitalRead(crate1) == true and crate1alertcheck == true)
{
  crate1error = true;
  Tray1EmptyFlag = true;
  Tray1Error = true;
  Tray1LightError = true;
  //error = true;
  digitalWrite(binLock1,HIGH);
  Serial.println("binlock high");
}

if (crate1error == true and digitalRead(crate1) == false) 
{
  digitalWrite(binUp1,HIGH);
  binup1inerrortime = millis();
  binup1inerrorok1 = true;
  crate1alertcheck = false;
  crate1error = false;
//  error = false;
  Serial.println("binup high");
  
 
}

if (binup1inerrorok1 == true and millis() - binup1inerrortime > 4000)
  {
    digitalWrite(binUp1,LOW);
    binup1inerrorok1 = false;
    binup1inerrortime2 = millis();
    binup1inerrorok2 = true;
    //Serial.println("binup low");
  }
    

  if (binup1inerrorok2 == true and millis() - binup1inerrortime2 > 500)//was400
  {
    digitalWrite(binLock1,LOW);
      
    binup1inerrorok2 = false;
    //Serial.println("binlock low"); 
    if 
      (digitalRead(crate1) == false)
      {
       Tray1EmptyFlag = false;
       Tray1Error = false;
       Tray1LightError = false;
       tray1check = false;
       tray1checkflag = false;
    }
  }





/*************************************************CrateEjection********************************************/

if( copStorage1Count >= 4 and crate1error == false and digitalRead(crate1) == false   and Initializationflag == false)
{
  copStorage1Countflag = copStorage1Count;
}
{
  if( copStorage1Countflag == numCopStorage1 and flag1CrateEjection1 ==false){
  timer1CrateEjection1 = millis();
  flag1CrateEjection1 = true;
  flag2CrateEjection1 = true;
  
  }

  if  (millis() - timer1CrateEjection1 > 2000 and flag2CrateEjection1 == true)
  {
  digitalWrite(crateEjector1,HIGH);
  Serial.println("crate eject high");
  timer2CrateEjection1 = millis();
  flag3CrateEjection1 = true;
  flag2CrateEjection1 = false;
 }

if( millis() - timer2CrateEjection1 > (crate1RemoveOndelay * 50) and flag3CrateEjection1 == true){
      digitalWrite(crateEjector1,LOW);
      copStorage1Countflag =0;
      flag1CrateEjection1 = false;
      flag3CrateEjection1 = false;
     // Serial.println("crate eject low");
      crate1Complete = true;
  }

/*****************************************************CrateReplacement************************************************************/
if( flag3CrateEjection1 == true and millis() - timer2CrateEjection1 > 1700 and crate1ProcessFlag == false   and Initializationflag == false){
  if( digitalRead(crate1) == false){
  //  error = true;
   // errorCode="crateBlock";
    crate1ProcessFlag =true;
  }
}

if( crate1Complete == true  and crate1ProcessFlag == false and flag1binUp1 ==false){
  digitalWrite(binUp1,HIGH);
  timer1binUp1 = millis();
  flag1binUp1 = true;
}

if(millis() - timer1binUp1 > 1000  and flag1binUp1 == true){
  digitalWrite(binLock1,HIGH);
 if( millis() - timer1binUp1 > 1000 and millis() - timer1binUp1 < 2000 and digitalRead(crate1) == false){
//    error = true;
//    errorCode="BinUpError";
  }
}

if(millis() - timer1binUp1 > 2400 and flag1binUp1 == true){
  digitalWrite(binLock1,LOW);
 
}
if(millis() - timer1binUp1 > 2000 and flag1binUp1 == true){
  digitalWrite(binUp1,LOW);
  
}
if(millis() - timer1binUp1 > 4000 and flag1binUp1 == true){
 if( digitalRead(crate1) == true){
 //   errorCode="NoCrate";
  }

  crate1Complete =false;
  crate1ProcessFlag =false;
  flag1binUp1 = false;
  

}
}

/*shutter2 end*/
/*shutter3 start*/

/*s3 error*/

if (errors3 == true and errorflag == false)
  {
    errors3occured = true;
    errorflag = true;
    //digitalWrite(alarmLightred,HIGH);
  }
 
  
if (errors3 == true and errors3occured == true)
  {
    errors3ontmr = millis();
    errors3occured = false;
    errors3occured1 = true;
    
  }

if (millis() - errors3ontmr > 1000 and errors3occured1 == true)
  {
    //digitalWrite(alarmLightyellow,HIGH);
    errors3offtmr = millis();
    errors3occuredoff = true;
    errors3occured1 = false;
  }
if (millis() - errors3offtmr > 200 and errors3occuredoff == true)
  {
    //digitalWrite(alarmLightyellow,LOW);
    errors3occuredoff = false;
  errorflag = false;

  }

//Object2 Sensor Error
if (digitalRead(obj2) == true and objerrflag2 == false)
  {
    objerrtimer2 = millis();
    objerrflag12 = true;
    objerrflag2 = true;  
  }

if (digitalRead(obj2) == false)
  {
    objerrflag2 = false;
    objerrflag12 = false;
  }
   
  
if ((millis() - objerrtimer2 > (objerrtimerondelay2 * 50) and objerrflag12 == true) )
  { 
//    lcd.setCursor(13,2);
//    lcd.print("Ob");
    Object2ErrorOccuredFlag = true;
    Serial.println("Object2 error occured");
    errors3 = true;
    
   // errorCode = "Object2 Sensor error";
  }




  //Color2 Sensor Error
  if (digitalRead(color2) == false and colorerrflag2 == false)
    {
      colorerrtimer2 = millis();
      colorerrflag12 = true;
      colorerrflag2 = true;
      //objerrtimer1 = millis();
//    Serial.println("Color2 error test error occured"); 
    }

  if (digitalRead(color2) == true )
    {
      colorerrflag2 = false;
      colorerrflag12 = false;
    }
   
  
  if ((millis() - colorerrtimer2 > (colorerrtimerondelay2 * 50) and colorerrflag12 == true) )
    { 
//      lcd.setCursor(12,2);
//      lcd.print("C");
      Color2ErrorOccuredFlag = true;
      //Serial.println("Color 2 error occured");
      errors3 = true;
    }
/*s3error ends*/


// ****************************Yarn  2 identification********************************//
    if( yarn2currentOutput >= 1 and yarn2flag == false and digitalRead(obj2) == true )
    {
    yarn2flag = true;
    Serial.println("yarn2 identified");
    }


// ****************************Color 2 identification********************************//

 if(digitalRead(color2) == false and  color2flag == false) //clr1false
 {
  color2flag = true;
  color2identified = true;
  Serial.println("colour2 identified");
  s3on = true;
}


// ****************************Object 2 identification********************************//

  if(digitalRead(obj2) == true  and Object2flag1 == false)
  {
    Object2Detected = true;//only used inside loop
    Object2flag1 = true;
    Shutter3OnReady = false;
    Serial.println("Object2 identified");
    yarn2flag = false;
    color2flag = false;
    color2identified = false;
  }
//TO TRIGGER SHUTTER3 for color2COP
 if (Object2flag1 == true and digitalRead(obj2) == false and color2identified == true and yarn2flag == false)
//if (Object2flag1 == true and digitalRead(obj2) == false  and yarn2flag == false)
      {
      Shutter3Onflag1 = true;
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color2 identified");
      }
  
  //Passing Shutter3 - cop must be 3rd color    
     if (Object2flag1 == true and digitalRead(obj2) == false and color2identified == false and yarn2flag == false) 

     {
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color3u empty Check");
    }
 //To find Yarn at Object2 sensor and use the information for shutter4     
        if   (Object2flag1 == true and digitalRead(obj2) == false and color2identified == false and yarn2flag == true)
    {
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color3 yarn Check");
    }
//Color 2 COP with Yarn (color 2 kirchi)
    if   (Object2flag1 == true and digitalRead(obj2) == false and color2identified == true and yarn2flag == true)
    {
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      krichi2 =  true;
      Serial.println("Color2 yarn Rested");
if (krichi2==true)
{
krichi2Cnt++;
Serial.print("krichi 2=");
Serial.println(krichi2Cnt);
krichiCntFlag=true;
krichi2==false;
}
      
}   
//Shutter3 ON flag
    if (Shutter3Onflag1 == true and Shutter3Onflag2 == false and Tray1EmptyFlag == false and Tray2EmptyFlag == false)
        {
          Shutter3OnReady = true;
          Shutter3Onflag2 = true;
          Shutter3Onflag1 = false;   
        }

     if (C2CountFlag1 == false)
     {
      Shutter3Onflag2 = false;
     }







// ****************************Color 2 Ejection********************************//
if( (millis()-Object2DetectTmr) > (s3OnDelay*1) and (millis()-Object2DetectTmr) < (s3OnDelay+s3OffDelay)*50 and Shutter3OnReady == true and Tray1EmptyFlag == false and Tray2EmptyFlag == false)//was*50
  
  {
  digitalWrite(s3,HIGH);
  C2CountFlag1 = true;
  }
else{
  digitalWrite(s3,LOW);
  C2CountFlag1 = false;
}



if (C2CountFlag1 == true and C2CountFlag2 == false)
{
  C2CountFlag2 = true;
  color2Cnt++;
  color2Cntflag++;
  Serial.print("color 2 count=");
  Serial.println(color2Cnt);
  lcd.setCursor(11,0);
  lcd.print(color2Cnt);
  
}

if (C2CountFlag1 == false)
{
  C2CountFlag2 = false;
}


if (color2Cntflag >=c1Cntreset and C2CountFlag1 == false)//40 //color 1 count resetted
{
  color2Cntflag = 0;
}




/************************************CopStorage 2*******************************/

if((color2Cnt>0) and ((color2Cnt %  c1Cntreset) ==  0 ) and flag1CopStorage2 == false and copstorage2loopflag == false) //40
{
  copstorage2loopflag = true;
  timer1CopStorage2 = millis(); 
//  flag2CopStorage2 = true;
  flag1CopStorage2 = true;
  copstorage2highflag = true;
  Serial.println("cop storage 2 ready to high ");
  
}

if (millis() - timer1CopStorage2 > copStorage2Ondelay*50  and copstorage2highflag == true)
{
  digitalWrite(copStorage2,HIGH); 
  
  timer2CopStorage2 = millis();
  copstorage2Shutterflag1 = true;
  Serial.println("cop storage  high ");
  copstorage2highflag = false;
  copstorage2lowflag = true;
}
if( millis() - timer2CopStorage2 > copStorage2Offdelay*50 and copstorage2lowflag==true){
     digitalWrite(copStorage2,LOW);
     copstorage2Shutterflag1 = false;
     color2Cntflag = 0;
      flag2CopStorage2 = false;//false
      flag1CopStorage2 = false;//false
      copstorage2lowflag = false;
      
  }

if (  ((color2Cnt %  c1Cntreset) !=  0 ) and copstorage2loopflag == true and flag1CopStorage2 == false)
{
  
copstorage2loopflag = false;
  
}




if (copstorage2Shutterflag1 == true and copstorage2Shutterflag2 == false)
{
  copstorage2Shutterflag2 = true;
  copStorage2Count++ ;
  Serial.print("copStorage2Count=");
  Serial.println(copStorage2Count);
}



if (copstorage2Shutterflag1 == false)
{
  copstorage2Shutterflag2 = false;
}




if (copStorage2Count >= 4 and copstorage2Shutterflag1 == false)
{
  copStorage2Count = 0;
}

   /**************************************************crate 2 error check********************************************/

if (digitalRead(crate2) == true and tray2check == false)
  {
    Tray2EmptyFlag = true;
    crate2alert = millis();
    tray2check = true;
    tray2checkflag = true;
    
    Serial.println("no tray2");
  }

  if (digitalRead(crate2) == false )
  {
    Tray2EmptyFlag = false;
    Tray2Error = false;
    Tray2LightError = false;
    tray2check = false;
    tray2checkflag = false;
  }
if (millis() - crate2alert > crate2alertondelay*50 and tray2checkflag == true)
{
  crate2alertcheck = true;
  Serial.println("tray2 alert");
}
if (digitalRead(crate2) == true and crate2alertcheck == true)
{
  Tray2EmptyFlag = true;
  Tray2Error = true;
  Tray2LightError = true;
  crate2error = true;
  digitalWrite(binLock2,HIGH);
  Serial.println("binlock 2 high");
}

if (crate2error == true and digitalRead(crate2) == false) 
{
  digitalWrite(binUp2,HIGH);
  binup2inerrortime = millis();
  binup2inerrorok1 = true;
  crate2alertcheck = false;
  crate2error = false;
  Serial.println("binup2 high");
  
 
}

if (binup2inerrorok1 == true and millis() - binup2inerrortime > 4000)
  {
    digitalWrite(binUp2,LOW);
    binup2inerrorok1 = false;
    binup2inerrortime2 = millis();
    binup2inerrorok2 = true;
  }
    

  if (binup2inerrorok2 == true and millis() - binup2inerrortime2 > 500)//was400
  {
    digitalWrite(binLock2,LOW);
      
    binup2inerrorok2 = false;
    if(digitalRead(crate2) == false)
      {
       Tray2EmptyFlag = false;
       Tray2Error = false;
       Tray2LightError = false;
       tray2check = false;
       tray2checkflag = false;
    }
  }




/*************************************************Crate2Ejection********************************************/

if( copStorage2Count >= 4 and crate2error == false and digitalRead(crate2) == false )
{
  copStorage2Countflag = copStorage2Count;
}
{
  if( copStorage2Countflag == numCopStorage2 and flag1CrateEjection2 ==false){
  timer1CrateEjection2 = millis();
  flag1CrateEjection2 = true;
  flag2CrateEjection2 = true;
  
  }

  if  (millis() - timer1CrateEjection2 > 2000 and flag2CrateEjection2 == true)
  {
  digitalWrite(crateEjector2,HIGH);
  Serial.println("crate eject 2 high");
  timer2CrateEjection2 = millis();
  flag3CrateEjection2 = true;
  flag2CrateEjection2 = false;
 }

if( millis() - timer2CrateEjection2 > (crate2RemoveOndelay * 50) and flag3CrateEjection2 == true){
      digitalWrite(crateEjector2,LOW);
      copStorage2Countflag =0;
      flag1CrateEjection2 = false;
      flag3CrateEjection2 = false;
      crate2Complete = true;
  }


/*****************************************************Crate2Replacement************************************************************/
if( flag3CrateEjection2 == true and millis() - timer2CrateEjection2 > 1700 and crate2ProcessFlag == false ){
  if( digitalRead(crate2) == false)
  {
    crate2ProcessFlag =true;
  }
}

if( crate2Complete == true  and crate2ProcessFlag == false and flag1binUp2 ==false){
  digitalWrite(binUp2,HIGH);
  timer1binUp2 = millis();
  flag1binUp2 = true;
}

if(millis() - timer1binUp2 > 1000  and flag1binUp2 == true){
  digitalWrite(binLock2,HIGH);
 if( millis() - timer1binUp2 > 1000 and millis() - timer1binUp2 < 2000 and digitalRead(crate2) == true){
//    errors3 = true;
//    errorCode="BinUpError";
  }
}

if(millis() - timer1binUp2 > 2400 and flag1binUp2 == true){
  digitalWrite(binLock2,LOW);
 
}
if(millis() - timer1binUp2 > 2000 and flag1binUp2 == true){
  digitalWrite(binUp2,LOW);
  
}
if(millis() - timer1binUp2 > 4000 and flag1binUp2 == true){
 if( digitalRead(crate2) == true){
 //   errorCode="NoCrate";
  }

  crate2Complete =false;
  crate2ProcessFlag =false;
  flag1binUp2 = false;
  

}
}

/*shutter2 end*/

/*error reset*/
//  error resetting
if (ResetSwitch == true and ((error == true) or (errorB == true) or (errors3 = true) or (errorFC == true) or (Tray1LightError == true) or (Tray2LightError == true)))
  { 
    ResetSwitch = false;
    BeltError = false;
    FullCopError  = false;
    Tray1Error  = false;
    Tray2Error  = false;

    Color1ErrorOccuredFlag = false;
    Object1ErrorOccuredFlag = false;
    YarnErrorOccuredFlag = false;
    Color2ErrorOccuredFlag = false;
    Object2ErrorOccuredFlag = false;

    
    Tray1LightError = false;
    Tray2LightError = false;
    error = false;
    errors3 = false;
    errorB = false;
    errorFC = false;
    digitalWrite(alarmLightyellow,LOW);
    errorflag =false;
    errors3flag =false;
    objerrflag = false;
    objerrflag1 = false;
    objerrflag2 = false;
    objerrflag12 = false;
    lcd.setCursor(0,2);
    lcd.print("                ");
    lcd.setCursor(0,3);
    lcd.print("                ");
}

/*error reset ends*/

/*krichi count add*/

if(krichiCntFlag  ==  true)
{

krichiCnt  =  krichi1Cnt + krichi2Cnt;
lcd.setCursor(7,1);
lcd.print(krichiCnt);
krichiCntFlag=false;
}



/*Tray  error */
if ((Tray1LightError == true or Tray2LightError == true) and Trayerrorflag == false)
  {
    
    Trayerroroccured = true;
    Trayerrorflag = true;
  }
 
  
if ((Tray1LightError == true or Tray2LightError == true) and Trayerroroccured == true)
  {
    Trayerrorontmr = millis();
    Trayerroroccured = false;
    Trayerroroccured1 = true;
    
  }

if (millis() - Trayerrorontmr > 100 and Trayerroroccured1 == true)
  {
    //digitalWrite(alarmLightyellow,HIGH);
    Trayerrorofftmr = millis();
    Trayerroroccuredoff = true;
    Trayerroroccured1 = false;
  }
if (millis() - Trayerrorofftmr > 100 and Trayerroroccuredoff == true)
  {
    //digitalWrite(alarmLightyellow,LOW);
    Trayerroroccuredoff = false;
    Trayerrorflag = false;
  }

  

//keypad
char customKey = customKeypad.getKey();

if (customKey=='D')
{
    ResetSwitch = true;
    ErrorCycle = false;
    Serial.println(customKey);
  }
else
{
  ErrorCycle = true;
  ResetSwitch = false;
}



//eeprom reset logic 
if ( (color1Cnt == 60000) or (color2Cnt == 60000) or (krichi1Cnt == 60000) or (krichi2Cnt == 60000) )
{ 
  color1Cnt = 0;
  color2Cnt = 0;
  krichi1Cnt = 0;
  krichi2Cnt = 0;
EEPROM.update(0,0);
EEPROM.update(1,0);
EEPROM.update(2,0);
EEPROM.update(3,0);
EEPROM.update(4,0);
EEPROM.update(5,0);
EEPROM.update(6,0);
EEPROM.update(7,0);
}

}


//multiple error
bool printError( )
{
  static int i = 0 ; 
  int temp = 0;
  if(i >18)
  {
    i = 0;
  }
  temp = i;
  i++;
 switch(temp)
 {
  case 0:
  if(BeltError == true)
  {
    lcd.setCursor(0,3);
    lcd.print("BELT    ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 1:
  if(BeltError == true)
  {
    lcd.setCursor(0,3);
    lcd.print("                ");
     digitalWrite(alarmLightyellow,LOW);
  return true;
  }
  else
  {
   return false; 
    }

  break;
  
  case 2:
  if(FullCopError == true)
  {
    lcd.setCursor(0,3);
    lcd.print("FULLCOP ERROR");
    digitalWrite(alarmLightyellow,HIGH);
     return true;
  }
  else
  {
   return false; 
    }

  break;
  case 3:
 if(FullCopError == true)
 {
    lcd.setCursor(0,3);
    lcd.print("                ");
    digitalWrite(alarmLightyellow,LOW);
   return true;
  }
  else
  {
   return false; 
    }

  break;

  case 4:
  if(Tray1Error == true)
  {
    lcd.setCursor(0,3);
    lcd.print("TRAY1   ERROR");
     digitalWrite(alarmLightyellow,HIGH);
     return true;
  }
  else
  {
   return false; 
    }

  break;
  case 5:
  if(Tray1Error == true)
  {
   
    lcd.setCursor(0,3);
    lcd.print("                ");
    digitalWrite(alarmLightyellow,LOW);
   return true;
  }
  else
  {
   return false; 
    }

  break;

  case 6:
  if(Tray2Error == true)
  {
   
    lcd.setCursor(0,3);
    lcd.print("TRAY2   ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 7:
  if(Tray2Error == true)
  {
   
    lcd.setCursor(0,3);
    lcd.print("                ");
    digitalWrite(alarmLightyellow,LOW);
   return true;
  }
  else
  {
   return false; 
    }

  break;

 case 8:
  if(Color1ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("COLOR1  ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 9:
  if(Color1ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("                ");
     digitalWrite(alarmLightyellow,LOW);
  return true;
  }
  else
  {
   return false; 
    }

  break;
   case 10:
  if(Object1ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("OBJECT1 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 11:
  if(Object1ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("                ");
    digitalWrite(alarmLightyellow,LOW);
   return true;
  }
  else
  {
   return false; 
    }

  break;
   case 12:
  if(YarnErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("YARN    ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 13:
  if(YarnErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("                ");
     digitalWrite(alarmLightyellow,LOW);
  return true;
  }
  else
  {
   return false; 
    }

  break;
   case 14:
  if(Color2ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("COLOR2  ERROR");
   digitalWrite(alarmLightyellow,HIGH);
     return true;
  }
  else
  {
   return false; 
    }

  break;
  case 15:
  if(Color2ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("                ");
     digitalWrite(alarmLightyellow,LOW);
  return true;
  }
  else
  {
   return false; 
    }

  break;
  

   case 16:
  if(Object2ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("OBJECT2 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 17:
  if(Object2ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("                ");
     digitalWrite(alarmLightyellow,LOW);
  return true;
  }
  else
  {
   return false; 
    }

  break;
   

  
  

  default:
  lcd.setCursor(0,3);
  lcd.print("                ");
  digitalWrite(alarmLightyellow,LOW);
  return false;
  
 }



 
}
