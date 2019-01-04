#include "Engine.h"

void initRow(int gameBoard[8][8], int startRow, int isBlack);

Board::Board()
{
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			gameBoard[i][j] = -1;
		}
	}
}

void Board::render(SDL_Rect*clip)
{
	gBoard.render(0, 0, clip);
}

void Board::initBoard() {
	initRow(gameBoard, 0, 1);
	//gameBoard[7][7] = WHITE_KING;
	initRow(gameBoard, BOARD_SIZE - 1, 0);

	for (int i = 0; i < BOARD_SIZE; i++) {
		gameBoard[1][i] = BLACK_PAWN;
		//gameBoard[1][i] = -1;
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		gameBoard[BOARD_SIZE - 2][i] = WHITE_PAWN;

	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {

			initGameBoard[i][j] = gameBoard[i][j];
			printf("%d ", gameBoard[i][j]);
		}
		printf("\n");
	}
}

void Board::renderBoard(SDL_Rect sprites[]) {
	int scaleWidth = gBoard.getWidth() / BOARD_SIZE;
	int scaleHeight = gBoard.getHeight() / BOARD_SIZE;
	SDL_Rect dstrect = { 0, 0, 0, 0 };
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1) {
				dstrect.h = scaleHeight;
				dstrect.w = scaleWidth;
				dstrect.x = scaleWidth * j;
				dstrect.y = scaleHeight * i;
				SDL_RenderCopy(gRenderer, gSpriteSheetTexture.getTexture(), &sprites[gameBoard[i][j]], &dstrect);
			}
		}
	}
}

int Board::getSprite(int x, int y) {
	int scaleWidth = gBoard.getWidth() / BOARD_SIZE;
	int scaleHeight = gBoard.getHeight() / BOARD_SIZE;

	return gameBoard[y / scaleHeight][x / scaleWidth];


}

int Board::scaleValue(int value) {
	int scaleWidth = gBoard.getWidth() / BOARD_SIZE;

	return value / scaleWidth;

}

void initRow(int gameBoard[8][8], int startRow, int isBlack) {
	int startValue = isBlack * 6 + 4;
	for (int i = 0; i < 5; i++) {
		gameBoard[startRow][i] = startValue;

		startValue--;
	}

	startValue = isBlack * 6 + 4;
	for (int i = 7; i >= 5; i--) {
		gameBoard[startRow][i] = startValue;
		startValue--;
	}
}

vector<array<int, 4>> Board::generateAllMovelists(bool whiteMove, int gameBoard[8][8]) {
	int lowerLimit = whiteMove == true ? WHITE_KING : BLACK_KING;
	int higherLimit = whiteMove == true ? WHITE_PAWN : BLACK_PAWN;
	//printf("CALLED HERE!!!!!!---------------------\n");
	/*for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			printf("%d ", gameBoard[i][j]);
		}
		printf("\n");
	}


	*/
	vector<array<int, 4>> allMoves;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				int moveList[2][64] = { { -1 } };
				getMoveList(whiteMove, j, i, moveList, gameBoard);
				for (int k = 0; k < 64 && moveList[0][k] != -1; k++) {
					//if(gameBoard[i][j] == WHITE_KING || gameBoard[i][j] == BLACK_KING) printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], i, j, moveList[0][k], moveList[1][k]);

					bool f = validMove(j, i, moveList[1][k], moveList[0][k], whiteMove, false, gameBoard);
					if (!f) continue;
					bool s = !checkKingMove(whiteMove, j, i, moveList[1][k], moveList[0][k], gameBoard);
					if (!s) continue;
					if ((gameBoard[i][j] == WHITE_KING || gameBoard[i][j] == BLACK_KING)) {
						//printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], i, j, moveList[0][k], moveList[1][k]);
						//printf("Checking equality %d %d\n", f, s);
					}
					array<int, 4> move = { j, i, moveList[1][k], moveList[0][k] };
					allMoves.push_back(move);
				
				}	
			}
		}
	}
	//printf("ENDED HERE\n");
	return allMoves;
}


