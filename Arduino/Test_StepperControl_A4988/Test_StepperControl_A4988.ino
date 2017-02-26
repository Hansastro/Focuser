#include <LM335.h>

#include <StepperControl_A4988.h>

StepperControl_A4988 myStepper(8, 9, 3, 4, 5, 2, 7, 6);
LM335 tempSensor(4);

int currentPosition = 0;
int isInitDone = 0;

long ping = 0;
long pong = 35000;
bool direction = false;
unsigned long timeStamp = 0;
unsigned long tempComp_timestamp = 0;

void setup()
{
  Serial.begin(9600);
  
  myStepper.setDirection(SC_CLOCKWISE);
  myStepper.setStepMode(SC_SIXTEENTH_STEP);
  myStepper.setBrakeMode(false);
  myStepper.setSpeed(7000);
  myStepper.setMoveMode(SC_MOVEMODE_SMOOTH);
  //myStepper.setMoveMode(SC_MOVEMODE_PER_STEP);
}

void loop()
{ 
  if (isInitDone == 0)
  {
    //myStepper.setTargetPosition(1000);
    Serial.println("Init");
    //Serial.println(myStepper.getTargetPosition());
    isInitDone = 1;
    //myStepper.goToTargetPosition();
  }

  myStepper.Manage();
  tempSensor.Manage();

  if (!myStepper.isInMove())
 {
     myStepper.setTargetPosition(direction?ping:pong);
     myStepper.goToTargetPosition();
     direction = !direction; 
  } 

//
//
  if(!myStepper.isInMove() && (millis() - tempComp_timestamp) >= 1000)
  {
//    myStepper.compensateTemperature(tempSensor.getTemperature(), 20);
    Serial.println(tempComp_timestamp);
//    Serial.print("Pos: ");
//    Serial.print(myStepper.getCurrentPosition());
//    Serial.print("\tcorrection: ");
//    Serial.println(myStepper.dbg_correction);
////    delay(1000);
////    myStepper.setTargetPosition(direction?ping:pong);
//    //myStepper.setTargetPosition(35000);
////    myStepper.goToTargetPosition();
////    direction = direction?0:1;
    tempComp_timestamp = millis();
  } 
    
//  if ((millis() - timeStamp) > 100)
//  {
//    Serial.print("Time: ");
//    Serial.print(millis());
//    Serial.print("\tTimestamp: ");
//    Serial.print(timeStamp);
//    Serial.print("\tSpeed: ");
//    Serial.print(myStepper.getSpeed());
//    Serial.print("\tTarget Speed: ");
//    Serial.print(myStepper.targetSpeed);
//    Serial.print("\tPos: ");
//    Serial.print(myStepper.getCurrentPosition());
//    Serial.print("\tTarget Position: ");
//    Serial.print(myStepper.getTargetPosition());
//    Serial.print("\t speed target reached: ");
//    Serial.println(myStepper.positionTargetSpeedReached);
//      Serial.println(myStepper.isInMove());
//    timeStamp = millis();
//  }

  //  delay(50);
}
