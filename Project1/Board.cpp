#include "Engine.h"

void initRow(int gameBoard[8][8], int startRow, int isBlack); 
bool pawnMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool rookMoves(int spriteX, int spriteY, int gameBoard[8][8], bool whiteMove, int newX, int newY);
bool bishopMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool horseMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool queenMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);

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
	//gameBoard[0][0] = 6;
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

bool Board::makeMove(int spriteX, int spriteY, int newX, int newY, int spriteValue, bool whiteMove, bool doMove) {
	printf("spriteX: %d spriteY:%d\n", spriteX, spriteY);

	bool validMoveCheck = false;
	validMoveCheck = validMove(spriteX, spriteY, newX, newY, spriteValue, whiteMove, doMove, gameBoard);
	int newGameBoard[8][8] = { { -1 } };
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			newGameBoard[i][j] = gameBoard[i][j];
		}
	}
	newGameBoard[newY][newX] = newGameBoard[spriteY][spriteX];
	newGameBoard[spriteY][spriteX] = -1;
	printf("Valid move? %d\n", validMoveCheck == true);
	if (doMove) {
		bool check = kingCheckMoves(whiteMove, newGameBoard);
		if (validMoveCheck == true && !check) {

			int king = whiteMove == true ? WHITE_KING : BLACK_KING;


			int distanceX = abs(newX - spriteX);
			int distanceY = abs(newY - spriteY);

			if (gameBoard[spriteY][spriteX] == king && distanceX == 2 && distanceY == 0 && kingCheckMoves(whiteMove, gameBoard)) return false;
			if (gameMoved[spriteY][spriteX] == false && initGameBoard[spriteY][spriteX] == gameBoard[spriteY][spriteX]) {
				gameMoved[spriteY][spriteX] = true;
			}

			if (gameBoard[spriteY][spriteX] == king) {
				printf("\nCASTLING....... %d %d %d %d\n", distanceX, distanceY, newX, spriteX);

				if (distanceX == 2 && distanceY == 0) {
					
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
			
			printf("\nCASTLING....... %d\n", gameBoard[newY][newX]);
		
			if ((gameBoard[newY][newX] == BLACK_PAWN && newY == BOARD_SIZE - 1) || (gameBoard[newY][newX] == WHITE_PAWN && newY == 0)) {
				gameBoard[newY][newX] = whiteMove ? WHITE_QUEEN : BLACK_QUEEN;
			}
			//printBoard();
			if (kingCheckMoves(!whiteMove, gameBoard)) {
				printf("WASMGFIASGMSAIGMASIGMAIGMAIGAMIGMASIGMASIGMASIGMAIGMAIGAM");
				checkWin(!whiteMove, gameBoard);
			} else if (checkStale(!whiteMove, gameBoard)) {
				printf("stale!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				//exit(0);
			}
			printf("hey activated");
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					printf("%d ", gameMoved[i][j]);
				}
				printf("\n");
			}
			return true;
		}
		else {

		}
	}
	else {
		if (validMoveCheck && !kingCheckMoves(whiteMove, newGameBoard)) {
			printBoard();
			return true;
		}
	}
	printBoard();
	return false;
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
				getMoveList(whiteMove, j, i, moveList);
				for (int k = 0; k < 64 && moveList[0][k] != -1; k++) {

					printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], i, j, moveList[0][k], moveList[1][k]);					
					bool f = validMove(j, i, moveList[1][k], moveList[0][k], gameBoard[i][j], whiteMove, false, gameBoard);
					bool s = !checkKingMove(whiteMove, j, i, moveList[1][k], moveList[0][k]);
					printf("CHECKING FUNCTIONN %d %d\n", f, s);
					if (f && s) {
						printf("CHESS PIECE %d %d %d %d!!!!!!!!!!!!!!!\n\n\n\n\n", i, j, moveList[0][k], moveList[1][k]);
						return false;
					}	
				}

			}
		}
	}
	return true;

}

