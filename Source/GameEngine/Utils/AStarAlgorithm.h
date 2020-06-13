#pragma once
#include <stdio.h>
#include <vcruntime_string.h>
#include <cfloat>
#include <set>
#include <stack>
// A C++ Program to implement A* Search Algorithm

#define ROW 9 
#define COL 10 

// Creating a shortcut for int, int pair type 
typedef std::pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type 
typedef std::pair<double, std::pair<int, int>> pPair;

// A structure to hold the neccesary parameters 
struct cell
{
	// Row and Column index of its parent 
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
	int parent_i, parent_j;
	// f = g + h 
	double f, g, h;
};

class AStarAlgorithm {

public:

	AStarAlgorithm(int rows, int cols) {
		this->rows = rows;
		this->cols = cols;
	}

	// A Function to find the shortest path between 
	// a given source cell to a destination cell according 
	// to A* Search Algorithm 
	std::stack<Pair> aStarSearch(int*** grid, Pair src, Pair dest)
	{
		// If the source is out of range 
		if (isValid(src.first, src.second) == false)
		{
			printf("Source is invalid\n");
			return std::stack<Pair>();
		}

		// If the destination is out of range 
		if (isValid(dest.first, dest.second) == false)
		{
			printf("Destination is invalid\n");
			return std::stack<Pair>();
		}

		// Either the source or the destination is blocked 
		if (isUnBlocked(*grid, src.first, src.second) == false ||
			isUnBlocked(*grid, dest.first, dest.second) == false)
		{
			printf("Source or the destination is blocked\n");
			return std::stack<Pair>();
		}

		// If the destination cell is the same as source cell 
		if (isDestination(src.first, src.second, dest) == true)
		{
			printf("We are already at the destination\n");
			return std::stack<Pair>();
		}

		// Create a closed list and initialise it to false which means 
		// that no cell has been included yet 
		// This closed list is implemented as a boolean 2D array 
		bool** closedList = new bool* [rows];
		for (int i = 0; i < rows; i++) {
			closedList[i] = new bool[cols];
		}
		
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++)
				closedList[i][j] = false;
		}

		// Declare a 2D array of structure to hold the details 
		//of that cell
		cell* cellDetails = new cell [rows * cols];

		int i, j;

		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				cellDetails[i * rows + j].f = FLT_MAX;
				cellDetails[i * rows + j].g = FLT_MAX;
				cellDetails[i * rows + j].h = FLT_MAX;
				cellDetails[i * rows + j].parent_i = -1;
				cellDetails[i * rows + j].parent_j = -1;
			}
		}

		// Initialising the parameters of the starting node 
		i = src.first, j = src.second;
		cellDetails[i * rows + j].f = 0.0;
		cellDetails[i * rows + j].g = 0.0;
		cellDetails[i * rows + j].h = 0.0;
		cellDetails[i * rows + j].parent_i = i;
		cellDetails[i * rows + j].parent_j = j;
		int** auxMatrix = *grid;
		auxMatrix[i][j] = 3;
		auxMatrix[dest.first][dest.second] = 3;


		/*
		Create an open list having information as-
		<f, <i, j>>
		where f = g + h,
		and i, j are the row and column index of that cell
		Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
		This open list is implenented as a set of pair of pair.*/
		std::set<pPair> openList;

		// Put the starting cell on the open list and set its 
		// 'f' as 0 
		openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

		// We set this boolean value as false as initially 
		// the destination is not reached. 
		bool foundDest = false;

		while (!openList.empty())
		{
			pPair p = *openList.begin();

			// Remove this vertex from the open list 
			openList.erase(openList.begin());

			// Add this vertex to the closed list 
			i = p.second.first;
			j = p.second.second;
			closedList[i][j] = true;

			/*
				Generating all the 8 successor of this cell

					N.W N N.E
					\ | /
					\ | /
					W----Cell----E
						/ | \
					/ | \
					S.W S S.E

				Cell-->Popped Cell (i, j)
				N --> North	 (i-1, j)
				S --> South	 (i+1, j)
				E --> East	 (i, j+1)
				W --> West		 (i, j-1)
				N.E--> North-East (i-1, j+1)
				N.W--> North-West (i-1, j-1)
				S.E--> South-East (i+1, j+1)
				S.W--> South-West (i+1, j-1)*/

				// To store the 'g', 'h' and 'f' of the 8 successors 
			double gNew, hNew, fNew;

			//----------- 1st Successor (North) ------------ 

			// Only process this cell if this is a valid one 
			if (isValid(i - 1, j) == true)
			{
				// If the destination cell is the same as the 
				// current successor 
				if (isDestination(i - 1, j, dest) == true)
				{
					// Set the Parent of the destination cell 
					cellDetails[(i - 1) * rows + j].parent_i = i;
					cellDetails[(i - 1) * rows + j].parent_j = j;
					printf("The destination cell is found\n");
					foundDest = true;
					return tracePath(cellDetails, dest, auxMatrix);
				}
				// If the successor is already on the closed 
				// list or if it is blocked, then ignore it. 
				// Else do the following 
				else if (closedList[i - 1][j] == false &&
					isUnBlocked(*grid, i - 1, j) == true)
				{
					gNew = cellDetails[i * rows + j].g + 1.0;
					hNew = calculateHValue(i - 1, j, dest);
					fNew = gNew + hNew;

					// If it isn’t on the open list, add it to 
					// the open list. Make the current square 
					// the parent of this square. Record the 
					// f, g, and h costs of the square cell 
					//			 OR 
					// If it is on the open list already, check 
					// to see if this path to that square is better, 
					// using 'f' cost as the measure. 
					if (cellDetails[(i - 1) * rows + j].f == FLT_MAX ||
						cellDetails[(i - 1) * rows + j].f > fNew)
					{
						openList.insert(std::make_pair(fNew,
							std::make_pair(i - 1, j)));

						// Update the details of this cell 
						cellDetails[(i - 1) * rows + j].f = fNew;
						cellDetails[(i - 1) * rows + j].g = gNew;
						cellDetails[(i - 1) * rows + j].h = hNew;
						cellDetails[(i - 1) * rows + j].parent_i = i;
						cellDetails[(i - 1) * rows + j].parent_j = j;
					}
				}
			}

			//----------- 2nd Successor (South) ------------ 

			// Only process this cell if this is a valid one 
			if (isValid(i + 1, j) == true)
			{
				// If the destination cell is the same as the 
				// current successor 
				if (isDestination(i + 1, j, dest) == true)
				{
					// Set the Parent of the destination cell 
					cellDetails[(i + 1) * rows + j].parent_i = i;
					cellDetails[(i + 1)* rows + j].parent_j = j;
					printf("The destination cell is found\n");
					foundDest = true;
					return tracePath(cellDetails, dest, auxMatrix);
				}
				// If the successor is already on the closed 
				// list or if it is blocked, then ignore it. 
				// Else do the following 
				else if (closedList[i + 1][j] == false &&
					isUnBlocked(*grid, i + 1, j) == true)
				{
					gNew = cellDetails[i * rows + j].g + 1.0;
					hNew = calculateHValue(i + 1, j, dest);
					fNew = gNew + hNew;

					// If it isn’t on the open list, add it to 
					// the open list. Make the current square 
					// the parent of this square. Record the 
					// f, g, and h costs of the square cell 
					//			 OR 
					// If it is on the open list already, check 
					// to see if this path to that square is better, 
					// using 'f' cost as the measure. 
					if (cellDetails[(i + 1) *rows + j].f == FLT_MAX ||
						cellDetails[(i + 1)* rows + j].f > fNew)
					{
						openList.insert(std::make_pair(fNew, std::make_pair(i + 1, j)));
						// Update the details of this cell 
						cellDetails[(i + 1)* rows + j].f = fNew;
						cellDetails[(i + 1)* rows + j].g = gNew;
						cellDetails[(i + 1)* rows + j].h = hNew;
						cellDetails[(i + 1)* rows + j].parent_i = i;
						cellDetails[(i + 1)* rows + j].parent_j = j;
					}
				}
			}

			//----------- 3rd Successor (East) ------------ 

			// Only process this cell if this is a valid one 
			if (isValid(i, j + 1) == true)
			{
				// If the destination cell is the same as the 
				// current successor 
				if (isDestination(i, j + 1, dest) == true)
				{
					// Set the Parent of the destination cell 
					cellDetails[i * rows + j + 1].parent_i = i;
					cellDetails[i * rows + j + 1].parent_j = j;
					printf("The destination cell is found\n");
					foundDest = true;
					return tracePath(cellDetails, dest, auxMatrix);
				}

				// If the successor is already on the closed 
				// list or if it is blocked, then ignore it. 
				// Else do the following 
				else if (closedList[i][j + 1] == false &&
					isUnBlocked(*grid, i, j + 1) == true)
				{
					gNew = cellDetails[i * rows + j].g + 1.0;
					hNew = calculateHValue(i, j + 1, dest);
					fNew = gNew + hNew;

					// If it isn’t on the open list, add it to 
					// the open list. Make the current square 
					// the parent of this square. Record the 
					// f, g, and h costs of the square cell 
					//			 OR 
					// If it is on the open list already, check 
					// to see if this path to that square is better, 
					// using 'f' cost as the measure. 
					if (cellDetails[i * rows + j + 1].f == FLT_MAX ||
						cellDetails[i * rows + j + 1].f > fNew)
					{
						openList.insert(std::make_pair(fNew,
							std::make_pair(i, j + 1)));

						// Update the details of this cell 
						cellDetails[i * rows + j + 1].f = fNew;
						cellDetails[i * rows + j + 1].g = gNew;
						cellDetails[i * rows + j + 1].h = hNew;
						cellDetails[i * rows + j + 1].parent_i = i;
						cellDetails[i * rows + j + 1].parent_j = j;
					}
				}
			}

			//----------- 4th Successor (West) ------------ 

			// Only process this cell if this is a valid one 
			if (isValid(i, j - 1) == true)
			{
				// If the destination cell is the same as the 
				// current successor 
				if (isDestination(i, j - 1, dest) == true)
				{
					// Set the Parent of the destination cell 
					cellDetails[i * rows + j - 1].parent_i = i;
					cellDetails[i * rows + j - 1].parent_j = j;
					printf("The destination cell is found\n");
					foundDest = true;
					return tracePath(cellDetails, dest, auxMatrix);
				}

				// If the successor is already on the closed 
				// list or if it is blocked, then ignore it. 
				// Else do the following 
				else if (closedList[i][j - 1] == false &&
					isUnBlocked(*grid, i, j - 1) == true)
				{
					gNew = cellDetails[i * rows + j].g + 1.0;
					hNew = calculateHValue(i, j - 1, dest);
					fNew = gNew + hNew;

					// If it isn’t on the open list, add it to 
					// the open list. Make the current square 
					// the parent of this square. Record the 
					// f, g, and h costs of the square cell 
					//			 OR 
					// If it is on the open list already, check 
					// to see if this path to that square is better, 
					// using 'f' cost as the measure. 
					if (cellDetails[i * rows + j - 1].f == FLT_MAX ||
						cellDetails[i * rows + j - 1].f > fNew)
					{
						openList.insert(std::make_pair(fNew,
							std::make_pair(i, j - 1)));

						// Update the details of this cell 
						cellDetails[i * rows + j - 1].f = fNew;
						cellDetails[i * rows + j - 1].g = gNew;
						cellDetails[i * rows + j - 1].h = hNew;
						cellDetails[i * rows + j - 1].parent_i = i;
						cellDetails[i * rows + j - 1].parent_j = j;
					}
				}
			}

			if (isAllowedDiagonally(*grid, i, j) == true) {

				//----------- 5th Successor (North-East) ------------ 

				// Only process this cell if this is a valid one 
				if (isValid(i - 1, j + 1) == true)
				{
					// If the destination cell is the same as the 
					// current successor 
					if (isDestination(i - 1, j + 1, dest) == true)
					{
						// Set the Parent of the destination cell 
						cellDetails[(i - 1) * rows + j + 1].parent_i = i;
						cellDetails[(i - 1) * rows + j + 1].parent_j = j;
						printf("The destination cell is found\n");
						foundDest = true;
						return tracePath(cellDetails, dest, auxMatrix);
					}

					// If the successor is already on the closed 
					// list or if it is blocked, then ignore it. 
					// Else do the following 
					else if (closedList[i - 1][j + 1] == false &&
						isUnBlocked(*grid, i - 1, j + 1) == true &&
						isAllowedDiagonally(*grid, i - 1, j + 1) == true)
					{
						gNew = cellDetails[i * rows + j].g + 1.414;
						hNew = calculateHValue(i - 1, j + 1, dest);
						fNew = gNew + hNew;

						// If it isn’t on the open list, add it to 
						// the open list. Make the current square 
						// the parent of this square. Record the 
						// f, g, and h costs of the square cell 
						//			 OR 
						// If it is on the open list already, check 
						// to see if this path to that square is better, 
						// using 'f' cost as the measure. 
						if (cellDetails[(i - 1) * rows + j + 1].f == FLT_MAX ||
							cellDetails[(i - 1) * rows + j + 1].f > fNew)
						{
							openList.insert(std::make_pair(fNew,
								std::make_pair(i - 1, j + 1)));

							// Update the details of this cell 
							cellDetails[(i - 1) * rows + j + 1].f = fNew;
							cellDetails[(i - 1) * rows + j + 1].g = gNew;
							cellDetails[(i - 1) * rows + j + 1].h = hNew;
							cellDetails[(i - 1) * rows + j + 1].parent_i = i;
							cellDetails[(i - 1) * rows + j + 1].parent_j = j;
						}
					}
				}

				//----------- 6th Successor (North-West) ------------ 

				// Only process this cell if this is a valid one 
				if (isValid(i - 1, j - 1) == true)
				{
					// If the destination cell is the same as the 
					// current successor 
					if (isDestination(i - 1, j - 1, dest) == true)
					{
						// Set the Parent of the destination cell 
						cellDetails[(i - 1) * rows + j - 1].parent_i = i;
						cellDetails[(i - 1) * rows + j - 1].parent_j = j;
						printf("The destination cell is found\n");
						foundDest = true;
						return tracePath(cellDetails, dest, auxMatrix);
					}

					// If the successor is already on the closed 
					// list or if it is blocked, then ignore it. 
					// Else do the following 
					else if (closedList[i - 1][j - 1] == false &&
						isUnBlocked(*grid, i - 1, j - 1) == true &&
						isAllowedDiagonally(*grid, i - 1, j - 1) == true)
					{
						gNew = cellDetails[i * rows + j].g + 1.414;
						hNew = calculateHValue(i - 1, j - 1, dest);
						fNew = gNew + hNew;

						// If it isn’t on the open list, add it to 
						// the open list. Make the current square 
						// the parent of this square. Record the 
						// f, g, and h costs of the square cell 
						//			 OR 
						// If it is on the open list already, check 
						// to see if this path to that square is better, 
						// using 'f' cost as the measure. 
						if (cellDetails[(i - 1) * rows + j - 1].f == FLT_MAX ||
							cellDetails[(i - 1) * rows + j - 1].f > fNew)
						{
							openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j - 1)));
							// Update the details of this cell 
							cellDetails[(i - 1) * rows + j - 1].f = fNew;
							cellDetails[(i - 1) * rows + j - 1].g = gNew;
							cellDetails[(i - 1) * rows + j - 1].h = hNew;
							cellDetails[(i - 1) * rows + j - 1].parent_i = i;
							cellDetails[(i - 1) * rows + j - 1].parent_j = j;
						}
					}
				}

				//----------- 7th Successor (South-East) ------------ 

				// Only process this cell if this is a valid one 
				if (isValid(i + 1, j + 1) == true)
				{
					// If the destination cell is the same as the 
					// current successor 
					if (isDestination(i + 1, j + 1, dest) == true)
					{
						// Set the Parent of the destination cell 
						cellDetails[(i + 1) * rows + j + 1].parent_i = i;
						cellDetails[(i + 1) * rows + j + 1].parent_j = j;
						printf("The destination cell is found\n");
						foundDest = true;
						return tracePath(cellDetails, dest, auxMatrix);
					}

					// If the successor is already on the closed 
					// list or if it is blocked, then ignore it. 
					// Else do the following 
					else if (closedList[i + 1][j + 1] == false &&
						isUnBlocked(*grid, i + 1, j + 1) == true &&
						isAllowedDiagonally(*grid, i + 1, j + 1) == true)
					{
						gNew = cellDetails[i * rows + j].g + 1.414;
						hNew = calculateHValue(i + 1, j + 1, dest);
						fNew = gNew + hNew;

						// If it isn’t on the open list, add it to 
						// the open list. Make the current square 
						// the parent of this square. Record the 
						// f, g, and h costs of the square cell 
						//			 OR 
						// If it is on the open list already, check 
						// to see if this path to that square is better, 
						// using 'f' cost as the measure. 
						if (cellDetails[(i + 1) * rows + j + 1].f == FLT_MAX ||
							cellDetails[(i + 1) * rows + j + 1].f > fNew)
						{
							openList.insert(std::make_pair(fNew,
								std::make_pair(i + 1, j + 1)));

							// Update the details of this cell 
							cellDetails[(i + 1) * rows + j + 1].f = fNew;
							cellDetails[(i + 1) * rows + j + 1].g = gNew;
							cellDetails[(i + 1) * rows + j + 1].h = hNew;
							cellDetails[(i + 1) * rows + j + 1].parent_i = i;
							cellDetails[(i + 1) * rows + j + 1].parent_j = j;
						}
					}
				}

				//----------- 8th Successor (South-West) ------------ 

				// Only process this cell if this is a valid one 
				if (isValid(i + 1, j - 1) == true)
				{
					// If the destination cell is the same as the 
					// current successor 
					if (isDestination(i + 1, j - 1, dest) == true)
					{
						// Set the Parent of the destination cell 
						cellDetails[(i + 1) * rows + j - 1].parent_i = i;
						cellDetails[(i + 1) * rows + j - 1].parent_j = j;
						printf("The destination cell is found\n");
						foundDest = true;
						return tracePath(cellDetails, dest, auxMatrix);
					}

					// If the successor is already on the closed 
					// list or if it is blocked, then ignore it. 
					// Else do the following 
					else if (closedList[i + 1][j - 1] == false &&
						isUnBlocked(*grid, i + 1, j - 1) == true &&
						isAllowedDiagonally(*grid, i + 1, j - 1) == true)
					{
						gNew = cellDetails[i * rows + j].g + 1.414;
						hNew = calculateHValue(i + 1, j - 1, dest);
						fNew = gNew + hNew;

						// If it isn’t on the open list, add it to 
						// the open list. Make the current square 
						// the parent of this square. Record the 
						// f, g, and h costs of the square cell 
						//			 OR 
						// If it is on the open list already, check 
						// to see if this path to that square is better, 
						// using 'f' cost as the measure. 
						if (cellDetails[(i + 1) * rows + j - 1].f == FLT_MAX ||
							cellDetails[(i + 1) * rows + j - 1].f > fNew)
						{
							openList.insert(std::make_pair(fNew,
								std::make_pair(i + 1, j - 1)));

							// Update the details of this cell 
							cellDetails[(i + 1) * rows + j - 1].f = fNew;
							cellDetails[(i + 1) * rows + j - 1].g = gNew;
							cellDetails[(i + 1) * rows + j - 1].h = hNew;
							cellDetails[(i + 1) * rows + j - 1].parent_i = i;
							cellDetails[(i + 1) * rows + j - 1].parent_j = j;
						}
					}
				}
			}
		}

		// When the destination cell is not found and the open 
		// list is empty, then we conclude that we failed to 
		// reach the destiantion cell. This may happen when the 
		// there is no way to destination cell (due to blockages) 
		if (foundDest == false)
			printf("Failed to find the Destination Cell\n");

		return std::stack<Pair>();
	}

