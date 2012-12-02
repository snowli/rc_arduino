/*
Prototype Movement API. Provide rotate, unitForward, unitReverse.

@author Sriram Jayakumar
@date 11/10/2012
   */
#define FORWARD_PIN 4
#define REVERSE_PIN 5
#define LEFT_PIN 6
#define RIGHT_PIN 7
#define FRONT_BUMPER 3

#define ROTATE_ALLOWABLE_MARGIN 20

#define LED_LEFT_PIN 8
#define LED_RIGHT_PIN 9

#include <Wire.h> 

//Compass hardware address
//The Wire library uses 7-bit I2C addressing, rather than 8-bits
const char compassAddr = 0x42>>1;


//TODO this is part of algo.c
char *grid;
int8_t curr_x;
int8_t curr_y;
//TODO this is part of algo.c

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

  Serial.print("rotate start heading:"); //TODO remove test
  Serial.println(start);//TODO remove test

  double finishUnnormalized = (start+degree)-floor((start+degree)/360)*360+360;
  double finish = finishUnnormalized - floor(finishUnnormalized/360)*360;
  
  double lowerBound = finish-ROTATE_ALLOWABLE_MARGIN;
  double upperBound = finish+ROTATE_ALLOWABLE_MARGIN;
  
  boolean inRange = false;
  while(!inRange)
  {
    double current = getHeading();
    Serial.print("current heading:");//TODO remove test
    Serial.println(current);//TODO remove test
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
    
    //TODO REMOVE SERIAL which was for debugging 
    Serial.begin(9600);
    attachInterrupt(1, interrupt_handler_explore, FALLING);
}
#define START_X 1
#define START_Y 0

void loop()
{
  initialize_grid();
  explore( START_X, START_Y);
  //init();
  //for
  // write
  //flush();
  free_grid();
}

//----------------------------------------------------------
// note: during exploration, default is to always begin in grid quadrant 1 

#define EXPLORE_RADIUS 5 
#define BLOCK_SIZE 1
#define STEP_SIZE 1
#define UNITS_PER_STEP 1

struct node_t{
    /*
    void *forward;
    void *back;
    void *left;
    void *right;
    void *prev;
    */

    // 0 not explored
    // 1 explored
    uint8_t is_explored;
    // 0 not obstructed 
    // 1 obstructed 
    uint8_t is_obstructed;
};


void initialize_grid()
{
    // *4 because we could be starting anywhere in the grid, and this covers it so that we can go at least EXPLORE_RADIUS in any direction
    grid = (char *)malloc( sizeof(node_t) * EXPLORE_RADIUS * EXPLORE_RADIUS * 4);
    memset(grid, 0, sizeof(node_t) * EXPLORE_RADIUS*EXPLORE_RADIUS*4);
}

void free_grid()
{
    free(grid);
}

// @param x - x location to be moved into
// @param y - y location to be moved into
// @return - 1 on success, 0 failure
int move_forward_block(uint8_t x, uint8_t y)
{
    curr_x = x;
    curr_y = y;
    int steps = BLOCK_SIZE / STEP_SIZE;
    int i;
    int j;
    node_t *node = (node_t *)(grid + x + y*EXPLORE_RADIUS*2);
    
    if( !node->is_explored )
    {
        for( i=0; i<steps; i++)
        {
            for( j=0; j<UNITS_PER_STEP; j++)
            {
                if(node->is_obstructed)
                {
                    for(;i>0;i--)
                    {
                        for(j=0; j<UNITS_PER_STEP; j++)
                        {
                            unitReverse();
                        }
                    }
                    return 0;
                }
                unitForward();
            }
        }
        node->is_explored = 1;
        return 1;
    }
    else
    {
        return 0;
    }
    
}

void move_backward_block()
{
    int steps = BLOCK_SIZE / STEP_SIZE;
    int i;
    int j;
    
    for( i=0; i<steps; i++)
    {
        for( j=0; j<UNITS_PER_STEP; j++)
        {
            unitReverse();
        }
    }
}

void explore(uint8_t x, uint8_t y)
{   
    if( x == EXPLORE_RADIUS*2 || x < 0 
        || y > EXPLORE_RADIUS*2 || y < 0)
        return;

    //FORWARD
    Serial.println("Forward");
    if(move_forward_block(x, y+1))
    {
        explore(x, y+1);
        move_backward_block();
    }

    //LEFT
    Serial.println("Left");
    //rotate(90);
    digitalWrite(LED_LEFT_PIN, HIGH);
    if(move_forward_block(x-1, y))
    {
        explore(x-1, y);
        digitalWrite(LED_LEFT_PIN, HIGH);
        digitalWrite(LED_LEFT_PIN, HIGH);
        //rotate(180);
        move_forward_block(x, y);
    }
    else
    {
        digitalWrite(LED_LEFT_PIN, HIGH);
        digitalWrite(LED_LEFT_PIN, HIGH);
        //rotate(180);
    }

    //RIGHT
    Serial.println("Right");
    if(move_forward_block(x+1, y))
    {
        explore(x+1, y);
        move_backward_block();
    }
    digitalWrite(LED_LEFT_PIN, HIGH);
    digitalWrite(LED_LEFT_PIN, HIGH);
    digitalWrite(LED_LEFT_PIN, HIGH);
    //rotate(270);

    //BACK
    Serial.println("Back");
    if(move_forward_block(x, y-1))
    {
        explore(x, y-1);
        move_backward_block();
        digitalWrite(LED_LEFT_PIN, HIGH);
        digitalWrite(LED_LEFT_PIN, HIGH);
        //rotate(180);
    }
    else
    {
        digitalWrite(LED_LEFT_PIN, HIGH);
        //rotate(90);
    }
}

void interrupt_handler_explore()
{
    node_t *curr_node = (node_t *)(grid + curr_x + curr_y*EXPLORE_RADIUS*2);
    curr_node->is_obstructed = 1;
}
