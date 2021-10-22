// pieces.cpp: implementation of the pieces class
// here are created all the pieces
//
//////////////////////////////////////////////////////////////////////

#include "pieces.h"
#include "time.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//default constructor
pieces::pieces()
{
	DefinePieces(); //define the pieces
	srand((unsigned)time(NULL)); //initialise the random seed
	nextPiece = NULL;
}

//destructor
pieces::~pieces()
{

}

//assign to currentPiece either the nextPiece or a random piece from the defined ones
//assign to nextPiece a random piece from the defined ones
BOOL pieces::CreateNewPiece()
{
	
	int matrix[PieceMaxRows][PieceMaxRows];
	int pieceIndex = rand() % PiecesCount;
	//int pieceIndex = 1;
	
		
	if (nextPiece != NULL) //check if there is a next piece
	{
		lNextPiece.GetPieceMatrix(matrix); //piece exists, copy it`s matrix
	}
	else //
	{	
		definedPieces[pieceIndex].GetPieceMatrix(matrix); //piece doesnt exists, get a random piece
	}

	lCurrentPiece.SetPieceMatrix(matrix); // set the current piece matrix
	currentPiece = &lCurrentPiece; // assign the current piece


	pieceIndex = rand() % PiecesCount; //randomize the next piece
	//pieceIndex = 1;

	definedPieces[pieceIndex].GetPieceMatrix(matrix); 
	lNextPiece.SetPieceMatrix(matrix);
	nextPiece = &lNextPiece;

	//randomize the rotation of the next piece;
	int rotateCount = rand() % 5;
	ResetMatrix(matrix);
	for (int rotateIndex = 0; rotateIndex < rotateCount; rotateIndex ++)
		RotateNextPiece(matrix);
	
	

	//initialise the starting point of the pieces
	currentPiece->_x = 0;
	currentPiece->_y = 0;
	nextPiece->_x = 0;
	nextPiece->_y = 0;
	
	
	return TRUE;
}

//set the elements of the given matrix to 0
BOOL pieces::ResetMatrix(int matrix[PieceMaxRows][PieceMaxRows])
{
	for (int rowIndex = 0; rowIndex<PieceMaxRows; rowIndex++)
	{
		for (int colIndex = 0; colIndex<PieceMaxRows; colIndex++)
		{
			matrix[rowIndex][colIndex] = 0;
		}
	}

	return TRUE;
}

// rotates the next piece 
BOOL pieces::RotateNextPiece(int matrix[PieceMaxRows][PieceMaxRows])
{
	return nextPiece->RotatePiece(matrix);
}


// rotates the current piece
BOOL pieces::RotatePiece(int matrix[PieceMaxRows][PieceMaxRows])
{
	return currentPiece->RotatePiece(matrix);
}

//define the tetris pieces
BOOL pieces::DefinePieces()
{
	//define line piece
	int matrix[PieceMaxRows][PieceMaxRows];
	int crtMatrixIndex = 0;
	int rowIndex;
	
	//define | piece
	ResetMatrix(matrix);
	for (rowIndex=0; rowIndex<PieceMaxRows; rowIndex++)
		matrix[rowIndex][0] = 1;
	definedPieces[crtMatrixIndex].SetPieceMatrix(matrix);
	crtMatrixIndex ++;

	//define |_ piece
	ResetMatrix(matrix);
	for (rowIndex=0; rowIndex<PieceMaxRows-1; rowIndex++)
		matrix[rowIndex][0] = 1;
	matrix[2][1] = 1;
	definedPieces[crtMatrixIndex].SetPieceMatrix(matrix);
	crtMatrixIndex ++;

	//define _| piece
	ResetMatrix(matrix);
	for (rowIndex=0; rowIndex<PieceMaxRows-1; rowIndex++)
		matrix[rowIndex][1] = 1;
	matrix[2][0] = 1;
	definedPieces[crtMatrixIndex].SetPieceMatrix(matrix);
	crtMatrixIndex ++;

	//define |- piece
	ResetMatrix(matrix);
	for (rowIndex=0; rowIndex<PieceMaxRows -1; rowIndex++)
		matrix[rowIndex][0] = 1;
	matrix[1][1] = 1;
	definedPieces[crtMatrixIndex].SetPieceMatrix(matrix);
	crtMatrixIndex ++;

	//define [] piece
	ResetMatrix(matrix);
	for (rowIndex=0; rowIndex<2; rowIndex++)
	{
		matrix[rowIndex][0] = 1;
		matrix[rowIndex][1] = 1;
	}
	definedPieces[crtMatrixIndex].SetPieceMatrix(matrix);
	crtMatrixIndex ++;

	//define _| piece
	//		|
	ResetMatrix(matrix);
	rowIndex = 0;
	matrix[rowIndex++][1] = 1;
	matrix[rowIndex][1] = 1;
	matrix[rowIndex++][0] = 1;
	matrix[rowIndex][0] = 1;

	definedPieces[crtMatrixIndex].SetPieceMatrix(matrix);
	crtMatrixIndex ++;

	//define  |_  piece
	//			|

	ResetMatrix(matrix);
	rowIndex = 0;
	matrix[rowIndex++][0] = 1;
	matrix[rowIndex][0] = 1;
	matrix[rowIndex++][1] = 1;
	matrix[rowIndex][1] = 1;
	definedPieces[crtMatrixIndex].SetPieceMatrix(matrix);
	crtMatrixIndex ++;

	return TRUE;
}