#include "protocol.h"
#include "Arduino.h"
#include "EEPROM.h"

Protocol::Protocol() {};

boolean Protocol::init(char dim, int startAddr)
{
	if(dim < 0 || dim > 90 || startAddr < 0 || startAddr > 1024)
	{
		return false;
	}
	
	else
	{
		_dim = dim;
		_startAddr = startAddr;
		_currAddr = currAddr;
		_initialized = true;
		return true;
	}
}

boolean Protocol::write(boolean val)
{

	//Not initialized or too many writes
	if(!_initialized || ((_currAddr-_startAddr)*8+_slot+1)>_dim^2)
	{
		return false;
	}
	
	else
	{
		if(!_written)
		{
			EEPROM.write(_currAddr, _dim);
			_currAddr = _currAddr++%1024;
			
			_written = true;
		}
		
		_currVal += val*2^_slot;
		
		if(_slot==7)
		{
			EEPROM.write(_currAddr, _currVal);
			_currVal = 0;
			_currAddr = _currAddr++%1024;
		}
		
		_slot = _slot++%8;
		
		

	}		
}