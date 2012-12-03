/*
Assume a 4x4 grid. 0 := no obstacle. 1 := obstacle.

0000
0100
0100
0100
*/
void insert_obstacles()
{
	int dim = 4;
    int i;
    for(i=0; i < EXPLORE_RADIUS*EXPLORE_RADIUS*4; i++)
	{
		if(i%dim == 1 && i != 13)
		{
			node_t *n = (node_t *)(grid+i*sizeof(node_t));
			n->is_obstructed = 1;
		}
    }
}

