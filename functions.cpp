#include "functions.h"

void header()
{
	cout <<
	"+-----------------------------------------------+\n"
	<<
	"|\tComputer Science and Engineering\t|\n"
	<<
	"| Cody Girouard codygirouard@my.unt.edu\t|\n"
	<<
	"+-----------------------------------------------+\n\n";

	return;
}

void intro()
{
	cout << "\n            W e l c o m e   t o   S t r a t e g o"
	<< "\n------------------------------------------------------------------------"
	<< "\nThis program will set up a 5x5 game board for a simple version of the game"
	<< "\nof Stratego. One player will compete against the computer, each assigned"
	<< "\n10 total pieces consisting of the following:"
	<< "\n  1 FLAG (F)\n  3 BOMB (B)\n  1 MARSHAL (1) or GENERAL (2)\n  3 COLONEL (3)"
	<< ", MAJOR (4), CAPTAIN (5), LIEUTENANT (6), or SERGEANT (7)\n  1 MINER (8)\n  1"
	<< "SPY (S)"
	<< "\n\nGENERAL RULES:"
	<< "\n--------------"
	<< "\nFor the most part, the game will follow the standard Stratego rules,"
	<< "\nalthough there are some exceptions."
	<< "\n1. Both players (BLUE and RED) will have all of their 10 game pieces"
	<< "\n   assigned randomly with the only requirement being that the FLAG must"
	<< "\n   be placed in the back row. RED will start the game first."
	<< "\n2. Higher ranked pieces can capture lower ranked pieces in the following"
	<< "\n   order: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> S, meaning that 1 (the"
	<< "\n   MARSHAL) can remove 2 (the GENERAL) and so forth. The MINER (8) piece"
	<< "\n   may strike a BOMB and remove it to occupy the now unoccupied space."
	<< "\n   A SPY (S), although the lowest ranked piece, may remove the MARSHAL (1)"
	<< "\n   or the GENERAL (2). When pieces have equal rank, then both pieces are"
	<< "\n   removed."
	<< "\n3. The FLAG and BOMBs are not moveable while all of the other pieces may"
	<< "\n   move one square at a time forward, backward, or sideward, but not"
	<< "\n   diagonally to an open square."
	<< "\n4. A player must either move or strike on his/her turn."
	<< "\n5. The game ends when a player strikes his/her opponent's flag."
	<< "\n------------------------------------------------------------------------ \n";

	return;
}

void initializeBoard(PieceData **board)
{
	cout << "Initializing game board..." << endl;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			// empty the board piece by piece
			emptyCoordinate(board, i, j, false);
		}
	}

	return;
}

