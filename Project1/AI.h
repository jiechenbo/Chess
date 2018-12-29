#include <unordered_map>
#include <array>
 
using namespace std;
class AI {
public:
	void makeBestMove(Board* b, bool whiteMove);
	void initZobrist();
private:
	double evaluateBoardPositions(int gameBoard[8][8]);
	int evaluateBoard(int gameBoard[8][8]);
	double minimax(Board* b, int depth, double alpha, double beta, int gameBoard[8][8], int whiteMove);

	unordered_map<int, array<double, 2>> transpo;
	int zobristTable[8][8][12] = { { 0 } };

	int hash(int gameBoard[8][8]);

};

