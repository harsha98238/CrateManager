/*Input Output pin declaration*/
const int ObjectSensor1 = 32;
const int ObjectSensor2 = 33;
const int Motor = 2;

/*assigning flags and timers*/
unsigned long ObjectSensor1Timer1 = 0;
bool ObjectSensor1Flag1 = false;
bool ObjectSensor1Flag2 = false;


unsigned long ObjectSensor2Timer1 = 0;
bool ObjectSensor2Flag1 = false;
bool ObjectSensor2Flag2 = false;

bool ObjectSensor1ErrorOccuredFlag = false;
bool ObjectSensor2ErrorOccuredFlag = false;

void setup()
{
Serial.begin(115200);
pinMode(ObjectSensor1, INPUT_PULLUP);
pinMode(ObjectSensor2, INPUT_PULLUP); 
pinMode(Motor,OUTPUT);
}

void loop()
{
if (digitalRead(ObjectSensor1) == false and ObjectSensor1Flag1 == false)
  {
    ObjectSensor1Timer1 = millis();
    ObjectSensor1Flag2 = true;
    ObjectSensor1Flag1 = true;
    //Serial.println("inside 1st true flag");
  }

if (digitalRead(ObjectSensor1) == true )
  {
    ObjectSensor1Flag1 = false;
    ObjectSensor1Flag2 = false;
    ObjectSensor1ErrorOccuredFlag = false;
  }  
if ((millis() - ObjectSensor1Timer1 > (2000) and ObjectSensor1Flag2 == true) )
  { 
    ObjectSensor1ErrorOccuredFlag = true;
    Serial.println("Object 1 sensor error occured");
  }



if (digitalRead(ObjectSensor2) == false and ObjectSensor2Flag1 == false)
  {
    ObjectSensor2Timer1 = millis();
    ObjectSensor2Flag2 = true;
    ObjectSensor2Flag1 = true;
    //Serial.println("inside 2nd true flag");
  }
if (digitalRead(ObjectSensor2) == true )
  {
    ObjectSensor2Flag1 = false;
    ObjectSensor2Flag2 = false;
    ObjectSensor2ErrorOccuredFlag = false;
  }
if ((millis() - ObjectSensor2Timer1 > (2000) and ObjectSensor2Flag2 == true) )
  { 
    ObjectSensor2ErrorOccuredFlag = true;
    Serial.println("Object 2 sensor error occured");
  }




if ((ObjectSensor1ErrorOccuredFlag == true) and (ObjectSensor2ErrorOccuredFlag == true))
{
  digitalWrite(Motor,LOW);
 Serial.println("motor off");
}
if ((ObjectSensor1ErrorOccuredFlag == false) and (ObjectSensor2ErrorOccuredFlag == false))
{
  digitalWrite(Motor,HIGH);
  Serial.println("motor on");
}


} 
