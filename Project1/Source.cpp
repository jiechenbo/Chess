#include "Engine.h"
#include "AI.h"

void makeBestMove(Board* b, bool whiteMove) {

	int lowerLimit = whiteMove == true ? WHITE_KING : BLACK_KING;
	int higherLimit = whiteMove == true ? WHITE_PAWN : BLACK_PAWN;

	int (*gameState)[8] = b->getGameBoard();
	printf("%d\n", gameState[0][0]);
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int moveList[2][64] = { { -1 } };

			if (gameState[i][j] != -1 && (gameState[i][j] >= lowerLimit && gameState[i][j] <= higherLimit)) {
				b->getMoveList(whiteMove, j, i, moveList);
				for (int k = 0; k < 64 && moveList[0][k] != -1; k++) {

					//printf("THIS IS IMPORTANT %d %d %d %d %d\n", gameBoard[i][j], j, i, moveList[0][k], moveList[1][k]);
					bool f = b->validMove(j, i, moveList[1][k], moveList[0][k], gameState[i][j], whiteMove, false, gameState);
					bool s = !b->checkKingMove(whiteMove, j, i, moveList[1][k], moveList[0][k]);
					if (f && s) {
						if (b->makeMove(j, i, moveList[0][k], moveList[1][k], gameState[i][j], whiteMove, true) == true) {
							break;
						}
					}

				}
			}
		}
	}

}
