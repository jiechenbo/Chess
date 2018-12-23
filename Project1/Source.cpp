#include "Engine.h"
#include "AI.h"

bool makeBestMove(Board b, bool whiteMove) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1 && (gameBoard[i][j] >= lowerLimit && gameBoard[i][j] <= higherLimit)) {
				int moveList[2][64] = { { -1 } };
				b.getMoveList(whiteMove, j, i, moveList);
			}
		}
	}

}