private:

	int rows;
	int cols;

	// A Utility Function to check whether given cell (row, col) 
	// is a valid cell or not. 
	bool isValid(int row, int col)
	{
		// Returns true if row number and column number 
		// is in range 
		return (row >= 0) && (row < rows) &&
			(col >= 0) && (col < cols);
	}

	// A Utility Function to check whether the given cell is 
	// blocked or not 
	bool isUnBlocked(int **grid, int row, int col)
	{
		// Returns true if the cell is not blocked else false 
		if (grid[row][col] != 1)
			return (true);
		else
			return (false);
	}

	bool isAllowedDiagonally(int** grid, int row, int col)
	{
		// Returns true if the cell is not blocked else false 
		if (grid[row][col] == 0)
			return (true);
		else
			return (false);
	}

	// A Utility Function to check whether destination cell has 
	// been reached or not 
	bool isDestination(int row, int col, Pair dest)
	{

		if (row == dest.first && col == dest.second)
			return (true);
		else
			return (false);
	}

	// A Utility Function to calculate the 'h' heuristics. 
	double calculateHValue(int row, int col, Pair dest)
	{
		// Return using the distance formula 
		return ((double)sqrt((row - dest.first) * (row - dest.first)
			+ (col - dest.second) * (col - dest.second)));
	}

	// A Utility Function to trace the path from the source 
	// to destination 
	std::stack<Pair> tracePath(cell *cellDetails, Pair dest, int** matrix)
	{
		printf("\nThe Path is ");
		int row = dest.first;
		int col = dest.second;

		std::stack<Pair> Path;
		std::stack<Pair> res;

		while (!(cellDetails[row * rows + col].parent_i == row
			&& cellDetails[row * rows + col].parent_j == col))
		{
			Path.push(std::make_pair(row, col));
			int temp_row = cellDetails[row * rows + col].parent_i;
			int temp_col = cellDetails[row * rows + col].parent_j;
			row = temp_row;
			col = temp_col;
		}

		Path.push(std::make_pair(row, col));
		res = Path;
		while (!Path.empty())
		{
			std::pair<int, int> p = Path.top();
			Path.pop();
			printf("\n-> (%d,%d)", p.first, p.second);
			matrix[p.first][p.second] = 2;
		}
		printf("\n");

		return res;
	}
	
};
