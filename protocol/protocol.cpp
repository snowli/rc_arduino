/*
@author Sriram Jayakumar
@date 12/1/2012
@ver 1.0
*/

#include "protocol.h"
#include "Arduino.h"
#include "EEPROM.h"

#define DIM_LOWER_BOUND 1
#define DIM_UPPER_BOUND 90
#define START_ADDR_LOWER_BOUND 0
#define START_ADDR_UPPER_BOUND 1024

Protocol::Protocol() 
{
	_initialized = false;
	_written = false;
	_currVal = 0;
	_slot = 0;
	_dim = 0;
}

boolean Protocol::init(uint8_t dim, int startAddr)
{
	if(dim < DIM_LOWER_BOUND || dim > DIM_UPPER_BOUND || startAddr < START_ADDR_LOWER_BOUND || startAddr > START_ADDR_UPPER_BOUND || _initialized)
	{
		return false;
	}
	
	else
	{
		_dim = dim;
		_startAddr = startAddr;
		_currAddr = startAddr;
		_initialized = true;
		return true;
	}
}

boolean Protocol::write(boolean val)
{

	//Not initialized or if this write will exceed
	//the threshold
	int currentWriteNum = 0;
	if(_currAddr < _startAddr)
	{
		currentWriteNum = (((START_ADDR_UPPER_BOUND-_startAddr+_currAddr)-1)*8)+_slot+1;
	}
	
	else
	{
		currentWriteNum = ((_currAddr-_startAddr-1)*8+_slot+1);
	}
	
	
	if(!_initialized || currentWriteNum>pow(_dim,2))
	{
		return false;
	}
	
	else
	{
		if(!_written)
		{
			EEPROM.write(_currAddr, _dim);
			_currAddr = (_currAddr++)%START_ADDR_UPPER_BOUND;
			
			_written = true;
		}
		
		
		//Set the relevant bit, specified by _slot, to val.
		_currVal += (val*(1 << _slot));
		
		if(_slot==7)
		{
			EEPROM.write(_currAddr, _currVal);
			_currVal = 0;
			_currAddr = (_currAddr+1)%START_ADDR_UPPER_BOUND;
		}
		
		_slot = (_slot+1)%8;
		
		return true;
	}		
}

boolean Protocol::flush()
{
	if(!_written || !_initialized)
	{
		return false;
	}
	
	else
	{
		if(_slot != 0)
		{
			EEPROM.write(_currAddr, _currVal);
		}
	
		return true;
	}
}

boolean Protocol::dump()
{
	uint8_t dim = EEPROM.read(_startAddr);
	
	if(dim < DIM_LOWER_BOUND || dim > DIM_UPPER_BOUND || !_initialized)
	{
		return false;
	}
	
	else
	{
		//ceil(dim^2/8)
		int numDataBytes = pow(dim,2)/8;
		if(numDataBytes*8 < pow(dim,2))
		{
			numDataBytes++;
		}
		
		Serial.print(dim, DEC); Serial.print(",");
		for(int i = 0; i < numDataBytes; i++)
		{
			uint8_t data = EEPROM.read((_startAddr+i+1)%START_ADDR_UPPER_BOUND);
			Serial.print(data, DEC);
			Serial.print(",");
		}
	
		return true;
	}
}