bool Board::checkWin(bool whiteMove, int gameBoard[8][8]) {
	printf("\n--------------CHECKING WIN--------------\n");
	
	int lowerLimit = whiteMove == true ? WHITE_KING : BLACK_KING;
	int higherLimit = whiteMove == true ? WHITE_PAWN : BLACK_PAWN;

	int king = whiteMove == true ? BLACK_KING : WHITE_KING ;
	bool check = true;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int moveList[2][64] = { { -1 } };
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				getMoveList(whiteMove, j, i, moveList);
				for (int k = 0; k < 64 && moveList[0][k] != -1; k++) {

					printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], j, i, moveList[0][k], moveList[1][k]);
					bool f = validMove(j, i, moveList[1][k], moveList[0][k], gameBoard[i][j], whiteMove, false, gameBoard);
					bool s = !checkKingMove(whiteMove, j, i, moveList[1][k], moveList[0][k]);
					printf("CHECKING FUNCTIONN %d %d\n", f, s);
					if (f && s) {
						check = false;
						printf("CHESS PIECE %d %d %d %d!!!!!!!!!!!!!!!\n\n\n\n\n", j, i, moveList[0][k], moveList[1][k]);
						break;
					}
				}

			}

			if (!check) break;
		}
		if (!check) break;
	}

	if (check == true) {
		printf("WE WIN!!!!!!!!!!!!!!!!!!!!!!!!!!");
		exit(0);
		return true;
	}
	return false;
}

bool Board::checkKingMove(bool whiteMove, int posX, int posY, int newX, int newY) {
	int newGameBoard[8][8] = { { -1 } };
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			newGameBoard[i][j] = gameBoard[i][j];
		}
	}
	newGameBoard[newY][newX] = newGameBoard[posY][posX];
	if (newY != posY || newX != posX) newGameBoard[posY][posX] = -1;
	printf("checlkKingMove %d %d %d %d %d %d\n", newY, newX, posY, posX, newGameBoard[newY][newX], newGameBoard[posY][posX]);
	return kingCheckMoves(whiteMove, newGameBoard);
}

bool Board::validMove(int spriteX, int spriteY, int newX, int newY, int spriteValue, bool whiteMove, bool doMove, int gameBoard[8][8]) {
	int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
	int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;



	if (!(gameBoard[newY][newX] >= lowerLimit && gameBoard[newY][newX] <= higherLimit) && gameBoard[newY][newX] != -1) {
		printf("entered in valid.");
		return false;
	}


printf("spriteX: %d spriteY:%d newX:%d newY: %d\n", spriteX, spriteY, newX, newY);
	bool validMove = false;
	if (whiteMove == true) {
		switch (spriteValue) {
		case 0: validMove = kingMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 1: validMove = queenMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 2: validMove = bishopMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 3: validMove = horseMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 4: validMove = rookMoves(spriteX, spriteY, gameBoard, whiteMove, newX, newY); break;

		case 5: validMove = pawnMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		default: printf("Error spriteValue is wrong");
		}
	}
	else {
		switch (spriteValue) {
		case 6: validMove = kingMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 7: validMove = queenMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 8: validMove = bishopMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove);  break;

		case 9: validMove = horseMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 10: validMove = rookMoves(spriteX, spriteY, gameBoard, whiteMove, newX, newY); break;

		case 11: validMove = pawnMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		default: printf("Error spriteValue is wrong");
		}
	}

	return validMove;
}

bool Board::kingCheckMoves(bool whiteMove, int gameBoard[8][8]) {
	printf("--------------CHECKING CHECK--------------\n");
	
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

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				printf("---------CHECKING VALID MOVES----- kings Pos X: %d kings Pos Y: %d chessPosX: %d chessPosY: %d %d\n", kingPosX, kingPosY, j, i, gameBoard[i][j]);
				if (validMove(j, i, kingPosX, kingPosY, gameBoard[i][j], !whiteMove, false, gameBoard)) {
					
					printf("--------------------IN-CHECK---------------------");
					return true;
				}
			}
		}

	}
	return false;
}
	
