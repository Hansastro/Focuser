/*
Moonlite.h - - Parser for the Moonlite serial focuser protocol - Version 1.0

History:
Version 1.0 - Author Jean-Philippe Bonnet
   First release

This file is part of the Moonlite library.

Moonlite library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Moonlite library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Moonlite library.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef Moonlite_h
#define Moonlite_h

#if ARDUINO < 100
#include <Wprogram.h>
#else
#include <Arduino.h>
#endif

//Definition of the Moonlite commands
#define ML_UNKNOWN_COMMAND 0;
#define ML_C 1    // Temperature convertion
#define ML_FG 10  // Go to target position
#define ML_FQ 11  // Stop movement
#define ML_GC 20  // Return temperature coefficient
#define ML_GD 21  // Return stepping delay (See command SD)
#define ML_GH 22  // Return 0xFF if the stepping mode is set to half-step otherwise returns 0x00
#define ML_GI 23  // Return "00" if the motor is not moving. "01" otherwisw 
#define ML_GN 24  // Return the target position set by the command SN
#define ML_GP 25  // Return the current position
#define ML_GT 26  // Return the current temperature
#define ML_GV 27  // Get the version of the firmware
#define ML_SC 30  // Set the new temperature coefficient
#define ML_SD 31  // Set the stepping delay (possible values are 02, 04, 08, 10 and 20 for a delay of respectively 250, 125, 63, 32, 16 step per second)
#define ML_SF 32  // Set full-step mode
#define ML_SH 33  // Set half-step mode
#define ML_SN 34  // Set the target position
#define ML_SP 35  // Set the new current position
#define ML_PLUS 40 // Activate the temperature compensation focusing
#define ML_MINUS 41 // Desactivate the temperature compensation focusing
#define ML_PO 50  // Set the temperature calibration offset
#define ML_GB 60 // Get the baklight LED value

#define ML_INPUT_BUFFER_SIZE 8 // Buffer size for the incomming command.
#define ML_OUTPUT_BUFFER_SIZE 5 // Buffer size for the answer message.

typedef struct MoonliteCommand_s
 {
   int commandID;
   long parameter;
} MoonliteCommand_t;

class Moonlite
{
 public:
  // Constructors:
  Moonlite();

  // Getters:
  MoonliteCommand_t getCommand();

  // Setters:
  void setAnswer(int nbChar, long answer);

  // Other public members
  void init(int baudRate);
  int isNewCommandAvailable();
  int Manage();

 private:
  MoonliteCommand_t currentCommand;
  int newCommandIsAvailable;
  char currentAsciiCommand[ML_INPUT_BUFFER_SIZE];
  char AsciiAnswer[ML_OUTPUT_BUFFER_SIZE];
  int currentAsciiIndex;
  void decodeCommand();
  static const int HexTable[16];

  void readNewAscii();
  long convert4CharToLong(char c1, char c2, char c3, char c4);
  long convert2CharToLong(char c1, char c2);
  long convert2CharToSignedLong(char c1, char c2);

  void convertLongToChar(long value, int nbChar, char *buffer);
};

#endif //Moonlite_h

