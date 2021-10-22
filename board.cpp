// board.cpp: implementation of the board class.
// this class has functionalities for moving a piece inside the board
// checking if the piece can move, counting the score, level, speed
// and checking if the game is over
//////////////////////////////////////////////////////////////////////
#include "board.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//constructor
board::board()
{
	InitialiseBoard();  //initialise board variables
}

//destructor
board::~board()
{

}

//initialise board variables
BOOL board::InitialiseBoard()
{
	ResetMatrix(BoardMatrix); //set all the elements of the board matrix to zero
	SetLineScore(); 
	SetLevelConditions();
	level = 1;
	_x = 0;
	_y = 0;
	dropSpeed = 20;
	score = 0;
	return true;
}

//initialise the score for lines; 4 lines at once -> 1000 points
BOOL board::SetLineScore()
{
	score_line[0] = 0;
	score_line[1] = 50;
	score_line[2] = 200;
	score_line[3] = 500;
	score_line[4] = 1000;
	
	return TRUE;
}

//set the condition of passing to next level
//for now only the score is a condition for next level
BOOL board::SetLevelConditions()
{
	score_level[0] = 200;
	score_level[1] = 300;
	score_level[2] = 400;
	score_level[3] = 500;
	score_level[4] = 600;
	score_level[6] = 700;
	score_level[7] = 800;
	score_level[8] = 900;
	score_level[9] = 1200;
	score_level[10] = 1300;
	score_level[11] = 1400;
	score_level[12] = 2000;
	score_level[13] = 3000;
	score_level[14] = 5000;
	score_level[15] = 10000;
	score_level[16] = 50000;

	return TRUE;
}

//set the elements of the given matrix to 0
BOOL board::ResetMatrix(int matrix[BoardRows][BoardColumns])
{
	for (int rowIndex = 0; rowIndex<BoardRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<BoardColumns; colIndex++)
		{
			matrix[rowIndex][colIndex] = 0;
		}
	}

	return TRUE;
}


//check if the piece can move down on the row
//parameter -> a pointer to a piece object
//returns TRUE if row is free, false if not
BOOL board::IsRowFree(piece * pCurrentPiece)
{
	BOOL BoolRetVal = TRUE;
	
	//get piece attributes
	int row = pCurrentPiece->_y;
	int col = pCurrentPiece->_x;
	int width = pCurrentPiece->_width;
	int height = pCurrentPiece->_height;
	
	int matrix[PieceMaxRows][PieceMaxRows]; //declare a local matrix

	pCurrentPiece->GetPieceMatrix(matrix); //store in the matrix the piece matrix

	if (row + height < BoardRows) //check if piece reached the bottom of the board
	{
		for (int colIndex = col; colIndex< col + width; colIndex ++)
		{
			for (int rowIndex = row; rowIndex < row + height; rowIndex ++)
			{
				if (BoardMatrix[rowIndex + 1][colIndex] != 0)	//check if current element of the board is filled
				{
					if (matrix[rowIndex - row][colIndex - col] != 0) //check if piece matrix overlaps the board element
					{
						BoolRetVal = FALSE; //the row is not free
						break;	//exit the loop
					}
				}
			}
			
			if (!BoolRetVal) //row not free
				break;	// exit the loop
		}
	}
	else
		BoolRetVal = FALSE;	//row not free, piece reached bottom of the board

	return BoolRetVal; //returns the result
}


//check if the piece can move left/right on the column
//parameters -> a pointer to a piece object
//			   the direction of movement : 1 -> right; -1 -> left
//returns TRUE if column is free, false if not
BOOL board::IsColumnFree(piece * pCurrentPiece, int direction)
{
	BOOL BoolRetVal = TRUE;

	//get piece attributes
	int row = pCurrentPiece->_y;
	int col = pCurrentPiece->_x;
	int width = pCurrentPiece->_width;
	int height = pCurrentPiece->_height;
	
	int matrix[PieceMaxRows][PieceMaxRows];//declare a local matrix

	pCurrentPiece->GetPieceMatrix(matrix); //store in the matrix the piece matrix

	if (direction == 1) //check right margin
	{
		if (col + width >= BoardColumns)
			BoolRetVal = FALSE;
	}
	else
	{
		if (col <= 0)	//check left margin
			BoolRetVal = FALSE;

	}


	if (BoolRetVal)	//if doesnt cross margins
	{
		for (int rowIndex = row; rowIndex< row + height; rowIndex ++)
		{
			for (int colIndex = col; colIndex < col + width; colIndex ++)
			{
				if (BoardMatrix[rowIndex][colIndex+direction] != 0) //check if current element of the board is filled
				{
						if (matrix[rowIndex - row][colIndex - col] != 0) //check if piece matrix overlaps the board element
						{
							BoolRetVal = FALSE; //the row is not free
							break; //exit the loop
						}
				}
			}
			if (!BoolRetVal) //the row is not free
				break; //exit the loop
		}
	}

	return BoolRetVal; //returns the result
}


//check if the game is over 
//meaning the new piece can not move anymore
//parameters: 
BOOL board::CheckGameOver(piece* pCurrentPiece)
{	
	//get piece attributes
	int row = pCurrentPiece->_y;
	int column = pCurrentPiece->_x;
	int width = pCurrentPiece->_width;
	int height = pCurrentPiece->_height;
	
	int matrix[PieceMaxRows][PieceMaxRows];//declare a local matrix

	pCurrentPiece->GetPieceMatrix(matrix); //store in the matrix the piece matrix


	for (int rowIndex = 0; rowIndex < height; rowIndex ++)
	{
		for (int colIndex = 0; colIndex < width; colIndex ++)
		{
			if (matrix[rowIndex][colIndex] == 1)
			{
				if (BoardMatrix[rowIndex + row][colIndex + column] == 1)
					return TRUE;
			}
		}
	}
	return FALSE;
}

