#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
using namespace sf;
using namespace std;

int absVal(int value)
{
	// returning absolute value
	if (value < 0)
	{
		return -value;
	}
	else
	{
		return value;
	}
}

int calcBarWidth(int matchedCount, int maxSpecies)
{
	// calculating the width of the progress bar
	float floatMaxSpecies = maxSpecies;
	float matchFloat = matchedCount;
	int percentFind = ((matchFloat / floatMaxSpecies) * 119);

	return percentFind;
}

void swapTiles(int gridSpecies[9][8], int row1, int row2, int col1, int col2)
{
	int temp;

	// swapping the species of the species array in the grid
	temp = gridSpecies[row1][col1];
	gridSpecies[row1][col1] = gridSpecies[row2][col2];
	gridSpecies[row2][col2] = temp;
}

void populateGrid(int tileSize, int gridXPos[9][8], int gridYPos[9][8], int gridRow[9][8], int gridCol[9][8], int gridSpecies[9][8])
{
	// seeding rand function
	srand(time(0));

	// populating properties of every tile in the grid
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			gridXPos[row][col] = col * tileSize; // setting the x position of every tile
			gridYPos[row][col] = row * tileSize; // setting the y position of every tile
			gridRow[row][col] = row;			 // setting the row number of every tile
			gridCol[row][col] = col;			 // setting the column number of every tile
			gridSpecies[row][col] = rand() % 7;	 // setting the animal species of every tile
		}
	}
}

void removeTilesHorizontal(int gridSpecies[9][8], int row, int col, int count)
{
	for (int r = row; r >= 0; r--)
	{
		for (int c = col; c < count + col; c++)
		{
			// After shifting the tiles, the top row is filled with random species
			if (r == 0)
			{
				gridSpecies[r][c] = rand() % 7;
			}
			// Shifting the tiles
			else
			{
				gridSpecies[r][c] = gridSpecies[r - 1][c];
			}
		}
	}
}

void removeTilesVertical(int gridSpecies[9][8], int row, int col, int count)
{
	for (int shift = 0; shift < count; shift++)
	{
		for (int r = row + count - 1; r >= 0; r--)
		{
			// After shifting the tiles, the top row is filled with random species
			if (r == 0)
			{
				gridSpecies[r][col] = rand() % 7;
			}
			// Shifting the tiles
			else
			{
				gridSpecies[r][col] = gridSpecies[r - 1][col];
			}
		}
	}
}

bool matchSpecies(int gridSpecies[9][8], int matchedCount[7], int &score, bool isDeleted = true)
{
	// Declaring check variable
	bool validSwap = false;

	// Horizontal Checking
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			// Checking for 3 or more tiles of the same species in a row
			int count = 1;
			while (col + count < 8 && gridSpecies[row][col] == gridSpecies[row][col + count])
				count++;

			// If there are 3 or more tiles of the same species in a row, they are removed
			if (count >= 3)
			{
				if (isDeleted)
				{
					score += (10 * (count - 2));								 // Adding to the score
					matchedCount[gridSpecies[row][col]] += count * 100;	 // Adding to the matched count
					removeTilesHorizontal(gridSpecies, row, col, count); // Removing the tiles
				}
				validSwap = true;
			}
		}
	}

	// Vertical Checking
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int count = 1;
			while (row + count < 9 && gridSpecies[row][col] == gridSpecies[row + count][col])
				count++;

			if (count >= 3)
			{
				if (isDeleted)
				{
					score += (10 * (count - 2));
					matchedCount[gridSpecies[row][col]] += count * 100;
					removeTilesVertical(gridSpecies, row, col, count);
				}
				validSwap = true;
			}
		}
	}

	return validSwap;
}

bool anyPossibleMoves(int gridSpecies[9][8], int &score)
{
	// Declaring a copy of the grid
	int copyGridSpecies[9][8] = {};

	// Copying the grid
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
			copyGridSpecies[row][col] = gridSpecies[row][col];
	}

	// Checking if there are any possible moves
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			// Checking if the tile is not on the bottom row
			if (col + 1 <= 7)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row, col, col + 1);
				if (matchSpecies(copyGridSpecies, 0, score, false))
					return true;
				else
					swapTiles(copyGridSpecies, row, row, col, col + 1);
			}
			// Checking if the tile is not on the rightmost column
			if (col - 1 >= 0)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row, col, col - 1);
				if (matchSpecies(copyGridSpecies, 0, score, false))
					return true;
				else
					swapTiles(copyGridSpecies, row, row, col, col - 1);
			}
			// Checking if the tile is not on the topmost row
			if (row - 1 >= 0)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row - 1, col, col);
				if (matchSpecies(copyGridSpecies, 0, score, false))
					return true;
				else
					swapTiles(copyGridSpecies, row, row - 1, col, col);
			}
			// Checking if the tile is not on the bottom row
			if (row + 1 <= 8)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row + 1, col, col);
				if (matchSpecies(copyGridSpecies, 0, score, false))
					return true;
				else
					swapTiles(copyGridSpecies, row, row + 1, col, col);
			}
		}
	}

	return false;
}
