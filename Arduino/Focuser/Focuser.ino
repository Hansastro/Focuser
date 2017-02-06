#include <LM335.h>
#include <Moonlite.h>
#include <StepperControl_A4988.h>

const int Dir_Pin = 9;
const int Step_Pin = 8;
const int Sleep_Pin = 7;
const int Reset_Pin = 6;
const int EN_Pin = 2;
const int MS3_Pin = 5;
const int MS2_Pin = 4;
const int MS1_Pin = 3;

LM335 TemperatureSensor(0);
StepperControl_A4988 Motor(Step_Pin,
                          Dir_Pin,
                          MS1_Pin,
                          MS2_Pin,
                          MS3_Pin,
                          EN_Pin,
                          Sleep_Pin,
                          Reset_Pin);

Moonlite SerialProtocol;

void processCommand()
{
  MoonliteCommand_t command;
  switch (SerialProtocol.getCommand().commandID)
  {
    case ML_C:
      // Initiate temperature convertion
      // Not needed
      // Not implemented
      break;
    case ML_FG:
      // Go to new position
      Motor.goToTargetPosition();
      break;
    case ML_FQ:
      // Stop the movement
      Motor.stopMovement();
      break;
    case ML_GB:
        // This command is needed if you use the official Moonlite software
        SerialProtocol.setAnswer(2,0x00);
    case ML_GC:
      // Return the temperature coefficient
      // Not implemented
      break;
    case ML_GD:
      // Return the stepping delay
      switch(Motor.getSpeed())
      {
        // The Speed used in not the Moonlite standart one
        // with 1/16 Step the standard speed is far too slow
        // and the A4899 motor controller can be faster
        case 160:
          SerialProtocol.setAnswer(2,(long)20);
          break;
        case 320:
          SerialProtocol.setAnswer(2,(long)10);
          break;
        case 630:
          SerialProtocol.setAnswer(2,(long)8);
          break;
        case 1250:
          SerialProtocol.setAnswer(2,(long)4);
          break;
        case 2500:
          SerialProtocol.setAnswer(2,(long)2);
          break;
        default:
          break;
      }
      break;
    case ML_GH:
      // Return the step mode (half step or full step)
      SerialProtocol.setAnswer(2, (long)(Motor.getStepMode() == SC_HALF_STEP?0xFF:0x00));
      break;
    case ML_GI:
      // Return the status of the motor movement (move or not)
      SerialProtocol.setAnswer(2, (long)(Motor.isInMove()?0x01:0x00));
      break;
    case ML_GN:
      // Return the target position
      SerialProtocol.setAnswer(4, (long)(Motor.getTargetPosition()));
      break;
    case ML_GP:
      // Return the current position
      SerialProtocol.setAnswer(4, (long)(Motor.getCurrentPosition()));
      break;
    case ML_GT:
      // Return the current temperature
      SerialProtocol.setAnswer(4, (long)((TemperatureSensor.getTemperature()*2)));
      break;
    case ML_GV:
      // Return the version of the firmware
      SerialProtocol.setAnswer(2, (long)(0x01));
      break;
    case ML_SC:
      // Set the temperature coefficient
      // Not implemented
      break;
    case ML_SD:
      // Set the stepping delay
      switch(SerialProtocol.getCommand().parameter)
      {
        // The Speed used in not the Moonlite standart one
        // with 1/16 Step the standard speed is far too slow
        // and the A4899 motor controller can be faster
        case 0x02:
          Motor.setSpeed(2500);
          break;
        case 0x04:
          Motor.setSpeed(1250);
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
      // Set full step
      // The A4988 motor controller can be used to 1/8 Step and 1/16 Step
      // Those two mode are used instead of the standard full of half step mode
      // to increase precision
      Motor.setStepMode(SC_EIGHTH_STEP);
      break;
    case ML_SH:
      // set half step
      // see comment of the ML_SF command above
      Motor.setStepMode(SC_SIXTEENTH_STEP);
      break;
    case ML_SN:
      // Set target position
      Motor.setTargetPosition(SerialProtocol.getCommand().parameter);
      break;
    case ML_SP:
      // Set current position
      Motor.setCurrentPosition(SerialProtocol.getCommand().parameter);
      break;
    case ML_PLUS:
      // Not implemented
      // This command is needed if you use the official Moonlite software
      break;
    case ML_MINUS:
      // Not implemented
      // This command is needed if you use the official Moonlite software
      break;
    case ML_PO:    
      // Not implemented
      // This command is needed if you use the official Moonlite software      
      break;
    default:
      break;
  }
}

void setup()
{
    SerialProtocol.init(9600);
    
    // Set the motor speed to a valid value for Moonlite
    Motor.setSpeed(2500);
}

void loop()
{
  if(!Motor.isInMove())
  {
    TemperatureSensor.Manage();
  }
  Motor.manage();
  SerialProtocol.Manage();
  
  if(SerialProtocol.isNewCommandAvailable())
    {
      processCommand();
    }
}
