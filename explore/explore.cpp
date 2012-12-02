#include "explore.h"

extern char *grid = NULL;
extern uint8_t curr_x = X_START;
extern uint8_t curr_y = Y_START;

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
    rotate(90);
    if(move_forward_block(x-1, y))
    {
        explore(x-1, y);
        rotate(180);
        move_forward_block(x, y);
    }
    else
    {
        rotate(180);
    }

    //RIGHT
    Serial.println("Right");
    if(move_forward_block(x+1, y))
    {
        explore(x+1, y);
        move_backward_block();
    }
    rotate(270);

    //BACK
    Serial.println("Back");
    if(move_forward_block(x, y-1))
    {
        explore(x, y-1);
        move_backward_block();
        rotate(180);
    }
    else
    {
        rotate(90);
    }
}

void interrupt_handler_explore()
{
    node_t *curr_node = (node_t *)(grid + curr_x + curr_y*EXPLORE_RADIUS*2);
    curr_node->is_obstructed = 1;
}
