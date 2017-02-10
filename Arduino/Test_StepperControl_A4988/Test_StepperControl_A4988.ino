#include <StepperControl_A4988.h>

StepperControl_A4988 myStepper(8, 9, 3, 4, 5, 2, 7, 6);

unsigned int currentPosition = 0;
int isInitDone = 0;

unsigned int ping = 0;
unsigned int pong = 10000; //35000;
int direction = 0;
unsigned long timeStamp = 0;

void setup()
{
  Serial.begin(9600);
  
  myStepper.setDirection(SC_CLOCKWISE);
  myStepper.setStepMode(SC_SIXTEENTH_STEP);
  myStepper.setBrakeMode(false);
  myStepper.setSpeed(10000);
}

void loop()
{ 
  if (isInitDone == 0)
  {
    //myStepper.setTargetPosition(500);
    Serial.println("Init");
    Serial.println(myStepper.getTargetPosition());
    isInitDone = 1;
  }

  myStepper.Manage();

  if(!myStepper.isInMove())
  {
    myStepper.setTargetPosition(direction?ping:pong);
    myStepper.goToTargetPosition();
    direction = direction?0:1;
  } 
    
  if ((millis() - timeStamp) > 500)
  {
    Serial.print(millis());
    Serial.print(" ");
    Serial.print(timeStamp);
    Serial.print(" ");
    Serial.println(myStepper.getSpeed());
    timeStamp = millis();
  }

  //  delay(50);
}