void startBoard(PieceData **board)
{
	cout << "Assigning BLUE and RED pieces to board..." << endl;

	srand(time(NULL)); // seed random numbers

	// place blue flag on random column in back row
	board[0][rand()%5].type = FLAG;
	// place red flag on random column in back row
	board[4][rand()%5].type = FLAG;

	// place 3 bombs on red and blue sides
	for (int i = 0; i < 6; i++) // iterate 3*2 for both sides
	{
		while (true) // the bomb has not been placed in an empty spot yet
		{
			// place the bomb on the blue side when "i" is even and on the red side when "i" is odd
			// select a random column
			int row = rand()%2 + i%2 * 3, column = rand()%5;
			if (board[row][column].type == EMPTY) // is spot empty?
			{
				board[row][column].type = BOMB;
				break; // a bomb has been placed, move to next iteration in for loop
			}
		}
	}

	// place 1 marshal or general randomly to both sides
	for (int i = 0; i <= 1; i++) // twice to do one for each side
	{
		bool marshal = rand()%2 == 0; // is it going to be a marshal or general?
		while (true)
		{
			// when "i" is even, put the piece on blue side, when "i" is odd, put the piece on red side
			// select random column
			int row = rand()%2 + 3 * i, column = rand()%5;
			if (board[row][column].type == EMPTY) // is spot empty?
			{
				if (marshal) // if marshal bool is true, place a marshal piece
				{
					board[row][column].type = MARSHAL;
				}
				else // else place general piece
				{
					board[row][column].type = GENERAL;
				}
				board[row][column].moveable = true;
				break; // piece has been placed, continue next iteration of for loop
			}
		}
	}

	// place 1 miner randomly to both sides
	for (int i = 0; i <= 1; i++)
	{
		while (true)
		{
			// when "i" is even, put the piece on blue side, when "i" is odd, put the piece on red side
			// select random column
			int row = rand()%2 + 3 * i, column = rand()%5;
			if (board[row][column].type == EMPTY) // is spot empty?
			{
				board[row][column].type = MINER;
				board[row][column].moveable = true;
				break; // piece has been placed, continue to next iteration of for loop
			}
		}
	}

	// place 1 spy randomly to both sides
	for (int i = 0; i <= 1; i++)
	{
		while (true)
		{
			// when "i" is even, put the piece on blue side, when "i" is odd, put the piece on red side
			// select random column
			int row = rand()%2 + 3 * i, column = rand()%5;
			if (board[row][column].type == EMPTY) // is spot empty
			{
				board[row][column].type = SPY;
				board[row][column].moveable = true;
				break; // piece has been placed, continue to next iteration of for loop
			}
		}
	}

	// place 3 pieces randomly chosen from colonel, major, captain, lieutenant, sergeant onto red and blue sides
	for (int i = 0; i < 6; i++)
	{
		while (true)
		{
			// when "i" is even, put the piece on blue side, when "i" is odd, put the piece on red side
			// select random column
			int row = rand()%2 + i%2 * 3, column = rand()%5;
			if (board[row][column].type == EMPTY) // is spot empty?
			{
				int piece = rand()%5 + 51;
				board[row][column].type = static_cast<Piece>(piece);
				board[row][column].moveable = true;
				break; // piece has been placed, continue to next iteration of for loop
			}
		}
	}

	// set top side to blue
	for (int i = 0; i <= 1; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			board[i][j].color = BLUE; // set colors to blue on top two rows
		}
	}

	// set bottom side to red
	for (int i = 3; i <= 4; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			board[i][j].color = RED; // set colors to red on bottom two rows
		}
	}

	return;
}

void showBoard(PieceData **board, bool revealBLUE)
{
	cout << "\n    1 2 3 4 5\n  +-----------+\n";

	for (int i = 0; i < SIZE; i++)
	{
		cout << static_cast<char>('A' + i) << " | ";
		for (int j = 0; j < SIZE; j++)
		{
			string blue = "\033[1;34m"; // set blue font color
			string red = "\033[1;31m"; // set red font color
			string endColor = "\033[0m"; // set normal font color

			if (board[i][j].color == BLUE)
			{
				string dispBLUE;
				if (revealBLUE) // user is allowed to see AI pieces
					dispBLUE = static_cast<char>(board[i][j].type);
				else // user is not allowed to see AI pieces
					dispBLUE = "X";
				cout << blue << dispBLUE << endColor << " ";
			}
			else if (board[i][j].color == RED)
			{
				cout << red << static_cast<char>(board[i][j].type) << endColor << " ";
			}
			else // piece is neither red or blue, print as a blank space
			{
				cout << "  ";
			}
		}
		cout << "|" << endl;
	}
	cout << "  +-----------+\n";

	return;
}

