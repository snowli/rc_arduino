/*
@author Sriram Jayakumar
@date 12/2/2012
@ver 0
@desc Parses and displays output meeting the format produced by protocol.cpp.
See usage() below and protocol.cpp for more details.
@improvements
	-Handle bad user input gracefully.
	-Accurately display unexplored areas of the grid, if a region is 
	blocked off
*/

public class Parser
{

	public static void main(String[] args)
	{
		if(args.length == 1)
		{
			//split. trim.
			String[] data = args[0].split(",");
			for(int i = 0; i < data.length; i++)
			{
				data[i] = data[i].trim();
			}
			
			//parse
			int dim = Integer.parseInt(data[0]);
			int bits = (int)Math.pow(dim,2);
			int[][] grid = new int[dim][dim];
			
			for(int i = 0; i < bits; i++)
			{
				int b = (Integer.parseInt(data[i/8 + 1]) & (1 << (i%8))) 
					>> (i%8);
				grid[i/dim][i%dim] = b;
			}
			
			//print
			for(int i = dim - 1; i >= 0; i--)
			{
				for(int j = 0; j < dim; j++)
				{
					System.out.printf("%d ", grid[i][j]);
				}
				
				System.out.println();
			}
		}

		else
		{
			Parser.usage();
		}
	
	}
	
	public static void usage()
	{
		System.out.println("Parser <s>; s -- A comma separated string of\n" +
			"bytes. Valid input must be comma separated, consist only of\n" +
			"byte-valued numbers 0-255, and be ordered as\n" +
			"{dimension, <data>}. dimension > 0.\n" +
			"0 =< start* < dimension.\n" +
			"<data>.length = ceil(pow(dimension,2)/8)");
	}
}