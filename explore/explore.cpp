/*
@author Snow Li
@date 12-1-12

TODO implement check for move before turning
*/

#include "explore.h"

#define OUTPUT_PATH
//#define MANUAL_TURNS
//#define DEBUG_EXPLORE 

extern uint8_t *grid = NULL;
extern uint8_t curr_x = X_START;
extern uint8_t curr_y = Y_START;
extern vector_t dirs_vec[4] = {{0,1},{-1,0},{0,-1},{1,0}};
char *names[4] = {"NORTH", "WEST", "SOUTH", "EAST"};
uint16_t move_number = 0;

// @param bit_index - index into grid to be set to 1
void set_bit(uint16_t bit_index)
{
    uint16_t byte_offset = bit_index >> 3;
    uint8_t bit_offset = 7 - (bit_index%8);
    uint8_t *byte = grid + byte_offset;
    uint8_t to_or = 1 << bit_offset;
    *byte |= to_or;
}

// @param bit_index- the index into grid array to return
// @return - the bit at bit_index in grid
uint8_t get_bit(uint16_t bit_index)
{
    uint16_t byte_offset = bit_index >> 3;
    uint8_t bit_offset = 7 - (bit_index%8);
    uint8_t *byte = grid + byte_offset;
    return (*byte & (1 << bit_offset))>>bit_offset;
}

// @param x - virtual x coordinate
// @param y - virtual y coordinate
// @purpose - translates a virtual x,y coordinate into a bit offset
uint16_t virt_to_bit_index(uint16_t x, uint16_t y)
{
    return (x + y*EXPLORE_RADIUS*2)*BITS_PER_NODE;
}

// @purpose - malloc memory for the virtual grid in which to store gathered information
void initialize_grid()
{
    grid = (uint8_t *)malloc( BITS_PER_NODE * EXPLORE_RADIUS * EXPLORE_RADIUS / 2);
    memset(grid, 0, BITS_PER_NODE * EXPLORE_RADIUS * EXPLORE_RADIUS / 2);

    // set the start position to status explored
    set_bit( virt_to_bit_index(X_START, Y_START) + EXPLORED_BIT_OFFSET);

#ifdef OUTPUT_PATH
    // 1st line is dimension of exploration grid
    // 2nd line is <move_number><start x coord><start y coord>
    Serial.println(EXPLORE_RADIUS*2);
    Serial.print(move_number);
    Serial.print(" ");
    Serial.print(X_START);
    Serial.print(" ");
    Serial.println(Y_START);
    move_number++;
#endif
}

// @purpose - free allocated memory for grid
void free_grid()
{
    free(grid);
}

