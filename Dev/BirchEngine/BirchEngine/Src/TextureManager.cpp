#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture); // load image file as a SDL surface
	SDL_Texture* returnTexture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface); // Create SDL texture from the SDL surface
	SDL_FreeSurface(tempSurface); // Free the SDL surface

	return returnTexture; // return the created SDL texture
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}
