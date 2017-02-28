#include <LM335.h>
#include <Moonlite.h>
#include <StepperControl_A4988.h>

#include <U8x8lib.h>
#include <U8g2lib.h>

const int stepPin = 8;
const int directionPin = 9;
const int stepMode1 = 3;
const int stepMode2 = 4;
const int stepMode3 = 5;
const int enablePin = 2;
const int sleepPin = 7;
const int resetPin = 6;

const int temperatureSensorPin = 3;

unsigned long timestamp;
unsigned long displayTimestamp;

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

// Declaration of the display
U8G2_SSD1306_128X64_NONAME_1_HW_I2C Display(U8G2_R0);

float temp = 0;
long pos = 0;
bool pageIsRefreshing = false;

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
      // Set the Red Led backligth value
      // Dump value necessary to run the official moonlite software
      SerialProtocol.setAnswer(2, 0x00);
    case ML_GC:
      // Return the temperature coefficient
      SerialProtocol.setAnswer(2, (long)Motor.getTemperatureCompensationCoefficient());
      break;
    case ML_GD:
      // Return the current motor speed
      switch (Motor.getSpeed())
      {
        case 500:
          SerialProtocol.setAnswer(2, (long)20);
          break;
        case 1000:
          SerialProtocol.setAnswer(2, (long)10);
          break;
        case 3000:
          SerialProtocol.setAnswer(2, (long)8);
          break;
        case 5000:
          SerialProtocol.setAnswer(2, (long)4);
          break;
        case 7000:
          SerialProtocol.setAnswer(2, (long)2);
          break;
        default:
          break;
      }
      break;
    case ML_GH:
      // Return the current stepping mode (half or full step)
      SerialProtocol.setAnswer(2, (long)(Motor.getStepMode() == SC_HALF_STEP ? 0xFF : 0x00));
      break;
    case ML_GI:
      // get if the motor is moving or not
      SerialProtocol.setAnswer(2, (long)(Motor.isInMove() ? 0x01 : 0x00));
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
      SerialProtocol.setAnswer(4, (long)((TemperatureSensor.getTemperature() * 2)));
      break;
    case ML_GV:
      // Get the version of the firmware
      SerialProtocol.setAnswer(2, (long)(0x01));
      break;
    case ML_SC:
      // Set the temperature coefficient
      Motor.setTemperatureCompensationCoefficient(SerialProtocol.getCommand().parameter);
      break;
    case ML_SD:
      // Set the motor speed
      switch (SerialProtocol.getCommand().parameter)
      {
        case 0x02:
          Motor.setSpeed(7000);
          break;
        case 0x04:
          Motor.setSpeed(5000);
          break;
        case 0x08:
          Motor.setSpeed(3000);
          break;
        case 0x10:
          Motor.setSpeed(1000);
          break;
        case 0x20:
          Motor.setSpeed(500);
          break;
        default:
          break;
      }
      break;
    case ML_SF:
      // Set the stepping mode to full step
      Motor.setStepMode(SC_EIGHTH_STEP);
      if (Motor.getSpeed() >= 6000)
      {
        Motor.setSpeed(6000);
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
      Motor.enableTemperatureCompensation();
      break;
    case ML_MINUS:
      // Disable temperature compensation focusing
      Motor.disableTemperatureCompensation();
      break;
    case ML_PO:
      // Temperature calibration
      TemperatureSensor.setCompensationValue(SerialProtocol.getCommand().parameter / 2.0);
      break;
    default:
      break;
  }
}

void setup()
{
  SerialProtocol.init(9600);

  Display.begin();
  Display.setContrast(0);
  Display.setFont(u8g2_font_crox4hb_tr);

  // Set the motor speed to a valid value for Moonlite
  Motor.setSpeed(7000);
  Motor.setMoveMode(SC_MOVEMODE_SMOOTH);
  //Motor.setMoveMode(SC_MOVEMODE_PER_STEP);

  timestamp = millis();
  displayTimestamp = millis();
}

void loop()
{
  if (!Motor.isInMove())
  {
    TemperatureSensor.Manage();
    if (Motor.isTemperatureCompensationEnabled() && ((millis() - timestamp) > 30000))
    {
      Motor.setCurrentTemperature(TemperatureSensor.getTemperature());
      Motor.compensateTemperature();
      timestamp = millis();
    }
  }
  Motor.Manage();
  SerialProtocol.Manage();

  if (SerialProtocol.isNewCommandAvailable())
  {
    processCommand();
  }

  if ((millis() - displayTimestamp) >= 1000 && !Motor.isInMove())
  {
    Display.firstPage();
    temp = TemperatureSensor.getTemperature();
    pos = Motor.getCurrentPosition();
    do
    {
      Display.drawStr(0, 16, ((String("T: ") + String(temp, 1) + " C").c_str()));
      Display.drawStr(0, 55, (String("Pos: ") + pos).c_str());
    } while (Display.nextPage());
    displayTimestamp = millis();
  }
}
