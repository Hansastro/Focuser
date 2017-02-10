#include <LM335.h>
#include <Moonlite.h>
#include <StepperControl_A4988.h>

const int stepPin = 8;
const int directionPin = 9;
const int stepMode1 = 3;
const int stepMode2 = 4;
const int stepMode3 = 5;
const int enablePin = 2;
const int sleepPin = 7;
const int resetPin = 6;

const int temperatureSensorPin = 4;

LM335 TemperatureSensor(temperatureSensorPin);
StepperControl_A4988 Motor(stepPin,
                            directionPin,
                            stepMode1, 
                            stepMode2,
                            stepMode3,
                            enablePin,
                            sleepPin,
                            resetPin);
Moonlite SerialProtocol;

void processCommand()
{
  MoonliteCommand_t command;
  switch (SerialProtocol.getCommand().commandID)
  {
    case ML_C:
      // Initiate temperature convertion
      // Not implemented
      break;
    case ML_FG:
      // Goto target position
      Motor.goToTargetPosition();
      break;
    case ML_FQ:
      // Motor stop movement
      Motor.stopMovement();
      break;
    case ML_GB:
        // Dump value necessary to run the official moonlite software
        SerialProtocol.setAnswer(2,0x00);
    case ML_GC:
      // Return the temperature coefficient
      // Not implemented
      break;
    case ML_GD:
      // Return the current motor speed
      switch(Motor.getSpeed())
      {
        case 160:
          SerialProtocol.setAnswer(2,(long)20);
          break;
        case 320:
          SerialProtocol.setAnswer(2,(long)10);
          break;
        case 630:
          SerialProtocol.setAnswer(2,(long)8);
          break;
        case 800:
        case 1250:
          SerialProtocol.setAnswer(2,(long)4);
          break;
        case 1000:
        case 2500:
          SerialProtocol.setAnswer(2,(long)2);
          break;
        default:
          break;
      }
      break;
    case ML_GH:
      // Return the current stepping mode (half or full step)
      SerialProtocol.setAnswer(2, (long)(Motor.getStepMode() == SC_HALF_STEP?0xFF:0x00));
      break;
    case ML_GI:
      // get if the motor is moving or not
      SerialProtocol.setAnswer(2, (long)(Motor.isInMove()?0x01:0x00));
      break;
    case ML_GN:
      // Get the target position
      SerialProtocol.setAnswer(4, (long)(Motor.getTargetPosition()));
      break;
    case ML_GP:
      // Return the current position
      SerialProtocol.setAnswer(4, (long)(Motor.getCurrentPosition()));
      break;
    case ML_GT:
      // Return the temperature
      SerialProtocol.setAnswer(4, (long)((TemperatureSensor.getTemperature()*2)));
      break;
    case ML_GV:
      // Get the version of the firmware
      SerialProtocol.setAnswer(2, (long)(0x01));
      break;
    case ML_SC:
      // Set the temperature coefficient
      // Not implemented
      break;
    case ML_SD:
      // Set the motor speed
      switch(SerialProtocol.getCommand().parameter)
      {
        case 0x02:
          Motor.setSpeed(Motor.getStepMode()==SC_SIXTEENTH_STEP?2500:1000);
          break;
        case 0x04:
          Motor.setSpeed(Motor.getStepMode()==SC_SIXTEENTH_STEP?1250:800);
          break;
        case 0x08:
          Motor.setSpeed(630);
          break;
        case 0x10:
          Motor.setSpeed(320);
          break;
        case 0x20:
          Motor.setSpeed(160);
          break;
        default:
          break;
      }
      break;
    case ML_SF:
      // Set the stepping mode to full step
      Motor.setStepMode(SC_EIGHTH_STEP);
      if(Motor.getSpeed()>1000)
      {
        Motor.setSpeed(630);
      }
      break;
    case ML_SH:
      // Set the stepping mode to half step
      Motor.setStepMode(SC_SIXTEENTH_STEP);
      break;
    case ML_SN:
      // Set the target position
      Motor.setTargetPosition(SerialProtocol.getCommand().parameter);
      break;
    case ML_SP:
      // Set the current motor position
      Motor.setCurrentPosition(SerialProtocol.getCommand().parameter);
      break;
    case ML_PLUS:
      // Activate temperature compensation focusing
      // Not implemented
      break;
    case ML_MINUS:
      // Disable temperature compensation focusing
      // Not implemented
      break;
    case ML_PO:
      // Temperature calibration    
      // Not implemented
      break;
    default:
      break;
  }
}

void setup()
{
    SerialProtocol.init(9600);
    
    // Set the motor speed to a valid value for Moonlite
    Motor.setSpeed(1000);
}

void loop()
{
  if(!Motor.isInMove())
  {
    TemperatureSensor.Manage();
  }
  Motor.Manage();
  SerialProtocol.Manage();
  
  if(SerialProtocol.isNewCommandAvailable())
    {
      processCommand();
    }
}
