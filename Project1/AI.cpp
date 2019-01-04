#include "Engine.h"
#include "AI.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>    // std::max
#include <float.h>
#include <random>
#include <limits>

void copyGamestate(int src[8][8], int dest[8][8]);

void AI::makeBestMove(Board* b, bool whiteMove) {
	int (*gameBoard)[8] = b->getGameBoard();
	bool(*gameMoved)[8] = b->getGameMoved();
	vector<array<int, 4>> allMoves = b->generateAllMovelists(whiteMove, gameBoard);
	
	double bestValue = -DBL_MAX;
	int bestMove = 0;
	int depth = 3;
	
	for (int i = 0; i < allMoves.size(); i++) {
		array<int, 4> randMove = allMoves.at(i);
		int newGameBoard[8][8] = { {-1} };
		bool newGameMoved[8][8];
		b->copyGameMoved(gameMoved, newGameMoved);
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				newGameBoard[i][j] = gameBoard[i][j];
			}
		}

		b->makeMove(randMove.at(0), randMove.at(1), randMove.at(2), randMove.at(3), whiteMove, newGameBoard, newGameMoved);
		double boardValue;
		//printf("%d\n", key);
		//for (int iterative = 0; iterative < depth; iterative++) {
			boardValue = minimax(b, depth - 1 , -DBL_MAX, DBL_MAX, newGameBoard, newGameMoved, !whiteMove);
		//}
		//	printf("%d %d %d %d %lf %lf\n", randMove.at(1), randMove.at(0), randMove.at(3), randMove.at(2), bestValue, boardValue);
		if (boardValue > bestValue) {
			
			bestValue = boardValue;
			bestMove = i;
		}
	}
	printf("FINAL VALUE: %lf %d %d\n", bestValue, bestMove, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
	array<int, 4> bestMoves = allMoves.at(bestMove);
	b->printBoard();
	b->makeMove(bestMoves.at(0), bestMoves.at(1), bestMoves.at(2), bestMoves.at(3), whiteMove, b->getGameBoard(), b->getGameMoved());
}

