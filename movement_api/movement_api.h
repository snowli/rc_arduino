/*
Prototype Movement API. Provide rotate, unitForward, unitReverse.

@author Sriram Jayakumar
@date 11/10/2012
   */

#ifndef _MOVEMENT_API_H_
#define _MOVEMENT_API_H_

#include <Arduino.h>
#include <Wire.h>

#define FORWARD_PIN 4
#define REVERSE_PIN 5
#define LEFT_PIN 6
#define RIGHT_PIN 7
#define FRONT_BUMPER 3

#define ROTATE_ALLOWABLE_MARGIN 20

//Compass hardware address
//The Wire library uses 7-bit I2C addressing, rather than 8-bits
const char compassAddr = 0x42>>1;

void unitForward();
void unitReverse();
boolean rotate(int degree);

#endif
