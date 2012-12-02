#include "movement_api.h"

void setup()
{
	initialize();
}

void loop()
{
	for(int i = 0; i < 5; i++)
	{
		unitForward();
	}
	
	rotateLeft();
}