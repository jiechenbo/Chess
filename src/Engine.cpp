
#define _CRT_SECURE_NO_DEPRECATE
#include "Engine.h"
#include "AI.h"
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;
const int SPRITES_TOTAL = 12;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

LTexture gTexture;
LTexture gBackground;

SDL_Rect gSpriteClips[SPRITES_TOTAL];
SDL_Rect wall;

LTexture gSpriteSheetTexture;
LTexture gButtonSpriteSheetTexture;

Board b;

const int BOARD_SIZE = 8;

int main(int argc, char* args[])
{
	
	if (!init()) {
		printf( "Failed to initialize!\n" );
		
		return -1;
	}

	if (!loadPack()) {
		printf("Failed to load media!\n");
		return -1;
	}

	if (!initBoard()) {


	}
	bool quit = false;

	SDL_Event e;

	wall.x = 300;
	wall.y = 40;
	wall.w = 40;
	wall.h = 400;
	SDL_Rect dstrect = { 0, 0, 200, 200 };
	int spriteValue = -1;
	int oldX = 0;
	int oldY = 0;
	bool firstTime = true;
	bool whiteMove = true;
	AI comp;
	comp.initZobrist();
	while (!quit) {
		auto y = std::chrono::high_resolution_clock::now();
		while (SDL_PollEvent(&e) != 0) {
			
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (whiteMove == false) {
				comp.makeBestMove(&b, whiteMove);
				if (b.kingCheckMoves(!whiteMove, b.getGameBoard())) {
					b.checkWin(!whiteMove, b.getGameBoard());
				}
				else if (b.checkStale(!whiteMove, b.getGameBoard())) {
					//exit(0);
				}

				whiteMove = whiteMove == true ? false : true;

			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (isInBoundingRegion(e.button.x, e.button.y)) {
					int lowerLimit = whiteMove == true ? WHITE_KING : BLACK_KING;
					int higherLimit = whiteMove == true ? WHITE_PAWN : BLACK_PAWN;
					if (spriteValue == -1 && b.getSprite(e.button.x, e.button.y) != -1) {
						spriteValue = b.getSprite(e.button.x, e.button.y);

						oldX = e.button.x;
						oldY = e.button.y;
					} else if (spriteValue != -1 && b.getSprite(e.button.x, e.button.y) >= lowerLimit && b.getSprite(e.button.x, e.button.y) <= higherLimit) {
						spriteValue = b.getSprite(e.button.x, e.button.y);

						oldX = e.button.x;
						oldY = e.button.y;
					} else if (spriteValue != -1) {
						int scaledOldX = b.scaleValue(oldX);
						int scaledOldY = b.scaleValue(oldY);

						int scaledFinalX = b.scaleValue(e.button.x);
						int scaledFinalY = b.scaleValue(e.button.y);
						if (b.UserMove(scaledOldX, scaledOldY, scaledFinalX, scaledFinalY, whiteMove) == true) {
							if (b.kingCheckMoves(!whiteMove, b.getGameBoard())) {
								b.checkWin(!whiteMove, b.getGameBoard());
							}
							else if (b.checkStale(!whiteMove, b.getGameBoard())) {
								//exit(0);
							}

							firstTime = false;
							whiteMove = whiteMove == true ? false : true;
						}
						spriteValue = -1;
						oldX = 0;
						oldY = 0;
					} else {
						spriteValue = -1;
						oldX = 0;
						oldY = 0;
					}
				}
			} 

			

		}
		int xx, yy;
		SDL_GetMouseState(&xx, &yy);
		if (firstTime == false) SDL_RenderClear(gRenderer);
		
		b.render();
		b.renderBoard(gSpriteClips);

		
		SDL_RenderPresent(gRenderer);
		
		auto x = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> second = x - y;
		std::chrono::seconds sec(1);

	}
	close();

	return 0;
	
}