// @param x - x location to be moved into
// @param y - y location to be moved into
// @return - 1 on success, 0 failure
int move_forward_block(int8_t x, int8_t y)
{
    
    curr_x = x;
    curr_y = y;
    int steps = BLOCK_SIZE / STEP_SIZE;
    int i;
    int j;

    uint16_t bit_index = virt_to_bit_index(x, y);
    uint8_t is_explored = get_bit(bit_index + EXPLORED_BIT_OFFSET);

    if( !is_explored )
    {

#ifdef DEBUG_EXPLORE
        Serial.print("x: ");
        Serial.print(x);
        Serial.print(", y: ");
        Serial.println(y);
#endif

        set_bit(bit_index + EXPLORED_BIT_OFFSET);

        for( i=0; i<steps; i++)
        {
            for( j=0; j<UNITS_PER_STEP; j++)
            {
                if( get_bit(bit_index + OBSTRUCTED_BIT_OFFSET) )
                {
#ifdef DEBUG_EXPLORE
                    Serial.println("OBSTRUCTION FOUND");
#endif
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
#ifdef DEBUG_EXPLORE 
        Serial.println("already explored");
#endif
        return 0;
    }
    
}

// @purpose - move the car backward by 1 block (where a block is an entire grid cell)
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

// @param x - x coordinate
// @param y - y coordinate
// @param dir - integer representation of direction faced
// @return 1 if in bounds, otherwise 0 
// @purpose - to check if x,y is within the grid efficiently using the dir provided
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

// @param x - x coord in virtual grid
// @param y - y coord in virtual grid
// @param dir - direction faced by the rc car currently
void explore(int8_t x, int8_t y, int8_t dir)
{   
    //FORWARD
    int8_t x0 = x + dirs_vec[dir].x;
    int8_t y0 = y + dirs_vec[dir].y;
    if( is_in_bounds(x0, y0, dir) )
    {
        if(move_forward_block(x0, y0))
        {

#ifdef DEBUG_EXPLORE
            Serial.print("explore: ");
            Serial.println(names[dir]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x0);
            Serial.print(" ");
            Serial.println(y0);
#endif

            explore(x0, y0, dir);
            move_backward_block();

#ifdef DEBUG_EXPLORE
            Serial.print("backwards: ");
            Serial.println(names[(dir+2)%4]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
#endif

        }
    }

    //LEFT
    int8_t dir1 = (dir+1)%4;
    int8_t x1 = x + dirs_vec[dir1].x;
    int8_t y1 = y + dirs_vec[dir1].y;
    if( is_in_bounds(x1, y1, dir1) )
    {
#ifdef MANUAL_TURNS
        rotateLeft();
#endif
        if(move_forward_block(x1, y1))
        {

#ifdef DEBUG_EXPLORE
            Serial.print("explore: ");
            Serial.println(names[dir1]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x1);
            Serial.print(" ");
            Serial.println(y1);
#endif

            explore(x1, y1, dir1);
            move_backward_block();

#ifdef DEBUG_EXPLORE
            Serial.print("backwards: ");
            Serial.println(names[(dir1+2)%4]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
#endif
        }
#ifdef MANUAL_TURNS
        rotateLeft(); 
        rotateLeft(); 
#endif
    }
    else
    {
#ifdef MANUAL_TURNS
        rotateLeft();
        rotateLeft();
        rotateLeft();
#endif
    }


    //RIGHT
    int8_t dir2 = (dir+3)%4;
    int8_t x2 = x + dirs_vec[dir2].x;
    int8_t y2 = y + dirs_vec[dir2].y;
    if( is_in_bounds(x2, y2, dir2) )
    {
        if(move_forward_block(x2, y2))
        {
#ifdef DEBUG_EXPLORE
            Serial.print("explore: ");
            Serial.println(names[dir2]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x2);
            Serial.print(" ");
            Serial.println(y2);
#endif

            explore(x2, y2, dir2);
            move_backward_block();

#ifdef DEBUG_EXPLORE
            Serial.print("backwards: ");
            Serial.println(names[(dir2+2)%4]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
#endif
        }
    }
#ifdef MANUAL_TURNS
    rotateLeft();
    rotateLeft();
    rotateLeft();
#endif

    //BACK
    int8_t dir3 = (dir+2)%4;
    int8_t x3 = x + dirs_vec[dir3].x;
    int8_t y3 = y + dirs_vec[dir3].y;
    if( is_in_bounds(x3, y3, dir3) )
    {
        if(move_forward_block(x3, y3))
        {
#ifdef DEBUG_EXPLORE
            Serial.print("explore: ");
            Serial.println(names[dir3]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x3);
            Serial.print(" ");
            Serial.println(y3);
#endif
            explore(x3, y3, dir3);
            move_backward_block();

#ifdef DEBUG_EXPLORE
            Serial.print("backwards: ");
            Serial.println(names[(dir3+2)%4]);
#endif

#ifdef OUTPUT_PATH
            Serial.print(move_number);
            move_number++;
            Serial.print(" ");
            Serial.print(x);
            Serial.print(" ");
            Serial.println(y);
#endif
        }
#ifdef MANUAL_TURNS
        rotateLeft();
        rotateLeft(); 
#endif
    }
}

// @purpose - interrupt handler to mark a node as obstructed when a physical obstruction is detected by the real world car
void interrupt_handler_explore()
{

#ifdef DEBUG_EXPLORE
    Serial.println("INTERRUPT!!");
#endif

    set_bit( virt_to_bit_index(curr_x, curr_y) + OBSTRUCTED_BIT_OFFSET );
}
