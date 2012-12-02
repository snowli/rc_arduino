/*
@ver 0
@author Sriram Jayakumar
@date 12/2/2012
@improvements
	-Handle bad user input gracefully
*/

import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

public class DfsParser
{

	public static void main(String[] args) throws FileNotFoundException, IOException
	{
		BufferedReader f = new BufferedReader(new FileReader(args[0]));
	
		int dim = 0;
		boolean start = true;
		ArrayList<Integer>[][] grid = new ArrayList[1][1]; //Placeholder
		
		while(true)
		{
			//File
			String line = f.readLine();
			if(line == null)
			{
				break;
			}
				
			//Parse
			if(start)
			{
				dim = Integer.parseInt(line);
				grid = new ArrayList[dim][dim];
				start = false;
			}
			
			else
			{
				String[] data = line.split("\\s+");
				int moveNum = Integer.parseInt(data[0]);
				int x = Integer.parseInt(data[1]);
				int y = Integer.parseInt(data[2]);
				
				if(grid[x][y] == null)
				{
					grid[x][y] = new ArrayList<Integer>();
				}
				
				grid[x][y].add(new Integer(moveNum));
			}
		}
		
		//Output
		for(int i = dim-1; i >= 0; i--)
		{
			for(int j = 0; j < dim; j++)
			{
				System.out.print("(");
				if(grid[j][i] != null)
				{
					for(Integer n : grid[j][i])
					{
						System.out.printf("%s, ", n.toString());
					}
				}
				System.out.print(")\t\t\t");
			}
			
			System.out.println();
		}
	}

}