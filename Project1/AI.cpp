#include "Engine.h"
#include "AI.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>    // std::max
#include <float.h>


void copyGamestate(int src[8][8], int dest[8][8]);

void AI::makeBestMove(Board* b, bool whiteMove) {
	int (*gameBoard)[8] = b->getGameBoard();
	vector<array<int, 4>> allMoves = b->generateAllMovelists(whiteMove, gameBoard);
	
	double bestValue = -DBL_MAX;
	int bestMove = 0;
	int depth = 4;
	for (int i = 0; i < allMoves.size(); i++) {
		array<int, 4> randMove = allMoves.at(i);
		int newGameBoard[8][8] = { {-1} };
		
		copyGamestate(gameBoard, newGameBoard);
		b->makeMove(randMove.at(0), randMove.at(1), randMove.at(2), randMove.at(3), whiteMove, newGameBoard, false);
		double boardValue;
		int key = hash(newGameBoard);
		//printf("%d\n", key);
		
		if (transpo.find(key) != transpo.end()) {
			array<double, 2> info = transpo[key];
			if (info.at(0) > depth) {

				//printf("WORKING!!! %d\n", info.at(1));
				bestMove = info.at(1);
			}
			else {
				boardValue = minimax(b, depth - 1, -DBL_MAX, DBL_MAX, newGameBoard, !whiteMove);
			}
		}
		else { 
			boardValue = minimax(b, depth - 1, -DBL_MAX, DBL_MAX, newGameBoard, !whiteMove);
		}

		if (transpo.find(key) == transpo.end()) {

			array<double, 2> info = { (double)depth, bestMove };

			// printf("%lf %d\n", info.at(0), depth);
			transpo[key] = info;
		}
		else {
			array<double, 2> info = transpo.at(key);
			//			if (depth == 2) printf("%lf %d\n", info.at(0), depth);

			if (info[0] > depth) {
				array<double, 2> info = { (double)depth, bestMove };
				transpo[key] = info;
			}
		}
		
		if (transpo.find(key) == transpo.end()) {
			array<double, 2> info = { (double) depth, boardValue };
			transpo[key] = info;
		}
	
		//	printf("%d %d %d %d %lf %lf\n", randMove.at(1), randMove.at(0), randMove.at(3), randMove.at(2), bestValue, boardValue);
		if (boardValue > bestValue) {
			
			bestValue = boardValue;
			bestMove = i;
		}
	}
	printf("FINAL VALUE: %lf %d %d\n", bestValue, bestMove, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
	array<int, 4> bestMoves = allMoves.at(bestMove);
	b->printBoard();
	b->makeMove(bestMoves.at(0), bestMoves.at(1), bestMoves.at(2), bestMoves.at(3), whiteMove, b->getGameBoard(), true);
}

double AI::minimax(Board* b, int depth, double alpha, double beta, int gameBoard[8][8], int whiteMove) {
	if (depth == 0) {
		return (double)-evaluateBoard(gameBoard) - evaluateBoardPositions(gameBoard);
	}
	vector<array<int, 4>> allMoves = b->generateAllMovelists(whiteMove, gameBoard);
	if (!whiteMove) {
		double bestMove = -DBL_MAX;
		for (int i = 0; i < allMoves.size(); i++) {

			array<int, 4> randMove = allMoves.at(i);
			int newGameBoard[8][8] = { {-1} };


			copyGamestate(gameBoard, newGameBoard);

			b->makeMove(randMove.at(0), randMove.at(1), randMove.at(2), randMove.at(3), whiteMove, newGameBoard, false);
			int key = hash(newGameBoard);
			//printf("%d\n", key);
		
			if (transpo.find(key) != transpo.end()) {
				array<double, 2> info = transpo[key];
				if (info.at(0) > depth) {

				//	printf("WORKING!!! %d\n", info.at(1));
					bestMove = info.at(1);
				}
				else {
					bestMove = max(bestMove, minimax(b, depth - 1, alpha, beta, newGameBoard, !whiteMove));
				}
			}
			else {
				bestMove = max(bestMove, minimax(b, depth - 1, alpha, beta, newGameBoard, !whiteMove));
			}

			if (transpo.find(key) == transpo.end()) {

				array<double, 2> info = { (double)depth, bestMove };

				// printf("%lf %d\n", info.at(0), depth);
				transpo[key] = info;
			}
			else {
				array<double, 2> info = transpo.at(key);
	//			if (depth == 2) printf("%lf %d\n", info.at(0), depth);

				if (info[0] > depth) {
					array<double, 2> info = { (double) depth, bestMove };
					transpo[key] = info;
				}
			} 
			//bestMove = max(bestMove, minimax(b, depth - 1, alpha, beta, newGameBoard, !whiteMove));

			alpha = max(alpha, bestMove);
			if (alpha >= beta) {
				//	printf("CUT-OFF\n");
				break;
			}
		}
		return bestMove;
	}
	else {
		double bestMove = DBL_MAX;
		for (int i = 0; i < allMoves.size(); i++) {

			array<int, 4> randMove = allMoves.at(i);
			int newGameBoard[8][8] = { {-1} };
			


			copyGamestate(gameBoard, newGameBoard);

			b->makeMove(randMove.at(0), randMove.at(1), randMove.at(2), randMove.at(3), whiteMove, newGameBoard, false);
			int key = hash(newGameBoard);
			bestMove = min(bestMove, minimax(b, depth - 1, alpha, beta, newGameBoard, !whiteMove));
	
			
			//bestMove = min(bestMove, minimax(b, depth - 1, alpha, beta, newGameBoard, !whiteMove));

			beta = min(beta, bestMove);
			if (alpha >= beta) {

				//printf("CUT-OFF\n");
				break;
			}
			array<double, 2> info = { (double)depth, bestMove };
			transpo[key] = info;

		}
		return bestMove;
	}

}
double AI::evaluateBoardPositions(int gameBoard[8][8]) {
	double overall = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			switch (gameBoard[i][j]) {

			case BLACK_PAWN:
			case WHITE_PAWN: {
				double positionTable[8][8] = {
					{0, 0, 0, 0, 0, 0, 0, 0},
					{5, 5, 5, 5, 5, 5, 5, 5},
					{1, 1, 2, 3, 3, 2, 1, 1},
					{0.5, 0.5, 1, 2.5, 2.5, 1, 0.5, 0.5},
					{0, 0, 0, 2, 2, 0, 0 ,0},
					{0.5, -0.5, -1.0, 0, 0, -1.0, -0.5, 0.5},
					{0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5},
					{0, 0, 0, 0, 0, 0, 0, 0}
				};

				if (gameBoard[i][j] == WHITE_PAWN) overall += positionTable[i][j];
				if (gameBoard[i][j] == BLACK_PAWN) overall -= positionTable[BOARD_SIZE - i - 1][j];

				break;
			}
			case BLACK_KNIGHT:
			case WHITE_KNIGHT: {
				double positionTable[8][8] = {
					{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
					{-4.0, -2.0, 0.0, 0.0, 0.0, 0.0, -2.0, -4.0},
					{-3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0, -3.0},
					{-3.0, 0.5, 1.5, 2.0, 2.0, 1.5, 0.5, -3.0},
					{-3.0, 0, 1.5, 2.0, 2.0, 1.5, 0, -3.0},
					{-3.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -3.0},
					{-4.0, -2.0, 0.0, 0.5, 0.5, 0.0, -2.0, -4.0},
					{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
				};
				if (gameBoard[i][j] == WHITE_KNIGHT) overall += positionTable[i][j];
				if (gameBoard[i][j] == BLACK_KNIGHT) overall -= positionTable[BOARD_SIZE - 1 - i][j];
				break;
			}
			case BLACK_BISHOP:
			case WHITE_BISHOP: {
				double positionTable[8][8] = {
					{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
					{-1.0, 0, 0, 0, 0, 0, 0, -1.0},
					{-1.0, 0, 0.5, 1.0, 1.0, 0.5, 0, -1.0},
					{-1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0},
					{-1.0, 0, 1, 1, 1, 1, 0, -1.0},
					{-1.0, 1, 1, 1, 1, 1, 1, -1.0},
					{-1.0, 0.5, 0, 0, 0, 0, 0.5, -1.0},
					{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
				};
				if (gameBoard[i][j] == WHITE_BISHOP) overall += positionTable[i][j];
				if (gameBoard[i][j] == BLACK_BISHOP) overall -= positionTable[BOARD_SIZE - i - 1][j];
				break;
			}
			case BLACK_ROOK:
			case WHITE_ROOK: {
				double positionTable[8][8] = {
					{0, 0, 0, 0, 0, 0, 0, 0},
					{0.5, 1, 1, 1, 1, 1, 1, 0.5},
					{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
					{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
					{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
					{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
					{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
					{0, 0, 0, 0.5, 0.5, 0, 0, 0}
				};
				if (gameBoard[i][j] == WHITE_ROOK) overall += positionTable[i][j];
				if (gameBoard[i][j] == BLACK_ROOK) overall -= positionTable[BOARD_SIZE - i - 1][j];
				break;
			}
			case BLACK_QUEEN:
			case WHITE_QUEEN: {
				double positionTable[8][8] = {
					{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
					{-1.0, 0, 0, 0, 0, 0, 0, -1.0},
					{-1.0, 0, 0.5, 0.5, 0.5, 0.5, 0, -1.0},
					{-0.5, 0, 0.5, 0.5, 0.5, 0.5, 0, -0.5},
					{0, 0, 0.5, 0.5, 0.5, 0.5, 0, -0.5},
					{-1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0, -1.0},
					{-1.0, 0, 0.5, 0, 0, 0, 0, -1.0},
					{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
				};
				if (gameBoard[i][j] == WHITE_QUEEN) overall += positionTable[i][j];
				if (gameBoard[i][j] == BLACK_QUEEN) overall -= positionTable[BOARD_SIZE - i - 1][BOARD_SIZE - 1 - j];
				break;
			}
			case BLACK_KING:
			case WHITE_KING: {
				double positionTable[8][8] = {
					{-3, -4, -4, -5, -5, -4, -4, -3},
					{-3, -4, -4, -5, -5, -4, -4, -3},
					{-3, -4, -4, -5, -5, -4, -4, -3},
					{-3, -4, -4, -5, -5, -4, -4, -3},
					{-2, -3, -3, -4, -4, -3, -3, -2},
					{-1, -2, -2, -2, -2, -2, -2, -1},
					{2, 2, 0, 0, 0, 0, 2, 2},
					{2, 3, 1, 0, 0, 1, 3, 2},
				};
				if (gameBoard[i][j] == WHITE_KING) overall += positionTable[i][j];
				if (gameBoard[i][j] == BLACK_KING) overall -= positionTable[BOARD_SIZE - i - 1][BOARD_SIZE - 1 - j];
				break;
			}

			}
		}
	}
	return overall;
}
int AI::evaluateBoard(int gameBoard[8][8]) {
	int overall = 0;
	//printf("-------------------BOARD EVALUATION-----------------\n");
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			switch (gameBoard[i][j]) {
			case WHITE_PAWN: overall += 10; break;
			case BLACK_PAWN: overall -= 10; break;
			case WHITE_KNIGHT: overall += 30; break;
			case BLACK_KNIGHT: overall -= 30; break;
			case WHITE_BISHOP: overall += 30; break;
			case BLACK_BISHOP: overall -= 30; break;
			case WHITE_ROOK: overall += 50; break;
			case BLACK_ROOK: overall -= 50; break;
			case WHITE_QUEEN: overall += 90; break;
			case BLACK_QUEEN: overall -= 90; break;
			case WHITE_KING: overall += 900; break;
			case BLACK_KING: overall -= 900; break;
			default: break;
			}
		}
	}
	/*if(overall != -10 && overall != 0) printf("overall =: %d\n", overall);
	if (overall != 0 && overall != -10) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				printf("%d ", gameBoard[i][j]);

			}
			printf("\n");
		}
	}*/
	return overall;
}

void AI::initZobrist() {
	srand(5);
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			for (int k = 0; k < ALL_PIECES; k++) {
				zobristTable[i][j][k] = rand();
			}
		}
	}
}

int AI::hash(int gameBoard[8][8]) {
	int h = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (gameBoard[i][j] != -1) {
				int pieceV = gameBoard[i][j];
				h = (h ^ zobristTable[i][j][pieceV]);

			}
		}
	}
	return h;
}

void copyGamestate(int src[8][8], int dest[8][8]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			dest[i][j] = src[i][j];
		}
	}
}