double AI::minimax(Board* b, int depth, double alpha, double beta, int gameBoard[8][8], bool gameMoved[8][8], int whiteMove) {
	double alphaOrig = alpha;
	unsigned long long key = hash(gameBoard);
	if (transpo.find(key) != transpo.end()) {
		array<double, 3> info = transpo[key];
		if (info.at(0) >= depth) {
		//	if (info.at(2) < 0 && info.at(1) >= beta) return info.at(1);
		//	if (info.at(2) > 0 && info.at(1) <= alpha) return info.at(1);
			if (info.at(2) == 0 && info.at(1) < beta && info.at(1) > alpha) return info.at(1);

			if (info.at(2) < 0) alpha = max(alpha, info.at(1));
			if (info.at(2) > 0) beta = min(beta, info.at(1));
			if (alpha >= beta) return info.at(1);
		}
	} 
	if (depth == 0) {
		int score = quiescenceSearch(b, gameBoard, gameMoved, whiteMove, alpha, beta, 4);
		if (whiteMove) score = -score;
		return score;
		//return -quiescenceSearch(b, gameBoard, gameMoved, whiteMove, alpha, beta);
		//return (double)-evaluateBoard(gameBoard) - evaluateBoardPositions(gameBoard);
	}
	vector<array<int, 4>> allMoves = b->generateAllMovelists(whiteMove, gameBoard);
	if (allMoves.size() == 0) {
		
		if  (allMoves.size() == 0 && b->kingCheckMoves(whiteMove, gameBoard)) {

			if (whiteMove) return 99999 + depth;
			return -99999 - depth;

		}	
		
	}
	double bestMove;
	int move[4] = { { -1 } };
	
	if (!whiteMove) {
		bestMove = -DBL_MAX;
		if (allMoves.size() > 2) {
			for (int i = 0; i < allMoves.size(); i++) {

				array<int, 4> randMove = allMoves.at(i);

				for (int j = 0; j < 2; j++) {
					if (killerMoves[j][depth][0] == randMove.at(0) && killerMoves[j][depth][1] == randMove.at(1)
						&& killerMoves[j][depth][2] == randMove.at(2) && killerMoves[j][depth][3] == randMove.at(3)) {
						//array<int, 4> test = allMoves.at(j);
					//	printf("before %d %d %d %d | %d %d\n", test[0], test[1], test[2], test[3], i, j);
						allMoves[i][0] = allMoves[j][0];
						allMoves[i][1] = allMoves[j][1];
						allMoves[i][2] = allMoves[j][2];
						allMoves[i][3] = allMoves[j][3];

						array<int, 4> move = { killerMoves[j][depth][0], killerMoves[j][depth][1], killerMoves[j][depth][2], killerMoves[j][depth][3] };
						allMoves[j] = move;
						allMoves[j][0] = move[0];
						allMoves[j][1] = move[1];
						allMoves[j][2] = move[2];
						allMoves[j][3] = move[3];
						//test = allMoves.at(j);

					//	printf("after %d %d %d %d\n", test[0], test[1], test[2], test[3]);
					}
				}


			}
		}
		for(int i = 0; i < allMoves.size(); i++) {

			array<int, 4> randMove = allMoves.at(i);
			int newGameBoard[8][8] = { {-1} };
			bool newGameMoved[8][8];
			b->copyGameMoved(gameMoved, newGameMoved);
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					newGameBoard[i][j] = gameBoard[i][j];
				}
			}

			b->makeMove(randMove.at(0), randMove.at(1), randMove.at(2), randMove.at(3), whiteMove, newGameBoard, newGameMoved);

			//printf("%d\n", key);
			double boardV = minimax(b, depth - 1, alpha, beta, newGameBoard, newGameMoved, !whiteMove);
			if (boardV > bestMove) {
				bestMove = boardV;


				move[0] = randMove.at(0);
				move[1] = randMove.at(1);
				move[2] = randMove.at(2);
				move[3] = randMove.at(3);

			}
			//bestMove = max(bestMove, minimax(b, depth - 1, alpha, beta, newGameBoard, newGameMoved, !whiteMove));

			alpha = max(alpha, bestMove);
			if (alpha >= beta) {
				//	printf("CUT-OFF\n");
				break;
			}
		}
	}
	else {
		bestMove = DBL_MAX;
		

		if (allMoves.size() > 2) {
			for (int i = 0; i < allMoves.size(); i++) {

				array<int, 4> randMove = allMoves.at(i);
				for (int j = 0; j < 2; j++) {
					if (killerMoves[j][depth][0] == randMove.at(0) && killerMoves[j][depth][1] == randMove.at(1)
						&& killerMoves[j][depth][2] == randMove.at(2) && killerMoves[j][depth][3] == randMove.at(3)) {
						array<int, 4> test = allMoves.at(j);
						//		printf("before %d %d %d %d | %d %d\n", test[0], test[1], test[2], test[3], i, j);
						allMoves[i][0] = allMoves[j][0];
						allMoves[i][1] = allMoves[j][1];
						allMoves[i][2] = allMoves[j][2];
						allMoves[i][3] = allMoves[j][3];

						array<int, 4> move = { killerMoves[j][depth][0], killerMoves[j][depth][1], killerMoves[j][depth][2], killerMoves[j][depth][3] };
						allMoves[j] = move;
						allMoves[j][0] = move[0];
						allMoves[j][1] = move[1];
						allMoves[j][2] = move[2];
						allMoves[j][3] = move[3];
						test = allMoves.at(j);

						//	printf("after %d %d %d %d\n", test[0], test[1], test[2], test[3]);
					}
				}

			}
		}

		for (int i = 0; i < allMoves.size(); i++) {

			array<int, 4> randMove = allMoves.at(i);
			int newGameBoard[8][8] = { {-1} };
			bool newGameMoved[8][8];
			b->copyGameMoved(gameMoved, newGameMoved);

			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					
					newGameBoard[i][j] = gameBoard[i][j];
				}
			}

			b->makeMove(randMove.at(0), randMove.at(1), randMove.at(2), randMove.at(3), whiteMove, newGameBoard, newGameMoved);
			
			double boardV = minimax(b, depth - 1, alpha, beta, newGameBoard, newGameMoved, !whiteMove);
			if (boardV < bestMove) {
				bestMove = boardV;

				move[0] = randMove.at(0);
				move[1] = randMove.at(1);
				move[2] = randMove.at(2);
				move[3] = randMove.at(3);
			}
			//bestMove = min(bestMove, minimax(b, depth - 1, alpha, beta, newGameBoard, newGameMoved, !whiteMove));
			beta = min(beta, bestMove);
			if (alpha >= beta) {

				//printf("CUT-OFF\n");
				break;
			}

		}
		


	}
	array<double, 3> info;
	if (bestMove <= alphaOrig) {
		info = { (double)depth, bestMove, 1 };
	}
	else if (bestMove > alpha && bestMove < beta) {
		info = { (double)depth, bestMove, 0 };
	}
	else if (bestMove >= beta) {
		info = { (double)depth, bestMove, -1 };

		if (gameBoard[move[3]][move[2]] == -1) {
			
			killerMoves[1][depth][0] = killerMoves[0][depth][0];
			killerMoves[1][depth][1] = killerMoves[0][depth][1];
			killerMoves[1][depth][2] = killerMoves[0][depth][2];
			killerMoves[1][depth][3] = killerMoves[0][depth][3];

			killerMoves[0][depth][0] = move[0];
			killerMoves[0][depth][1] = move[1];
			killerMoves[0][depth][2] = move[2];
			killerMoves[0][depth][3] = move[3];
		}
	}
	transpo[key] = info;
	return bestMove;

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
	return overall * 10;
}
int AI::evaluateBoard(int gameBoard[8][8]) {
	int overall = 0;
	//printf("-------------------BOARD EVALUATION-----------------\n");
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			switch (gameBoard[i][j]) {
			case WHITE_PAWN: overall += 100; break;
			case BLACK_PAWN: overall -= 100; break;
			case WHITE_KNIGHT: overall += 320; break;
			case BLACK_KNIGHT: overall -= 320; break;
			case WHITE_BISHOP: overall += 330; break;
			case BLACK_BISHOP: overall -= 330; break;
			case WHITE_ROOK: overall += 500; break;
			case BLACK_ROOK: overall -= 500; break;
			case WHITE_QUEEN: overall += 900; break;
			case BLACK_QUEEN: overall -= 900; break;
			case WHITE_KING: overall += 20000; break;
			case BLACK_KING: overall -= 20000; break;
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
	random_device rd;     //Get a random seed from the OS entropy device, or whatever
	mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator

	uniform_int_distribution<unsigned long long> distr;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			for (int k = 0; k < ALL_PIECES; k++) {
			
				zobristTable[i][j][k] = distr(eng);
			}
		}
	}
}



