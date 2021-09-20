#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
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
const int color3           = 49;
const int obj3             = 50;
const int crate3           = 51; 
const int yarn2            = 52;
const int yarn3            = 53;
//output variable
const int vcMotor          = 2;
const int ccb              = 3;
const int s1               = 4;
const int s2               = 5;
const int s3               = 6;
const int copStorage1      = 7; //orientation box 1
const int copStorage2      = 8; //orientation box 2
const int crateEjector1    = 9; //bin change 1
const int crateEjector2    = 10; //bin change 2
const int binLock1         = 11;  
const int binLock2         = 12;
const int binUp1           = 14; //bin down 1
const int binUp2           = 15;  //bin down 2
const int alarmLightyellow = 16;
const int s4               = 17;
const int copStorage3      = 18; //orientation box 1
const int crateEjector3    = 19; //bin change 1
const int binLock3         = 22; 
const int binUp3           = 23; //bin down 1
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
int yarn2indexpresent = 0;
int yarn2simTrainarray[100] ;
unsigned long timeryarn1 = 0;
bool yarn1detected = false;

//yarn3pulse
int yarn3pulsedelay = 32;
int yarn3indexpresent = 0;
int yarn3simTrainarray[100] ;
int yarn3index = 0;


//yar2 and 3
unsigned long timeryarn2 = 0;
unsigned long timeryarn3 = 0;

//color1
bool color1flag = false;
bool s2on = false;
bool color1identified = false;
//object1
//bool Object1flag1 = false;
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
int yarn2PresentOutput = 0;
bool yarn2flag =  false;
bool yarn2detected =false;
//color2
bool Color2flag = false;
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
bool krichi3 = false;
unsigned long Krichi1Cnt;
unsigned long Krichi2Cnt;
unsigned long Krichi3Cnt;


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
bool obj2errflag=false;
bool obj2errflag2=false;
unsigned long obj2errtimer=0;
unsigned long obj2errtimerondelay=100;
//color2
bool color2errflag=false;
bool color2errflag2=false;
unsigned long color2errtimer=0;
unsigned long color2errtimerondelay=100;
//yarn2
bool yarn2errflag=false;
bool yarn2errflag1=false;
unsigned long yarn2errtimer=0;
unsigned long yarn2errtimerondelay=100;



//colorcount reset
unsigned long c1Cntreset = 40;
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
bool FullCopCountFlag = false;
unsigned long FullCopCount;


//EEPROM
unsigned long previousMillis = 0;
unsigned long currentMillis;
const unsigned long interval = 1UL*60UL*60UL*1000UL;   // Change  every 60 minute

//new copstorage
bool copstorage1loopflag = false;
bool copstorage2loopflag = false;
bool CopStorage3loopflag = false;
//tray empty
bool Tray1EmptyFlag = false;
bool Tray2EmptyFlag = false;
bool Tray3EmptyFlag = false;


//2 ERROR
bool  BeltError = false;
bool  FullCopError  = false;
bool  Tray1Error  = false;
bool  Tray2Error  = false;
bool  Tray3Error  = false;

bool Color1ErrorOccuredFlag = false;
bool Object1ErrorOccuredFlag = false;
bool YarnErrorOccuredFlag = false;
bool Color2ErrorOccuredFlag = false;
bool Object2ErrorOccuredFlag = false;

bool Color3ErrorOccuredFlag = false;
bool Object3ErrorOccuredFlag = false;

bool Yarn2ErrorOccuredFlag = false;
bool Yarn3ErrorOccuredFlag = false;


bool printErrorStatus = false;
unsigned long timer, timerInterval = 0;


//errors  tray
bool Tray1LightError=false;
bool Tray2LightError=false;
bool Tray3LightError=false;
bool Trayerrorflag=false;
bool Trayerroroccured=false;
bool Trayerroroccured1=false;
bool Trayerroroccuredoff=false;
unsigned long Trayerrorontmr=0;
unsigned long Trayerrorofftmr=0;


//keypad
bool ResetSwitch = false;
bool ErrorCycle = false; 

/*shutter4*/
//yarn3
int yarn3PresentOutput = 0;
bool yarn3flag =  false;
bool yarn3detected =false;
//color3
bool color3flag = false;
bool s4on = false;
bool color3identified = false;
//object3
bool Object3flag1 = false;
bool Object3Detected = false;
unsigned long Object3DetectTmr = 0;
bool Shutter4OnReady = false;
bool Shutter4Onflag1 = false;
bool Shutter4Onflag2 = false;
bool C3CountFlag2 = false;
bool C3CountFlag1 = false;
short s4OnDelay = 2; //was2
short s4OffDelay=6;//was 6
unsigned long color3Cnt;
unsigned long color3Cntflag = 0;

//copstorage3
bool flag1CopStorage3 = false;
bool flag2CopStorage3 = false;
unsigned long timer1CopStorage3 =  0;
unsigned long timer2CopStorage3 =  0;
bool CopStorage3highflag = false;
bool CopStorage3lowflag = false;
short CopStorage3Ondelay=20;
short CopStorage3Offdelay=20;
bool CopStorage3Shutterflag1 = false;
bool CopStorage3Shutterflag2 = false;
unsigned long CopStorage3Count = 0 ;
unsigned long CopStorage3Countflag = 0;
short numCopStorage3 = 4;


//crate3error 
bool Tray3check = false;
bool Tray3checkflag = false;
unsigned long crate3alert = 0;
short crate3alertondelay=200;
bool crate3error = false;
unsigned long  binUp3inerrortime = 0;
unsigned long  binUp3inerrortime2 = 0;
bool binUp3inerrorok1 = false;
bool binUp3inerrorok2 = false;
bool crate3alertcheck = false;

//crate3ejection
bool flag1CrateEjection3 = false;
bool flag2CrateEjection3 = false;
unsigned long timer1CrateEjection3 = 0 ;
unsigned long timer2CrateEjection3 = 0 ;
bool flag3CrateEjection3 = false;
short crate3RemoveOndelay=40;
bool crate3Complete = false;

//crate3replacement
bool crate3ProcessFlag = false;
bool flag1binUp3 =  false;
unsigned long timer1binUp3 = 0 ;




//errors4
bool errors4=false;
bool errors4flag=false;
bool errors4occured=false;
bool errors4occured1=false;
bool errors4occuredoff=false;
unsigned long errors4ontmr=0;
unsigned long errors4offtmr=0;
//obj3
bool obj3errflag=false;
bool obj3errflag2=false;
unsigned long obj3errtimer=0;
unsigned long obj3errtimerondelay=100;
//color3
bool color3errflag=false;
bool color3errflag2=false;
unsigned long color3errtimer=0;
unsigned long color3errtimerondelay=100;
//yarn1
bool yarn3errflag=false;
bool yarn3errflag1=false;
unsigned long yarn3errtimer=0;
unsigned long yarn3errtimerondelay=100;




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


//new color count
int Color1ReadValue1  = 0;
int Color1ReadValue2  = 0;
int Color1ReadValue3  = 0;
int Color2ReadValue1  = 0;
int Color2ReadValue2  = 0;
int Color2ReadValue3  = 0;
int Color3ReadValue1  = 0;
int Color3ReadValue2  = 0;
int Color3ReadValue3  = 0;
int Krichi1ReadValue1  = 0;
int Krichi1ReadValue2  = 0;
int Krichi1ReadValue3  = 0;
int Krichi2ReadValue1  = 0;
int Krichi2ReadValue2  = 0;
int Krichi2ReadValue3  = 0;
int Krichi3ReadValue1  = 0;
int Krichi3ReadValue2  = 0;
int Krichi3ReadValue3  = 0;
int FullCopReadValue1  = 0;
int FullCopReadValue2  = 0;
int FullCopReadValue3  = 0;

int RecoverdColor1Count  = 0;
int RecoverdColor2Count  = 0;
int RecoverdColor3Count  = 0;
int RecoverdKrichi1Count  = 0;
int RecoverdKrichi2Count  = 0;
int RecoverdKrichi3Count  = 0;
int RecoverdFullCopCount = 0;



/*shutter 3 declaration*/

/*menu*/
bool InsideClearFlag = false;
bool InsideMenuFlag = false;
bool InsideOption1Flag = false;
bool InsideOption2Flag = false;
bool InsideOption3Flag = false;
bool Flag140 ;
bool Flag160 ;
bool Flag180 ;
bool Flag200 ;
bool BobbinMixupflag ;
bool ColorSensorOption ;

char customKey;
bool KeyPressFlag = false;
char functionParameter = '\0';
unsigned long MenuTimer = 0; 


bool CountDisplayFlag = true;
bool KrichiCountZeroUpdateFlag = false;




