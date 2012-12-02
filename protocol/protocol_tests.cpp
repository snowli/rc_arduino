#include "protocol.h"
#include "EEPROM.h"

boolean _tested = false;

void setup()
{
	Serial.begin(9600);
	

}

void loop()
{
	if(!_tested)
	{
		/*
		A dimension 0 Protocol should perform no EEPROM operations.
		
		Expect: 
		EEPROM
			0: 91
		Serial:
			00091
		*/
		EEPROM.write(0, 91);
		Protocol p;
		Serial.print("dimension 0\n");
		boolean success = p.init(0, 0); Serial.print(success);
		success = p.write(true); Serial.print(success);
		success = p.flush(); Serial.print(success);
		
		int val = EEPROM.read(0);
		Serial.print(val, DEC);
	
	
		/*
		dump() unit test
		
		Serial:
			1
			67,0,
			
		*/
		EEPROM.write(15, 3); EEPROM.write(16, 76); EEPROM.write(17, 0);
		Serial.print("\n");
		Serial.print("dump(...)\n");
		success = p.init(3, 15); Serial.print(success); Serial.print("\n");
		p.dump();
		Serial.print("\n");
		
		
	
		/*
		write test
		Expect:
		Serial:
			11111111111 
			41,0 
			1
		*/
		Protocol p1;
		
		Serial.print("write(...) test\n");
		success = p1.init(3, 1023); Serial.print(success);
		boolean array [] = {true,false,false,true,false,true,false,false,false};
		for(int i = 0; i < 9; i++)
		{
			success = p1.write(array[i]); Serial.print(success);
		}
		success = p1.flush(); Serial.print(success);
		Serial.print("\n");
		success = p1.dump(); 
		Serial.print("\n"); Serial.print(success);
	
		_tested = true;
		Serial.end();
	}
	
	

}