bool movePiece(PieceData **board, char currX, int currY, char newX, int newY)
{
	PieceData strikingPiece = board[currX - 65][currY - 1];
	PieceData strikedPiece = board[newX - 65][newY - 1];

	string color1, color2;
	string blue = "\033[1;34m"; // set blue font color
	string red = "\033[1;31m"; // set red font color
	string end = "\033[0m";
	if (strikedPiece.color == BLUE)
	{
		color1 = red + "RED " + end, color2 = blue + "BLUE " + end;
	}
	else if (strikedPiece.color == RED)
	{
		color1 = blue + "BLUE " + end, color2 = red + "RED " + end;
	}
	else if (strikingPiece.color == RED)
	{
		color1 = red + "RED " + end, color2 = "EMPTY ";
	}
	else
	{
		color1 = blue + "BLUE " + end, color2 = "EMPTY ";
	}

	// hide information about AIs piece type if AI moved
	char type1, type2;
	if (strikingPiece.color == BLUE)
	{
		type1 = 'X', type2 = strikedPiece.type;
	}
	else
	{
		type1 = strikingPiece.type, type2 = strikedPiece.type;
	}

	if(strikedPiece.type == FLAG)
	{
		board[newX - 65][newY - 1] = strikingPiece;
		emptyCoordinate(board, currX, currY, true);

		cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " captured " << color2 << static_cast<char>(type2) << " at " << newX << newY << ". Congratulations!" << endl;
		showBoard(board, true);

		return true;
	}
	else if (strikedPiece.type == EMPTY)
	{
		board[newX - 65][newY - 1] = strikingPiece;
		emptyCoordinate(board, currX, currY, true);

		cout << color1 << static_cast<char>(type1) << " moved from " << currX << currY << " to " << newX << newY << ".\n";
	}
	else if (strikedPiece.type == BOMB)
	{
		if (strikingPiece.type == MINER) // if miner strikes bomb, remove it and take it's place
		{
			board[newX - 65][newY - 1] = strikingPiece;
			emptyCoordinate(board, currX, currY, true);

			cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " defused " << color2 << static_cast<char>(type2) << " at " << newX << newY << ".\n";
		}
		else // blow up current piece if not a miner
		{
			emptyCoordinate(board, currX, currY, true);

			cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " blown up by " << color2 << static_cast<char>(type2) << " at " << newX << newY << ".\n";
		}
	}
	else if (strikingPiece.type == strikedPiece.type) // pieces are equally matched, remove both
	{
		emptyCoordinate(board, currX, currY, true);
		emptyCoordinate(board, newX, newY, true);

		cout << color1 << static_cast<char>(strikingPiece.type) << " at " << currX << currY << " and " << color2 << static_cast<char>(type2) << " at " << newX << newY << " were equally matched and took each other out.\n";
	}
	else if (strikingPiece.type == SPY) // spy takes out 1/2 only if it strikes first, if it's striked by 1/2 it is taken out
	{
		if (strikedPiece.type == '1' || strikedPiece.type == '2')
		{
			board[newX - 65][newY - 1] = strikingPiece;
			emptyCoordinate(board, currX, currY, true);

			cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " captured " << color2 << static_cast<char>(type2) << " at " << newX << newY << ".\n";
		}
		else
		{
			emptyCoordinate(board, currX, currY, true);

			cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " captured by " << color2 << static_cast<char>(type2) << " at " << newX << newY << ".\n";
		}
	}
	else if (strikedPiece.type == SPY)
	{
		emptyCoordinate(board, currX, currY, true);
		board[newX - 65][newY - 1] = strikingPiece;

		cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " captured " << color2 << static_cast<char>(type2) << " at " << newX << newY << ".\n";
	}
	else // no special cases, is strikingPiece stronger than strikedPiece
	{
		if (strikingPiece.type < strikedPiece.type)
		{
			board[newX - 65][newY - 1] = strikingPiece;
			emptyCoordinate(board, currX, currY, true);

			cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " captured " << color2 << static_cast<char>(type2) << " at " << newX << newY << ".\n";
		}
		else
		{
			emptyCoordinate(board, currX, currY, true);

			cout << color1 << static_cast<char>(type1) << " at " << currX << currY << " captured by " << color2 << static_cast<char>(type2) << " at " << newX << newY << ".\n";
		}
	}

	checkNoMoves(board); // if either side has no moves left, end game
	showBoard(board, false);

	return false;
}

void emptyCoordinate(PieceData **board, char x, int y, bool userInput)
{
	if (userInput)
	{
		x -= 65; // account for char values (ie rows A-E)
		y -= 1; // account for column values (1-5 to 0-4)
	}
	board[x][y].type = EMPTY;
	board[x][y].color = NONE;
	board[x][y].moveable = false;

	return;
}

