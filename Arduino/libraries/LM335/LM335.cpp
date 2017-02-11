/*
LM335.cpp - - LM335 temperature sensor libraie - Version 0.1

History:
Version 1.0 - Author Jean-Philippe Bonnet
   First release

This file is part of the LM335 library.

StepperControl library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LM335 library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LM335 library.  If not, see <http://www.gnu.org/licenses/>.

 */

#include "LM335.h"

//-----------------------------------------------------------------------------
// Constructors

LM335::LM335(int aquisitionPin)
{
  this->aquisitionPin = aquisitionPin;
  this->integrationLoop = LM335_DEFAULT_INTEGRATION_LOOP;
  this->integrationLoopNumber = 0;
  this->temperature = -65535;
  this->integratedTemperature = 0;
  this->temperatureCompensationValue = 0;
}

//-----------------------------------------------------------------------------
// Setters

void LM335::setNumberOfIntegration(unsigned long integrationLoop)
{
  this->integrationLoop = integrationLoop;
}

// Set the offset of the temperature in C°
void LM335::setCompensationValue(float compensationValue)
{
   this->temperatureCompensationValue = compensationValue;
}

//-----------------------------------------------------------------------------
// Getters

// Get the temperature in C°
float LM335::getTemperature()
{
  return this->temperature;
}

unsigned long LM335::getNumberOfIntegrationLoop()
{
  return this->integrationLoop;
}

float LM335::getCompensationValue()
{
   return this->temperatureCompensationValue;
}

//-----------------------------------------------------------------------------
// Public Members
void LM335::integrateTemperature()
{
  this->integratedTemperature += this->readTemperature();
  this->integrationLoopNumber ++;
  if (this->integrationLoopNumber >= this->integrationLoop)
    {
      temperature = (this->integratedTemperature / this->integrationLoop)
			+ this->temperatureCompensationValue;
      this->integratedTemperature = 0;
      this->integrationLoopNumber = 0;
    }
}

void LM335::Manage()
{
  if(temperature<-65000)
  {
     this->temperature = this->readTemperature();
  }
  this->integrateTemperature();
}

//-----------------------------------------------------------------------------
// Private
float LM335::readTemperature()
{
  return (500.0*analogRead(this->aquisitionPin)/1024.0) - 273.15;
}
