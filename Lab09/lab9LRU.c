bool LRU (int newnum, int size)
{
	for (i = 0,i < size, i++) //check the entire array to see if the number is in there already or if there are any empty spots
	{
		if (array[i] == newnum) //if the number is in there
		{
			for (k = (i + 1), k < size, i++) // loop over the rest of the array
			{
				if (array[k] == -1) // if you reach a blank spot (I used -1 to represent a spot that has no data)
				{
					array[k-1] = newnum; // make the last number you saw equal to the new entry
					return false; // then retun false (false means no page fault true means page fault)
				}
				else if (k == size-1) // if you have reached the last entry 
				{
					array[k-1] = array[k]; // move the last entry over 1
					array[k] = newnum; // make the new number the last entry
					return false; // return false
				}
				else // if you dont do either of those you are in the middle of the table
				{
					array[k-1] = array [k]; // move the entry you are on down one
				}
			}
		}
		else if (array[i] == -1) // if there is a free space 
		{
			array[i] = newnum; // just put the new number there
			return true; // then return true
		}
	}
	for (i = 1,i < size, i++) // once you are sure that there are blanks and the number is unique loop over the array
	{
		if (k == size-1) // if you are at the end 
		{
			array[k-1] = array[k]; // shift the number over one
			array[k] = newnum; // make the last entry the new number
			return true; // return true
		}
		else // if you are a number in the middle
		{
			array[k-1] = array[k]; // just shift the numbers over one
		}
	}
}