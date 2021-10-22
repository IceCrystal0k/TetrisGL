// piece.cpp: implementation of the piece class.
// functionality for: 
//	set/get the matrix of a piece;
//	translate/rotate the matrix of a piece
//////////////////////////////////////////////////////////////////////

#include "piece.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//constructor
//initialise attributes of the piece
piece::piece()
{
	_rotation = 0;
	_x = 0;
	_y = 0;
	_height = 0;
	_width = 0;
}

piece::~piece()
{

}

//shows a message box with the matrix representation
//for debug purpouse
BOOL piece::DisplayMatrix()
{
	char msg[100];
	msg[0] = 0;
	char tmpNumber[8];
	tmpNumber[0] = 0;
	for (int rowIndex = 0; rowIndex<PieceMaxRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<PieceMaxRows; colIndex++)
		{
			_itoa_s(pieceMatrix[rowIndex][colIndex], tmpNumber, 10);
			strcat_s(msg, tmpNumber);
			strcat_s(msg, " ");
		}
		strcat_s(msg, "\n");
	}
	MessageBox(NULL,msg, "matrix", MB_OK);
	return TRUE;
}

//copy the source matrix into the destination matrix
BOOL piece::CopyMatrix(int matrixDestination[PieceMaxRows][PieceMaxRows], int matrixSource[PieceMaxRows][PieceMaxRows])
{
	for (int rowIndex = 0; rowIndex<PieceMaxRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<PieceMaxRows; colIndex++)
		{
			matrixDestination[rowIndex][colIndex] = matrixSource[rowIndex][colIndex];
		}
	}
	return TRUE;
}

//create the piece from a given matrix
//translates the piece to get the width and height of the piece
BOOL piece::SetPieceMatrix(int matrix[PieceMaxRows][PieceMaxRows])
{
	CopyMatrix(pieceMatrix, matrix);
	TranslatePiece(pieceMatrix);
	_width = _lwidth;
	_height = _lheight;

	return TRUE;
}

//get a copy of the piece matrix
BOOL piece::GetPieceMatrix(int matrix[PieceMaxRows][PieceMaxRows])
{
	CopyMatrix(matrix, pieceMatrix);
	return TRUE;
}

//after rotation, translates the matrix so the pieces parts always start from row 0, column 0
//this means removing the leading zero rows and columns
//also calculates the width and height of the piece
BOOL piece::TranslatePiece(int matrix[PieceMaxRows][PieceMaxRows])
{
	int intLeadingRows = 0, intLeadingColumns = 0; //declare leading rows and columns
	bool boolIsLeadingRow, boolIsLeadingColumn; //semaphors if exists leading rows/columns
	int rowIndex, colIndex;

	_lheight = 0;
	_lwidth = 0;
	
	boolIsLeadingRow = true; 

	//get the count of leading rows and calculate the height of the piece
	for (rowIndex = 0; rowIndex<PieceMaxRows; rowIndex++)
	{		
		for (colIndex = 0; colIndex<PieceMaxRows; colIndex++)
		{
			if (matrix[rowIndex][colIndex] == 1)
			{
				_lheight ++;
				boolIsLeadingRow = false;
				break;
			}
		}
		if (boolIsLeadingRow && _lheight == 0)
			intLeadingRows ++;
	}

	boolIsLeadingColumn = true; 
	//get the count of leading columns and calculate the width of the piece
	for (colIndex = 0; colIndex<PieceMaxRows; colIndex++)
	{
		for (rowIndex = 0; rowIndex <PieceMaxRows; rowIndex ++)
		{
			if (matrix[rowIndex][colIndex] == 1)
			{
				_lwidth ++;
				boolIsLeadingColumn = false;
				break;
			}
		}
		if (boolIsLeadingColumn && _lwidth == 0)
			intLeadingColumns ++;
	}

	//remove leading rows and columns
	if (intLeadingRows > 0 || intLeadingColumns > 0)
		for (rowIndex = intLeadingRows; rowIndex <PieceMaxRows; rowIndex ++)
		{
			for (colIndex = 0; colIndex<PieceMaxRows; colIndex++)
			{
				if (rowIndex - intLeadingRows >=0 && colIndex - intLeadingColumns >= 0)
				matrix[rowIndex - intLeadingRows][colIndex - intLeadingColumns] = matrix[rowIndex][colIndex];
				//set remaining element to 0
				matrix[rowIndex][colIndex] = 0;
			}
		}

	
	return TRUE;
}


//rotates the piece matrix by 90 degrees
//params: a PieceMaxRows x PieceMaxRows matrix of the board, to check if it can rotate the piece matrix
BOOL piece::RotatePiece(int matrix[PieceMaxRows][PieceMaxRows])
{
	//rotate the matrix by 90 degrees
	bool canRotate;

	//get the rotation matrix
	for (int rowIndex = 0; rowIndex<PieceMaxRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<PieceMaxRows; colIndex++)
		{
			rotatedMatrix[rowIndex][colIndex] = pieceMatrix[PieceMaxRows - 1 - colIndex][rowIndex];
		}
	}

	TranslatePiece(rotatedMatrix); //translate the rotated matrix

	//check the rotated matrix to see if the piece doesnt overlaps the given board matrix
	canRotate = true;
	for (int rowIndex = 0; rowIndex<PieceMaxRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<PieceMaxRows; colIndex++)
		{
			if (matrix[rowIndex][colIndex] != 0)
				if (rotatedMatrix[rowIndex][colIndex] != 0)
				{
					canRotate = false;
					break;
				}
		}
		if (!canRotate)
			break;
	}

	//if the piece can be rotated, set the new width and height;
	//and store the rotated matrix as the new matrix
	if (canRotate)
	{		
		_width = _lwidth;
		_height = _lheight;
		CopyMatrix(pieceMatrix, rotatedMatrix);
	}
	
	return TRUE;
}