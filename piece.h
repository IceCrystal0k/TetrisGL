// piece.h: interface for the piece class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(piece_included_)
#define piece_included_
#include "windows.h"
#endif

#define PieceMaxRows 4	//maximum number of rows for a piece

class piece  
{
private:
	
	int rotatedMatrix[PieceMaxRows][PieceMaxRows]; //declare the matrix of rotation
	BOOL CopyMatrix(int matrixDestination[PieceMaxRows][PieceMaxRows], int matrixSource[PieceMaxRows][PieceMaxRows]);
	int _lwidth, _lheight; //local width and height
public:
	int _x, _y, _width, _height; //related to the number of rows/columns of the board, measured in units
	int _rotation;
	piece();
	virtual ~piece();
	BOOL RotatePiece(int matrix[PieceMaxRows][PieceMaxRows]);
	BOOL DisplayMatrix();
	BOOL SetPieceMatrix(int matrix[PieceMaxRows][PieceMaxRows]);
	BOOL GetPieceMatrix(int matrix[PieceMaxRows][PieceMaxRows]);
	
protected:
	int pieceMatrix[PieceMaxRows][PieceMaxRows];	
	BOOL TranslatePiece(int matrix[PieceMaxRows][PieceMaxRows]);
};
