#include "Engine.h"
void initRow(int gameBoard[8][8], int startRow, int isBlack); 
bool pawnMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool rookMoves(int spriteX, int spriteY, int gameBoard[8][8], bool whiteMove, int newX, int newY);
bool bishopMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool horseMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool queenMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool kingMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);

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
	initRow(gameBoard, BOARD_SIZE - 1, 0);

	for (int i = 0; i < BOARD_SIZE; i++) {
		gameBoard[1][i] = BLACK_PAWN;
	}

for (int i = 0; i < BOARD_SIZE; i++) {
	gameBoard[BOARD_SIZE - 2][i] = WHITE_PAWN;
}

for (int i = 0; i < BOARD_SIZE; i++) {
	for (int j = 0; j < BOARD_SIZE; j++) {
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

bool Board::validMove(int spriteX, int spriteY, int newX, int newY, int spriteValue, bool whiteMove, bool doMove) {

	int scaleWidth = gBoard.getWidth() / BOARD_SIZE;
	int scaleHeight = gBoard.getHeight() / BOARD_SIZE;

	int scaledSpriteY = spriteY / scaleHeight;
	int scaledSpriteX = spriteX / scaleWidth;

	int scaledNewX = newX / scaleWidth;
	int scaledNewY = newY / scaleHeight;
	printf("spriteX: %d spriteY:%d\n", scaledSpriteX, scaledSpriteY);
	int allValidMoves[2][64] = { {-1} };
	for (int i = 0; i < 64; i++) {

		//printf("%d %d\n", allValidMoves[0][i], allValidMoves[1][i]);
		allValidMoves[0][i] = -1;
		allValidMoves[1][i] = -1;

	}

	bool validMove = false;
	if (whiteMove == true) {
		switch (spriteValue) {
		case 0: validMove = kingMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		case 1: validMove = queenMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		case 2: validMove = bishopMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		case 3: validMove = horseMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		case 4: validMove = rookMoves(scaledSpriteX, scaledSpriteY, gameBoard, whiteMove, scaledNewX, scaledNewY); break;

		case 5: validMove = pawnMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		default: printf("Error spriteValue is wrong");
		}
	}
	else {
		switch (spriteValue) {
		case 6: validMove = kingMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		case 7: validMove = queenMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		case 8: validMove = bishopMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove);  break;

		case 9: validMove = horseMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		case 10: validMove = rookMoves(scaledSpriteX, scaledSpriteY, gameBoard, whiteMove, scaledNewX, scaledNewY); break;

		case 11: validMove = pawnMoves(scaledSpriteX, scaledSpriteY, scaledNewX, scaledNewY, gameBoard, whiteMove); break;

		default: printf("Error spriteValue is wrong");
		}
	}

	for (int i = 0; i < 64; i++) {

		//printf("%d %d %d %d %d\n", allValidMoves[0][i], allValidMoves[1][i], scaledNewY, scaledNewX, spriteValue);
		if (allValidMoves[0][i] == -1 && allValidMoves[1][i] == -1) {
			//printf("%d %d\n", allValidMoves[0][i], allValidMoves[1][i]);
			//printBoard();
			//return false;
		} else if (allValidMoves[0][i] == scaledNewY && allValidMoves[1][i] == scaledNewX) {
			//gameBoard[scaledNewY][scaledNewX] = gameBoard[scaledSpriteY][scaledSpriteX];
			//gameBoard[scaledSpriteY][scaledSpriteX] = -1;
			//printBoard();
			//return true;
		}
		
	}

	printf("Valid move? %d\n", validMove == true);
	if (doMove) {
		if (validMove == true && !kingCheckMoves(whiteMove)) {
			gameBoard[scaledNewY][scaledNewX] = gameBoard[scaledSpriteY][scaledSpriteX];
			gameBoard[scaledSpriteY][scaledSpriteX] = -1;
			//printBoard();
			printf("hey activated");
			return true;
		}
	} else {
		if (validMove) return true;	
	}
	printBoard();
	return false;
}

bool Board::kingCheckMoves(bool whiteMove) {
	printf("--------------CHECKING CHECK--------------\n");
	int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
	int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;

	int king = whiteMove == true ? WHITE_KING : BLACK_KING;
	int kingPosX = -1;
	int kingPosY = -1;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] == king) {
				kingPosX = i;
				kingPosY = j;
				break;
			}
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				printf("---------CHECKING VALID MOVES----- kings Pos X: %d kings Pos Y: %d chessPosX: %d chessPosY: %d\n", kingPosX, kingPosY, j, i);
				int scaleWidth = gBoard.getWidth() / BOARD_SIZE;
				int scaleHeight = gBoard.getHeight() / BOARD_SIZE;

				int scaledSpriteY = i * scaleHeight;
				int scaledSpriteX = j * scaleWidth;

				int scaledKingPosX = kingPosX * scaleWidth;
				int scaledKingPosY = kingPosY * scaleHeight;
				if (validMove(scaledSpriteX, scaledSpriteY, scaledKingPosX, scaledKingPosY, gameBoard[i][j], !whiteMove, false)) {
					
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

bool kingMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {
	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);

	if ((distanceX == 1 && distanceY == 1) || (distanceY == 1 && distanceX == 0) || (distanceX == 1 && distanceY == 0)) {
		return true;
	}
	return false;
}

