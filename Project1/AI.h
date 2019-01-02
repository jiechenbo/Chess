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
	double AI::minimax(Board* b, int depth, double alpha, double beta, int gameBoard[8][8], bool gameMoved[8][8], int whiteMove);

	unordered_map<unsigned long long, array<double, 3>> transpo;
	unsigned long long zobristTable[8][8][12] = { { 0 } };
	double killerMoves[2][30][3] = { {{-1}} };
	unsigned long long hash(int gameBoard[8][8]);

};

