/*
Tests front bumper functionality (hardware). Moves the car
forward until the car hits something, then reverses.

@author Sriram Jayakumar
@date 11/8/2012, 11/27/2012
   */
#define FORWARD_PIN 4
#define REVERSE_PIN 5
#define LEFT_PIN 6
#define RIGHT_PIN 7
#define FRONT_BUMPER 3

//1 := forward. 0 := reverse
volatile int direction; 

void forward()
{
    directional_pins_off();
    digitalWrite(FORWARD_PIN, HIGH);
}

void reverse()
{
    directional_pins_off();
    digitalWrite(REVERSE_PIN, HIGH);
}

void left()
{
    directional_pins_off();
    digitalWrite(LEFT_PIN, HIGH);
}

void right()
{
    directional_pins_off();
    digitalWrite(RIGHT_PIN, HIGH);
}

void directional_pins_off()
{
    digitalWrite(FORWARD_PIN, LOW);
    digitalWrite(REVERSE_PIN, LOW);
    digitalWrite(LEFT_PIN, LOW);
    digitalWrite(RIGHT_PIN, LOW);
}

void setup()
{
    pinMode(FORWARD_PIN, OUTPUT);
    pinMode(REVERSE_PIN, OUTPUT);
    pinMode(LEFT_PIN, OUTPUT);
    pinMode(RIGHT_PIN, OUTPUT);
    directional_pins_off();
    
	direction = 1;
	attachInterrupt(1, bumper, FALLING);
}

/*
Bumper ISR
*/
void bumper()
{
	direction = !direction;
}	

void loop()
{
	//Each step, in either direction, should be small.
	//Accelerate for a small amount of time, then coast
	//to a stop.
	if(direction==1){
		forward();
		delay(100);
		directional_pins_off();
		delay(1000);
	} else{
		reverse();
		delay(100);
		directional_pins_off();
		delay(1000);
	}
}