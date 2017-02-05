/*
StepperControl_A4988.h - - Driver for a Stepper Motor with controler A4988- Version 1.0

History:
Version 1.0 - Author Jean-Philippe Bonnet
   First release

This file is part of the StepperControl_A4988 library.

StepperControl_A4988 library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

StepperControl_A4988 library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with StepperControl library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "StepperControl_A4988.h"

//------------------------------------------------------------------------------------
// Constructors:
StepperControl_A4988::StepperControl_A4988(int stepPin,
				int directionPin,
				int stepModePin1,
				int stepModePin2,
				int stepModePin3,
				int enablePin,
				int sleepPin,
				int resetPin)
{
  this->stepPin = stepPin;
  this->directionPin = directionPin;
  this->stepModePin1 = stepModePin1;
  this->stepModePin2 = stepModePin2;
  this->stepModePin3 = stepModePin3;
  this->enablePin = enablePin;
  this->sleepPin = sleepPin;
  this->resetPin = resetPin;

  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(stepModePin1, OUTPUT);
  pinMode(stepModePin2, OUTPUT);
  pinMode(stepModePin3, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(sleepPin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  digitalWrite(directionPin, LOW);
  digitalWrite(stepPin, LOW);

  digitalWrite(sleepPin, HIGH);
  digitalWrite(resetPin, HIGH);

  digitalWrite(enablePin, HIGH);


  this->direction = SC_CLOCKWISE;
  this->inMove = false;
  this->currentPosition = 0;
  this->targetPosition = 0;
  this->speed = SC_DEFAULT_SPEED;
  this->timeStamp = 0;
  this->brakeMode = false;

  this->setStepMode(SC_FULL_STEP);
}

//------------------------------------------------------------------------------------
// Setters
void StepperControl_A4988::setTargetPosition(unsigned int position)
{
  this->targetPosition = position;
}

void StepperControl_A4988::setCurrentPosition(unsigned int position)
{
  this->currentPosition = position;
}

void StepperControl_A4988::setDirection(int direction)
{
  this->direction = direction;
}

void StepperControl_A4988::setStepMode(int stepMode)
{
  this->stepMode = stepMode;
  digitalWrite(stepModePin1, this->stepMode & 0x04);
  digitalWrite(stepModePin2, this->stepMode & 0x02);
  digitalWrite(stepModePin3, this->stepMode & 0x01);
}

void StepperControl_A4988::setMoveMode(int moveMode)
{
  this->moveMode = moveMode;
}

void StepperControl_A4988::setSpeed(unsigned int speed)
{
  this->speed = speed;
}

void StepperControl_A4988::setBrakeMode(int brakeMode)
{
  this->brakeMode = brakeMode;
}

//------------------------------------------------------------------------------------
// Getters
unsigned int StepperControl_A4988::getCurrentPosition()
{
  return this->currentPosition;
}

unsigned int StepperControl_A4988::getTargetPosition()
{
  return this->targetPosition;
}

int StepperControl_A4988::getDirection()
{
  return this->direction;
}

int StepperControl_A4988::getStepMode()
{
  return this->stepMode;
}

int StepperControl_A4988::getMoveMode()
{
  return this->moveMode;
}

unsigned int StepperControl_A4988::getSpeed()
{
  return this->speed;
}

int StepperControl_A4988::getBrakeMode()
{
  return this->brakeMode;
}

//------------------------------------------------------------------------------------
// Other public members
void StepperControl_A4988::manage()
{
  if(this->inMove)
    {
      this->moveMotor();
    }
}

void StepperControl_A4988::goToTargetPosition()
{
  digitalWrite(this->enablePin, LOW);
  this->inMove = true;
}

void StepperControl_A4988::stopMovement()
{
  if (! this->getBrakeMode())
    digitalWrite(this->enablePin, HIGH);
  this->inMove = false;
}

int StepperControl_A4988::isInMove()
{
  return this->inMove;
}

//------------------------------------------------------------------------------------
// Privates
void StepperControl_A4988::moveMotor()
{
  if(targetPosition != currentPosition)
    {
      if ((millis() - this->timeStamp) >= ((unsigned long)((1/(float)this->speed)*1000)))
	{
	  if(((long)this->targetPosition - (long)this->currentPosition) > 0)
	    {
	      if(this->direction == SC_CLOCKWISE)
		{
		  digitalWrite(this->directionPin, LOW);
		}
	      else
		{
		  digitalWrite(this->directionPin, HIGH);
		  
		}
	      this->currentPosition++;
	    }
	  else
	    {
	      if(this->direction == SC_CLOCKWISE)
		{
		  digitalWrite(this->directionPin, HIGH);
		}
	      else
		{
		  digitalWrite(this->directionPin, LOW);
		}     
	      this->currentPosition--;
	    }
	  digitalWrite(this->enablePin, LOW);
	  delayMicroseconds(5);
	  digitalWrite(this->stepPin, HIGH);
  	  delayMicroseconds(5);
	  digitalWrite(this->stepPin, LOW);
	  
	  this->timeStamp = millis();
	}
    }
  else
    {
      this->stopMovement();
    }
}