vector<array<int, 4>> Board::generateAllCapture(bool whiteMove, int gameBoard[8][8]) {
	int lowerLimit = whiteMove == true ? WHITE_KING : BLACK_KING;
	int higherLimit = whiteMove == true ? WHITE_PAWN : BLACK_PAWN;
	//printf("CALLED HERE!!!!!!---------------------\n");
	/*for (int i = 0; i < BOARD_SIZE; i++) {
	for (int j = 0; j < BOARD_SIZE; j++) {
	printf("%d ", gameBoard[i][j]);
	}
	printf("\n");
	}


	*/
	vector<array<int, 4>> allMoves;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				int moveList[2][64] = { { -1 } };
				getMoveList(whiteMove, j, i, moveList, gameBoard);
				for (int k = 0; k < 64 && moveList[0][k] != -1; k++) {


					//if(gameBoard[i][j] == WHITE_KING || gameBoard[i][j] == BLACK_KING) printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], i, j, moveList[0][k], moveList[1][k]);

					bool f = validMove(j, i, moveList[1][k], moveList[0][k], whiteMove, false, gameBoard);
					if (!f) continue;
					bool s = !checkKingMove(whiteMove, j, i, moveList[1][k], moveList[0][k], gameBoard);
					if (!s) continue;
					if (gameBoard[moveList[0][k]][moveList[1][k]] == -1) continue;
					if ((gameBoard[i][j] == WHITE_KING || gameBoard[i][j] == BLACK_KING)) {
						//printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], i, j, moveList[0][k], moveList[1][k]);
						//printf("Checking equality %d %d\n", f, s);
					}
					array<int, 4> move = { j, i, moveList[1][k], moveList[0][k] };
					allMoves.push_back(move);

				}
			}
		}
	}
	//printf("ENDED HERE\n");
	return allMoves;
}

bool Board::UserMove(int spriteX, int spriteY, int newX, int newY, bool whiteMove) {
	printf("spriteX: %d spriteY:%d\n", spriteX, spriteY);

	bool validMoveCheck = false;
	validMoveCheck = validMove(spriteX, spriteY, newX, newY, whiteMove, true, gameBoard);
	int newGameBoard[8][8] = { { -1 } };
	memcpy(&newGameBoard, &gameBoard, sizeof(gameBoard));
	
	newGameBoard[newY][newX] = newGameBoard[spriteY][spriteX];
	newGameBoard[spriteY][spriteX] = -1;

	printf("Valid move? %d %d\n", validMoveCheck == true, sizeof(gameBoard));
	bool check = kingCheckMoves(whiteMove, newGameBoard);
	if (validMoveCheck == true && !check) {
		makeMove(spriteX, spriteY, newX	, newY, whiteMove, gameBoard, gameMoved);
		printf("hey activated");
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				printf("%d ", gameMoved[i][j]);
			}
			printf("\n");
		}
		return true;
	}
	printBoard();
	return false;
}

void Board::makeMove(int spriteX, int spriteY, int newX, int newY, bool whiteMove, int gameBoard[8][8], bool gameMoved[8][8]) {
	int king = whiteMove == true ? WHITE_KING : BLACK_KING;

	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);

	if (gameMoved[spriteY][spriteX] == false && initGameBoard[spriteY][spriteX] == gameBoard[spriteY][spriteX]) {
		gameMoved[spriteY][spriteX] = true;
	}

	if (gameBoard[spriteY][spriteX] == king) {
		if (distanceX == 2 && distanceY == 0) {
			
		//	printf("\nWTF CASTLING....... %d %d %d %d\n", distanceX, distanceY, newX, spriteX);

			if (newX > spriteX) {
				gameBoard[spriteY][newX - 1] = gameBoard[spriteY][7];
				gameBoard[spriteY][7] = -1;
			}
			else {
				gameBoard[spriteY][newX + 1] = gameBoard[spriteY][0];
				gameBoard[spriteY][0] = -1;
			}
		}
	}

	gameBoard[newY][newX] = gameBoard[spriteY][spriteX];
	gameBoard[spriteY][spriteX] = -1;

	//printf("\nCASTLING....... %d\n", gameBoard[newY][newX]);

	if ((gameBoard[newY][newX] == BLACK_PAWN && newY == BOARD_SIZE - 1) || (gameBoard[newY][newX] == WHITE_PAWN && newY == 0)) {
		gameBoard[newY][newX] = whiteMove ? WHITE_QUEEN : BLACK_QUEEN;
	}
}
bool Board::checkStale(bool whiteMove, int gameBoard[8][8]) {
	printf("\n--------------CHECKING STALE--------------\n");

	int lowerLimit = whiteMove == true ? WHITE_KING : BLACK_KING;
	int higherLimit = whiteMove == true ? WHITE_PAWN : BLACK_PAWN;
	if (kingCheckMoves(whiteMove, gameBoard)) return false;
	int king = whiteMove == true ? BLACK_KING : WHITE_KING;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int moveList[2][64] = { { -1 } };
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				getMoveList(whiteMove, j, i, moveList, gameBoard);
				for (int k = 0; k < 64 && moveList[0][k] != -1; k++) {

					printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], i, j, moveList[0][k], moveList[1][k]);					
					bool f = validMove(j, i, moveList[1][k], moveList[0][k], whiteMove, false, gameBoard);
					if (!f) continue;
					bool s = !checkKingMove(whiteMove, j, i, moveList[1][k], moveList[0][k], gameBoard);
					if (!s) continue;
					printf("CHESS PIECE %d %d %d %d!!!!!!!!!!!!!!!\n\n\n\n\n", i, j, moveList[0][k], moveList[1][k]);
					return false;
				
				}

			}
		}
	}
	return true;

}