bool bishopMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {
	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);
	printf("distanceX %d distanceY %d", spriteX, spriteY);
	if ((distanceY > 0 && distanceX > 0) && (distanceX == distanceY)) {
		//check scanline
		bool positiveDirectionX = newX - spriteX > 0 ? true : false;
		bool positiveDirectionY = newY - spriteY > 0 ? true : false;
		bool seenUnit = false;
		printf("positionCheck %d %d\n", newX, newY);
		int positionX = 0, positionY = 0;
		for (positionX = spriteX, positionY = spriteY; positionX < BOARD_SIZE && positionX >= 0 && positionY < BOARD_SIZE && positionY >= 0;) {
			if (positionX == spriteX && positionY == spriteY) {
				positionX += positiveDirectionX ? +1 : -1;
				positionY += positiveDirectionY ? +1 : -1;
				continue;
			}
			//printf("positionCheck %d %d %d %d\n", spriteX, spriteY, position, seenUnit);

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
	printf("HIHHUH MOVE NOT VALID BISHOP");
	return false;
}

bool horseMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove) {
	int distanceX = abs(newX - spriteX);
	int distanceY = abs(newY - spriteY);
	printf("hihihihihi %d %d", distanceX, distanceY);
	if ((distanceY == 2 && distanceX == 1) || (distanceX == 2 && distanceY == 1)) {
		printf("hihihihihi");
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

	printf("%d %d, %d, %d limit %d\n", spriteX, spriteY, newX, newY, limit);
	for (posY, i = 0; i < limit && posY >= 0 && posY < BOARD_SIZE;i++) {
		printf("%d\n", posY);
		if (gameBoard[posY][spriteX] != -1 && spriteY != posY) {
			printf("hsfasfasfasfi");
			break;
		}
		else if (gameBoard[posY][spriteX] == -1) {
			if (newY == posY && spriteX == newX) {
				printf("%d %d, %d, %d\n", posY, spriteY, newX, newY);
				printf("Found Valid Move\n");
				return true;
			}
		}
		posY = whiteMove == true ? posY - 1 : posY + 1;

	}
	int offsetPosY = whiteMove == true ? spriteY - 1 : spriteY + 1;
	int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
	int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;
	if (spriteX + 1 < BOARD_SIZE && gameBoard[offsetPosY][spriteX + 1] >= lowerLimit && gameBoard[offsetPosY][spriteX + 1] <= higherLimit) {
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
		printf("positionCheck %d %d\n", newX, newY);
		for (int position = isHorizontal ? spriteX : spriteY; position < BOARD_SIZE && position >= 0;) {
			if ((position == spriteX && isHorizontal) || (position == spriteY && !isHorizontal)) {
				position += positiveDirection ? +1 : -1;
				continue;
			}
			printf("positionCheck %d %d %d %d\n", spriteX, spriteY, position, seenUnit);
			
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
	printf("HIHHUH MOVE NOT VALID ROOK");
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