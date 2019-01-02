#include <SDL/SDL.h>
#include <json/json.h>
#include <stdio.h>
#include <string>
#include <chrono>
#include <memory>
#include <iostream>

#include <vector>
#include <array>


using namespace std;

//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const int BOARD_SIZE;
extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;
extern const int TOTAL_BUTTONS;
extern const int SPRITES_TOTAL;

enum LButtonSprite
{
    WHITE_KING,
	WHITE_QUEEN,
	WHITE_BISHOP,
	WHITE_KNIGHT,
	WHITE_ROOK,
	WHITE_PAWN,
	BLACK_KING,
	BLACK_QUEEN,
	BLACK_BISHOP,
	BLACK_KNIGHT,
	BLACK_ROOK,
	BLACK_PAWN,
	ALL_PIECES

};

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path );

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render( int x, int y , SDL_Rect*clip = NULL);

        //Gets image dimensions
        int getWidth();
        int getHeight();

		SDL_Texture* getTexture();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
}; 

//The mouse button
class LButton
{
    public:
        //Initializes internal variables
        LButton();

        //Sets top left position
        void setPosition( int x, int y );

        //Handles mouse event
        void handleEvent( SDL_Event* e );
    
        //Shows button sprite
        void render();

    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
        LButtonSprite mCurrentSprite;
};

class Board {
public:
	//Initializes internal variables
	Board();
	//Board textures
	LTexture gBoard;

	void initBoard();
	//Shows board sprite
	void render(SDL_Rect*clip = NULL);

	void renderBoard(SDL_Rect sprites[]);

	int getSprite(int x, int y);

	void printBoard();

	bool validMove(int spriteX, int spriteY, int newX, int newY, bool whiteMove, bool doMove, int gameBoard[8][8]);

	void makeMove(int spriteX, int spriteY, int newX, int newY, bool whiteMove, int gameBoard[8][8], bool gameMoved[8][8]);

	bool UserMove(int spriteX, int spriteY, int newX, int newY, bool whiteMove);

	bool kingCheckMoves(bool whiteMove, int gameBoard[8][8]);

	void getMoveList(bool whiteMove, int posX, int posY, int moveList[2][64], int gameBoard[8][8]);

	bool checkStale(bool whiteMove, int gameBoard[8][8]);

	bool checkWin(bool whiteMove, int gameBoard[8][8]);

	int scaleValue(int value);

	bool checkKingMove(bool whiteMove, int posX, int posY, int newX, int newY, int gameBoard[8][8]);

	int (*getGameBoard())[8];

	bool (*getGameMoved())[8];
	void copyGameMoved(bool src[8][8], bool dest[8][8]);

	vector<array<int, 4>> Board::generateAllMovelists(bool whiteMove, int gameBoard[8][8]);
private:

	bool kingMoves(int spriteX, int spriteY, int newX, int newY, int gameBoard[8][8], bool whiteMove);


	int gameBoard[8][8];

	bool gameMoved[8][8] = { false };

	int initGameBoard[8][8];
}; 

bool init();
bool initBoard();
bool loadPack();
bool loadImage(const std::string fileName);
bool isInBoundingRegion(int x, int y);

bool loadSprites();
Json::Value JsonFileInput(const std::string fileName);
int File_getSize(FILE* m_fp);


void close();

extern SDL_Window* gWindow;

extern SDL_Renderer* gRenderer;

extern LTexture gTexture;
extern LTexture gBackground;

extern SDL_Rect wall;
extern LTexture gSpriteSheetTexture;
extern LTexture gButtonSpriteSheetTexture;


extern LButton gButtons[4];



