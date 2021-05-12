/* Author:	Cody Girouard (codygirouard@my.unt.edu)
   Description: This C++ program allows you to play a
		simplified version of Stratego in Linux. The
		board state can be saved at any time and can be 
		loaded any time after. Enemy AI controls red side
		while the user controls blue side.
*/
#include "functions.h"

int main()
{
	// create dynamic arrays of arrays for the game board and bombs the AI has found
	PieceData **board = new PieceData*[SIZE];
	int **bombs = new int*[3];

	// set each index in the array to another array of size [SIZE] and type PieceData (store information on each piece)
	for (int i = 0; i < SIZE; i++)
	{
		board[i] = new PieceData[SIZE];
	}

	// set each index in the array to another array of size 2 (representing the x&y coordinates of each bomb)
	for (int i = 0; i < 3; i++)
	{
		bombs[i] = new int[2];
		bombs[i][0] = -1;
		bombs[i][1] = -1;
	}

	// beginning information
	header();
	intro();
	// empties all spots in the array
	initializeBoard(board);

	// ask if user wants to load a previous game board
	char userInput[4];
	cout << "Would you like to load a previous game ((y)es/(n)o)? ";
	cin >> userInput;
	while (true) // keep checking until the user inputs an answer with 'y' or 'n' as the first character
	{
		if (tolower(userInput[0]) == 'y') // the user answered yes, load from a file
		{
			char fileName[20];
			cout << "Enter the file name you want to load from (max 19 characters): ";
			cin >> fileName; // ask for and store file name up to 19 characters

			ifstream inStream;
			inStream.open(fileName); // open a stream for the file

			if (inStream.fail()) // if file could not be opened end the program
			{
				cout << "ERROR - Could not open file: " << fileName << endl;
				exit(EXIT_FAILURE);
			}

			for (int i = 0; i < SIZE; i++)
			{
				for (int j = 0; j < SIZE; j++)
				{
					int nextNum; // number to hold the loaded information

					inStream >> nextNum;
					board[i][j].type = static_cast<Piece>(nextNum); // the first int read in is the character value of piece's type

					inStream >> nextNum;
					board[i][j].color = static_cast<Color>(nextNum); // the second int read in is the character value of piece's color

					inStream >> nextNum;
					board[i][j].moveable = static_cast<bool>(nextNum); // the third int read in is the boolean value of piece's moveability
				}
			}

			cout << "Previous game board loaded." << endl << endl;

			inStream.close(); // close stream

			break;
		}
		else if (tolower(userInput[0]) == 'n') // user has no board to load
		{
			startBoard(board); // set the board up with each side's pieces
			break;
		}
		// if the loop has made it this far no correct answer was input, ask again
		cout << "Would you like to load a previous game? ";
		cin >> userInput;
	}

	showBoard(board, false); // show the initial board with no AI pieces revealed

	bool gameOver = false;

	while (!gameOver) // keep playing game until gameOver is true
	{
		// Ask for user input, store in char array, if == "qq" quit game, == "ss" save game, else treat as if they entered coordinate
		cout << "RED MOVE: Enter current coordinates of piece (e.g., D2, or QQ to quit, SS to save): ";
		cin >> userInput;
		if (tolower(userInput[0]) == 'q' && tolower(userInput[1]) == 'q')
		{
			gameOver = true;
			cout << "RED player forfeits the game. Please play again soon!\n";
			showBoard(board, true); // forfeited, show board with all pieces revealed
		}
		else if (tolower(userInput[0]) == 's' && tolower(userInput[1]) == 's')
		{
			// ask for and store user's input for file name
			char fileName[20];
			cout << "Enter the file name you want to save to (max 19 characters): ";
			cin >> fileName;

			ofstream outStream;
			outStream.open(fileName);

			if (outStream.fail()) // if file failed to open, end program
			{
				cout << "ERROR - Could not open file: " << fileName << endl;
				exit(EXIT_FAILURE);
			}

			// file was opened successfully, copy data from file to board
			// data for each coordinate is stored as: PIECE.type PIECE.color PIECE.moveable
			// for example, a BLUE MINER would be stored as such: 56 66 1
			// where 1 is true and 0 is false for .moveable
			// 56 is character value of '8' (8 is MINER)
			// 66 is character value of 'B' (color BLUE = 'B')
			for (int i = 0; i < SIZE; i++)
			{
				for (int j = 0; j < SIZE; j++)
				{
					outStream << board[i][j].type << " " << board[i][j].color << " " << board[i][j].moveable << "     ";
				}
				outStream << "\n";
			}

			outStream.close();

			cout << "You have saved the board successfully under: " << fileName << endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			char userX = userInput[0];
			userX = toupper(userX); // if user input lowercase letter, change to uppercase
			int userY = userInput[1] - 48; // reading in char value of a number, subtract 48 to get int value
			if (userInput[2] >= 48 && userInput[2] <= 57) // if char[2] is a number then treat char[1] is tens place
			{
				userY *= 10; // user inputted two digits, the first digit represents tens place
				userY += userInput[2] - 48; // reading in char value of a number, subtract 48 to get int value

			}

			if (userX > 69 || userX < 65) // is the char entered by user not between A-E
			{
				cout << "Error: Invalid row location " << userX << ". Try again..." << endl;
				continue;
			}
			else if (userY < 1 || userY > 5) // is the int entered by user not between 1-5
			{
				cout << "Error: Invalid column location " << userY << ". Try again..." << endl;
				continue;
			}
			else if (board[userX - 65][userY - 1].color != RED) // is the coordinate entered by the player the correct color type
			{
				cout << "Error: Invalid piece color (BLUE). Try again..." << endl;
				continue;
			}
			else if (board[userX - 65][userY - 1].moveable == false) // is the coordinate entered by the player occupied by a moveable piece
			{
				cout << "Error: Invalid piece - cannot be moved. Try again..." << endl;
				continue;
			}
			else // the coordinate meets all requirements
			{
				// store the user's second coordinate
				char userInput2[4], userX2;
				int userY2;

				// hold value for if user chooses to change their first coordinate
				bool cancelMove = false;

				while (true) // keep looping until a correct second coordinate is entered
				{
					// ask for and store user's second coordinates
					cout << "RED MOVE: Enter new coordinates of piece at " << userX << userY << " (type CM (cancel move) to change pieces): ";
					cin >> userInput2;

					userX2 = userInput2[0];
					userY2 = userInput2[1] - 48; // reading in char value of a number, subtract 48 to get int value
					if (tolower(userInput2[0]) == 'c' && tolower(userInput2[1]) == 'm') // user decided to switch original coordinates
					{
						cancelMove = true;
						break;
					}
					else if (userInput2[2] >= 48 && userInput2[2] <= 57) // second coordinate has two digits for column
					{
						userY2 *= 10; // user inputted two digits, the first digit represents tens place
						userY2 += userInput2[2] - 48; // reading in char value of a number, subtract 48 to get int value
					}

					if (userX2 > 69 || userX2 < 65) // second coordinate has incorrect row (not between A-E)
					{
						cout << "Error: Invalid row location " << userX2 << ". Try again..." << endl;
					}
					else if (userY2 < 1 || userY2 > 5) // second coordinate has incorrect column (not between 1-5)
					{
						cout << "Error: Invalid column location " << userY2 << ". Try again..." << endl;
					}
					else if (board[userX2 - 65][userY2 - 1].color == RED) // second coordinate is of user's color, cannot move on own piece
					{
						cout << "Error: Invalid coordinate - cannot move on own piece (RED " << static_cast<char>(board[userX2 - 65][userY2 - 1].type) << "). Try again..." << endl;
					}
					else if ((abs(userX - userX2) + abs(userY - userY2)) != 1) // second coordinate is more than 1 units away
					{
						cout << "Error: Invalid move (attempted to move " << (abs(userX - userX2) + abs(userY - userY2)) << " spaces) - can only move 1 space down/up or right/left. Try again..." << endl;
					}
					else // correct coordinate was entered
					{
						break;
					}
				}
				if (!cancelMove) // if user did not decide to switch original coordinate
				{
					gameOver = movePiece(board, userX, userY, userX2, userY2); // movePiece returns true if a flag was captured, if true gameOver causes loop to stop
					if (!gameOver) // if the game isn't done after the user has moved, allow AI to make a move
					{
						gameOver = moveAI(board, bombs); // if AI took user's flag, end game
					}
				}
			}
		}
	}

	// return memory to freestore
	for (int i = 0; i < SIZE; i++)
	{
		delete [] board[i];
	}
	delete [] board;

	for (int i = 0; i < 3; i++)
	{
		delete [] bombs[i];
	}
	delete [] bombs;
}