// color 2 simulator logic 
unsigned long Color2simulatorPreviousPulse = 0;
int Color2pulsedelay = 16;
bool Color2simulatorflag = false;
int Color2index = 0;
int Color2indexpresent = 0;
int Color2simTrainarray[100] ;
bool Color2detected = false;
int Color2PresentOutput = 0;

/*to store previous values */
int PreviousBinSize;
int PreviousBobbinMix;
int PreviousSensorOption;



/*object 2 simulator*/
unsigned long Object2simulatorPreviousPulse = 0;
//to adjust Object2 pulse delay
int Object2pulsedelay = 16;
bool Object2simulatorflag = false;
bool Object1flag1 =  false;
int Object2index = 0;
int Object2indexpresent = 0;
int Object2simTrainarray[100] ;
bool Object1detected = false;
int Object2PresentOutput = 0;
 
void setup() {
  
  Serial.begin(9600);//bits per second
  
  // initialize the LCD
  lcd.init(); 
  // Turn on the blacklight.
  lcd.backlight();
  pinMode(beltDetection, INPUT);
  pinMode(fullCopDetection, INPUT);
  pinMode(color1, INPUT);
  pinMode(color2, INPUT);
  pinMode(color3, INPUT);
  pinMode(obj1, INPUT);
  pinMode(obj2, INPUT);
  pinMode(obj3, INPUT);
  pinMode(yarn1, INPUT);
  pinMode(crate1, INPUT);
  pinMode(crate2, INPUT);
  pinMode(crate3, INPUT);

  pinMode(ccb,OUTPUT);
  pinMode(vcMotor,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(binLock2,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(s4,OUTPUT);
  pinMode(copStorage1,OUTPUT);
  pinMode(copStorage2,OUTPUT);
  pinMode(crateEjector1,OUTPUT);
  pinMode(crateEjector2,OUTPUT);
  pinMode(binUp1,OUTPUT);
  pinMode(binUp2,OUTPUT);
  pinMode(binLock1,OUTPUT);
  pinMode(alarmLightyellow,OUTPUT);
  
  pinMode(copStorage3,OUTPUT);
  pinMode(crateEjector3,OUTPUT);
  pinMode(binUp3,OUTPUT);
  pinMode(binLock3,OUTPUT);
  
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
  
  delay(50);

  lcd.setCursor(0,0);
  lcd.print("TB1=");
  lcd.setCursor(10,0);
  lcd.print("TB2=");
  lcd.setCursor(0,1);
  lcd.print("RB1=");
  lcd.setCursor(10,1);
  lcd.print("RB2=");

  delay(50);

  Color1ReadValue1=EEPROM.read(0);  //Color1UpdatedValue1
  Color1ReadValue2=EEPROM.read(1);  //Color1UpdatedValue2
  Color1ReadValue3=EEPROM.read(2);  //Color1UpdatedValue3
  RecoverdColor1Count=((Color1ReadValue3)+(Color1ReadValue2*255)+(Color1ReadValue1*(255*255)));
  color1Cnt = RecoverdColor1Count;
  lcd.setCursor(4,0);
  lcd.print(color1Cnt);

  Color2ReadValue1=EEPROM.read(3);  //Color2UpdatedValue1
  Color2ReadValue2=EEPROM.read(4);  //Color2UpdatedValue2
  Color2ReadValue3=EEPROM.read(5);  //Color2UpdatedValue3
  RecoverdColor2Count=((Color2ReadValue3)+(Color2ReadValue2*255)+(Color2ReadValue1*(255*255)));
  color2Cnt = RecoverdColor2Count;
  lcd.setCursor(14,0);
  lcd.print(color2Cnt);
  
  Color3ReadValue1=EEPROM.read(6);  //Color3UpdatedValue1
  Color3ReadValue2=EEPROM.read(7);  //Color3UpdatedValue2
  Color3ReadValue3=EEPROM.read(8);  //Color3UpdatedValue3
  RecoverdColor3Count=((Color3ReadValue3)+(Color3ReadValue2*255)+(Color3ReadValue1*(255*255)));
  color3Cnt = RecoverdColor3Count;
//  lcd.setCursor(4,1);
//  lcd.print(color3Cnt);


  Krichi1ReadValue1=EEPROM.read(9);  //Krichi1UpdatedValue1
  Krichi1ReadValue2=EEPROM.read(10);  //Krichi1UpdatedValue2
  Krichi1ReadValue3=EEPROM.read(11);  //Krichi1UpdatedValue3
  RecoverdKrichi1Count=((Krichi1ReadValue3)+(Krichi1ReadValue2*255)+(Krichi1ReadValue1*(255*255)));
  Krichi1Cnt = RecoverdKrichi1Count;
 



  Krichi2ReadValue1=EEPROM.read(12);  //Krichi2UpdatedValue1
  Krichi2ReadValue2=EEPROM.read(13);  //Krichi2UpdatedValue2
  Krichi2ReadValue3=EEPROM.read(14);  //Krichi2UpdatedValue3
  RecoverdKrichi2Count=((Krichi2ReadValue3)+(Krichi2ReadValue2*255)+(Krichi2ReadValue1*(255*255)));
  Krichi2Cnt = RecoverdKrichi2Count;
 
  
  Krichi3ReadValue1=EEPROM.read(15);  //Krichi3UpdatedValue1
  Krichi3ReadValue2=EEPROM.read(16);  //Krichi3UpdatedValue2
  Krichi3ReadValue3=EEPROM.read(17);  //Krichi3UpdatedValue3
  RecoverdKrichi3Count=((Krichi3ReadValue3)+(Krichi3ReadValue2*255)+(Krichi3ReadValue1*(255*255)));
  Krichi3Cnt = RecoverdKrichi3Count;
 
  krichiCnt  =  Krichi1Cnt + Krichi2Cnt + Krichi3Cnt;
  lcd.setCursor(14,1);
  lcd.print(krichiCnt);

  FullCopReadValue1=EEPROM.read(21);  //Color1UpdatedValue1
  FullCopReadValue2=EEPROM.read(22);  //Color1UpdatedValue2
  FullCopReadValue3=EEPROM.read(23);  //Color1UpdatedValue3
  RecoverdFullCopCount=((FullCopReadValue3)+(FullCopReadValue2*255)+(FullCopReadValue1*(255*255)));
  FullCopCount = RecoverdFullCopCount;
  lcd.setCursor(4,1);
  lcd.print(FullCopCount);



  //multiple error
  timer = millis();

/*previous menu value**/

PreviousBinSize       = EEPROM.read(18);  
PreviousBobbinMix     = EEPROM.read(19);  
PreviousSensorOption  = EEPROM.read(20);  

if (PreviousBinSize == 0)
{
  Flag140 = true;
  Flag160 = false;
  Flag180 = false;
  Flag200 = false;
  
  }
if (PreviousBinSize == 1)
{
  Flag140 = false;
  Flag160 = true;
  Flag180 = false;
  Flag200 = false;
  }
if (PreviousBinSize == 2)
{
  Flag140 = false;
  Flag160 = false;
  Flag180 = true;
  Flag200 = false;
  }
if (PreviousBinSize == 3)
{
  Flag140 = false;
  Flag160 = false;
  Flag180 = false;
  Flag200 = true;
  }


if (PreviousBobbinMix == 0)
{
  BobbinMixupflag = false;
  }
if (PreviousBobbinMix == 1)
{
  BobbinMixupflag = true;
  }


if (PreviousSensorOption == 0)
{
  ColorSensorOption = false;
  }
if (PreviousSensorOption == 1)
{
  ColorSensorOption = true;
  }
  

}



void loop(){ 

if (Initializationflag = true and InitCopStorageFlag == false )
{
  digitalWrite(copStorage1,HIGH); 
  digitalWrite(copStorage2,HIGH);
  digitalWrite(copStorage3,HIGH);
  InitCopStorageFlag = true;
   
  InitCopStorageTimer2 = millis();
  InitCopStorageFlag2 = true;

  Serial.println("cop storage  high at setup");
}
  if( millis() - InitCopStorageTimer2 > InitCopStorageOffDelay*50 and InitCopStorageFlag2 ==  true)
{
  digitalWrite(copStorage1,LOW);
  digitalWrite(copStorage2,LOW);
  digitalWrite(copStorage3,LOW);
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
  digitalWrite(crateEjector3,HIGH);
  InitCrateEjectionFlag1 = false;
    
  InitCrateEjectionTimer2 = millis();
  InitCrateEjectionFlag2 = true;

  Serial.println("crate eject high at setup");

}

  if( millis() - InitCrateEjectionTimer2 > (InitCrateRemoveOnDelay * 50) and InitCrateEjectionFlag2 == true)
{
  digitalWrite(crateEjector1,LOW);
  digitalWrite(crateEjector2,LOW);
  digitalWrite(crateEjector3,LOW);
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
  digitalWrite(binUp3,HIGH);
  InitTimer1BinUp1 = millis();
  InitBinUpFlag1 = true;
}

  if(millis() - InitTimer1BinUp1 > 1000  and InitBinUpFlag1 == true)
{
   digitalWrite(binLock1,HIGH);
   digitalWrite(binLock2,HIGH);
   digitalWrite(binLock3,HIGH);

}

  if(millis() - InitTimer1BinUp1 > 2400 and InitBinUpFlag1 == true)
{
  digitalWrite(binLock1,LOW);
  digitalWrite(binLock2,LOW);
  digitalWrite(binLock3,LOW);
 
}
  if(millis() - InitTimer1BinUp1 > 2000 and InitBinUpFlag1 == true)
{
  digitalWrite(binUp1,LOW);
  digitalWrite(binUp2,LOW);
  digitalWrite(binUp3,LOW);   
}
  if(millis() - InitTimer1BinUp1 > 4000 and InitBinUpFlag1 == true)
{
  InitCrateReplacementFlag = false;
  InitCrateComplete =false;
  InitCrateProcessFlag =false;
  InitBinUpFlag1 = false;
  Initializationflag = false;
  

}


/*bin size*/
if( (Flag140 == true)  and (Flag160 == false)  and (Flag180 == false)  and (Flag200 == false) ) 
{
Serial.println("140 Bin size");
c1Cntreset = 35;
numCopStorage1 = 4; 
numCopStorage2 = 4;
numCopStorage3 = 4;
Flag140 = false;
}

if( (Flag140 == false)  and (Flag160 == true)  and (Flag180 == false)  and (Flag200 == false) ) 
{
Serial.println("160 Bin size");
c1Cntreset = 40;
numCopStorage1 = 4; 
numCopStorage2 = 4;
numCopStorage3 = 4;
Flag160 = false;
}

if( (Flag140 == false)  and (Flag160 == false)  and (Flag180 == true)  and (Flag200 == false) ) 
{
Serial.println("180 Bin size");
c1Cntreset = 45;
numCopStorage1 = 4; 
numCopStorage2 = 4;
numCopStorage3 = 4;
Flag180 = false;
}

if( (Flag140 == false)  and (Flag160 == false)  and (Flag180 == false)  and (Flag200 == true) ) 
{
Serial.println("200 Bin size");
c1Cntreset = 40;
numCopStorage1 = 5; 
numCopStorage2 = 5;
numCopStorage3 = 5;
Flag200 = false;
}


/*To store in EEPROM*/
currentMillis = millis();
if ((currentMillis - previousMillis)  >=  interval) 
 {
  previousMillis += interval;

  int Color1UpdatedValue1  = (color1Cnt/(255*255));
  int b1  = Color1UpdatedValue1*255*255;

  int c1  = color1Cnt-b1;
  int d1 = c1/255;

  int Color1UpdatedValue2 = d1 * 255;
  int Color1UpdatedValue3 = c1 - Color1UpdatedValue2;

  EEPROM.update(0,Color1UpdatedValue1);
  EEPROM.update(1,Color1UpdatedValue2);
  EEPROM.update(2,Color1UpdatedValue3);


  int Color2UpdatedValue1  = (color2Cnt/(255*255));
  int b2  = Color2UpdatedValue1*255*255;

  int c2  = color2Cnt-b2;
  int d2 = c2/255;

  int Color2UpdatedValue2 = d2 * 255;
  int Color2UpdatedValue3 = c2 - Color2UpdatedValue2;

  EEPROM.update(3,Color2UpdatedValue1);
  EEPROM.update(4,Color2UpdatedValue2);
  EEPROM.update(5,Color2UpdatedValue3);

  int Color3UpdatedValue1  = (color3Cnt/(255*255));
  int b3  = Color3UpdatedValue1*255*255;

  int c3  = color3Cnt-b3;
  int d3 = c3/255;

  int Color3UpdatedValue2 = d3 * 255;
  int Color3UpdatedValue3 = c3 - Color3UpdatedValue2;

  EEPROM.update(6,Color3UpdatedValue1);
  EEPROM.update(7,Color3UpdatedValue2);
  EEPROM.update(8,Color3UpdatedValue3);

  int Krichi1UpdatedValue1  = (Krichi1Cnt/(255*255));
  int b4  = Krichi1UpdatedValue1*255*255;

  int c4  = Krichi1Cnt-b4;
  int d4 = c4/255;

  int Krichi1UpdatedValue2 = d4 * 255;
  int Krichi1UpdatedValue3 = c4 - Krichi1UpdatedValue2;

  EEPROM.update(9,Krichi1UpdatedValue1);
  EEPROM.update(10,Krichi1UpdatedValue2);
  EEPROM.update(11,Krichi1UpdatedValue3);


  int Krichi2UpdatedValue1  = (Krichi2Cnt/(255*255));
  int b5  = Krichi2UpdatedValue1*255*255;

  int c5  = Krichi2Cnt-b5;
  int d5 = c5/255;

  int Krichi2UpdatedValue2 = d5 * 255;
  int Krichi2UpdatedValue3 = c5 - Krichi2UpdatedValue2;

  EEPROM.update(12,Krichi2UpdatedValue1);
  EEPROM.update(13,Krichi2UpdatedValue2);
  EEPROM.update(14,Krichi2UpdatedValue3);



  int Krichi3UpdatedValue1  = (Krichi3Cnt/(255*255));
  int b6  = Krichi3UpdatedValue1*255*255;

  int c6  = Krichi3Cnt-b6;
  int d6 = c6/255;

  int Krichi3UpdatedValue2 = d6 * 255;
  int Krichi3UpdatedValue3 = c6 - Krichi3UpdatedValue2;

  EEPROM.update(15,Krichi3UpdatedValue1);
  EEPROM.update(16,Krichi3UpdatedValue2);
  EEPROM.update(17,Krichi3UpdatedValue3);

  int FullCopUpdatedValue1  = (FullCopCount/(255*255));
  int b7  = FullCopUpdatedValue1*255*255;

  int c7  = FullCopCount-b7;
  int d7 = c7/255;

  int FullCopUpdatedValue2 = d7 * 255;
  int FullCopUpdatedValue3 = c7 - FullCopUpdatedValue2;

  EEPROM.update(21,FullCopUpdatedValue1);
  EEPROM.update(22,FullCopUpdatedValue2);
  EEPROM.update(23,FullCopUpdatedValue3);

}


//multiple error
  if((millis()-timer) > timerInterval)
  {
    printErrorStatus = printError();
    timer = millis();
    if(printErrorStatus == true)
    {
     timerInterval = 1000;
    }
    else
    {
      ErrorCycle = false;
      timerInterval = 0;
    }
    
  }

//Cop Collecting Box
  if ((millis() - timerccb) > 2000)
  {
    loaderState = !loaderState;
    digitalWrite(ccb, loaderState);
    timerccb = millis();
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
    BeltError = true;
    Serial.println("Vertical belt error occured");
    errorB = true;
  }

/*error for belt*/
if (errorB == true and errorflag == false)
  {
  //this is both horizontal and vertical
  digitalWrite(vcMotor,LOW);
  errorflag = true;
  }
   
if (errorB == false)
  {
  //this is both horizontal and vertical
  digitalWrite(vcMotor,HIGH);
  }

/*full bobin ejection*/
/**************Full bobbin Sensor error**************/
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
    FullCopError  = true;
    Serial.println("Full bobbin error occured");
    errorFC = true;
    errorB = true;
  }




//Full Cop detection
  if (digitalRead(fullCopDetection) == true and togglefc == false)
  {
    fullcopBeltSeperationTimerStart= millis();
    //Serial.println("full cop sens on");
    togglefc = true;
  }
  
  if (digitalRead(fullCopDetection) == false and togglefc == true )
  { 
    fullcopBeltSeperationTimerEnd = millis();
    //Serial.println("full cop sens off");
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
  FullCopCountFlag = true; 
  }
  else
  {
  digitalWrite(s1,LOW);
   if(FullCopCountFlag == true)
  {
    FullCopCount++;
    FullCopCountFlag = false;
    }
      if(CountDisplayFlag == true)
{
  lcd.setCursor(4,1);
  lcd.print(FullCopCount);
}
  }

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
if (digitalRead(obj1) == false and objerrflag == false)
  {
    objerrtimer = millis();
    objerrflag1 = true;
    objerrflag = true;
    
  }

if (digitalRead(obj1) == true )
  {
    objerrflag = false;
    objerrflag1 = false;
  }
   
if ((millis() - objerrtimer > (objerrtimerondelay * 50) and objerrflag1 == true) )
  { 
    Object1ErrorOccuredFlag = true;
    Serial.println("Object 1 sensor error occured");
    error = true;
   
  }



//color1 error
  if (digitalRead(color1) == false and colorerrflag == false)
    {
      colorerrtimer = millis();
      colorerrflag1 = true;
      colorerrflag = true;
     
    }

  if (digitalRead(color1) == true )
    {
      colorerrflag = false;
      colorerrflag1 = false;
    }
   
  
  if ((millis() - colorerrtimer > (colorerrtimerondelay * 50) and colorerrflag1 == true) )
    { 
      Color1ErrorOccuredFlag = true;
      Serial.println("color 1 sensor error occured");
      error = true;
      
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
    YarnErrorOccuredFlag = true;
    Serial.println("Yarn sensor error occured");
    error = true;
   
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
      
      yarn2indexpresent = yarn2index - yarn2pulsedelay;
      if (yarn2indexpresent < 0)
      {
        yarn2indexpresent = yarn2indexpresent + 100 ;
      }

      yarn2PresentOutput = yarn2simTrainarray[yarn2indexpresent] ;
      if (yarn2indexpresent < 99 and yarn2indexpresent > 0 )
      {
      yarn2PresentOutput = yarn2simTrainarray[yarn2indexpresent] +
                            yarn2simTrainarray[yarn2indexpresent - 1] +
                            yarn2simTrainarray[yarn2indexpresent + 1];
      }
      yarn2index = (yarn2index+1) % 100 ;
      yarn2simulatorPreviousPulse = millis();
      yarn2simulatorflag = false;
      
    }

/****************************Color 1 identification********************************/
 if(digitalRead(color1) == false and  color1flag == false) //clr1false
 {
  color1flag = true;
  color1identified = true;
  Serial.println("colour1 identified");
  s2on = true;
}   
/****************************Object 1 identification********************************/
// if(digitalRead(obj1) == false and Object1flag1 == false  )
//    {
//    Object1Detected = true;//only used inside loop
//    Object1flag1 = true;
//    Shutter2OnReady = false;
//    Serial.println("Object1 identified");
//    color1flag = false;
//    color1identified = false;
//    if (ColorSensorOption == true)
//    { 
//    Color2flag = false;
//    }
//    Object2simulatorflag = true;
//   }
//if( (millis() - Object2simulatorPreviousPulse) >50 )
//    {
//      if( Object2simulatorflag == true)
//      {
//       Object2simTrainarray[Object2index] = 1;
//      }
//      if( Object2simulatorflag == false)
//      {
//       Object2simTrainarray[Object2index] = 0;
//      }
//      
//      Object2indexpresent = Object2index - Object2pulsedelay;
//      if (Object2indexpresent < 0)
//      {
//        Object2indexpresent = Object2indexpresent + 100 ;
//      }
//
//      Object2PresentOutput = Object2simTrainarray[Object2indexpresent] ;
//      if (Object2indexpresent < 99 and Object2indexpresent > 0 )
//      {
//      Object2PresentOutput = Object2simTrainarray[Object2indexpresent] +
//                            Object2simTrainarray[Object2indexpresent - 1] +
//                            Object2simTrainarray[Object2indexpresent + 1];
//      }
//      Object2index = (Object2index+1) % 100 ;
//      Object2simulatorPreviousPulse = millis();
//      Object2simulatorflag = false;
//      }
//
//

  if(digitalRead(obj1) == false  and Object1flag1 == false)
  {
    Object1Detected = true;//only used inside loop
    Object1flag1 = true;
    Shutter2OnReady = false;
    Serial.println("Object1 identified");
   // yarn1flag = false;
    color1flag = false;
    color1identified = false;
  if (ColorSensorOption == true)
  { 
    Color2flag = false;
  }
  }
//TO TRIGGER SHUTTER2 for color1COP
  if (Object1flag1 == true and digitalRead(obj1) == true and color1identified == true and yarn1flag == false)
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
      Serial.println("trigger s2 for c1 cop");
      }
  
  //Passing Shutter2 - cop must be 2nd or 3rd color    
     if (Object1flag1 == true and digitalRead(obj1) == true and color1identified == false and yarn1flag == false) 

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
if(Object1flag1 == true and digitalRead(obj1) == true and yarn1flag == true)
    {
      Shutter2Onflag1 = false;
      
      krichi1 =  true;
      Serial.println("Color1 krichi");
      
      if (krichi1==true)
      {
      Krichi1Cnt++;
      Serial.print("krichi 1=");
      Serial.println(Krichi1Cnt);
      krichiCntFlag=true;
      krichi1==false;
      }   
      
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
    if   (Object1flag1 == true and digitalRead(obj1) == true and color1identified == true and yarn1flag == true)
    {
      Shutter2Onflag1 = false;
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false;
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      
 
}

//Shutter2 ON flag
    if (Shutter2Onflag1 == true and Shutter2Onflag2 == false and Tray1EmptyFlag == false and Tray2EmptyFlag == false  and  Tray3EmptyFlag == false   )
        {
          Shutter2OnReady = true;
          Shutter2Onflag2 = true;
          Shutter2Onflag1 = false;   
        }

     if (C1CountFlag1 == false)
     {
      Shutter2Onflag2 = false;
     }

// ****************************Color 1 Ejection********************************//
if( (millis()-Object1DetectTmr) > (s2OnDelay*1) and (millis()-Object1DetectTmr) < (s2OnDelay+s2OffDelay)*50 and Shutter2OnReady == true and Tray1EmptyFlag == false and Tray2EmptyFlag == false and  Tray3EmptyFlag == false  )//was*50
  
  {
  digitalWrite(s2,HIGH);
  Serial.println("C1CountFlag1 = true");
  C1CountFlag1 = true;
  }
else{
  digitalWrite(s2,LOW);
  C1CountFlag1 = false;
}



if (C1CountFlag1 == true and C1CountFlag2 == false)
{
  C1CountFlag2 = true;
  color1Cnt++;
  color1Cntflag++;
  Serial.println("inside color 1 loop");
  Serial.print("color 1 count=");
  Serial.println(color1Cnt);
if(CountDisplayFlag == true)
{  
  lcd.setCursor(4,0);
  lcd.print(color1Cnt);
}  
}

if (C1CountFlag1 == false)
{
  C1CountFlag2 = false;
}


if (color1Cntflag >=c1Cntreset and C1CountFlag1 == false)//40 //color 1 count resetted
{
  color1Cntflag = 0;
}

/************************************CopStorage 1*******************************/
if((color1Cnt>0) and ((color1Cnt %  c1Cntreset) ==  0 ) and flag1CopStorage1 == false and copstorage1loopflag == false) //40
{
  
 copstorage1loopflag = true;
  timer1CopStorage1 = millis(); 
  flag1CopStorage1 = true;
  copstorage1highflag = true;
  Serial.println("cop storage ready to high ");
  
}

if (millis() - timer1CopStorage1 > copStorage1Ondelay*50  and copstorage1highflag == true)//50
{
  digitalWrite(copStorage1,HIGH); 
  
  timer2CopStorage1 = millis();
  copstorage1Shutterflag1 = true;
  Serial.println("cop storage  high ");
  copstorage1highflag = false;
  copstorage1lowflag = true;
}
if( millis() - timer2CopStorage1 > copStorage1Offdelay*50 and copstorage1lowflag==true) //50
{
     digitalWrite(copStorage1,LOW);
     copstorage1Shutterflag1 = false;
     color1Cntflag = 0;
      flag2CopStorage1 = false;
      flag1CopStorage1 = false;
      copstorage1lowflag = false;
      
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




if (copStorage1Count >= numCopStorage1 and copstorage1Shutterflag1 == false)
{
  copStorage1Count = 0;
}

   /**************************************************crate error check********************************************/

if (digitalRead(crate1) == true and tray1check == false)
  {
    Tray1EmptyFlag = true;
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
  Serial.println("tray alert");
}
if (digitalRead(crate1) == true and crate1alertcheck == true)
{
  crate1error = true;
  Tray1EmptyFlag = true;
  Tray1Error = true;
  Tray1LightError = true;
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
  Serial.println("binup high");
  
 
}

if (binup1inerrorok1 == true and millis() - binup1inerrortime > 4000)
  {
    digitalWrite(binUp1,LOW);
    binup1inerrorok1 = false;
    binup1inerrortime2 = millis();
    binup1inerrorok2 = true;
  }
    

  if (binup1inerrorok2 == true and millis() - binup1inerrortime2 > 500)//was400
  {
    digitalWrite(binLock1,LOW);
      
    binup1inerrorok2 = false;
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

if( copStorage1Count >= numCopStorage1 and crate1error == false /*and digitalRead(crate1) == false*/)
{
  copStorage1Countflag = copStorage1Count;
}

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
      crate1Complete = true;
  }

/*****************************************************CrateReplacement************************************************************/
if( flag3CrateEjection1 == true and millis() - timer2CrateEjection1 > 3000 and crate1ProcessFlag == false)
{
  if( digitalRead(crate1) == false){
    crate1ProcessFlag =true;
    Crate1RepeatFlag = true;
  }
}

if(( crate1Complete == true  and crate1ProcessFlag == false and flag1binUp1 ==false) or (Crate1RepeatFlag == true)){
  digitalWrite(binUp1,HIGH);
  timer1binUp1 = millis();
  flag1binUp1 = true;
}

if(millis() - timer1binUp1 > 1000  and flag1binUp1 == true){
  digitalWrite(binLock1,HIGH);
 if( millis() - timer1binUp1 > 1000 and millis() - timer1binUp1 < 2000 and digitalRead(crate1) == false){
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
  Crate1RepeatFlag = false;
  

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
if (digitalRead(obj2) == false and obj2errflag == false)
  {
    obj2errtimer = millis();
    obj2errflag2 = true;
    obj2errflag = true;  
  }

if (digitalRead(obj2) == true)
  {
    obj2errflag = false;
    obj2errflag2 = false;
  }
   
  
if ((millis() - obj2errtimer > (obj2errtimerondelay * 50) and obj2errflag2 == true) )
  { 
    Object2ErrorOccuredFlag = true;
    Serial.println("Object2 error occured");
    errors3 = true;
    
  }




  //Color2 Sensor Error
  if (digitalRead(color2) == false and color2errflag == false)
    {
      color2errtimer = millis();
      color2errflag2 = true;
      color2errflag = true;
      //objerrtimer1 = millis();
    }

  if (digitalRead(color2) == true )
    {
      color2errflag = false;
      color2errflag2 = false;
    }
   
  
  if ((millis() - color2errtimer > (color2errtimerondelay * 50) and color2errflag2 == true) )
    { 
      Color2ErrorOccuredFlag = true;
      errors3 = true;
    }

//yarn2 error
  if (digitalRead(yarn2) == false and yarn2errflag == false)
  {
    yarn2errtimer = millis();
    yarn2errflag = true;
    yarn2errflag1 = true;
  }

  if (digitalRead(yarn2) == true )
    {
      yarn2errflag = false;
      yarn2errflag1 = false;
    }
    
  if ((millis() - yarn2errtimer > (yarn2errtimerondelay * 50) and yarn2errflag1 == true) )
  { 
    Yarn2ErrorOccuredFlag = true;
    Serial.println("Yarn2 sensor error occured");
    error = true;
   
  }    
/*s3error ends*/
// ****************************Yarn 2 identification********************************//

if((yarn2PresentOutput >= 1 and yarn2flag == false and digitalRead(obj2) == false ) or (digitalRead(yarn2) == false and yarn2flag == false))
{
    timeryarn2 = millis();
    yarn2flag = true;
    Serial.println("yarn2 came");
}

/***************************Color 2 identification********************************/
if(digitalRead(color2) == false and  Color2flag == false and ColorSensorOption == false) 
 {
  Color2flag = true;
  color2identified = true;
  Serial.println("colour2 identified");
  s3on = true;
}


/*lueze color*/

 if(digitalRead(color2) == false and Color2flag == false and ColorSensorOption == true )
    {
    Color2flag = true;
    Serial.println("Color2 identified");
    Color2simulatorflag = true;
    }

  if( (millis() - Color2simulatorPreviousPulse) >50 )
    {
      if( Color2simulatorflag == true)
      {
       Color2simTrainarray[Color2index] = 1;
      }
      if( Color2simulatorflag == false)
      {
       Color2simTrainarray[Color2index] = 0;
      }
      
      Color2indexpresent = Color2index - Color2pulsedelay;
      if (Color2indexpresent < 0)
      {
        Color2indexpresent = Color2indexpresent + 100 ;
      }

      Color2PresentOutput = Color2simTrainarray[Color2indexpresent] ;
      if (Color2indexpresent < 99 and Color2indexpresent > 0 )
      {
      Color2PresentOutput = Color2simTrainarray[Color2indexpresent] +
                            Color2simTrainarray[Color2indexpresent - 1] +
                            Color2simTrainarray[Color2indexpresent + 1];
      }
      Color2index = (Color2index+1) % 100 ;
      Color2simulatorPreviousPulse = millis();
      Color2simulatorflag = false;
      }
if(Color2PresentOutput >= 1 and color2identified == false and digitalRead(obj2) == false ) 
{
    color2identified = true;
    Serial.println("Color2 came");
}

    

// ****************************Object 2 identification********************************//

///*object simulator @ S3*/
//if((Object2PresentOutput >= 1 and Object2flag1 == false) or (digitalRead(obj2) == false and Object2flag1 == false))
//{
//    Object2Detected = true;//only used inside loo
//    Object2flag1 = true;
//    Shutter3OnReady = false;
//    Serial.println("Object2 identified");
//    //yarn2flag = false;
//    
//    if (ColorSensorOption == false)
//    {
//    Color2flag = false;
//    color2identified = false;
//    }
//}

  if(digitalRead(obj2) == false  and Object2flag1 == false)
  {
    Object2Detected = true;//only used inside loo
    Object2flag1 = true;
    Shutter3OnReady = false;
    Serial.println("Object2 identified");
    //yarn2flag = false;
    
    if (ColorSensorOption == false)
    {
    Color2flag = false;
    color2identified = false;
    }
    
  }
//TO TRIGGER SHUTTER3 for color2COP
  if (Object2flag1 == true and digitalRead(obj2) == true and color2identified == true and yarn2flag == false)
      {
      Shutter3Onflag1 = true;
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      Color2flag = false;
      color2identified = false;
      
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("trigger s3 for c2 cop");
      }
  
  //Passing Shutter3 - cop must be 3rd color    
     if (Object2flag1 == true and digitalRead(obj2) == true and color2identified == false and yarn2flag == false) 

     {
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;

      Color2flag = false;
      color2identified = false;
      
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color3 empty Check");
    }
 //To find Yarn at Object2 sensor and use the information for shutter4     
if(Object2flag1 == true and digitalRead(obj2) == true and yarn2flag == true)
    {
      Shutter3Onflag1 = false;
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;

//      krichi2 =  true;
//      Serial.println("c2 krichi");



      Color2flag = false;
      color2identified = false;

      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color3 yarn Check");
    }
//Color 2 COP with Yarn (color 2 kirchi)
    if   (Object2flag1 == true and digitalRead(obj2) == true and color2identified == true and yarn2flag == true)
    {
      Shutter3Onflag1 = false;
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;

      Color2flag = false;
      color2identified = false;

      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      krichi2 =  true;
      Serial.println("c2 krichi");

      
}
//if (krichi2==true)
//{
//Krichi2Cnt++;
//Serial.print("krichi 2=");
//Serial.println(Krichi2Cnt);
//krichiCntFlag=true;
//krichi2==false;
//}   
//Shutter3 ON flag
    if (Shutter3Onflag1 == true and Shutter3Onflag2 == false and Tray1EmptyFlag == false and Tray2EmptyFlag == false  and  Tray3EmptyFlag == false  )
        {
          Shutter3OnReady = true;
          Shutter3Onflag2 = true;
          Shutter3Onflag1 = false;   
        }

     if (C2CountFlag1 == false)
     {
      Shutter3Onflag2 = false;
     }
/***************************Color 2 Ejection********************************/
if( (millis()-Object2DetectTmr) > (s3OnDelay*1) and (millis()-Object2DetectTmr) < (s3OnDelay+s3OffDelay)*50 and Shutter3OnReady == true and Tray1EmptyFlag == false and Tray2EmptyFlag == false and  Tray3EmptyFlag == false  )//was*50
  
  {
  digitalWrite(s3,HIGH);
  Serial.println("C2CountFlag1 = true");
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
  Serial.println("inside color 2 loop");
  color2Cntflag++;
  Serial.print("color 2 count=");
  Serial.println(color2Cnt);
  if(CountDisplayFlag == true)
{
  lcd.setCursor(14,0);
  lcd.print(color2Cnt);
} 
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

if (copStorage2Count >= numCopStorage2 and copstorage2Shutterflag1 == false)
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
if( copStorage2Count >= numCopStorage2 and crate2error == false /*and digitalRead(crate2) == false */)
{
  copStorage2Countflag = copStorage2Count;
}

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
if( flag3CrateEjection2 == true and millis() - timer2CrateEjection2 > 3000 and crate2ProcessFlag == false )
{
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


/*shutter2 end*/


/*shutter4 start*/

/*s4 error*/

if (errors4 == true and errorflag == false)
  {
    errors4occured = true;
    errorflag = true;
  }
 
  
if (errors4 == true and errors4occured == true)
  {
    errors4ontmr = millis();
    errors4occured = false;
    errors4occured1 = true;
    
  }

if (millis() - errors4ontmr > 1000 and errors4occured1 == true)
  {
    //digitalWrite(alarmLightyellow,HIGH);
    errors4offtmr = millis();
    errors4occuredoff = true;
    errors4occured1 = false;
  }
if (millis() - errors4offtmr > 200 and errors4occuredoff == true)
  {
    //digitalWrite(alarmLightyellow,LOW);
    errors4occuredoff = false;
    errorflag = false;

  }

//Object3 Sensor Error
if (digitalRead(obj3) == false and obj3errflag == false)
  {
    obj3errtimer = millis();
    obj3errflag2 = true;
    obj3errflag = true;  
  }

if (digitalRead(obj3) == true)
  {
    obj3errflag = false;
    obj3errflag2 = false;
  }
   
  
if ((millis() - obj3errtimer > (obj3errtimerondelay * 50) and obj3errflag2 == true) )
  { 
    Object3ErrorOccuredFlag = true;
    Serial.println("Object3 error occured");
    errors4 = true;
    
  }




  //color3 Sensor Error
  if (digitalRead(color3) == false and color3errflag == false)
    {
      color3errtimer = millis();
      color3errflag2 = true;
      color3errflag = true;
    }

  if (digitalRead(color3) == true )
    {
      color3errflag = false;
      color3errflag2 = false;
    }
   
  
  if ((millis() - color3errtimer > (color3errtimerondelay * 50) and color3errflag2 == true) )
    { 
      Color3ErrorOccuredFlag = true;
      errors4 = true;
    }

//yarn3 error
  if (digitalRead(yarn3) == false and yarn3errflag == false)
  {
    yarn3errtimer = millis();
    yarn3errflag = true;
    yarn3errflag1 = true;
  }

  if (digitalRead(yarn3) == true )
    {
      yarn3errflag = false;
      yarn3errflag1 = false;
    }
    
  if ((millis() - yarn3errtimer > (yarn3errtimerondelay * 50) and yarn3errflag1 == true) )
  { 
    Yarn3ErrorOccuredFlag = true;
    Serial.println("Yarn3 sensor error occured");
    error = true;
   
  }      
/*s4error ends*/


// ****************************Yarn  3 identification********************************//
    
    if(digitalRead(yarn3) == false and yarn3flag == false)
    {
    timeryarn3 = millis();
    yarn3flag = true;
    Serial.println("yarn3 came");
    }

// ****************************Color 3 identification********************************//

 if(digitalRead(color3) == false and  color3flag == false) 
 {
  color3flag = true;
  color3identified = true;
  Serial.println("colour3 identified");
  s4on = true;
}


// ****************************Object 3 identification********************************//

  if(digitalRead(obj3) == false  and Object3flag1 == false)
  {
    Object3Detected = true;//only used inside loop
    Object3flag1 = true;
    Shutter4OnReady = false;
    Serial.println("Object3 identified");
    yarn3flag = false;
    color3flag = false;
    color3identified = false;
  }
//TO TRIGGER SHUTTER4 for color3COP

  
  if (Object3flag1 == true and digitalRead(obj3) == true and color3identified == true and yarn3flag == false and BobbinMixupflag == false)
{
      Shutter4Onflag1 = true;
      Object3DetectTmr = millis();
      yarn3flag = false;
      yarn3detected = false;
      color3flag = false;
      color3identified = false;
      s4on = false;
      Object3Detected = false;
      Object3flag1 = false;
      Serial.println("color3 eject");
      }
  if (Object3flag1 == true and digitalRead(obj3) == true and yarn3flag == false and BobbinMixupflag == true)
      
      {
      Shutter4Onflag1 = true;
      Object3DetectTmr = millis();
      yarn3flag = false;
      yarn3detected = false;
      color3flag = false;
      color3identified = false;
      s4on = false;
      Object3Detected = false;
      Object3flag1 = false;
      Serial.println("bobbin mixup true");
      }
  
  //Passing Shutter4 -    
     if (Object3flag1 == true and digitalRead(obj3) == true and color3identified == false and yarn3flag == false) 

     {
      Shutter4Onflag1 = false;
      Object3DetectTmr = millis();
      yarn3flag = false;
      yarn3detected = false;
      color3flag = false;
      color3identified = false;
      s4on = false;
      Object3Detected = false;
      Object3flag1 = false;
      Serial.println("Color4 empty Check");
    }
 //To find Yarn at Object3 sensor and use the information      
//if(Object3flag1 == true and digitalRead(obj3) == true and color3identified == false and yarn3flag == true)
if(Object3flag1 == true and digitalRead(obj3) == true and yarn3flag == true)
    {
      Shutter4Onflag1 = false;
//       krichi3 =  true;
//      Serial.println("color3 yarn Rested");
     
      Object3DetectTmr = millis();
      yarn3flag = false;
      yarn3detected = false;
      color3flag = false;
      color3identified = false;
      s4on = false;
      Object3Detected = false;
      Object3flag1 = false;
      krichi3 =  true;
      Serial.println("Color3 yarn Check");
    }
//Color 3 COP with Yarn (color 3 kirchi)
    if   (Object3flag1 == true and digitalRead(obj3) == true and color3identified == true and yarn3flag == true  )
    {
      Shutter4Onflag1 = false;   
      Object3DetectTmr = millis();
      yarn3flag = false;
      yarn3detected = false;
      color3flag = false;
      color3identified = false;
      s4on = false;
      Object3Detected = false;
      Object3flag1 = false;
      
}
//if (krichi3==true)
//{
//Krichi3Cnt++;
//Serial.print("krichi 3=");
//Serial.println(Krichi3Cnt);
//krichiCntFlag=true;
//krichi3==false;
//}
   
//Shutter4 ON flag
    if (Shutter4Onflag1 == true and Shutter4Onflag2 == false and Tray1EmptyFlag == false and Tray2EmptyFlag == false and  Tray3EmptyFlag == false  )
        {
          Shutter4OnReady = true;
          Shutter4Onflag2 = true;
          Shutter4Onflag1 = false;   
        }

     if (C3CountFlag1 == false)
     {
      Shutter4Onflag2 = false;
     }
     
/****************************Color 3 Ejection********************************/
if( (millis()-Object3DetectTmr) > (s4OnDelay*1) and (millis()-Object3DetectTmr) < (s4OnDelay+s4OffDelay)*50 and Shutter4OnReady == true and Tray1EmptyFlag == false and Tray2EmptyFlag == false and  Tray3EmptyFlag == false  )//was*50
  
  {
  digitalWrite(s4,HIGH);
  C3CountFlag1 = true;
  }
else{
  digitalWrite(s4,LOW);
  C3CountFlag1 = false;
}

if (C3CountFlag1 == true and C3CountFlag2 == false)
{
  C3CountFlag2 = true;
  color3Cnt++;
  Serial.println("inside color 3 loop");
  color3Cntflag++;
  Serial.print("color 3 count=");
  Serial.println(color3Cnt);
  if(CountDisplayFlag == true)
{
//  lcd.setCursor(4,1);
//  lcd.print(color3Cnt);
} 
}

if (C3CountFlag1 == false)
{
  C3CountFlag2 = false;
}


if (color3Cntflag >=c1Cntreset and C3CountFlag1 == false)//40 //color 1 count resetted
{
  color3Cntflag = 0;
}

/************************************CopStorage 3*******************************/

if((color3Cnt>0) and ((color3Cnt %  c1Cntreset) ==  0 ) and flag1CopStorage3 == false and CopStorage3loopflag == false) //40
{
  CopStorage3loopflag = true;
  timer1CopStorage3 = millis(); 
  flag1CopStorage3 = true;
  CopStorage3highflag = true;
  Serial.println("cop storage 3 ready to high ");
  
}

if (millis() - timer1CopStorage3 > CopStorage3Ondelay*50  and CopStorage3highflag == true)
{
  digitalWrite(copStorage3,HIGH); 
  
  timer2CopStorage3 = millis();
  CopStorage3Shutterflag1 = true;
  Serial.println("cop storage  high ");
  CopStorage3highflag = false;
  CopStorage3lowflag = true;
}
if( millis() - timer2CopStorage3 > CopStorage3Offdelay*50 and CopStorage3lowflag==true){
     digitalWrite(copStorage3,LOW);
     CopStorage3Shutterflag1 = false;
     color3Cntflag = 0;
      flag2CopStorage3 = false;//false
      flag1CopStorage3 = false;//false
      CopStorage3lowflag = false;
      
  }

if (  ((color3Cnt %  c1Cntreset) !=  0 ) and CopStorage3loopflag == true and flag1CopStorage3 == false)
{
CopStorage3loopflag = false;
}

if (CopStorage3Shutterflag1 == true and CopStorage3Shutterflag2 == false)
{
  CopStorage3Shutterflag2 = true;
  CopStorage3Count++ ;
  Serial.print("CopStorage3Count=");
  Serial.println(CopStorage3Count);
}
if (CopStorage3Shutterflag1 == false)
{
  CopStorage3Shutterflag2 = false;
}
if (CopStorage3Count >= numCopStorage3 and CopStorage3Shutterflag1 == false)
{
  CopStorage3Count = 0;
}

/**************************************************crate 3 error check********************************************/

if (digitalRead(crate3) == true and Tray3check == false)
  {
    Tray3EmptyFlag = true;
    crate3alert = millis();
    Tray3check = true;
    Tray3checkflag = true;
    
    Serial.println("no Tray3");
  }

  if (digitalRead(crate3) == false )
  {
    Tray3EmptyFlag = false;
    Tray3Error = false;
    Tray3LightError = false;
    Tray3check = false;
    Tray3checkflag = false;
  }
if (millis() - crate3alert > crate3alertondelay*50 and Tray3checkflag == true)
{
  crate3alertcheck = true;
  Serial.println("Tray3 alert");
}
if (digitalRead(crate3) == true and crate3alertcheck == true)
{
  Tray3EmptyFlag = true;
  Tray3Error = true;
  Tray3LightError = true;
  crate3error = true;
  digitalWrite(binLock3,HIGH);
  Serial.println("binlock 3 high");
}

if (crate3error == true and digitalRead(crate3) == false) 
{
  digitalWrite(binUp3,HIGH);
  binUp3inerrortime = millis();
  binUp3inerrorok1 = true;
  crate3alertcheck = false;
  crate3error = false;
  Serial.println("binUp3 high");
  
 
}

if (binUp3inerrorok1 == true and millis() - binUp3inerrortime > 4000)
  {
    digitalWrite(binUp3,LOW);
    binUp3inerrorok1 = false;
    binUp3inerrortime2 = millis();
    binUp3inerrorok2 = true;
  }
    

  if (binUp3inerrorok2 == true and millis() - binUp3inerrortime2 > 500)//was400
  {
    digitalWrite(binLock3,LOW);
      
    binUp3inerrorok2 = false;
    if(digitalRead(crate3) == false)
      {
       Tray3EmptyFlag = false;
       Tray3Error = false;
       Tray3LightError = false;
       Tray3check = false;
       Tray3checkflag = false;
    }
  }
/*************************************************crate3Ejection********************************************/

if( CopStorage3Count >= numCopStorage3 and crate3error == false /*and digitalRead(crate3) == false */)
{
  CopStorage3Countflag = CopStorage3Count;
}

  if( CopStorage3Countflag == numCopStorage3 and flag1CrateEjection3 ==false){
  timer1CrateEjection3 = millis();
  flag1CrateEjection3 = true;
  flag2CrateEjection3 = true;
  
  }

  if  (millis() - timer1CrateEjection3 > 2000 and flag2CrateEjection3 == true)
  {
  digitalWrite(crateEjector3,HIGH);
  Serial.println("crate eject 3 high");
  timer2CrateEjection3 = millis();
  flag3CrateEjection3 = true;
  flag2CrateEjection3 = false;
 }

if( millis() - timer2CrateEjection3 > (crate3RemoveOndelay * 50) and flag3CrateEjection3 == true){
      digitalWrite(crateEjector3,LOW);
      CopStorage3Countflag =0;
      flag1CrateEjection3 = false;
      flag3CrateEjection3 = false;
      crate3Complete = true;
  }


/*****************************************************crate3Replacement************************************************************/
if( flag3CrateEjection3 == true and millis() - timer2CrateEjection3 > 3000 and crate3ProcessFlag == false )
{
  if( digitalRead(crate3) == false)
  {
    crate3ProcessFlag =true;
  }
}

if( crate3Complete == true  and crate3ProcessFlag == false and flag1binUp3 ==false){
  digitalWrite(binUp3,HIGH);
  timer1binUp3 = millis();
  flag1binUp3 = true;
}

if(millis() - timer1binUp3 > 1000  and flag1binUp3 == true){
  digitalWrite(binLock3,HIGH);
 if( millis() - timer1binUp3 > 1000 and millis() - timer1binUp3 < 2000 and digitalRead(crate3) == true){

  }
}

if(millis() - timer1binUp3 > 2400 and flag1binUp3 == true){
  digitalWrite(binLock3,LOW);
 
}
if(millis() - timer1binUp3 > 2000 and flag1binUp3 == true){
  digitalWrite(binUp3,LOW);
  
}
if(millis() - timer1binUp3 > 4000 and flag1binUp3 == true){
 if( digitalRead(crate3) == true){
 //   errorCode="NoCrate";
  }

  crate3Complete =false;
  crate3ProcessFlag =false;
  flag1binUp3 = false;
}

/*shutter4 end*/

/*error reset*/
if (ResetSwitch == true and ((error == true) or (errorB == true) or (errors3 = true) or(errors4 = true) or (errorFC == true) or (Tray1LightError == true) or (Tray2LightError == true)or (Tray3LightError == true)))
  { 
    ResetSwitch = false;
    BeltError = false;
    FullCopError = false;
    Tray1Error  = false;
    Tray2Error  = false;
    Tray3Error  = false;

    YarnErrorOccuredFlag = false;
    Yarn2ErrorOccuredFlag = false;
    Yarn3ErrorOccuredFlag = false;

    Color1ErrorOccuredFlag = false;
    Color2ErrorOccuredFlag = false;
    Color3ErrorOccuredFlag = false;

    Object1ErrorOccuredFlag = false;
    Object2ErrorOccuredFlag = false;
    Object3ErrorOccuredFlag = false;
    

    Tray3LightError = false;
    Tray1LightError = false;
    Tray2LightError = false;
    error = false;
    errors3 = false;
    errors4 = false;
    errorB = false;
    errorFC = false;
    digitalWrite(alarmLightyellow,LOW);
    errorflag =false;
    errors3flag =false;
    objerrflag = false;
    objerrflag1 = false;
    obj2errflag = false;
    obj2errflag2 = false;
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("                    ");
    Serial.println("Error reset at loop");
}
/*error reset ends*/


/*krichi count add*/
if(krichiCntFlag  ==  true)
{
krichiCnt  =  Krichi1Cnt + Krichi2Cnt + Krichi3Cnt;

if(CountDisplayFlag == true)
{
lcd.setCursor(14,1);
lcd.print(krichiCnt);
krichiCntFlag=false;
}

}


/*menu keypad*/
customKey = customKeypad.getKey();

if (((millis() - MenuTimer) >= 500) and  KeyPressFlag == true )
{
Serial.println("function executed"+customKey);
Function(functionParameter);
KeyPressFlag = false;
functionParameter = '\0';
}

if (customKey != '\0' ) 
{
    Serial.println("keypressdflag true");  
    Serial.println(customKey);
    KeyPressFlag = true;
    MenuTimer = millis();
    functionParameter = customKey;
    customKey = '\0';
    
  }



//eeprom reset logic 
if (color1Cnt >= 99999) 
{
color1Cnt = 0;
if(CountDisplayFlag == true)
{ 
lcd.setCursor(4,0);
lcd.print(color1Cnt);
}
EEPROM.update(0,0);
EEPROM.update(1,0);
EEPROM.update(2,0);
}
if (color2Cnt >= 99999) 
{
color2Cnt = 0;
if(CountDisplayFlag == true)
{ 
lcd.setCursor(14,0);
lcd.print(color2Cnt);
}
EEPROM.update(3,0);
EEPROM.update(4,0);
EEPROM.update(5,0);
}
if (color3Cnt >= 99999) 
{
color3Cnt = 0;
if(CountDisplayFlag == true)
{ 
//lcd.setCursor(4,1);
//lcd.print(color3Cnt);
}
EEPROM.update(6,0);
EEPROM.update(7,0);
EEPROM.update(8,0);
}
if (Krichi1Cnt >= 99999) 
{
Krichi1Cnt = 0;
KrichiCountZeroUpdateFlag = true;
EEPROM.update(9,0);
EEPROM.update(10,0);
EEPROM.update(11,0);
}
if (Krichi2Cnt >= 99999) 
{
Krichi2Cnt = 0;
KrichiCountZeroUpdateFlag = true;
EEPROM.update(12,0);
EEPROM.update(13,0);
EEPROM.update(14,0);
}
if (Krichi3Cnt >= 99999) 
{
Krichi3Cnt = 0;
KrichiCountZeroUpdateFlag = true;
EEPROM.update(15,0);
EEPROM.update(16,0);
EEPROM.update(17,0);
}

if(KrichiCountZeroUpdateFlag == true)
{

if((Krichi1Cnt >= 99999) or (Krichi2Cnt >= 99999) or (Krichi3Cnt >= 99999))
{
krichiCnt  =  Krichi1Cnt + Krichi2Cnt + Krichi3Cnt;
if(CountDisplayFlag == true)
{ 
lcd.setCursor(14,1);
lcd.print(krichiCnt);
}
KrichiCountZeroUpdateFlag = false;
}


}


if (FullCopCount >= 99999) 
{
FullCopCount = 0;
if(CountDisplayFlag == true)
{ 
//lcd.setCursor(4,1);
//lcd.print(color3Cnt);
}
EEPROM.update(21,0);
EEPROM.update(22,0);
EEPROM.update(23,0);
}

}


//multiple error
bool printError()
{
  static int i = 0 ; 
  int temp = 0;
  if(i >27)
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
    lcd.print("BELT ERROR");
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
  if(YarnErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("YARN1  ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 5:
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
 case 6:
  if(Color1ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("COLOR1 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 7:
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
   case 8:
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
  case 9:
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
 case 10:
  if(Yarn2ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("YARN2  ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 11:
  if(Yarn2ErrorOccuredFlag == true)
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
  if(Color2ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("COLOR2 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 13:
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
  

   case 14:
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
  case 15:
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
   

  case 16:
  if(Tray1Error == true)
  {
    lcd.setCursor(0,3);
    lcd.print("TRAY1 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
      return true;
  }
  else
  {
   return false; 
    }

  break;
  case 17:
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

  case 18:
  if(Tray2Error == true)
  {
   
    lcd.setCursor(0,3);
    lcd.print("TRAY2 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 19:
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

   case 20:
  if(Yarn3ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("YARN3  ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 21:
  if(Yarn3ErrorOccuredFlag == true)
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
   



  case 22:
  if(Color3ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("COLOR3  ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 23:
  if(Color3ErrorOccuredFlag == true)
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
  

   case 24:
  if(Object3ErrorOccuredFlag == true)
  {
    lcd.setCursor(0,3);
    lcd.print("OBJECT3 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 25:
  if(Object3ErrorOccuredFlag == true)
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


case 26:
  if(Tray3Error == true)
  {
   
    lcd.setCursor(0,3);
    lcd.print("TRAY3 ERROR");
    digitalWrite(alarmLightyellow,HIGH);
    return true;
  }
  else
  {
   return false; 
    }

  break;
  case 27:
  if(Tray3Error == true)
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
  lcd.print("                    ");
  digitalWrite(alarmLightyellow,LOW);
  return false;
  
 }



 
}



void Function(char customKey)
{
 Serial.print("Custom inside function = ");
 Serial.println(customKey);
 
if (customKey=='D')
{
    ResetSwitch = true;
    ErrorCycle = false;
    Serial.println("Error Reset");
  }
//if((ResetSwitch == true) or (ErrorCycle == false))
//{
//  Serial.print("error cleared");
//  ErrorCycle = true;
//  ResetSwitch = false;
//}

if (customKey=='B' /*and (InsideMenuFlag == true)*/)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TB1=");
  lcd.setCursor(10,0);
  lcd.print("TB2=");
  lcd.setCursor(0,1);
  lcd.print("RB1=");
  lcd.setCursor(10,1);
  lcd.print("RB2=");
  
  lcd.setCursor(4,0);
  lcd.print(color1Cnt);
  lcd.setCursor(14,0);
  lcd.print(color2Cnt);
  lcd.setCursor(4,1);
  lcd.print(FullCopCount);
  lcd.setCursor(14,1);
  lcd.print(krichiCnt);

  
  InsideMenuFlag = false;
  CountDisplayFlag = true;
  InsideOption1Flag = false;
  InsideOption2Flag = false;
  InsideOption3Flag = false;
  
  InsideClearFlag = false;

  }
if ((InsideClearFlag == false) and (customKey=='C')and (InsideMenuFlag == false)  and (InsideOption1Flag == false) and  (InsideOption2Flag == false)and (InsideOption3Flag == false) )
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CLEAR COUNT?");
  lcd.setCursor(0,1);
  lcd.print("PRESS 1 TO CLEAR");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideClearFlag = true;
  CountDisplayFlag = false;
}

if( (InsideClearFlag == true) and (customKey=='1') )
{
  color1Cnt = 0;
  color2Cnt = 0; 
  color3Cnt = 0;
  krichiCnt = 0;
  FullCopCount = 0;
  
  EEPROM.update(0,0);
  EEPROM.update(1,0);
  EEPROM.update(2,0);
  EEPROM.update(3,0);
  EEPROM.update(4,0);
  EEPROM.update(5,0);
  EEPROM.update(6,0);
  EEPROM.update(7,0);
  EEPROM.update(8,0);
  EEPROM.update(9,0);
  EEPROM.update(10,0);
  EEPROM.update(11,0);
  EEPROM.update(12,0);
  EEPROM.update(13,0);
  EEPROM.update(14,0);
  EEPROM.update(15,0);
  EEPROM.update(16,0);
  EEPROM.update(17,0);
  EEPROM.update(21,0);
  EEPROM.update(22,0);
  EEPROM.update(23,0);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("COUNT CLEARED");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideClearFlag = false;
  
}



if ((InsideMenuFlag == false) and (customKey=='A'))
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1. BIN SIZE");
  lcd.setCursor(0,1);
  lcd.print("2. BOBBIN MIX");
  lcd.setCursor(0,2);
  lcd.print("3. SENSOR OPTION");
  InsideMenuFlag = true;
  CountDisplayFlag = false;
}

  if( (InsideMenuFlag == true) and (customKey=='1')  and (InsideOption1Flag == false) and  (InsideOption2Flag == false)and (InsideOption3Flag == false)  )
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SELECT BIN SIZE");
  lcd.setCursor(0,1);
  lcd.print("1. 140");
  lcd.setCursor(0,2);
  lcd.print("2. 160");
  
  lcd.setCursor(7,1);
  lcd.print("3. 180");
  lcd.setCursor(7,2);
  lcd.print("4. 200");
  
  InsideOption1Flag = true; 
  return ;

}

  if( (InsideMenuFlag == true) and (customKey=='2') and (InsideOption1Flag == false) and  (InsideOption2Flag == false)and (InsideOption3Flag == false) )
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1. BOBBIN MIX ON");
  lcd.setCursor(0,1);
  lcd.print("2. BOBBIN MIXOFF");
  InsideOption2Flag = true; 
  return ;
}

 if( (InsideMenuFlag == true) and (customKey=='3')  and (InsideOption1Flag == false) and  (InsideOption2Flag == false)  and (InsideOption3Flag == false) )
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1. MULTI COLOUR");
  lcd.setCursor(0,1);
  lcd.print("2. SINGLE COLOUR");
  InsideOption3Flag = true; 
  return ;
}
if( (InsideOption1Flag == true) and (customKey=='1'))
{
  Flag140 = true;
  Flag160 = false;
  Flag180 = false;
  Flag200 = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Bin Size = 140");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption1Flag = false;
  EEPROM.update(18,0);
}

      if( (InsideOption1Flag == true) and (customKey=='2'))
{
  Flag140 = false;
  Flag160 = true;
  Flag180 = false;
  Flag200 = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Bin Size = 160"); 
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption1Flag = false;
  EEPROM.update(18,1);
}
  

if( (InsideOption1Flag == true) and (customKey=='3'))
{
  Flag140 = false;
  Flag160 = false;
  Flag180 = true;
  Flag200 = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Bin Size = 180");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption1Flag = false;
  EEPROM.update(18,2);

}

      if( (InsideOption1Flag == true) and (customKey=='4'))
{
  Flag140 = false;
  Flag160 = false;
  Flag180 = false;
  Flag200 = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Bin Size = 200");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption1Flag = false;
  EEPROM.update(18,3);
}
  


 if( (InsideOption2Flag == true) and (customKey=='1'))
{
  BobbinMixupflag = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BOBBIN MIX ON");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption2Flag = false;
  EEPROM.update(19,1);
}

      if( (InsideOption2Flag == true) and (customKey=='2'))
{
  BobbinMixupflag = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BOBBIN MIX OFF");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption2Flag = false;
  EEPROM.update(19,0);
}


 if( (InsideOption3Flag == true) and (customKey=='1'))
{
  ColorSensorOption = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MULTICOLOUR SENSOR");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption3Flag = false;
  EEPROM.update(20,1);
}
 if( (InsideOption3Flag == true) and (customKey=='2'))
{
  ColorSensorOption = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SINGLE COLOUR SENSOR");
  lcd.setCursor(0,2);
  lcd.print("PRESS B TO EXIT");
  InsideMenuFlag = false;
  InsideOption3Flag = false;
  EEPROM.update(20,0);
}


}
