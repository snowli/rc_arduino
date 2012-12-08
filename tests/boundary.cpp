/*
4x4
1111
1001
1001
1111

start at 2,2

*/
void insert_obstacles()
{
    int i;
	const int dim = 4;
    for(i=0; i< EXPLORE_RADIUS*EXPLORE_RADIUS*4; i++)
    {
		int row = i/dim;
		int col = i%dim;
		if(row == 0 || row == (dim-1) || col == 0 || col == (dim-1))
		{
            set_bit( i*BITS_PER_NODE + OBSTRUCTED_BIT_OFFSET );
		}
	}
	
    
}
