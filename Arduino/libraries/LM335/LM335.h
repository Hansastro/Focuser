/*
LM335.h - - LM335 temperature sensor libraie - Version 1.0

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

#if ARDUINO < 100
#include <Wprogram.h>
#else
#include <Arduino.h>
#endif

#define LM335_DEFAULT_INTEGRATION_LOOP 5

class LM335
{
 public:
  // Constructors:
  LM335(int aquisistionPin);

  // Setters:
  void setNumberOfIntegration(int integrationLoop);

  // Getter:
  float getTemperature();
  int getNumberOfIntegrationLoop();

  // Other Public Members
  void Manage();

 private:
  float readTemperature();

  float temperature; // Temperature in C°
  float integratedTemperature;

  int integrationLoop;
  int integrationLoopNumber;
  int aquisitionPin;

  void integrateTemperature();
};
