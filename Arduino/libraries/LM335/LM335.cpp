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
  pinMode(this->aquisitionPin, INPUT);
  this->integrationLoop = LM335_DEFAULT_INTEGRATION_LOOP;
  this->integrationLoopNumber = 0;
  this->temperature = readTemperature();
  this->integratedTemperature = 0;
}

//-----------------------------------------------------------------------------
// Setters

void LM335::setNumberOfIntegration(int integrationLoop)
{
  this->integrationLoop = integrationLoop;
}

//-----------------------------------------------------------------------------
// Getters

// Get the temperature in C°
float LM335::getTemperature()
{
  return temperature;
}

int LM335::getNumberOfIntegrationLoop()
{
  return integrationLoop;
}

//-----------------------------------------------------------------------------
// Public Members
void LM335::integrateTemperature()
{
  integratedTemperature += readTemperature();
  integrationLoopNumber ++;
  if (integrationLoopNumber >= integrationLoop)
    {
      temperature = integratedTemperature / integrationLoop;
      integratedTemperature = 0;
      integrationLoopNumber = 0;
    }
}

void LM335::Manage()
{
  integrateTemperature();
}

//-----------------------------------------------------------------------------
// Private
float LM335::readTemperature()
{
  return (500.0*analogRead(this->aquisitionPin)/1024.0) - 273.15;
}