// store the piece on the board
BOOL board::PlacePiece(piece* pCurrentPiece)
{
	//get piece attributes
	int row = pCurrentPiece->_y;
	int column = pCurrentPiece->_x;
	int width = pCurrentPiece->_width;
	int height = pCurrentPiece->_height;
	
	int matrix[PieceMaxRows][PieceMaxRows];//declare a local matrix

	pCurrentPiece->GetPieceMatrix(matrix); //get the current piece matrix

	
	// store the piece matrix on the board
	for (int rowIndex = 0; rowIndex < height; rowIndex ++)
	{
		for (int colIndex = 0; colIndex < width; colIndex ++)
		{
			if (matrix[rowIndex][colIndex] == 1)
			{
				BoardMatrix[rowIndex + row][colIndex + column] = 1;
			}
		}
	}

	return TRUE;
}


// tries to moves a piece on the board
BOOL board::MovePiece(piece* pCurrentPiece, int direction)
{
	BOOL canMove = TRUE;
	switch (direction)
	{
		case 0: //try to move it down 
			canMove = IsRowFree(pCurrentPiece);
			if (canMove)
				pCurrentPiece->_y ++;
			break;

		case 1: //try to move it left
			canMove = IsColumnFree(pCurrentPiece, -1);
			if (canMove)
				pCurrentPiece->_x --;

			break;

		case 2: //try to move it right
			canMove = IsColumnFree(pCurrentPiece, 1);
			if (canMove)
				pCurrentPiece->_x ++;

			break;

	}

	return canMove;
}

//shows a message box with the board matrix representation
//for debug purpouse
BOOL board::DisplayMatrix()
{
	char msg[800];
	msg[0] = 0;
	char tmpNumber[8];
	tmpNumber[0] = 0;
	for (int rowIndex = 0; rowIndex<BoardRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<BoardColumns; colIndex++)
		{
			_itoa_s(BoardMatrix[rowIndex][colIndex], tmpNumber, 10);
			strcat_s(msg, tmpNumber);
			strcat_s(msg, " ");
		}
		strcat_s(msg, "\n");
	}
	MessageBox(NULL,msg, "matrix", MB_OK);
	return TRUE;
}

//copy the source matrix into the destination matrix
BOOL board::CopyMatrix(int matrixDestination[BoardRows][BoardColumns], int matrixSource[BoardRows][BoardColumns])
{
	for (int rowIndex = 0; rowIndex<BoardRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<BoardColumns; colIndex++)
		{
			matrixDestination[rowIndex][colIndex] = matrixSource[rowIndex][colIndex];
		}
	}
	return TRUE;
}


//get a copy of the board matrix
BOOL board::GetBoardMatrix(int matrix[BoardRows][BoardColumns])
{
	CopyMatrix(matrix, BoardMatrix);
	return TRUE;
}


//shifts rows down, after one or more rows were completed
//parameters: rowsIndex -> the position of the completed rows in the board
BOOL board::MoveLines(int rowsIndex[])
{

	for (int rIndex = 0; rIndex < PieceMaxRows; rIndex ++)
	{
		if (rowsIndex[rIndex] != 0) //check if row is completed
		{
			for (int rowIndex = rowsIndex[rIndex]; rowIndex >= 0 ; rowIndex --)
			{
				for (int colIndex = 0; colIndex< BoardColumns; colIndex ++)
				{
					if (rowIndex == 0)	//fill first row (the top one) with 0
						BoardMatrix[rowIndex][colIndex] = 0;
					else	//shift the rest of the rows down
						BoardMatrix[rowIndex][colIndex] = BoardMatrix[rowIndex - 1][colIndex];
				}
			}
		}
	}

	return TRUE;
}


//check if level condition is verified, to go to next level
//count the score
//decrease the dropSpeed of the piece if next level
BOOL board::CheckLevelCondition(int rowsCount)
{
	score += score_line[rowsCount];
	while (score >= score_level[level-1])
	{
		level ++;
		dropSpeed --;
	}

	return TRUE;
}

//build a vector with the completed rows
//and rebuild the board matrix, if necessary
//returns the number of the completed rows
int board::EraseExistingLines(int row, int height)
{
	bool fullRow;
	int rowsIndex[PieceMaxRows];
	int rowsCount = 0;

	for (int rowIndex = 0; rowIndex< PieceMaxRows; rowIndex ++)
		rowsIndex[rowIndex] = 0;

	for (int rowIndex = row; rowIndex< row + height; rowIndex ++)
	{
		fullRow = true;
		for (int colIndex = 0; colIndex< BoardColumns; colIndex ++)
		{
			if (BoardMatrix[rowIndex][colIndex] == 0)
			{
				fullRow = false;
				break;
			}
		}
		if (fullRow)
		{
			rowsIndex[rowIndex - row] = rowIndex;
			rowsCount ++;
		}
	}

	CheckLevelCondition(rowsCount);
	MoveLines(rowsIndex);
	return rowsCount;
}

//get a matrix of PieceMaxRows x PieceMaxRows, from the board matrix
//at the specified coordinates
BOOL board::GetPieceSurroundMatrix(int x, int y, int matrix[PieceMaxRows][PieceMaxRows])
{
	for (int rowIndex = 0; rowIndex < PieceMaxRows; rowIndex ++)
		for (int colIndex = 0; colIndex< PieceMaxRows; colIndex ++)
		{
			if ( (y + rowIndex >= BoardRows) || (x + colIndex >= BoardColumns) )
				matrix[rowIndex][colIndex] = 5; //out of board
			else
				matrix[rowIndex][colIndex] = BoardMatrix[y + rowIndex][x + colIndex];
		}
	
	return TRUE;
}
