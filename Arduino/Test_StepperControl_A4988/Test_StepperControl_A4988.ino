#include <StepperControl_A4988.h>u, 3, 4, 5, 2, 7, 6);

unsigned int currentPosition = 0;
int isInitDone = 0;

unsigned int ping = 0;
unsigned int pong = 10000;
int direction = 0;
unsigned long timeStamp = 0;

void setup()
{
  Serial.begin(9600);

  myStepper.setDirection(SC_CLOCKWISE);
  myStepper.setStepMode(SC_SIXTEENTH_STEP);
  myStepper.setBrakeMode(false);
  myStepper.setSpeed(2000);
}

void loop()
{
  static int i = 0;
  
  if (isInitDone == 0)
  {
    //myStepper.setTargetPosition(10000);
    Serial.println("Init");
    Serial.println(myStepper.getTargetPosition());
    myStepper.goToTargetPosition();
    isInitDone = 1;
  }

  myStepper.manage();

  //    if(!myStepper.isInMove())
  //    {
  //      myStepper.setTargetPosition(direction?ping:pong);
  //      delay(1000);
  //      myStepper.goToTargetPosition();
  //      direction = direction?0:1;
  //    }

  if(i <= 10000)
  {
    if (!myStepper.isInMove())
    {
      myStepper.setTargetPosition(i);
      myStepper.goToTargetPosition();
      i += 1;
    }

    //    if ((millis() - timeStamp) > 500)
    //    {
    //      Serial.print(millis());
    //      Serial.print(" ");
    //      Serial.print(timeStamp);
    //      Serial.print(" ");
    //      Serial.println(myStepper.getTargetPosition());
    //      timeStamp = millis();
    //    }
  }
}
