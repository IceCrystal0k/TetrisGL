// pieces.h: interface for the pieces class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(piece_included_)
#include "piece.h"
#endif


#define PiecesCount 7

class pieces  
{
public:
	pieces();
	virtual ~pieces();
	BOOL CreateNewPiece();
	BOOL RotatePiece(int matrix[PieceMaxRows][PieceMaxRows]);
	piece* currentPiece; //pointer to a piece object
	piece* nextPiece; //pointer to a piece object
protected:
	piece lCurrentPiece; //piece object
	piece lNextPiece; //piece object
	piece definedPieces[PiecesCount];
	BOOL DefinePieces();
	BOOL ResetMatrix(int matrix[PieceMaxRows][PieceMaxRows]);
	BOOL RotateNextPiece(int matrix[PieceMaxRows][PieceMaxRows]);
};