bool queenMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {

	if (bishopMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove) || rookMoves(spriteX, spriteY, gameBoard, whiteMove, newX, newY)) {
		return true;
	}
	else {
		return false;
	}
}

bool Board::kingMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {
	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);

	if ((distanceX == 1 && distanceY == 1) || (distanceY == 1 && distanceX == 0) || (distanceX == 1 && distanceY == 0)) {

		int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
		int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;

		if (gameBoard[newY][newX] >= lowerLimit && gameBoard[newY][newX] <= higherLimit) {
			return true;
		}
		else if (gameBoard[newY][newX] == -1) {
			return true;
		}
	
	}
	else if (distanceX == 2 && distanceY == 0) {
		if (initGameBoard[spriteY][spriteX] != gameBoard[spriteY][spriteX] || gameMoved[spriteY][spriteX] == true) {
			return false;
		}
		bool seenRook = false;
		bool positiveDirection = newX - spriteX > 0 ? true : false;
		for (int position = spriteX; position < BOARD_SIZE && position >= 0;) {
			if (position == spriteX) {
				position += positiveDirection ? +1 : -1;
				continue;
			}
			//printf("positionCheck %d %d %d %d\n", spriteX, spriteY, position, seenUnit);
			if (gameBoard[spriteY][position] != -1) {
				int rook = whiteMove == true ? WHITE_ROOK : BLACK_ROOK;
				if (gameBoard[spriteY][position] == rook && initGameBoard[spriteY][spriteX] == gameBoard[spriteY][spriteX] && gameMoved[spriteY][position] == false) {
					seenRook = true;
				}
				break;
			}

			if (checkKingMove(whiteMove, spriteX, spriteY, position, spriteY)) return false;
			position += positiveDirection ? +1 : -1;
		}
		return seenRook;
	}
	 return false;
}

bool bishopMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {
	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);
	//printf("distanceX %d distanceY %d %d %d", spriteX, spriteY, newX, newY);
	if ((distanceY > 0 && distanceX > 0) && (distanceX == distanceY)) {
		//check scanline
		bool positiveDirectionX = newX - spriteX > 0 ? true : false;
		bool positiveDirectionY = newY - spriteY > 0 ? true : false;
		bool seenUnit = false;
		//printf("positionCheck %d %d\n", newX, newY);
		int positionX = 0, positionY = 0;
		for (positionX = spriteX, positionY = spriteY; positionX < BOARD_SIZE && positionX >= 0 && positionY < BOARD_SIZE && positionY >= 0;) {
			if (positionX == spriteX && positionY == spriteY) {
				positionX += positiveDirectionX ? +1 : -1;
				positionY += positiveDirectionY ? +1 : -1;
				continue;
			}
			//printf("positionCheck %d %d %d %d\n", positionX, positionY, seenUnit);

			if (seenUnit == true) break;
			if (gameBoard[positionY][positionX] != -1) {
				int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
				int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;
				if (positionY == newY && positionX == newX && gameBoard[positionY][positionX] >= lowerLimit && gameBoard[positionY][positionX] <= higherLimit) {
					return true;
				}
				seenUnit = true;
			}
			else {
				if (positionY == newY && positionX == newX) {
					return true;
				}
			}
		
			positionX += positiveDirectionX ? +1 : -1;
			positionY += positiveDirectionY ? +1 : -1;
		}
	}
	//printf("HIHHUH MOVE NOT VALID BISHOP");
	return false;
}

bool horseMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {
	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);
	//printf("hihihihihi %d %d", distanceX, distanceY);
	if ((distanceY == 2 && distanceX == 1) || (distanceX == 2 && distanceY == 1)) {
		//printf("hihihihihi");
		int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
		int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;
		if ((gameBoard[newY][newX] >= lowerLimit && gameBoard[newY][newX] <= higherLimit) || gameBoard[newY][newX] == -1) {
			return true;
		}
	}
	return false;
}

