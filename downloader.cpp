#include <EEPROM.h>
#include <Wire.h>
#include <movement_api.h>
#include <protocol.h>
#include <explore.h>
#include <app.h>

boolean downloaded = false;
Protocol p;


void setup()
{
	//dimension, address
	p.init(EXPLORE_RADIUS*2,START_ADDR);
	Serial.begin(9600);
}
void loop()
{
	if(!downloaded)
	{
		p.dump();
		downloaded = true;
		Serial.end();
	}
	
}
