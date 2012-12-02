/*
@desc
@author Sriram Jayakumar
@date 11/30/2012
@ver 1.0
@bugs
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "Arduino.h"
#include <EEPROM.h>
class Protocol
{
	private:
		//**Status variables**
		boolean _initialized;
		boolean _written;
		
		//**Array**
		char _dim;
		
		//**EEPROM**
		int _startAddr;
		
		//EEPROM address to be written and value to be written
		int _currAddr;
		int _currVal;
		
		//Which bit in the byte needs to be written next
		int _slot;
		
	public:
		/*
		@desc. Provides read and write access to an nxn array of booleans stored in EEPROM. See init(char dim, int startAddr) for how Protocol instances will differ. The first byte stores the dimesnion n of the array. Subsequent bits store booleans, row by row. In any particular byte, the LSB stores the first entry.
		@future improvements
			-Allow 
				rectangular arrays.
				arbitrary data types to be stored in the array.
				out of order writes to any array index.
				easy portability to an EEPROM of a different size.
			-Check for hardware failures.
		*/
		Protocol();
		
		/*
		@desc. Initializes with parameters. Reinitializing multiple times is not possible.
		@in
			-dim Length of one side of the nxn array. 0 < dim <= 90 must be true. 90 is the upper limit because there are only 1023 bytes available for the array data with this formatting scheme.
			-startAddr The address in EEPROM to start writing data at. This makes it somewhat easier to avoid writing the same addresses of EEPROM each time. 0 <= startAddr <= 1024 must hold true.
		@out Whether the initialization was successful. This depends on valid input.
		*/
		boolean init(char dim, int startAddr);
		
		/*
		@desc Flushes any outstanding data to EEPROM, since there are no guarantees on when data is written. An init(...) and write(...), both of which return true, must be called before flush(). This must be called when all writes are finished to guarantee everything is pushed to EEPROM.
		@out Whether the flush succeeded.
		*/
		boolean flush();
		
		/*
		@desc Dumps data to the serial port in a comma-separated list of bytes. An init(...) which returns true must be called before dump(). Behavior is undefined if a serial connection is not opened beforehand.
		@out Whether the dump succeeded. The data in EEPROM must meet the format specification.
		*/
		boolean dump();
		
		/*
		@desc Queues an array value to be written. An init(...) which returns true must be called before write(...).
		@out Whether the write was successful.
		*/
		boolean write(boolean val);
};


#endif
