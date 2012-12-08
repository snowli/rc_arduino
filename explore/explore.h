/*
@author Snow Li
@date 12-1-12

Defines the parameters of the virtual grid which the rc_car will logically explore

Notes:
Virtual Grid Abstraction
-a BLOCK is a cell's dimension in the virtual grid
-a BLOCK is made up of multiple STEP's
-a STEP can further be divided into UNIT's of calls to the movement api where a single UNIT is a single call to movement_api's unitForward()
-the purpose of this abstraction is to more precisely back out from a grid cell that is determined to be obstructed, the more STEP's per BLOCK, the more precise the movement of the rc car
-the default start location is x = EXPLORE_RADIUS and y = EXPLORE_RADIUS

Search Algorithm i.e. explore
-this algorithm is recursive and will explore neighboring cells in the following order: Forward, Left, Right, and Back
-movement of the physical car is managed such that any exploration of a previously unexplored grid cell is done facing the grid cell, thus rotate calls are made generously to properly orient the car 
-the coordinates of the grid cell currently being explored are made global so that any interrupts (due to obstructions) may inform any movement functions that the cell is now obstructed and should be retreated from
-retreating from a cell is performed using unitReverse() of the movement api and the algorithms logic attempts to undo all completed STEP's forward, that is, if a BLOCK is 5 steps and 3 steps have been completed when an interrupt occurrs during the 4th step, 3 steps back will be made by the algorithm to retreat from the obstructed block

Memory Management
-memory for the grid abstraction is allocated on call of initalize_grid()
-each grid cell is represented as 2 bits, the higher order bit is reserved for marking an already explored cell and the lower order bit represents whether a cell is obstructed or not
*/

#ifndef _EXPLORE_H_
#define _EXPLORE_H_

#define EXPLORE_RADIUS 2 
#define BLOCK_SIZE 1
#define STEP_SIZE 1
#define UNITS_PER_STEP 1
#define BITS_PER_NODE 2
#define OBSTRUCTED_BIT_OFFSET 1
#define EXPLORED_BIT_OFFSET 0

#define X_START EXPLORE_RADIUS 
#define Y_START EXPLORE_RADIUS

#include <Arduino.h>
#include <movement_api.h>

struct vector_t{
    uint8_t x;
    uint8_t y;
};

extern vector_t dirs_vec[4];
extern uint8_t *grid;
extern uint8_t curr_x;
extern uint8_t curr_y;

void set_bit(uint16_t bit_index);
uint8_t get_bit(uint16_t bit_index);
uint16_t virt_to_bit_index(uint16_t x, uint16_t y);

void initialize_grid();
void free_grid();
int move_forward_block(int8_t x, int8_t y);
void move_backward_block();
void explore(int8_t x, int8_t y, int8_t dir);
void interrupt_handler_explore();

#endif
