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
			node_t *n = (node_t *)(grid+i*sizeof(node_t));
			n->is_obstructed = 1;
		}
	}
	
    
}