#include <map>

class AI {
public:
	void makeBestMove(Board* b, bool whiteMove);

private:
	double evaluateBoardPositions(int gameBoard[8][8]);
	int evaluateBoard(int gameBoard[8][8]);
	double minimax(Board* b, int depth, double alpha, double beta, int gameBoard[8][8], int whiteMove);

	map<int, int> transpo;

};

