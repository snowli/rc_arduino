/*
Prototype Movement API. Provide rotate, unitForward, unitReverse.

@author Sriram Jayakumar, Snow Li
@date 11/10/2012
   */

#ifndef _MOVEMENT_API_H_
#define _MOVEMENT_API_H_

/*
ROTATE_LEFT_COMPLETE -- User input to specify when a manual left rotation is complete. The input should be low when the rotation is complete.
LEFT_LED -- Indicates the car is requesting a left rotate
*/
#define FORWARD_PIN 4
#define REVERSE_PIN 5
#define LEFT_PIN 6
#define RIGHT_PIN 7
#define LEFT_LED 8
#define ROTATE_LEFT_COMPLETE 9

#define ROTATE_ALLOWABLE_MARGIN 20

//Define MANUAL_ROTATE to let an external user rotate the car
//whenever a rotation is required.
#define MANUAL_ROTATE

#include <Arduino.h>
#include <Wire.h>

//Compass hardware address
//The Wire library uses 7-bit I2C addressing, rather than 8-bits
const char compassAddr = 0x42>>1;

void unitForward();
void unitReverse();

/*
@desc Rotates left by 90 degrees
*/
void rotateLeft();

/*
@desc Initialize the car for movement. Sets up pins etc.
*/
void initialize();

#endif