bool moveAI(PieceData **board, int **bombs)
{
	int **pieces = new int*[SIZE * 2], count = 0; // 2 rows of SIZE characters to begin with, store count to place pieces' positions in array
	int **enemyPieces = new int*[SIZE * 2], enemyCount = 0;

	for (int i = 0; i < SIZE * 2; i++) // set each array to negative coordinates for easy checking
	{
		pieces[i] = new int[2];
		pieces[i][0] = -1;
		pieces[i][1] = -1;

		enemyPieces[i] = new int[2];
		enemyPieces[i][0] = -1;
		enemyPieces[i][1] = -1;
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j].color == BLUE && board[i][j].moveable)
			{
				// storing x,y coordinates of each moveable blue piece in a dynamic int array
				pieces[count][0] = i;
				pieces[count++][1] = j;
			}
			else if (board[i][j].color == RED && board[i][j].moveable)
			{
				// storing x,y coordinates of each red piece in a dynamic int array
				enemyPieces[enemyCount][0] = i;
				enemyPieces[enemyCount++][1] = j;
			}
		}
	}

	checkNoMoves(board); // if either side has no moveable pieces end game

	count = 0; // check for valid moves 20 times, if none found end
	while (true)
	{
		// select a random piece for each side out of the moveable pieces array
		int randPiece = rand() % (SIZE * 2), randEnemyPiece = rand() % (SIZE * 2);
		int x = pieces[randPiece][0], y = pieces[randPiece][1];
		int holdX = x, holdY = y;
		int xEnemy = enemyPieces[randEnemyPiece][0], yEnemy = enemyPieces[randEnemyPiece][1];

		if (x < 0 || xEnemy < 0) // x value was negative which means it hit a blank coordinate in pieces/enemyPieces
		{
			continue;
		}
		else
		{
			// check if the coordinate we want to go to is a known bomb
			bool isBomb = false;
			for (int i = 0; i < 3; i++)
			{
				if (bombs[i][0] == xEnemy && bombs[i][1] == yEnemy)
				{
					isBomb = true;
					break;
				}
			}
			if (isBomb && board[x][y].type == MINER) // if MINER is moving allow to target bomb
			{
				for (int i = 0; i < 12; i++) // attempt a move in a random direction 12 times
				{
					x = holdX;
					y = holdY;
					int direction = rand() % 4;
					int origDist = (abs(x - xEnemy) + abs(y - yEnemy)), newDist;

					switch (direction){
						// each case is same functionaly, just different movement
						case 0:
							if ((x - 1) >= 0) // if going "up" a row leads to outside the array dimensions
							{
								x--;
							}
							break;
						case 1:
							if ((x + 1)  <= 4)
							{
								x++;
							}
							break;
						case 2:
							if ((y - 1) >= 0)
							{
								y--;
							}
							break;
						case 3:
							if ((y + 1) <= 4)
							{
								y++;
							}
							break;
					}

					newDist = (abs(x - xEnemy) + abs(y - yEnemy));
					if (newDist < origDist && board[x][y].color  != BLUE)
					{
						if (board[x][y].type == BOMB) // miner is moving on and defusing bomb, remove from bomb list if it's there
						{
							for (int i = 0; i < 3; i++)
							{
								if (bombs[i][0] == (x) && bombs[i][1] == (y))
								{
									bombs[i][0] = -1;
									bombs[i][1] = -1;
								}
							}
						}
						return movePiece(board, holdX + 65, holdY + 1, x + 65, y + 1);
					}
				}
			}
			else if (!isBomb) // the target is not a known enemy bomb
			{
				for (int i = 0; i < 20; i++)
				{
					x = holdX;
					y = holdY;
					int direction = rand() % 4;
					int origDist = (abs(x - xEnemy) + abs(y - yEnemy)), newDist;
					bool foundBomb = false; // is the coordinate we want to move to a known bomb?

					switch (direction){
						// each case is same functionaly, just different movement
						case 0:
							if ((x - 1) >= 0) // if going "up" a row leads to outside the array dimensions
							{
								x--;
							}
							break;
						case 1:
							if ((x + 1)  <= 4)
							{
								x++;
							}
							break;
						case 2:
							if ((y - 1) >= 0)
							{
								y--;
							}
							break;
						case 3:
							if ((y + 1) <= 4)
							{
								y++;
							}
							break;
					}

					newDist = (abs(x - xEnemy) + abs(y - yEnemy));
					if (newDist < origDist && board[x][y].color  != BLUE)
					{
						// check if AI knows it's a bomb, if so don't move on it
						for (int i = 0; i < 3; i++)
						{
							if (bombs[i][0] == (x) && bombs[i][1] == (y))
							{
								foundBomb = true;
								break;
							}
						}
						if (foundBomb)
						{
							break;
						}
						if (board[x][y].type == BOMB && board[x][y].type != MINER) // AI didn't know it was moving on a bomb and is not a miner. get's blown up and adds it to it's known bomb list
						{
							for (int i = 0; i < 3; i++)
							{
								if (bombs[i][0] < 0)
								{
									bombs[i][0] = x;
									bombs[i][1] = y;
									break;
								}
							}
						}
						return movePiece(board, holdX + 65, holdY + 1, x + 65, y + 1);
					}
				}
			}
		}

		if (++count > 20) // attempted 20 times to no avail
		{
			for (int i = 0; i < SIZE *2; i++) // loop through moveable pieces and see if any have valid movements
			{
				x = pieces[i][0], y = pieces[i][1];

				if ((x - 1 >= 0) && board[x - 1][y].color != BLUE)
				{
					return movePiece(board, x + 65, y + 1, (x + 65) - 1, y + 1);
				}
				else if ((x + 1 <= 4) && board[x + 1][y].color != BLUE)
				{
					return movePiece(board, x + 65, y + 1, (x + 65) + 1, y + 1);
				}
				else if ((y - 1 >= 0) && board[x][y - 1].color != BLUE)
				{
					return movePiece(board, x + 65, y + 1, x + 65, (y + 1) - 1);
				}
				else if ((y + 1 <= 4) && board[x][y + 1].color != BLUE)
				{
					return movePiece(board, x + 65, y + 1, x + 65, (y + 1) + 1);
				}
			}

			// no valid movement was found
			cout << "BLUE Army could not find any correct moves.\n";
			break;
		}
	}

	checkNoMoves(board); // if either side has no moves left, end game

	// free memory back to freestore
	for (int i = 0; i < SIZE * 2; i++)
	{
		delete [] pieces[i];
		delete [] enemyPieces[i];
	}
	delete [] pieces;
	delete [] enemyPieces;

	return false;
}

