#include <EEPROM.h>
#include <Wire.h>
#include <explore.h>
#include <movement_api.h>
#include <protocol.h>

Protocol p;
int flag = 1;
void setup()
{
    initialize();
    p = Protocol();
    
    Serial.begin(9600);
    attachInterrupt(1, interrupt_handler_explore, FALLING);
}

void loop()
{
    if( flag )
    {
        flag = 0;

        initialize_grid();
        insert_obstacles();

        explore( X_START, Y_START, 0);

        p.init(EXPLORE_RADIUS*2, 0);

        int i;
        for( i=0; i<EXPLORE_RADIUS*EXPLORE_RADIUS*4; i++)
        {
            node_t *n = (node_t *)(grid+i*sizeof(node_t));
            //Serial.println(n->is_obstructed);
            p.write(n->is_obstructed);
        }
        p.flush();
        free_grid();

        //p.dump();
    }
}

void insert_obstacles()
{
    int i;
    for(i=0; i< EXPLORE_RADIUS*EXPLORE_RADIUS*4; i+=2)
    {
        node_t *n = (node_t *)(grid+i*sizeof(node_t));
        n->is_obstructed = 1;
    }
    
}

