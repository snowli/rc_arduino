/*

0000
1000
1000
1000
*/
void insert_obstacles()
{
	int dim = 4;
    int i;
    for(i=0; i < EXPLORE_RADIUS*EXPLORE_RADIUS*4; i++)
	{
		if(i%dim == 0 && i != 12)
		{
            set_bit( i * BITS_PER_NODE + OBSTRUCTED_BIT_OFFSET );
		}
    }
}