void checkNoMoves(PieceData **board)
{
	// declare and intialize moveable pieces for each color
	int **pieces = new int*[SIZE * 2], count = 0;
	int **enemyPieces = new int*[SIZE * 2], enemyCount = 0;

	for (int i = 0; i < SIZE * 2; i++)
	{
		pieces[i] = new int[2];
		pieces[i][0] = -1;
		pieces[i][1] = -1;

		enemyPieces[i] = new int[2];
		enemyPieces[i][0] = -1;
		enemyPieces[i][1] = -1;
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j].color == BLUE && board[i][j].moveable)
			{
				// storing x,y coordinates of each moveable blue piece in a dynamic int array
				pieces[count][0] = i;
				pieces[count++][1] = j;
			}
			else if (board[i][j].color == RED && board[i][j].moveable)
			{
				// storing x,y coordinates of each red piece in a dynamic int array
				enemyPieces[enemyCount][0] = i;
				enemyPieces[enemyCount++][1] = j;
			}
		}
	}

	// if any pieces are found to be moveable, set bools to true
	bool redHasMoves = false, blueHasMoves = false;

	for (int i = 0; i < SIZE * 2; i++)
	{
		if (pieces[i][0] >= 0)
		{
			blueHasMoves = true;
		}

		if (enemyPieces[i][0] >= 0)
		{
			redHasMoves = true;
		}
	}

	// both ran out of pieces that can be moved
	if ((!blueHasMoves) && (!redHasMoves))
	{
		cout << "Neither side has any moveable pieces! Good game!\n";
		showBoard(board, true);
		exit(EXIT_FAILURE);
	}

	if (!blueHasMoves)
	{
		cout << "BLUE Army has no moveable pieces left. Congratulations RED Army!\n";
		showBoard(board, true);
		exit(EXIT_FAILURE);
	}

	if (!redHasMoves)
	{
		cout << "RED Army has no moveable pieces left. Congratulations BLUE Army!\n";
		showBoard(board, true);
		exit(EXIT_FAILURE);
	}
}
