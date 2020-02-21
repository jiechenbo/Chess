#include "Engine.h"

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	free();

	SDL_Texture* newTexture = NULL;
	
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError() );
		return false;
	} 
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

	if (newTexture == NULL)
	{
		printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		return false;
	} 
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	
	SDL_FreeSurface( loadedSurface );
	
	mTexture = newTexture;
	return (mTexture != NULL);
}

void LTexture::free()
{
	if (mTexture != NULL) 
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);

}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

SDL_Texture* LTexture::getTexture() {
	return mTexture;
}