double AI::quiescenceSearch(Board* b, int gameBoard[8][8], bool gameMoved[8][8], bool whiteMove, double alpha, double beta, int depth) {	
	double boardV;

	if (whiteMove) boardV = (double)evaluateBoard(gameBoard) + evaluateBoardPositions(gameBoard);
	if (!whiteMove) boardV = (double)-evaluateBoard(gameBoard) - evaluateBoardPositions(gameBoard);
	if (depth == 0) return boardV;
	if (boardV >= beta) {

		return boardV;
	}

	if (boardV > alpha) {
		alpha = boardV;
	}

	vector<array<int, 4>> allMoves = b->generateAllCapture(whiteMove, gameBoard);
	if (allMoves.size() == 0) {

		if (whiteMove) boardV = (double)evaluateBoard(gameBoard) + evaluateBoardPositions(gameBoard);
		if (!whiteMove) boardV = (double)-evaluateBoard(gameBoard) - evaluateBoardPositions(gameBoard);

		return boardV;
	}
	for (int i = 0; i < allMoves.size(); i++) {

	
		array<int, 4> randMove = allMoves.at(i);

		int newGameBoard[8][8] = { { -1 } };
		bool newGameMoved[8][8];

		b->copyGameMoved(gameMoved, newGameMoved);

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {

				newGameBoard[i][j] = gameBoard[i][j];
			}
		}
		b->makeMove(randMove.at(0), randMove.at(1), randMove.at(2), randMove.at(3), whiteMove, newGameBoard, newGameMoved);


	//	if (b->kingCheckMoves(!whiteMove, newGameBoard)) continue;
		double score = -quiescenceSearch(b, newGameBoard, newGameMoved, !whiteMove, -beta, -alpha, depth - 1);

		if (score >= beta) return score;
		if (score > alpha) {
			alpha = score;
		}
	}
	//printf("%lf\n", alpha);
	return alpha;
} 

unsigned long long AI::hash(int gameBoard[8][8]) {
	unsigned long long h = 0;
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

void Board::copyGameMoved(bool src[8][8], bool dest[8][8]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			dest[i][j] = src[i][j];
		}
	}

}