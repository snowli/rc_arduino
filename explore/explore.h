// note: during exploration, default is to always begin in grid quadrant 1 

#ifndef _EXPLORE_H_
#define _EXPLORE_H_

#define EXPLORE_RADIUS 2 
#define BLOCK_SIZE 1
#define STEP_SIZE 1
#define UNITS_PER_STEP 1

#define X_START 2
#define Y_START 2

#include <Arduino.h>
#include <movement_api.h>

struct node_t{
    // 0 not explored
    // 1 explored
    uint8_t is_explored;
    // 0 not obstructed 
    // 1 obstructed 
    uint8_t is_obstructed;
};

struct vector_t{
    uint8_t x;
    uint8_t y;
};

extern vector_t dirs_vec[4];
extern char *grid;
extern uint8_t curr_x;
extern uint8_t curr_y;

void initialize_grid();
void free_grid();
int move_forward_block(int8_t x, int8_t y);
void move_backward_block();
void explore(int8_t x, int8_t y, int8_t dir);
void interrupt_handler_explore();

#endif
