// board.h: interface for the board class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(piece_included_)
#include "piece.h"
#endif

#define BoardRows 20
#define BoardColumns 12

class board  
{
private:
	int BoardMatrix[BoardRows][BoardColumns];
	BOOL IsRowFree(piece * pCurrentPiece);
	BOOL IsColumnFree(piece * pCurrentPiece, int direction);	
	BOOL ResetMatrix(int matrix[BoardRows][BoardColumns]);
	BOOL CopyMatrix(int matrixDestination[BoardRows][BoardColumns], int matrixSource[BoardRows][BoardColumns]);
	BOOL MoveLines(int rowsIndex[]);
	BOOL SetLineScore();
	BOOL SetLevelConditions();
	BOOL CheckLevelCondition(int rowsCount);	
	int score_line[PieceMaxRows + 1];
public:
	float _x, _y;
	int score;
	int level;
	int dropSpeed;
	int score_level[30];
	board();
	virtual ~board();
	BOOL InitialiseBoard();
	BOOL MovePiece(piece* pCurrentPiece, int direction);
	BOOL DisplayMatrix();
	BOOL GetBoardMatrix(int matrix[BoardRows][BoardColumns]);
	BOOL PlacePiece(piece* pCurrentPiece);
	BOOL CheckGameOver(piece* pCurrentPiece);
	BOOL GetPieceSurroundMatrix(int x, int y, int matrix[PieceMaxRows][PieceMaxRows]);
	int EraseExistingLines(int row, int height);
};