bool Board::checkWin(bool whiteMove, int gameBoard[8][8]) {
	//printf("\n--------------CHECKING WIN--------------\n");
	
	int lowerLimit = whiteMove == true ? WHITE_KING : BLACK_KING;
	int higherLimit = whiteMove == true ? WHITE_PAWN : BLACK_PAWN;

	bool check = true;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int moveList[2][64] = { { -1 } };
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				getMoveList(whiteMove, j, i, moveList, gameBoard);
				for (int k = 0; k < 64 && moveList[0][k] != -1; k++) {

					if (!whiteMove) printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], j, i, moveList[0][k], moveList[1][k]);
					bool f = validMove(j, i, moveList[1][k], moveList[0][k], whiteMove, false, gameBoard);
					if (!f) continue;
					bool s = !checkKingMove(whiteMove, j, i, moveList[1][k], moveList[0][k], gameBoard);
					if (!s) continue;
					if (!whiteMove) printf("CHECKING FUNCTIONN %d %d\n", f, s);

					check = false;
					if (!whiteMove) printf("CHESS PIECE %d %d %d %d!!!!!!!!!!!!!!!\n\n", j, i, moveList[0][k], moveList[1][k]);
					break;
					
				}

			}

			if (!check) break;
		}
		if (!check) break;
	}

	if (check == true) {
		
		printf("WE WIN!!!!!!!!!!!!!!!!!!!!!!!!!! %d\n", whiteMove);

		//exit(0);
		return true;
	}
	return false;
}

bool Board::checkKingMove(bool whiteMove, int posX, int posY, int newX, int newY, int gameBoard[8][8]) {
	int newGameBoard[8][8] = { { -1 } };
	bool newGameMoved[8][8];
	copyGameMoved(gameMoved, newGameMoved);
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			newGameBoard[i][j] = gameBoard[i][j];
		}
	}
	
	makeMove(posX, posY, newX, newY, whiteMove, newGameBoard, newGameMoved);
	
	//printf("checlkKingMove %d %d %d %d %d %d\n", newY, newX, posY, posX, newGameBoard[newY][newX], newGameBoard[posY][posX]);
	return kingCheckMoves(whiteMove, newGameBoard);
}

bool Board::kingCheckMoves(bool whiteMove, int gameBoard[8][8]) {
	//printf("--------------CHECKING CHECK--------------\n");
	
	int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
	int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;

	int king = whiteMove == true ? WHITE_KING : BLACK_KING;
	int kingPosX = -1;
	int kingPosY = -1;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] == king) {
				kingPosX = j;
				kingPosY = i;
				break;
			}
		}
	}
	if (kingPosX == -1 && kingPosY == -1) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				printf("%d ", gameBoard[i][j]);
			}
			printf("\n");
		}
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				//printf("---------CHECKING VALID MOVES----- kings Pos X: %d kings Pos Y: %d chessPosX: %d chessPosY: %d %d\n", kingPosX, kingPosY, j, i, gameBoard[i][j]);
				if (validMove(j, i, kingPosX, kingPosY, !whiteMove, false, gameBoard)) {
					
					//printf("--------------------IN-CHECK---------------------\n");
					return true;
				}
			}
		}

	}
	return false;
}

