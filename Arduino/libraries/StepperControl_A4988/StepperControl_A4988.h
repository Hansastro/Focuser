/*
StepperControl_a4988.h - - Driver for a Stepper Motor with controler a4988 - Version 1.0

History:
Version 1.0 - Author Jean-Philippe Bonnet
   First release

This librarie allow the control of a stepper motor controled with a A4988 Stepper motor controler. This libraie allow absolut positioning and can be used for horizontal movement.

If the brake mode is enabled the the motor will be premenantly powered. This increase the power consomption but the motor will be fix. If the brake mode is disabled, the motor is able to run freely if it not in move. If the motor turn the counter which indicate the steps will be wrong.

This file is part of the StepperControl_A4988 library.

StepperControl_A4988 library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

StepperControl_4988 library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with StepperControl library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef stepperControl_A4988_h
#define stepperControl_A4988_h

#if ARDUINO < 100
#include <Wprogram.h>
#else
#include <Arduino.h>
#endif

#define SC_CLOCKWISE 0
#define SC_COUNTER_CLOCKWISE 1

#define SC_FULL_STEP 0
#define SC_HALF_STEP 4
#define SC_QUATER_STEP 2
#define SC_EIGHTH_STEP 6
#define SC_SIXTEENTH_STEP 7

#define SC_MOVEMODE_QUICK 0
#define SC_MOVEMODE_PER_STEP 1

#define SC_DEFAULT_SPEED 1000

class StepperControl_A4988
{
 public:
  // Constructors:
  StepperControl_A4988(int stepPin,
			int directionPin,
			int stepModePin1,
			int stepModePin2,
			int stepModePin3,
			int enablePin,
			int sleepPin,
			int resetPin);

  // Setters
  void setTargetPosition(unsigned int position);
  void setCurrentPosition(unsigned int position);
  void setDirection(int direction);
  void setStepMode(int stepMode);
  void setMoveMode(int moveMode);
  void setSpeed(unsigned int speed);
  void setBrakeMode(int state);

  // Getters
  unsigned int getCurrentPosition();
  unsigned int getTargetPosition();
  int getDirection();
  int getStepMode();
  int getMoveMode();
  unsigned int getSpeed();
  int getBrakeMode();

  // Other public members
  void Manage();
  void goToTargetPosition();
  void stopMovement();
  int isInMove();
  
 private:
  int direction;
  int stepMode;
  int moveMode;
  int inMove;
  int brakeMode;
  unsigned int currentPosition;
  unsigned int targetPosition;
  unsigned int speed;  // Speed in ticks per seconds
  unsigned int maxSpeed; // Maximal Speed

  unsigned long timeStamp;

  int stepPin;
  int directionPin;
  int stepModePin1;
  int stepModePin2;
  int stepModePin3;
  int enablePin;
  int sleepPin;
  int resetPin;

  void moveMotor();
};

#endif //stepperControl_A4988_h
