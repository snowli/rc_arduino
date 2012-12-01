/*
Prototype Movement API. Provide rotate, unitForward, unitReverse.

@author Sriram Jayakumar
@date 11/10/2012
   */
#define FORWARD_PIN 1
#define REVERSE_PIN 2
#define LEFT_PIN 3
#define RIGHT_PIN 4

#include <Wire.h> 

//Compass hardware address
//The Wire library uses 7-bit I2C addressing, rather than 8-bits
const char compassAddr = 0x42>>1;

/*
Provides the heading, according to the compass hardware. This function
waits 10 ms for the hardware processing to occur. File private. 

@args
-none
@return 
-The heading of the car, in degrees, as a value between
0 and 360. The precision is to tenths of degrees.
-Returns -1 on errors
@todo
-Handle I2C, hardware errors. Perhaps set an error flag that another
periodic function checks.
-Testing
-Power management
@testing
-Looped making readings and wrote to Serial.println(...)
@references
-http://wiring.org.co/learning/libraries/hmc6352sparkfun.html . 
The code is reused from here, with minor modifications.
*/
static double getHeading() {
  //Measure angle
  Wire.beginTransmission(compassAddr);  
  Wire.write('A');
  Wire.endTransmission();

  //Wait for readings to happen 
  delay(10);

  //Read
  Wire.requestFrom(compassAddr, 2);
  if (2 <= Wire.available()) 
  { 
    int reading = Wire.read();  
    reading = reading << 8; 
    reading += Wire.read(); 
    return (reading * 0.1);
  } 
  
  else 
  {
    return -1;
  }
}

static void forward()
{
    directional_pins_off();
    digitalWrite(FORWARD_PIN, HIGH);
}

static void reverse()
{
    directional_pins_off();
    digitalWrite(REVERSE_PIN, HIGH);
}

static void left()
{
    directional_pins_off();
    digitalWrite(LEFT_PIN, HIGH);
}

static void right()
{
    directional_pins_off();
    digitalWrite(RIGHT_PIN, HIGH);
}

static void directional_pins_off()
{
    digitalWrite(FORWARD_PIN, LOW);
    digitalWrite(REVERSE_PIN, LOW);
    digitalWrite(LEFT_PIN, LOW);
    digitalWrite(RIGHT_PIN, LOW);
}

/*
Perfoms a unit step forward

@todo
-Monitor heading and compensate
*/
void unitForward() 
{
  directional_pins_off();
  forward();
  delay(100);
  directional_pins_off();
  delay(1000);
}

/*
Perfoms a unit step forward
*/
void unitReverse()
{
  directional_pins_off();
  reverse();
  delay(100);
  directional_pins_off();
  delay(1000);
}


/*
Rotates counterclockwise to the specified angle, +/- 2 degree.

@args
-int degree Rotate, relatively, by the number of degrees specified. A positive number
means counterclockwise, negative clockwise.
@return
-boolean true success; false failure
@todo
-Error handling
-Rotate clockwise
*/
boolean rotate(int degree)
{
  double start = getHeading();
  double finishUnnormalized = (start+degree)-floor((start+degree)/360)*360+360;
  double finish = finishUnnormalized - floor(finishUnnormalized/360)*360;
  
  double lowerBound = finish-2;
  double upperBound = finish+2;
  
  boolean inRange = false;
  while(!inRange)
  {
    double current = getHeading();
    double lowerAdjusted = lowerBound;
    double upperAdjusted = upperBound;

    //359.5 is within [-.75,1.25]
    if(current>180 && lowerBound<0)
    {
      lowerAdjusted = lowerBound+360;
      upperAdjusted = upperBound+360;
    }
    
    //0.25 is within [358.5,360.5]
    if(current<180 && upperBound>360)
    {
        lowerAdjusted = lowerBound-360;
        upperAdjusted = upperBound-360;
    }

    if(current>lowerAdjusted && current<upperAdjusted)
    {
       inRange = true; 
    }
    
    else
    {
       left();
       delay(1000);
       unitForward();
       delay(1000);
       straightenFromLeft();
       delay(1000);
       unitReverse();
       delay(1000);       
    }
  }
  
  return true;
}

/*
Straightens the wheels after a left turn

@todo
-Guarantee that the wheel are straight using compass
*/
static void straightenFromLeft()
{
  directional_pins_off();
  right();
  delay(40);
  directional_pins_off();   
}

/*
Straightens the wheels after a right turn
*/
static void straightenFromRight()
{
  directional_pins_off();
  left();
  delay(40);
  directional_pins_off();   
}

void setup()
{
    //Direction pins
    pinMode(FORWARD_PIN, OUTPUT);
    pinMode(REVERSE_PIN, OUTPUT);
    pinMode(LEFT_PIN, OUTPUT);
    pinMode(RIGHT_PIN, OUTPUT);
    directional_pins_off();

    //Compass
    Wire.begin();
    
    
}

void loop()
{
	//Rotate 10 deg, then pause.
  rotate(10);
  delay(10000);
}