bool isInBoundingRegion(int x, int y) {

	printf("%d %d %d %d\n", x, y, b.gBoard.getWidth(), b.gBoard.getHeight());
	if (x >= b.gBoard.getWidth() || y >= b.gBoard.getHeight()) {
		return false;
		
	}
	return true;
}

bool initBoard() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	
	b.render();



	b.initBoard();
	for (int i = 0; i < SPRITES_TOTAL; i++) {
		printf("x: %d y: %d\n", gSpriteClips[i].x, gSpriteClips[i].y);
	}
	b.renderBoard(gSpriteClips);

	SDL_RenderPresent(gRenderer);
 	return true;
}

bool init()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	return true;
}

bool loadPack()
{
	Json::Value dataPack = JsonFileInput("sprites/chessBoard.json");
	if (!dataPack) return false;

	int numImages = dataPack["background"].size();
	loadImage(dataPack["background"].asString());
	
	
	int numSprites = dataPack["sprites"].size();
	
	printf("%d\n", numSprites);
	std::string fileName = dataPack["sprites"].asString();
	if (!gSpriteSheetTexture.loadFromFile(fileName))
	{
		std::cout << "Failed to load " << fileName;
	}
	loadSprites();
	return true;
}

bool loadImage(const std::string fileName)
{
	if (!b.gBoard.loadFromFile(fileName)) 
	{
		std::cout << "Failed to load " << fileName;
		return false;
	}
	return true;
}

bool loadSprites() {
	int offSetWidth = gSpriteSheetTexture.getWidth() / 6;
	int offSetHeight = gSpriteSheetTexture.getHeight() / 2;
	int currWidth = 0;
	int currHeight = 0;
	int k = 0;
	for (int i = 0; i < 2; i++) {
		currHeight = i * offSetHeight;
		for (int j = 0; j < 6; j++) {
			currWidth = j * offSetWidth;
			//printf("currWidth: %d\n", currWidth);
			gSpriteClips[k].x = currWidth;
			gSpriteClips[k].y = currHeight;
			gSpriteClips[k].w = offSetWidth;
			gSpriteClips[k].h = offSetHeight;
			k++;
		}
	}
	return true;
}

Json::Value JsonFileInput(const std::string fileName)
{
	auto file = fopen(fileName.c_str(), "rb");
	
    // Bail out if file failed to open.
	if (!file) {
		printf("Could not load file %s\n", fileName.c_str());
		return nullptr;
	}

    // Read in the file.
	int count = File_getSize(file);
	char* data = new char[count];
	if (!data) {
        return nullptr;
    }

	size_t nr = fread(data, 1, count, file);

	if (nr != count) {
        printf("Could not read enough bytes (%d vs %d).", nr, count);
        count = nr;
    }


	Json::CharReaderBuilder rbuilder;
	std::unique_ptr<Json::CharReader> reader(rbuilder.newCharReader());
	Json::Value obj;

    // Parse JSON content.
    bool ret = reader->parse((const char* ) data, (const char* ) data + (count), &obj, NULL);
    if (!ret) {
        //printf("Failed to parse json file:\n\n%s\n", reader->getFormattedErrorMessages().c_str());
        return nullptr;
    }
	return obj;
}

int File_getSize(FILE* m_fp)
{
    off_t prev = ftell(m_fp);
	off_t sz;
    if (prev == -1) {
        printf("Could not tell current FP offset.\n");
        goto error1;
    }

    if (fseek(m_fp, 0 , SEEK_END) != 0) {
        printf("Could not seek to end of file. Not regular file?\n");
        goto error1;
    }

    sz = ftell(m_fp);
    if (sz == -1) {
        printf("Could not tell end FP offset.\n");
        goto error1;
    }

    fseek(m_fp, prev , SEEK_SET);
    return (uint64_t) sz;

    // Exit stack.
error1:
    if (prev != -1) {
        fseek(m_fp, prev , SEEK_SET);
    }
	return -1;
}


void close()
{
	gTexture.free();
	gBackground.free();
	gButtonSpriteSheetTexture.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}
