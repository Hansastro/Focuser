/*
Moonlite.cpp - - Parser for the Moonlite serial focuser protocol - Version 1.0

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


#include "Moonlite.h"

const int Moonlite::HexTable[16] = {'0', '1', '2', '3', '4',
				    '5', '6', '7', '8', '9',
				    'A', 'B', 'C', 'D', 'E',
				    'F'};

//------------------------------------------------------------------------------
// Constructors
Moonlite::Moonlite()
{
  int i;

  this->newCommandIsAvailable = false;
  for (i=0;i<ML_INPUT_BUFFER_SIZE;i++)  
    this->currentAsciiCommand[i] = 0;
  this->currentAsciiIndex = 0;
  for (i=0; i<ML_OUTPUT_BUFFER_SIZE;i++)
    this->AsciiAnswer[i] = 0;
  this->currentCommand.commandID = 0;
  this->currentCommand.parameter = 0;
}

//------------------------------------------------------------------------------
// Getters

MoonliteCommand_t Moonlite::getCommand()
{
  newCommandIsAvailable = false;
  return currentCommand;
}

//------------------------------------------------------------------------------
// Setters

void Moonlite::setAnswer(int nbChar, long answer)
{
  int i;
  int unknownFormat = false;
  int messageLength = 0;

  // Initialisation of the output buffer
  for (i=0; i<ML_OUTPUT_BUFFER_SIZE;i++)
    this->AsciiAnswer[i] = 0;
  
  convertLongToChar(answer, nbChar, AsciiAnswer);

  if(!unknownFormat)
    {
      // Send the answer
      for(i=0; i < nbChar; i++)
	Serial.write(AsciiAnswer[i]);
      // the last caracter should be a hash
      Serial.write('#');
    }
}

//------------------------------------------------------------------------------
// Other public Members
void Moonlite::init(int baudRate)
{
  // Set the baudrate of the serial port
  Serial.begin(baudRate);
}

int Moonlite::isNewCommandAvailable()
{
  // Return true if a new command was received
  return newCommandIsAvailable;
}

int Moonlite::Manage()
{
  // This procedure should be called regularly.
  // It fills the input buffer with the caracter recived from the serial port
  // On caracter is read every loop. 

  if(Serial.available() > 0)
    {
      readNewAscii();
    }
}

//------------------------------------------------------------------------------
void Moonlite::readNewAscii()
{
  int i;
  char incomingByte = 0;

  incomingByte = Serial.read();
  // The ':' caracter signalizes the begining of the message
  if (incomingByte == ':')
    {
      newCommandIsAvailable = false;
      for(i=0; i<ML_INPUT_BUFFER_SIZE;i++)
	currentAsciiCommand[i] = 0;
      currentAsciiIndex = 0;      
    }
  else
    {
      // The '#' caracter sigalizes the end of the message
      // if '#' is received the message can be decoded
      if (incomingByte == '#')
	{
	  decodeCommand();
	}
      else
	{
	  // If the end of the command is not reach
	  // the next caracter is added to the inout buffer
	  if(currentAsciiIndex < ML_INPUT_BUFFER_SIZE)
	    {
	      currentAsciiCommand[currentAsciiIndex] = incomingByte;
	      currentAsciiIndex ++;
	    }
	}
    }
}

void Moonlite::decodeCommand()
{
  currentCommand.commandID = 0;
  currentCommand.parameter = 0;

  // The command is decoded caracter per caracter and the parameter added if needed.
  switch (currentAsciiCommand[0])
    {
    case 'C':
      currentCommand.commandID = ML_C;
      break;
    case 'F':
      switch (currentAsciiCommand[1])
	{
	case 'G':
	  currentCommand.commandID = ML_FG;
	  break;
	case 'Q':
	  currentCommand.commandID = ML_FQ;
	  break;
	default:
	  currentCommand.commandID = ML_UNKNOWN_COMMAND;
	  break;
	}
      break;
    case 'G':
      switch (currentAsciiCommand[1])
	{
	case 'B':
	  currentCommand.commandID = ML_GB;
	  break;
	case 'C':
	  currentCommand.commandID = ML_GC;
	  break;
	case 'D':
	  currentCommand.commandID = ML_GD;
	  break;
	case 'H':
	  currentCommand.commandID = ML_GH;
	  break;
	case 'I':
	  currentCommand.commandID = ML_GI;
	  break;
	case 'N':
	  currentCommand.commandID = ML_GN;
	  break;
	case 'P':
	  currentCommand.commandID = ML_GP;
	  break;
	case 'T':
	  currentCommand.commandID = ML_GT;
	  break;
	case 'V':
	  currentCommand.commandID = ML_GV;
	  break;
	default:
	  currentCommand.commandID = ML_UNKNOWN_COMMAND;
	  break;
	}
      break;
    case 'S':
      switch (currentAsciiCommand[1])
	{
	case 'C':
	  currentCommand.commandID = ML_SC;
	  currentCommand.parameter = convert2CharToSignedLong(currentAsciiCommand[2], currentAsciiCommand[3]);
	  break;
	case 'D':
	  currentCommand.commandID = ML_SD;
	  currentCommand.parameter = convert2CharToLong(currentAsciiCommand[2], currentAsciiCommand[3]);
	  break;
	case 'F':
	  currentCommand.commandID = ML_SF;
	  break;
	case 'H':
	  currentCommand.commandID = ML_SH;
	  break;
	case 'N':
	  currentCommand.commandID = ML_SN;
	  currentCommand.parameter = convert4CharToLong(currentAsciiCommand[2], currentAsciiCommand[3], currentAsciiCommand[4], currentAsciiCommand[5]);
	  break;
	case 'P':
	  currentCommand.commandID = ML_SP;
	  currentCommand.parameter = convert4CharToLong(currentAsciiCommand[2], currentAsciiCommand[3], currentAsciiCommand[4], currentAsciiCommand[5]);
	  break;
	default:
	  currentCommand.commandID = ML_UNKNOWN_COMMAND;
	  break;
	}
      break;
    case '+':
      currentCommand.commandID = ML_PLUS;
      break;
    case '-':
      currentCommand.commandID = ML_MINUS;
      break;
    case 'P':
      currentCommand.commandID = ML_PO;
      currentCommand.parameter = convert2CharToSignedLong(currentAsciiCommand[2], currentAsciiCommand[3]);
      break;
    default:
      currentCommand.commandID = ML_UNKNOWN_COMMAND;
      break;
    }
  newCommandIsAvailable = true;
}

long Moonlite::convert4CharToLong(char c1, char c2, char c3, char c4)
{
  long value = 0;
  int i;

  for (i=0; i<16; i++)
    {
      if(HexTable[i] == c4)
	{
	  value |= (long)i;
	}
      if(HexTable[i] == c3)
	{
	  value |= ((long)i)<<4;
	}
      if(HexTable[i] == c2)
	{
	  value |= ((long)i)<<8;
	}
      if(HexTable[i] == c1)
	{
	  value |= ((long)i)<<12;
	}
    }
  return value;
}

long Moonlite::convert2CharToLong(char c1, char c2)
{
  long value = 0;
  int i;

  for (i=0; i<16; i++)
    {
      if(HexTable[i] == c2)
	{
	  value |= (long)i;
	}
      if(HexTable[i] == c1)
	{
	  value |= ((long)i)<<4;
	}
    }
  return value;
}

long Moonlite::convert2CharToSignedLong(char c1, char c2)
{
  char value = 0;
  int i;

  for (i=0; i<16; i++)
    {
      if(HexTable[i] == c2)
	{
	  value |= (char)i;
	}
      if(HexTable[i] == c1)
	{
	  value |= ((char)i)<<4;
	}
    }
  return value;
}

void Moonlite::convertLongToChar(long value, int nbChar, char *buffer)
{
  // This function convert signed and unsigned values

  int i;
  int shift = 0;

  for (i=nbChar-1; i >= 0; i--)
    {
      buffer[i] = HexTable[(value >> shift) & 0xF];
      shift = shift + 4;
    }
}
