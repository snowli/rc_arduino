// note: during exploration, default is to always begin in grid quadrant 1 

#ifndef _EXPLORE_H_
#define _EXPLORE_H_

#define EXPLORE_RADIUS 5 
#define BLOCK_SIZE 1
#define STEP_SIZE 1
#define UNITS_PER_STEP 1

#include "/usr/share/arduino/libraries/movement_api/movement_api.cpp"

struct node_t{
    // 0 not explored
    // 1 explored
    uint8_t is_explored;
    // 0 not obstructed 
    // 1 obstructed 
    uint8_t is_obstructed;
};

char *grid;
uint8_t curr_x;
uint8_t curr_y;

void intialize_grid();
void free_grid();
int move_forward_block(uint8_t x, uint8_t y);
void move_backward_block();
void explore(uint8_t x, uint8_t y);
void interrupt_handler_explore();

#endif
