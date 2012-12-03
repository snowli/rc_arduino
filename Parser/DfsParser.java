/*
@ver 0
@author Sriram Jayakumar
@date 12/2/2012
@desc Given a list, populates and prints an array. The format of the list is
	<dim>
	<data>
	<data>
	...
	
	-<dim> is a single non-negative integer specifying the side length
	of the 2-dimensional array
	-<data> is a space separated list of 3 integers. The first specifies
	the number to print in the array. The next two specify x and y coordinates
	to place the number in. The x and y coordinates must fit in the array.
@improvements
	-Handle bad user input and exceptions gracefully
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
		//##Setup##
		BufferedReader f = new BufferedReader(new FileReader(args[0]));
		int dim = 0;
		boolean start = true;
		
		//Instantiate a placeholder to avoid compilation errors.
		ArrayList<Integer>[][] grid = new ArrayList[1][1];
		
		//##Parse##
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
		
		//##Output##

		//Column Alignment Calc. Make sure all the columns are vertically
		//aligned by using the Java Formatter's width property.
		//The Formatter will output at least <width> number of characters
		//, using spaces as padding. Right justification is the default.
		int[] maxCharsPerCol = new int[dim];
		for(int i = 0; i < dim; i++)
		{
			for(int j = 0; j < dim; j++)
			{
				int numChars = 0;
				if(grid[i][j] != null)
				{
					for(Integer n : grid[i][j])
					{
						//Each number is assigned an additional comma,
						//hence the +1
						numChars += n.toString().length() + 1;
					}
				}
				
				//2 parentheses, space
				numChars += 3;
			
				if(numChars > maxCharsPerCol[i])
				{
					maxCharsPerCol[i] = numChars;
				}
			}		
		}
		
		//Actual Output
		for(int i = dim-1; i >= 0; i--)
		{
			for(int j = 0; j < dim; j++)
			{
				String out = "(";
				if(grid[j][i] != null)
				{
					for(Integer n : grid[j][i])
					{
						out = out.concat(n.toString()+",");
					}
				}
				out = out.concat(")");
				
				//The first two percents come out to
				//one literal %. Determined by trial and error.
				String format = String.format("%%%1$ds", maxCharsPerCol[j]);
				System.out.printf(format,out);
			}
			System.out.println();
		}
	}
}