#include "explore.h"

extern char *grid = NULL;
extern uint8_t curr_x = X_START;
extern uint8_t curr_y = Y_START;
extern vector_t dirs_vec[4] = {{0,1},{-1,0},{0,-1},{1,0}};

void initialize_grid()
{
    // *4 because we could be starting anywhere in the grid, and this covers it so that we can go at least EXPLORE_RADIUS in any direction
    grid = (char *)malloc( sizeof(node_t) * EXPLORE_RADIUS * EXPLORE_RADIUS * 4);
    memset(grid, 0, sizeof(node_t) * EXPLORE_RADIUS*EXPLORE_RADIUS*4);

    // set the start position to status explored
    node_t *node = (node_t *)(grid +(X_START + Y_START*EXPLORE_RADIUS*2)*sizeof(node_t));
    node->is_explored = 1;
}

void free_grid()
{
    free(grid);
}

// @param x - x location to be moved into
// @param y - y location to be moved into
// @return - 1 on success, 0 failure
int move_forward_block(int8_t x, int8_t y)
{
    Serial.print("x: ");
    Serial.print(x);
    Serial.print("y: ");
    Serial.println(y);
    curr_x = x;
    curr_y = y;
    int steps = BLOCK_SIZE / STEP_SIZE;
    int i;
    int j;
    node_t *node = (node_t *)(grid + (x + y*EXPLORE_RADIUS*2)*sizeof(node_t));
    
    if( !(node->is_explored) )
    {
        node->is_explored = 1;
        for( i=0; i<steps; i++)
        {
            for( j=0; j<UNITS_PER_STEP; j++)
            {
                if((node->is_obstructed))
                {
                    Serial.println("OBSTRUCTION FOUND");
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
        return 1;
    }
    else
    {
        Serial.println("already explored");
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

// @return 1 in bounds, 0 out of bounds
int is_in_bounds(int8_t x, int8_t y, int8_t dir)
{
    switch( dir )
    {
        case 0:
            return !(y > EXPLORE_RADIUS*2-1); 
        break;
        case 1:
            return !(x < 0);
        break;
        case 3:
            return !(x > EXPLORE_RADIUS*2-1);
        break;
        case 2:
            return !(y < 0);
        break;
        default:
            return 1;
    }

    return 0;
}

void explore(int8_t x, int8_t y, int8_t dir)
{   
    //FORWARD
    int8_t x0 = x + dirs_vec[dir].x;
    int8_t y0 = y + dirs_vec[dir].y;
    if( is_in_bounds(x0, y0, dir) )
    {
        Serial.println(dir);
        if(move_forward_block(x0, y0))
        {
            explore(x0, y0, dir);
            move_backward_block();
        }
    }

    //LEFT
    int8_t dir1 = (dir+1)%4;
    int8_t x1 = x + dirs_vec[dir1].x;
    int8_t y1 = y + dirs_vec[dir1].y;
    rotateLeft();
    if( is_in_bounds(x1, y1, dir1) )
    {
        Serial.println(dir1);
        if(move_forward_block(x1, y1))
        {
            explore(x1, y1, dir1);
            rotateLeft();
            rotateLeft();
            move_forward_block(x1, y1);
        }
        else
        {
            rotateLeft();
            rotateLeft();
        }
    }

    //RIGHT
    int8_t dir2 = (dir+3)%4;
    int8_t x2 = x + dirs_vec[dir2].x;
    int8_t y2 = y + dirs_vec[dir2].y;
    if( is_in_bounds(x2, y2, dir2) )
    {
        Serial.println(dir2);
        if(move_forward_block(x2, y2))
        {
            explore(x2, y2, dir2);
            move_backward_block();
        }
    }
    rotateLeft();
    rotateLeft();
    rotateLeft();

    //BACK
    int8_t dir3 = (dir+2)%4;
    int8_t x3 = x + dirs_vec[dir3].x;
    int8_t y3 = y + dirs_vec[dir3].y;
    if( is_in_bounds(x3, y3, dir3) )
    {
        Serial.println(dir3);
        if(move_forward_block(x3, y3))
        {
            explore(x3, y3, dir3);
            move_backward_block();
            rotateLeft();
            rotateLeft();
        }
        else
        {
            rotateLeft();
            rotateLeft(); //TODO verify that this should actually be this 180 turn
        }
    }
}

void interrupt_handler_explore()
{
    Serial.println("INTERRUPT!!");
    node_t *curr_node = (node_t *)(grid + curr_x + curr_y*EXPLORE_RADIUS*2);
    curr_node->is_obstructed = 1;
}
