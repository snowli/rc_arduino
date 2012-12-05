#include <EEPROM.h>
#include <Wire.h>
#include <explore.h>
#include <movement_api.h>
#include <protocol.h>
#include <app.h>

Protocol p;
int flag = 1;
void setup()
{
	p = Protocol();
	
	//movement_api
    initialize(); 
    
	
    attachInterrupt(1, interrupt_handler_explore, FALLING);
}

void loop()
{
    if( flag )
    {
        flag = 0;

        initialize_grid();

        explore( X_START, Y_START, 0);

        p.init(EXPLORE_RADIUS*2, START_ADDR);

        int i;
        for( i=0; i<EXPLORE_RADIUS*EXPLORE_RADIUS*4; i++)
        {
            node_t *n = (node_t *)(grid+i*sizeof(node_t));
            p.write(n->is_obstructed);
        }
        p.flush();
        free_grid();

    }
}