void Board::printBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			printf("%d ", gameBoard[i][j]);
		}
		printf("\n");
	}
}

int	(*Board::getGameBoard())[8] {
	return gameBoard;
}

bool (*Board::getGameMoved())[8]{
	return gameMoved;
}

void Board::getMoveList(bool whiteMove, int posX, int posY, int moveList[2][64], int gameBoard[8][8]) {
	int i = 0;
	for (int i = 0; i < 64; i++) {
		moveList[0][i] = -1;
		moveList[1][i] = -1;

		//printf("THIS IS IMPORTANT %d %d\n", moveList[0][i], moveList[1][i]);
	}
	switch (gameBoard[posY][posX]) {
		case BLACK_KING:
		case WHITE_KING: 
		{
			//printf("entered\n");
			int j = 0, startPosY = 0;
			for (j = 0, startPosY = posY - 1; j < 3; j++, startPosY++) {
				int k = 0, startPosX = 0;
				for (k = 0, startPosX = posX - 1; k < 3; k++, startPosX++) {
					if (startPosY < 0 || startPosY >= BOARD_SIZE) continue;
					if (startPosX < 0 || startPosX >= BOARD_SIZE) continue;
					if (startPosY == posY && startPosX == posX) continue;
					moveList[0][i] = startPosY;
					moveList[1][i] = startPosX;
					i++;
				}
			}
			
			if (posX - 2 >= 0) {
				moveList[0][i] = posY;
				moveList[1][i] = posX - 2;
				i++;
			}
			if (posX + 2 < BOARD_SIZE) {
				moveList[0][i] = posY;
				moveList[1][i] = posX + 2;
				i++;
			}   
			break;
		}
		case WHITE_QUEEN:
		case BLACK_QUEEN: {
			int j = 0;
			for (j = 0; j < BOARD_SIZE; j++) {
				if (j == 0) continue;

				if (posY + j < BOARD_SIZE && posX + j < BOARD_SIZE) {
					moveList[0][i] = posY + j;
					moveList[1][i] = posX + j;
					i++;
				}

				if (posY + j < BOARD_SIZE && posX - j >= 0) {
					moveList[0][i] = posY + j;
					moveList[1][i] = posX - j;
					i++;
				}

				if (posY - j >= 0 && posX + j < BOARD_SIZE) {
					moveList[0][i] = posY - j;
					moveList[1][i] = posX + j;
					i++;
				}

				if (posY - j >= 0 && posX - j >= 0) {
					moveList[0][i] = posY - j;
					moveList[1][i] = posX - j;
					i++;
				}
			}
			j = 0;
			for (j = 0; j < BOARD_SIZE; j++) {

				if (j == 0) continue;
				if (posY + j < BOARD_SIZE) {
					moveList[0][i] = posY + j;
					moveList[1][i] = posX;
					i++;
				}

				if (posY - j >= 0) {
					moveList[0][i] = posY - j;
					moveList[1][i] = posX;
					i++;
				}

				if (posX + j < BOARD_SIZE) {
					moveList[0][i] = posY;
					moveList[1][i] = posX + j;
					i++;
				}

				if (posX - j >= 0) {
					moveList[0][i] = posY;
					moveList[1][i] = posX - j;
					i++;
				}
			}
			break;
		}
		case BLACK_BISHOP:
		case WHITE_BISHOP: {
			int j = 0, startPosY = 0;
			for (j = 0, startPosY = posY; j < BOARD_SIZE; j++) {
				if (j == 0) continue;

				if (posY + j < BOARD_SIZE && posX + j < BOARD_SIZE) {
					moveList[0][i] = posY + j;
					moveList[1][i] = posX + j;
					i++;
				}

				if (posY + j < BOARD_SIZE && posX - j >= 0) {
					moveList[0][i] = posY + j;
					moveList[1][i] = posX - j;
					i++;
				}

				if (posY - j >= 0 && posX + j < BOARD_SIZE) {
					moveList[0][i] = posY - j;
					moveList[1][i] = posX + j;
					i++;
				}

				if (posY - j >= 0 && posX - j >= 0) {
					moveList[0][i] = posY - j;
					moveList[1][i] = posX - j;
					i++;
				}
			}
			break;
		}
		case BLACK_KNIGHT:
		case WHITE_KNIGHT: {
			if (posY + 2 < BOARD_SIZE && posX - 1 >= 0) {
				moveList[0][i] = posY + 2;
				moveList[1][i] = posX - 1;
				i++;
			}

			if (posY + 2 < BOARD_SIZE && posX + 1 < BOARD_SIZE) {
				moveList[0][i] = posY + 2;
				moveList[1][i] = posX + 1;
				i++;
			}

			if (posY + 1 < BOARD_SIZE && posX + 2 < BOARD_SIZE) {
				moveList[0][i] = posY + 1;
				moveList[1][i] = posX + 2;
				i++;
			}

			if (posY + 1 < BOARD_SIZE && posX - 2 >= 0) {
				moveList[0][i] = posY + 1;
				moveList[1][i] = posX - 2;
				i++;
			}

			if (posY - 2 >= 0 && posX - 1 >= 0) {
				moveList[0][i] = posY - 2;
				moveList[1][i] = posX - 1;
				i++;
			}

			if (posY - 2 >= 0 && posX + 1 < BOARD_SIZE) {
				moveList[0][i] = posY - 2;
				moveList[1][i] = posX + 1;
				i++;
			}

			if (posY - 1 >= 0 && posX + 2 < BOARD_SIZE) {
				moveList[0][i] = posY - 1;
				moveList[1][i] = posX + 2;
				i++;
			}

			if (posY - 1 >= 0 && posX - 2 >= 0) {
				moveList[0][i] = posY - 1;
				moveList[1][i] = posX - 2;
				i++;
			}
			break;
		}
		case BLACK_ROOK:
		case WHITE_ROOK: {
			int j = 0;
			for (j = 0; j < BOARD_SIZE; j++) {
				if (j == 0) continue;
				if (posY + j < BOARD_SIZE) {
					moveList[0][i] = posY + j;
					moveList[1][i] = posX;
					i++;
				}

				if (posY - j >= 0) {
					moveList[0][i] = posY - j;
					moveList[1][i] = posX;
					i++;
				}

				if (posX + j < BOARD_SIZE) {
					moveList[0][i] = posY;
					moveList[1][i] = posX + j;
					i++;
				}

				if (posX - j >= 0) {
					moveList[0][i] = posY;
					moveList[1][i] = posX - j;
					i++;
				}
			}
			break;
		}
		case BLACK_PAWN:
		case WHITE_PAWN: {
			if (whiteMove) {
				if (posY - 1 >= 0) {
					moveList[0][i] = posY - 1;
					moveList[1][i] = posX;
					i++;
				}

				if (posY - 1 >= 0 && posX - 1 >= 0) {
					moveList[0][i] = posY - 1;
					moveList[1][i] = posX - 1;
					i++;
				}

				if (posX + 1 < BOARD_SIZE && posY - 1 >= 0) {
					moveList[0][i] = posY - 1;
					moveList[1][i] = posX + 1;
					i++;
				}

				if (posY - 2 >= 0) {
					moveList[0][i] = posY - 2;
					moveList[1][i] = posX;
					i++;
				}
			}
			else {
				if (posY + 1 < BOARD_SIZE) {
					moveList[0][i] = posY + 1;
					moveList[1][i] = posX;
					i++;
				}

				if (posY + 1 < BOARD_SIZE && posX - 1 >= 0) {
					moveList[0][i] = posY + 1;
					moveList[1][i] = posX - 1;
					i++;
				}

				if (posX + 1 < BOARD_SIZE && posY + 1 < BOARD_SIZE) {
					moveList[0][i] = posY + 1;
					moveList[1][i] = posX + 1;
					i++;
				}

				if (posY + 2 < BOARD_SIZE) {
					moveList[0][i] = posY + 2;
					moveList[1][i] = posX;
					i++;
				}
			}
			break;
		}
		default: printf("Error spriteValue is wrong in getMovesList %d\n", gameBoard[posY][posX]);
		}
	}