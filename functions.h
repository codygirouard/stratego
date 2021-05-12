#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

const int SIZE = 5; // size of game board (SIZE by SIZE)

enum Piece {EMPTY = ' ', FLAG = 'F', BOMB = 'B', MARSHAL = '1', GENERAL, COLONEL, MAJOR, CAPTAIN, LIEUTENANT, SERGEANT, MINER, SPY = 'S'}; // piece types
enum Color {RED = 'R', BLUE = 'B', NONE = ' '}; // piece colors

struct PieceData // information on each piece
{
	Piece type;
	Color color;
	bool moveable; // is the piece moveable? (bombs/flags not moveable)
};

void header();
/*
============================================================================
Function:	header
Definition:	This function prints student name and student email address.
============================================================================
*/

void intro();
/*
============================================================================
Function:	intro
Definition:	This function prints Stratego intro message, giving basic details
		and rules of the game
============================================================================
*/

void initializeBoard(PieceData **board);
/*
============================================================================
Function:	initializeBoard
Parameters:	double pointer PieceData representing 2D game board
Definition:	Empties the board, setting each spots' piece type in the
		array to EMPTY and Color to NONE
============================================================================
*/

void startBoard(PieceData **board);
/*
============================================================================
Function:	startBoard
Parameters:	double pointer PieceData representing 2D game board
Definition:	Sets the board up according to game rules. Sets each player's
		initial pieces and colors on their respective rows.
============================================================================
*/

void showBoard(PieceData **board, bool revealBLUE);
/*
============================================================================
Function:	showBoard
Parameters:	double pointer PieceData representing the 2D game board,
		bool revealBLUE to know if the AI pieces should be shown
Definition:	Prints out current game board, with pieces and their colors
============================================================================
*/

bool movePiece(PieceData **board, char currX, int currY, char newX, int newY);
/*
============================================================================
Function:	movePiece
Parameters:	double pointer PieceData representing the 2D game board,
		with the X/Y values of the current position and the
		X/Y values of the position to be moved to.
Return:		boolean representing if game is over
Definition:	moves a piece from point A to point B, checking what type
		the striking piece is and what type the striked piece is.
============================================================================
*/

void emptyCoordinate(PieceData **board, char x, int y, bool userInput);
/*
============================================================================
Function:	emptyCoordinate
Parameters:	double pointer PieceData representing the 2D game board,
		coordinates of position to be cleared out, bool to let
		the function know if the x/y coordinates are from the user
		or from another function
Definition:	Clears out the x/y coordinate
============================================================================
*/

bool moveAI(PieceData **board, int **bombs);
/*
============================================================================
Function:	moveAI
Parameters:	double pointer PieceData representing the 2D game board,
		double pointer int representing all known bomb positions
		to the AI.
Definition:	Moves on each AI turn.
============================================================================
*/

void checkNoMoves(PieceData **board);
/*
============================================================================
Function:	checkNoMoves
Parameters:	double pointer PieceData representing the 2D game board
Definition:	Goes through each position of the board and checks if each
		side has at least one moveable piece. Ends the game if either
		side has no moveable pieces.
============================================================================
*/

