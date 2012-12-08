/*
1000
0100
0001
1000

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
		if((row==0&&col==0) || (row==1&&col==3) || (row==2&&col==1) || (row==3&&col==0))
		{
            set_bit( i*BITS_PER_NODE + OBSTRUCTED_BIT_OFFSET );
		}
	}
}
