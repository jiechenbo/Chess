#include "Engine.h"
bool pawnMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool rookMoves(int spriteX, int spriteY, int gameBoard[8][8], bool whiteMove, int newX, int newY);
bool bishopMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool horseMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);
bool queenMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);	

bool Board::validMove(int spriteX, int spriteY, int newX, int newY, bool whiteMove, bool doMove, int gameBoard[8][8]) {
	int lowerLimit = whiteMove == true ? BLACK_KING : WHITE_KING;
	int higherLimit = whiteMove == true ? BLACK_PAWN : WHITE_PAWN;

	if (!(gameBoard[newY][newX] >= lowerLimit && gameBoard[newY][newX] <= higherLimit) && gameBoard[newY][newX] != -1) {
		//printf("entered in valid.");
		return false;
	}

	int spriteValue = gameBoard[spriteY][spriteX];
	//printf("spriteX: %d spriteY:%d newX:%d newY: %d\n", spriteX, spriteY, newX, newY);
	bool validMove = false;
	if (whiteMove == true) {
		switch (spriteValue) {
		case 0: validMove = kingMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 1: validMove = queenMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 2: validMove = bishopMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 3: validMove = horseMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		case 4: validMove = rookMoves(spriteX, spriteY, gameBoard, whiteMove, newX, newY); break;

		case 5: validMove = pawnMoves(spriteX, spriteY, newX, newY, gameBoard, whiteMove); break;

		default: printf("Error spriteValue is wrong %d", spriteValue);
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

		default:	printf("Error spriteValue is wrong %d", spriteValue);
		}
	}

	return validMove;
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
		printf("---------Checking castle availability------------------ %d\n", kingCheckMoves(whiteMove, gameBoard));
		if (kingCheckMoves(whiteMove, gameBoard)) return false;
		//printf("1nd test!!!!\n");
		if (initGameBoard[spriteY][spriteX] != gameBoard[spriteY][spriteX] || gameMoved[spriteY][spriteX] == true) {
			return false;
		}
		//printf("2nd test!!!!\n");
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

			if (checkKingMove(whiteMove, spriteX, spriteY, position, spriteY, gameBoard)) return false;
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
	for (posY, i = 0; i < limit && posY >= 0 && posY < BOARD_SIZE; i++) {
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

	//printf("limit %d %d\n", lowerLimit, higherLimit);
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
				}
				else if (spriteX == newX && position == newY) {
					return true;
				}
			}
			position += positiveDirection ? +1 : -1;
		}
	}
	//printf("HIHHUH MOVE NOT VALID ROOK");
	return false;

}