bool pawnMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {
	bool haveMoved = true;
	if (whiteMove && spriteY == 6) {
		haveMoved = false;
	}
	else if (!whiteMove && spriteY == 1) {
		haveMoved = false;
	}

	int limit = haveMoved == true ? 1 : 2;
	bool seenUnit = false;
	int i = 0, posY = whiteMove == true ? spriteY - 1 : spriteY + 1;

	//printf("%d %d, %d, %d limit %d\n", spriteX, spriteY, newX, newY, limit);
	for (posY, i = 0; i < limit && posY >= 0 && posY < BOARD_SIZE;i++) {
	//	printf("%d\n", posY);
		if (gameBoard[posY][spriteX] != -1 && spriteY != posY) {
		//	printf("hsfasfasfasfi");
			break;
		}
		else if (gameBoard[posY][spriteX] == -1) {
			if (newY == posY && spriteX == newX) {
			//	printf("%d %d, %d, %d\n", posY, spriteY, newX, newY);
			//	printf("Found Valid Move\n");
				return true;
			}
		}
		posY = whiteMove == true ? posY - 1 : posY + 1;

	}
	int offsetPosY = whiteMove == true ? spriteY - 1 : spriteY + 1;
	int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
	int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;

	printf("limit %d %d\n", lowerLimit, higherLimit);
	if (spriteX + 1 < BOARD_SIZE && gameBoard[offsetPosY][spriteX + 1] >= lowerLimit && gameBoard[offsetPosY][spriteX + 1] <= higherLimit) {

		printf("wtf!?!?!? %d %d %d  %d\n", offsetPosY, spriteX + 1, newY, newX);
		if (offsetPosY == newY && spriteX + 1 == newX) return true;
	}
	if (spriteX - 1 >= 0 && gameBoard[offsetPosY][spriteX - 1] >= lowerLimit && gameBoard[offsetPosY][spriteX - 1] <= higherLimit) {
		if (offsetPosY == newY && spriteX - 1 == newX) return true;
	}


	return false;
}

bool rookMoves(int spriteX, int spriteY, int gameBoard[8][8], bool whiteMove, int newX, int newY) {
	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);

	if ((distanceY == 0 && distanceX > 0) || (distanceX == 0 && distanceY > 0)) {
		//check scanline
		bool isHorizontal = distanceX > 0 ? true : false;
		bool positiveDirection = newX - spriteX > 0 || newY - spriteY > 0 ? true : false;
		bool seenUnit = false;
		//printf("positionCheck %d %d\n", newX, newY);
		for (int position = isHorizontal ? spriteX : spriteY; position < BOARD_SIZE && position >= 0;) {
			if ((position == spriteX && isHorizontal) || (position == spriteY && !isHorizontal)) {
				position += positiveDirection ? +1 : -1;
				continue;
			}
			//printf("positionCheck %d %d %d %d\n", spriteX, spriteY, position, seenUnit);
			
			if (seenUnit == true) break;
			
			if (isHorizontal) {
				if (gameBoard[spriteY][position] != -1) {
					int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
					int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;
					if (spriteY == newY && position == newX && gameBoard[spriteY][position] >= lowerLimit && gameBoard[spriteY][position] <= higherLimit) {
						return true;
					}
					seenUnit = true;
				}
				else {
					if (spriteY == newY && position == newX) {
						return true;
					}
				}
			}
			else {
				if (gameBoard[position][spriteX] != -1) {
					int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
					int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;
					if (spriteX == newX && position == newY && gameBoard[position][spriteX] >= lowerLimit && gameBoard[position][spriteX] <= higherLimit) {
						return true;
					}
					seenUnit = true;
				} else if (spriteX == newX && position == newY) {
					return true;
				}
			}
			position += positiveDirection ? +1 : -1;
		}
	}
	//printf("HIHHUH MOVE NOT VALID ROOK");
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

void Board::getMoveList(bool whiteMove, int posX, int posY, int moveList[2][64]) {
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
			printf("entered\n");
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
		default: printf("Error spriteValue is wrong");
		}
	}

	void getValidMoves() {

	}