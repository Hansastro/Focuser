#include <LM335.h>
#include <Moonlite.h>
#include <StepperControl.h>

LM335 TemperatureSensor(0);
StepperControl Motor(9, 7, 8, 10);
Moonlite SerialProtocol;

void processCommand()
{
  MoonliteCommand_t command;
  switch (SerialProtocol.getCommand().commandID)
  {
    case ML_C:
      // Not implemented
      break;
    case ML_FG:
      Motor.goToTargetPosition();
      break;
    case ML_FQ:
      Motor.stopMovement();
      break;
    case ML_GB:
        SerialProtocol.setAnswer(2,0x00);
    case ML_GC:
      // Not implemented
      break;
    case ML_GD:
      switch(Motor.getSpeed())
      {
        case 16:
          SerialProtocol.setAnswer(2,(long)20);
          break;
        case 32:
          SerialProtocol.setAnswer(2,(long)10);
          break;
        case 63:
          SerialProtocol.setAnswer(2,(long)8);
          break;
        case 125:
          SerialProtocol.setAnswer(2,(long)4);
          break;
        case 250:
          SerialProtocol.setAnswer(2,(long)2);
          break;
        default:
          break;
      }
      break;
    case ML_GH:
      SerialProtocol.setAnswer(2, (long)(Motor.getStepMode() == SC_HALF_STEP?0xFF:0x00));
      break;
    case ML_GI:
      SerialProtocol.setAnswer(2, (long)(Motor.isInMove()?0x01:0x00));
      break;
    case ML_GN:
      SerialProtocol.setAnswer(4, (long)(Motor.getTargetPosition()));
      break;
    case ML_GP:
      SerialProtocol.setAnswer(4, (long)(Motor.getCurrentPosition()));
      break;
    case ML_GT:
      SerialProtocol.setAnswer(4, (long)((TemperatureSensor.getTemperature()*2)));
      break;
    case ML_GV:
      SerialProtocol.setAnswer(2, (long)(0x01));
      break;
    case ML_SC:
      // Not implemented
      break;
    case ML_SD:
      switch(SerialProtocol.getCommand().parameter)
      {
        case 0x02:
          Motor.setSpeed(250);
          break;
        case 0x04:
          Motor.setSpeed(125);
          break;
        case 0x08:
          Motor.setSpeed(63);
          break;
        case 0x10:
          Motor.setSpeed(32);
          break;
        case 0x20:
          Motor.setSpeed(16);
          break;
        default:
          break;
      }
      break;
    case ML_SF:
      Motor.setStepMode(SC_FULL_STEP);
      break;
    case ML_SH:
      Motor.setStepMode(SC_HALF_STEP);
      break;
    case ML_SN:
      Motor.setTargetPosition(SerialProtocol.getCommand().parameter);
      break;
    case ML_SP:
      Motor.setCurrentPosition(SerialProtocol.getCommand().parameter);
      break;
    case ML_PLUS:
      // Not implemented
      break;
    case ML_MINUS:
      // Not implemented
      break;
    case ML_PO:    
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
    Motor.setSpeed(16);
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
